#include "data.h"
#include "shooter.h"
#include <stdlib.h>

static const sPhysical phy_flank = {
    { 5, 3 },
    {
        '\0',   0x00,   '.',    0x00,   'v',    0x06,   '.',    0x00,   '\0',   0x00,
        '/',    0x06,   '=',    0x02,   '#',    0x02,   '=',    0x02,   '\\',   0x06,
        '\0',   0x00,   '\0',   0x00,   'v',    0x0a,   '\0',   0x00,   '\0',   0x00
    }
};
static const sPhysical phy_flank_hit = {
    { 5, 3 },
    {
        '\0',   0x00,   '.',    0x00,   'v',    0x07,   '.',    0x00,   '\0',   0x00,
        '/',    0x06,   '=',    0x07,   '#',    0x07,   '=',    0x07,   '\\',   0x06,
        '\0',   0x00,   '\0',   0x00,   'v',    0x0f,   '\0',   0x00,   '\0',   0x00
    }
};
static const sPhysical phy_flank_dying = {
    { 5, 3 },
    {
        '\0',   0x00,   '.',    0x00,   '*',    0x08,   '.',    0x00,   '\0',   0x00,
        '\0',   0x00,   '*',    0x08,   '#',    0x08,   '*',    0x08,   '\0',   0x00,
        '\0',   0x00,   '\0',   0x00,   'v',    0x08,   '\0',   0x00,   '\0',   0x00
    }
};

static const sTurret turret_flank_left      = TURRET_GRID(0, 2, -10, 9, &ot_foe_bullet);
static const sTurret turret_flank_right     = TURRET_GRID(4, 2, +10, 9, &ot_foe_bullet);
static const sTurret turret_flank_center    = TURRET_GRID(2, 2,  0, 16, &ot_foe_laser);

#pragma argsused
void flank_init(hsObject obj, unsigned char state) {

    if (state == FLANK_GO_LEFT_DOWN) {
    
        obj->pos.x = OBJ_POS_LEFT(obj);
    }
    else {
    
        obj->pos.x = OBJ_POS_RIGHT(obj);
    }

    obj->pos.y = 0;
    obj->speed.x = 0;
    obj->speed.y = 8;
    obj->state[0] = state;
    obj->state[2] = 0;

    fire_turret(obj, &turret_flank_center);
}

static void flank_behave(hsObject obj) {

    if (obj->flags & OBJ_FLG_DYING)
        return;

    obj->physical = &phy_flank;

    if (obj->state[2] == 0) {
    
        obj->state[2] = 8 + rand() % 4;
    }
    else {
    
        obj->state[2]--;
    }

    if (obj->state[0] == FLANK_GO_LEFT_DOWN) {
    
        if (world2grid(obj->pos.y) == (VIEWGRID_DIVIDER - 8)) {
        
            obj->speed.x    = OBJ_POS_RIGHT(obj) / (32 * 4);
            obj->speed.y    = -8;
            obj->state[0]   = FLANK_GO_RIGHT_UP;
        }

        if (obj->state[2] == 8)
            fire_turret(obj, &turret_flank_right);
    }
    else
    if (obj->state[0] == FLANK_GO_RIGHT_UP) {
    
        if (OBJ_IS_RIGHT(obj)) {
        
            obj->speed.x = 0;
            obj->speed.y = 8;
            obj->state[0] = FLANK_GO_RIGHT_DOWN;

            fire_turret(obj, &turret_flank_center);
        }
    }
    else
    if (obj->state[0] == FLANK_GO_RIGHT_DOWN) {

        if (world2grid(obj->pos.y) == (VIEWGRID_DIVIDER - 8)) {
        
            obj->speed.x    = -(OBJ_POS_RIGHT(obj) / (32 * 4));
            obj->speed.y    = -8;
            obj->state[0]   = FLANK_GO_LEFT_UP;
        }

        if (obj->state[2] == 0)
            fire_turret(obj, &turret_flank_left);
    }
    else
    if (obj->state[0] == FLANK_GO_LEFT_UP) {
    
        if (OBJ_IS_LEFT(obj)) {
        
            obj->speed.x = 0;
            obj->speed.y = 8;
            obj->state[0] = FLANK_GO_LEFT_DOWN;

            fire_turret(obj, &turret_flank_center);
        }
    }
}

static void flank_die(hsObject obj) {

    obj->physical = &phy_flank_dying;
    obj->ttl = 3;
}

static void flank_hit(hsObject obj) {

    obj->physical = &phy_flank_hit;
}

const sObjType ot_flank = {
    &phy_flank,
    OBJTYPE_NAT_FOE_OBJ,
    20,
    flank_behave,
    flank_die,
    flank_hit,
    NULL
};
