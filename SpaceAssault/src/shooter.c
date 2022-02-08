#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "shooter.h"
#include "screen.h"
#include "input.h"
#include "objpool.h"

// too lazy to learn TLINK :-)
//#include "screen.c"
//#include "input.c"
//#include "objpool.c"
//#include "d_gen.c"
//#include "d_grch.c"
//#include "d_octo.c"
//#include "d_bible.c"
//#include "d_supply.c"
//#include "d_flank.c"
//#include "data.c"

#define VIEWGRID_SCREEN_X               1
#define VIEWGRID_SCREEN_Y               0

#define BAR_LEFT                        (VIEWGRID_WIDTH + 3)
#define BAR_WIDTH                       16
#define BAR_POS_INTEGRITY               5

extern const sScenarioPoint g_scenario[];

hsObject g_map[VIEWGRID_HEIGHT][VIEWGRID_WIDTH][2]; // 0 is ally, 1 is foe
hsObject player, shield;
unsigned char player_weapon_delay;
hcsScenarioPoint scpoint;
unsigned char scpoint_timer;
unsigned char scpoint_repeat;
hcsDisplayText show_text_first;
unsigned char show_text_count;
unsigned char paused;

/*
static FILE *fp = NULL;

void xlog(char *msg) {

    if (fp == NULL)
        fp = fopen("./log.txt", "w");
        
    if (fp != NULL) {
    
        fprintf(fp, "%s\n", msg);
        fflush(fp);
    }
}
*/
#define xlog(x)

static const sDisplayText text_fail[] = {
    { { 26, 10 }, 0x07, "YOU FAILED" },
};

int adjust(int value, int min, int max) {

    if (value < min)
        return min;
        
    if (value > max)
        return max;
        
    return value;
}

hsObject fire_turret(hsObject obj, hcsTurret turret) {

    hsObject bullet;
            
    if ((bullet = objpool_alloc(turret->type)) != NULL) {
    
        bullet->pos.x = obj->pos.x + turret->pos.x;
        bullet->pos.y = obj->pos.y + turret->pos.y;
        bullet->speed.x = turret->speed.x;
        bullet->speed.y = turret->speed.y;
    }

    return bullet;
}

#define PERIOD_1PERSEC          (!(cycle & 63))
#define PERIOD_2PERSEC          (!(cycle & 31))
#define PERIOD_4PERSEC          (!(cycle & 15))
#define PERIOD_8PERSEC          (!(cycle & 7))
#define PERIOD_16PERSEC         (!(cycle & 3))

unsigned int d1[64], d2[64];
unsigned char dd;
unsigned char debug;
    
