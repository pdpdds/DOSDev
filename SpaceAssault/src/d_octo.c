#include "data.h"
#include <stdlib.h>

static const sPhysical phy_octo = {
    { 5, 3 },
    {
        '\0',   0x00,   '_',    0x07,   'V',    0x09,   '_',    0x07,   '\0',   0x00,
        '/',    0x07,   '=',    0x07,   '#',    0x09,   '=',    0x07,   '\\',   0x07,
        '+',    0x08,   '\0',   0x00,   '!',    0x08,   '\0',   0x00,   '+',    0x08
    }
};
static const sPhysical phy_octo_hit = {
    { 5, 3 },
    {
        '\0',   0x00,   '_',    0x0f,   'V',    0x0f,   '_',    0x0f,   '\0',   0x00,
        '/',    0x0f,   '=',    0x0f,   '#',    0x0f,   '=',    0x0f,   '\\',   0x0f,
        '+',    0x07,   '\0',   0x00,   '!',    0x07,   '\0',   0x00,   '+',    0x07
    }
};
static const sPhysical phy_octo_dying = {
    { 5, 3 },
    {
        '\0',   0x00,   '\0',   0x00,   'V',    0x08,   '\0',   0x00,   '\0',   0x00,
        '.',    0x08,   '*',    0x08,   '*',    0x08,   '*',    0x08,   '.',    0x08,
        '\0',   0x00,   '\0',   0x00,   '\0',   0x00,   '\0',   0x00,   '\0',   0x00
    }
};

static const sTurret turret_octo_lbullet    = TURRET_GRID(0, 3, 0, 8, &ot_foe_bullet);
static const sTurret turret_octo_rbullet    = TURRET_GRID(4, 3, 0, 8, &ot_foe_bullet);
static const sTurret turret_octo_laser      = TURRET_GRID(2, 3, 0, 16, &ot_foe_laser);

void octo_init(hsObject obj, unsigned char state) {

    obj->pos.x      = 0;
    obj->speed.x    = 5;
    obj->speed.y    = 0;
    obj->state[2]   = 50;
    obj->state[0]   = state;
}

static void octo_behave(hsObject obj) {

    if (obj->flags & OBJ_FLG_DYING)
        return;

    obj->physical = &phy_octo;

    if (obj->state[2] == 0) {
    
        obj->state[2] = 60 + rand() % 20;
    }
    else {
    
        if (obj->state[2] == 23 || obj->state[2] == 25 || obj->state[2] == 27) {
        
            fire_turret(obj, &turret_octo_laser);
        }
        else
        if (obj->state[2] == 3 || obj->state[2] == 11 || obj->state[2] == 19) {

            fire_turret(obj, &turret_octo_lbullet);
            fire_turret(obj, &turret_octo_rbullet);
        }
    
        obj->state[2]--;
    }
        
    if (obj->state[0] == OCTO_ST_CLW_GO_RIGHT && OBJ_IS_RIGHT(obj)) {
    
        obj->speed.x = 0;
        obj->speed.y = 3;
        obj->state[0] = OCTO_ST_CLW_GO_DOWN;
        obj->state[1] = 30;
    }
    else
    if (obj->state[0] == OCTO_ST_CLW_GO_DOWN) {
    
        if (--obj->state[1] == 0) {
        
            obj->speed.x    = -5;
            obj->speed.y    = 0;
            obj->state[0]   = OCTO_ST_CLW_GO_LEFT;
        }
    }
    else
    if (obj->state[0] == OCTO_ST_CLW_GO_LEFT && OBJ_IS_LEFT(obj)) {
    
        obj->speed.x = 0;
        obj->speed.y = -3;
        obj->state[0] = OCTO_ST_CLW_GO_UP;
        obj->state[1] = 30;
    }    
    else
    if (obj->state[0] == OCTO_ST_CLW_GO_UP) {
    
        if (--obj->state[1] == 0) {
        
            obj->speed.x    = 5;
            obj->speed.y    = 0;
            obj->state[0]   = OCTO_ST_CLW_GO_RIGHT;
        }
    }
}

static void octo_die(hsObject obj) {

    obj->physical = &phy_octo_dying;
    obj->ttl = 3;
}

static void octo_hit(hsObject obj) {

    obj->physical = &phy_octo_hit;
}

const sObjType ot_octo = {
    &phy_octo,
    OBJTYPE_NAT_FOE_OBJ,
    18,
    octo_behave,
    octo_die,
    octo_hit,
    NULL
};
