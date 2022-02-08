#include "data.h"
#include <stdlib.h>

// ---------------------------------------------------------------------------
// images
// ---------------------------------------------------------------------------

static const sPhysical phy_grchkrx_bee = {
    { 5, 2 },
    {
        '/',    0x04,   'o',    0x06,   'o',    0x06,   'o',    0x06,   '\\',   0x04,
        '\\',   0x04,   '|',    0x04,   '!',    0x09,   '|',    0x04,   '/',    0x04
    }
};
static const sPhysical phy_grchkrx_bee_hit = {
    { 5, 2 },
    {
        '/',    0x0c,   'o',    0x06,   'o',    0x06,   'o',    0x06,   '\\',   0x0c,
        '\\',   0x0c,   '|',    0x0c,   '!',    0x09,   '|',    0x0c,   '/',    0x0c
    }
};
static const sPhysical phy_grchkrx_bee_dying = {
    { 5, 2 },
    {
        '\0',   0x07,   'o',    0x08,   'o',    0x08,   'o',    0x08,   '\0',   0x07,
        '\0',   0x08,   '|',    0x08,   ' ',    0x07,   '|',    0x08,   '\0',   0x08
    }
};

static const sTurret turret_grchkrx_bee = TURRET_GRID(1, 2, 0, 8, &ot_foe_bullet);

static const sPhysical phy_grchkrx_queen = {
    { 8, 4 },
    {
        '\0',   0x00,   '/',    0x04,   'o',    0x06,   'o',    0x06,   'o',    0x06,   'o',    0x06,   '\\',   0x04,   '\0',   0x00,
        '#',    0x04,   ' ',    0x04,   ' ',    0x04,   '_',    0x04,   '_',    0x04,   ' ',    0x04,   ' ',    0x04,   '#',    0x04, 
        '|',    0x04,   ' ',    0x04,   '|',    0x04,   '!',    0x09,   '!',    0x09,   '|',    0x04,   ' ',    0x04,   '|',    0x04, 
        '\0',   0x00,   '\\',   0x04,   '|',    0x04,   '\0',   0x09,   '\0',   0x09,   '|',    0x04,   '/',    0x04,   '\0',   0x00
    }
};
static const sPhysical phy_grchkrx_queen_hit = {
    { 8, 4 },
    {
        '\0',   0x00,   '/',    0x0c,   'o',    0x06,   'o',    0x06,   'o',    0x06,   'o',    0x06,   '\\',   0x0c,   '\0',   0x00,
        '#',    0x0c,   ' ',    0x0c,   ' ',    0x0c,   '_',    0x0c,   '_',    0x0c,   ' ',    0x0c,   ' ',    0x0c,   '#',    0x0c, 
        '|',    0x0c,   ' ',    0x0c,   '|',    0x0c,   '!',    0x09,   '!',    0x09,   '|',    0x0c,   ' ',    0x0c,   '|',    0x0c, 
        '\0',   0x00,   '\\',   0x0c,   '|',    0x0c,   '\0',   0x09,   '\0',   0x09,   '|',    0x0c,   '/',    0x0c,   '\0',   0x00
    }
};
static const sPhysical phy_grchkrx_queen_dying = {
    { 8, 4 },
    {
        '\0',   0x00,   '/',    0x08,   '*',    0x08,   '*',    0x08,   '*',    0x08,   '*',    0x08,   '\\',   0x08,   '\0',   0x00,
        '#',    0x08,   ' ',    0x08,   ' ',    0x08,   '_',    0x08,   '_',    0x08,   ' ',    0x08,   ' ',    0x08,   '#',    0x08, 
        '|',    0x08,   ' ',    0x08,   '|',    0x08,   ':',    0x09,   ':',    0x09,   '|',    0x08,   ' ',    0x08,   '|',    0x08, 
        '\0',   0x00,   '\\',   0x08,   '|',    0x08,   '\0',   0x09,   '\0',   0x09,   '|',    0x08,   '/',    0x08,   '\0',   0x00
    }
};