void progress(unsigned char cycle) {

    hsObject obj;
    hsObject target;
    hcsObjType otype;
    hcsPhysical otphy;
    sPosGrid pos;
    sVecGrid vec;
    unsigned char i;

    obj = NULL;
    
    // remove dead objects
    objpool_garbage();

    // clear damage map
    memset(g_map, 0, sizeof(g_map)); // TODO - asm...

    // status changes to objects (just the active, because new objects 
    // may be created in this loop and some of them would be handled now 
    // and some in the next iteration, creating uncertainity)
    while ((obj = objpool_next_active(obj)) != NULL) {

        otype = obj->type;
        otphy = obj->physical;

        // perform object's own behaviour
        if (PERIOD_16PERSEC && otype->cb_behave != NULL)
            otype->cb_behave(obj);
        
        // update object position
        obj->pos.x = adjust(obj->pos.x + obj->speed.x, 0, grid2world(VIEWGRID_WIDTH - otphy->dim.x));
        obj->pos.y = adjust(obj->pos.y + obj->speed.y, 0, grid2world(VIEWGRID_HEIGHT - otphy->dim.y));

        // don't allow monsters to get too close to player
        if (otype->nature == OBJTYPE_NAT_FOE_OBJ && obj->pos.y > grid2world(VIEWGRID_HEIGHT - 9))
            obj->pos.y = grid2world(VIEWGRID_HEIGHT - 9);
        
        // update object action
        if (PERIOD_16PERSEC) {

            if (obj->ttl > 0)
                obj->ttl--;
        
            if ((obj->flags & OBJ_FLG_DYING) && obj->ttl == 0)
                obj->flags |= OBJ_FLG_DESTROY;
        }
        
        // handle bullet
        if (otype->nature & OBJTYPE_NATMASK_BULLET) {
        
            // TODO - bullets are now harmless at the edges :-(

            pos.x = world2grid(obj->pos.x); 
            pos.y = world2grid(obj->pos.y);

            if (pos.y == 0 || (pos.y + otphy->dim.y) >= VIEWGRID_HEIGHT || pos.x == 0 || (pos.x + otphy->dim.x) >= VIEWGRID_WIDTH) {
            
                objpool_free(obj);
                continue;
            }
        }

        // handle "mortal" object
        if (otype->nature & OBJTYPE_NATMASK_OBJ) {

            // handle object's damage
            obj->damage_total += obj->damage_now;
            obj->damage_now = 0;
        
            if (obj->damage_total >= otype->hp) {

                obj->damage_total = otype->hp;
                
                // object has taken enough damage, let it die
                if (!(obj->flags & OBJ_FLG_DYING)) {
                
                    obj->flags |= OBJ_FLG_DYING;
                    obj->ttl = 1;
                    
                    if (otype->cb_die != NULL)
                        otype->cb_die(obj);
                }
            }
            else
            if (otype->nature & (OBJTYPE_NATMASK_ALLY | OBJTYPE_NATMASK_FOE)) {

                // TODO - tohle zkraslit...
                // add the object to the damage map
                char imgoff;
                unsigned char objindex;
                char xtype = (otype->nature & OBJTYPE_NATMASK_FOE) ? 1 : 0;
        
                otphy = obj->physical;

                pos.x = world2grid(obj->pos.x);
                pos.y = world2grid(obj->pos.y);

                // TODO - asm...
                imgoff = 0;
                for (vec.y = 0; vec.y < otphy->dim.y; vec.y++)
                    for (vec.x = 0; vec.x < otphy->dim.x; vec.x++, imgoff += 2)
                        if (otphy->image[imgoff] != 0)
                            g_map[vec.y + pos.y][vec.x + pos.x][xtype] = obj;
            }
        }
    }

    // go through bullets
    while ((obj = objpool_next_active(obj)) != NULL) {

        otype = obj->type;
        if (otype->nature & OBJTYPE_NATMASK_BULLET) {
        
            char imgoff;
            
            otphy = obj->physical;

            pos.x = world2grid(obj->pos.x);
            pos.y = world2grid(obj->pos.y);

            imgoff = 0;
            for (vec.y = 0; vec.y < otphy->dim.y; vec.y++) {
            
                for (vec.x = 0; vec.x < otphy->dim.x; vec.x++, imgoff += 2) {
                
                    if (otphy->image[imgoff] == 0)
                        continue;
                    
                    target = g_map[vec.y + pos.y][vec.x + pos.x][(otype->nature & OBJTYPE_NATMASK_FOE) ? 0 : 1];
                    if (target == NULL) 
                        continue;

                    if (otype->cb_deal != NULL)
                        otype->cb_deal(obj, target);
                    
                    if (target->type->cb_hit != NULL)
                        target->type->cb_hit(target);
                        
                    vec.y = otphy->dim.y;
                    break;
                }
            }
        }
    }

    // wait for screen's lunch time
    dd = (dd + 1) & 63;
    d1[dd] = 0;
    d2[dd] = 0;
    while((inp(0x03da) & 0x08))
        d1[dd]++;
    while(!(inp(0x03da) & 0x08))
        d2[dd]++;

    screen_fill_region(' ', 0, screen_offset_xy(VIEWGRID_SCREEN_X, VIEWGRID_SCREEN_Y), VIEWGRID_WIDTH, VIEWGRID_HEIGHT);

    // draw the objects
    while ((obj = objpool_next(obj)) != NULL) {

        obj->flags &= ~OBJ_FLG_INACTIVE;
    
        otype = obj->type;
        otphy = obj->physical;
        
        pos.x = world2grid(obj->pos.x);
        pos.y = world2grid(obj->pos.y);

        screen_blit(otphy->image, screen_offset_xy(pos.x + VIEWGRID_SCREEN_X, pos.y + VIEWGRID_SCREEN_Y), otphy->dim.x, otphy->dim.y);
    }

    // draw texts
    for (i = 0; i < show_text_count; i++) {
    
        hcsDisplayText dt = show_text_first + i;
        
        screen_color(dt->color);
        screen_print_text(dt->pos.x, dt->pos.y, dt->text);
    }
}

void show_bar(char x, char y, char total, char current) {

    screen_print_bar(x, y, BAR_WIDTH, (BAR_WIDTH * current + BAR_WIDTH / 2) / total);
}