static const sTurret turret_grchkrx_queen_0 = TURRET_GRID(3, 3, -1, 10, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_1 = TURRET_GRID(4, 3, +1, 10, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_2 = TURRET_GRID(3, 3, -2, 9, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_3 = TURRET_GRID(4, 3, +2, 9, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_4 = TURRET_GRID(3, 3, -3, 8, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_5 = TURRET_GRID(4, 3, +3, 8, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_6 = TURRET_GRID(3, 3, -4, 7, &ot_foe_ball);
static const sTurret turret_grchkrx_queen_7 = TURRET_GRID(4, 3, +4, 7, &ot_foe_ball);

// ---------------------------------------------------------------------------
// object types
// ---------------------------------------------------------------------------

void cb_bee_behave(hsObject obj) {

    obj->physical = &phy_grchkrx_bee;

    if (obj->pos.x == grid2world(0) && obj->speed.x < 0)
        obj->speed.x = 6;
    else
    if (obj->pos.x >= grid2world(VIEWGRID_WIDTH - obj->physical->dim.x) && obj->speed.x > 0)
        obj->speed.x = -6;
    else
    if (obj->speed.x == 0)
        obj->speed.x = 6;

    obj->speed.y = 2 - (rand() % 5);
        
    if (!(obj->flags & OBJ_FLG_DYING)) {
    
        if (obj->ttl == 0) {

            obj->ttl = (rand() & 3) + 20;
            fire_turret(obj, &turret_grchkrx_bee);
        }
    }
}

void cb_bee_die(hsObject obj) {

    obj->physical = &phy_grchkrx_bee_dying;
    obj->ttl = 3;
}

void cb_bee_hit(hsObject obj) {

    obj->physical = &phy_grchkrx_bee_hit;
}

void cb_queen_behave(hsObject obj) {

    obj->speed.y = adjust(obj->speed.y + 1 - (rand() % 3), -3, 3);
    obj->speed.x = adjust(obj->speed.x + 3 - (rand() % 7), -5, 5);

    if (world2grid(obj->pos.y) > 10)
        obj->speed.y = -2;
    
    if (!(obj->flags & OBJ_FLG_DYING)) {
    
        obj->physical = &phy_grchkrx_queen;

        if (obj->ttl == 0) {

            obj->ttl = 50;
        }
        else
        if (obj->ttl == 16) {

            fire_turret(obj, &turret_grchkrx_queen_0);
            fire_turret(obj, &turret_grchkrx_queen_1);
        }
        else
        if (obj->ttl == 12) {

            fire_turret(obj, &turret_grchkrx_queen_2);
            fire_turret(obj, &turret_grchkrx_queen_3);
        }
        else
        if (obj->ttl == 8) {

            fire_turret(obj, &turret_grchkrx_queen_4);
            fire_turret(obj, &turret_grchkrx_queen_5);
        }
        else
        if (obj->ttl == 4) {

            fire_turret(obj, &turret_grchkrx_queen_6);
            fire_turret(obj, &turret_grchkrx_queen_7);
        }
    }
}

void cb_queen_die(hsObject obj) {

    obj->physical = &phy_grchkrx_queen_dying;
    obj->ttl = 7;
}

void cb_queen_hit(hsObject obj) {

    obj->physical = &phy_grchkrx_queen_hit;
}

const sObjType ot_grchkrx_bee = {
    &phy_grchkrx_bee,
    OBJTYPE_NAT_FOE_OBJ,
    10,
    cb_bee_behave,
    cb_bee_die,
    cb_bee_hit,
    NULL
};

const sObjType ot_grchkrx_queen = {
    &phy_grchkrx_queen,
    OBJTYPE_NAT_FOE_OBJ,
    50,
    cb_queen_behave,
    cb_queen_die,
    cb_queen_hit,
    NULL
};