int main(/*int argc, char *argv[]*/) {

    hsObject emit;
    unsigned int cycle;
    unsigned char event;

    xlog("start");
    
    srand(time(NULL));
    
    objpool_clear();

    if ((player = objpool_alloc(&ot_player)) == NULL)
        return 1;
        
    player->pos.x = grid2world(VIEWGRID_WIDTH / 2 - 2);
    player->pos.y = grid2world(VIEWGRID_HEIGHT - 1 - player->physical->dim.y);

    shield = NULL;
    
    // prepare playfield
    screen_init();
    screen_clear(0, 0);
    screen_fill_region('|', 8, screen_offset_xy(0, 0), 1, 50);
    screen_fill_region('|', 8, screen_offset_xy(VIEWGRID_WIDTH + 1, 0), 1, 50);

    // prepare integrity bar
    screen_color(7);
    screen_print_text(BAR_LEFT, BAR_POS_INTEGRITY - 2, "INTEGRITY");

    // add texts
    screen_color(8);
    screen_print_text(BAR_LEFT, 35, "Z - CANNON");
    screen_print_text(BAR_LEFT, 37, "X - LASER");
    screen_print_text(BAR_LEFT, 39, "C - LASER");
    screen_print_text(BAR_LEFT, 41, "V - MEGABLAST");
    screen_print_text(BAR_LEFT, 45, "P - PAUSE");
    screen_print_text(BAR_LEFT, 47, "Q - QUIT");
    
    input_init();

    scpoint = &g_scenario[0];
    scpoint_timer = 0;
    scpoint_repeat = 0;
    show_text_first = NULL;
    show_text_count = 0;
    paused = 0;
    
    while (!uifn_hold[UIFN_QUIT]) {

        event = input_pop_event();

        if (event == UIFN_PAUSE)
            paused = !paused;
        if (event == UIFN_DEBUG)
            debug = !debug;

        if (paused)
            continue;
    
        if (debug && PERIOD_4PERSEC) {
        
            unsigned int ad1, ad2;
            unsigned char i;
            
            ad1 = 0;
            ad2 = 0;
            for (i = 0; i < dimof(d1); i++) {
            
                ad1 += d1[i];
                ad2 += d2[i];
            }
        
            screen_color(0x07);
            screen_print_text(70, 20, "         ");
            screen_print_hex16(70, 20, ad1 >> 6);
            screen_print_hex16(75, 20, ad2 >> 6);
        }
            
        // handle scenario
        if (PERIOD_4PERSEC) {
        
            if (scpoint_timer > 0) {
            
                scpoint_timer--;
            }
            else {
            
                scpoint_timer = scpoint->delay;
                show_text_first = scpoint->ds_text;
                show_text_count = scpoint->ds_count;
                
                if (scpoint->cb_tick != NULL)
                    (scpoint->cb_tick)(scpoint, scpoint_repeat);
                
                switch (scpoint->type) {
                
                    case SCPOINT_TYPE_WAIT_DEAD:

                        emit = NULL;
                        while ((emit = objpool_next(emit)) != NULL) {
                        
                            if (emit->type->nature != OBJTYPE_NAT_FOE_OBJ)
                                continue;
                                
                            scpoint_timer = scpoint->delay;
                            scpoint_repeat = 0;
                            break;
                        }
                        break;
                        
                    case SCPOINT_TYPE_END:
                        
                        scpoint_repeat = 0;
                        break;
                }

                if (scpoint_repeat >= scpoint->repeat) {
                
                    scpoint_repeat = 0;
                    scpoint++;
                }
                else
                    scpoint_repeat++;
            }
        }
    
        if (player->flags & OBJ_FLG_DYING) {
        
            player->ttl = 255;
            player->speed.x = 0;
            player->speed.y = 0;

            show_text_first = text_fail;
            show_text_count = dimof(text_fail);
        }
        else {
        
            // handle movements
            player->speed.x = 0 + (uifn_hold[UIFN_LEFT] ? -8 : 0) + (uifn_hold[UIFN_RIGHT] ? 8 : 0);
    //        player->speed.y = 0 + (uifn_hold[UIFN_UP] ? -8 : 0) + (uifn_hold[UIFN_DOWN] ? 8 : 0);

            // handle cannons and lasers
            if (player_weapon_delay > 0) {
            
                player_weapon_delay--;
            }
            else
            if (uifn_hold[UIFN_FIRE_CANNON]) {
            
                player_weapon_delay = 30;
                fire_turret(player, &turret_player_bullet);
            }
            else
            if (uifn_hold[UIFN_FIRE_LASER]) {

                player_weapon_delay = 12;
                fire_turret(player, &turret_player_lleft);
                fire_turret(player, &turret_player_lright);
            }
            else
            if (uifn_hold[UIFN_FIRE_SIDE_LASER]) {

                player_weapon_delay = 15;
                fire_turret(player, &turret_player_llleft);
                fire_turret(player, &turret_player_llright);
            }
            else
            if (uifn_hold[UIFN_FIRE_MEGABLAST]) {

                player_weapon_delay = 90;
                fire_turret(player, &turret_player_megablast);
            }
/*            
            else
            if (uifn_hold[UIFN_SHIELD] && shield == NULL) {

                if ((shield = objpool_alloc(&ot_shield)) != NULL) {

                    shield->pos.x = player->pos.x - grid2world(((signed int)shield->physical->dim.x - (signed int)player->physical->dim.x) / 2);
                    shield->pos.y = player->pos.y - grid2world(2);
                }
            }
*/            
        }
        
        // show integrity bar
        if (player->damage_total == 0)
            screen_color(2);
        else
        if (player->damage_total > (3 * player->type->hp) / 4)
            screen_color(4);
        else
            screen_color(14);

        show_bar(BAR_LEFT, BAR_POS_INTEGRITY, player->type->hp, player->type->hp - player->damage_total);
        
        // handle objects and bullets
        progress(cycle++);
    }
    
    input_close();
    screen_close();
    return 0;
}
