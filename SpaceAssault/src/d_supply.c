#include "data.h"
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  healing salve
// ---------------------------------------------------------------------------

static const sPhysical phy_bonus_heal = {
    { 3, 3 },
    {
        '\0',   0x00,   0xc4,   0x07,   '\0',   0x00,
        0xb3,   0x07,   0xfe,   0x47,   0xb3,   0x07,
        '\0',   0x00,   0xc4,   0x07,   '\0',   0x00
    }
};

static void heal_behave(hsObject obj) {

    if (obj->speed.x > 0) {
    
        obj->speed.x--;
    }
    else
    if (obj->speed.x < 0) {

        obj->speed.x++;
    }
    
    if (obj->speed.y < 4) {
    
        obj->speed.y++;
    }
}

static void heal_deal(hsObject bullet, hsObject target) {

    bullet->flags |= OBJ_FLG_DESTROY;
    
    if (target->damage_total > bullet->type->hp)
        target->damage_total -= bullet->type->hp;
    else
        target->damage_total = 0;
}

const sObjType ot_bonus_heal = {
    &phy_bonus_heal,
    OBJTYPE_NAT_FOE_BULLET,
    20,
    heal_behave,
    NULL,
    NULL,
    heal_deal
};

// ---------------------------------------------------------------------------
//  supply crate
// ---------------------------------------------------------------------------

static const sPhysical phy_supply = {
    { 5, 2 },
    {
        '_',    0x08,   '_',    0x08,   '_',    0x08,   '_',    0x08,   '_',    0x08,
        ':',    0x78,   ' ',    0x74,   '+',    0x74,   ' ',    0x74,   ':',    0x78
    }
};
static const sPhysical phy_supply_dying = {
    { 5, 2 },
    {
        '\0',   0x00,   '\0',   0x00,   '\0',   0x00,   '\0',   0x00,   '\0',   0x00,
        ':',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   ':',    0x08
    }
};

static const sTurret turret_supply_1 = TURRET_GRID(0, 0, +12, -12, &ot_bonus_heal);
static const sTurret turret_supply_2 = TURRET_GRID(0, 0, -8, -6, &ot_bonus_heal);
static const sTurret turret_supply_3 = TURRET_GRID(0, 0, +6, +2, &ot_bonus_heal);

static void supply_behave(hsObject obj) {

    if (obj->flags & OBJ_FLG_DYING)
        return;

    obj->physical = &phy_supply;

    obj->speed.y = 1;
    
    if (obj->ttl == 0) {
    
        obj->ttl = 60;
        obj->speed.x = -2;
    }
    else
    if (obj->ttl == 30) {

        obj->speed.x = 2;
    }
    
    if (OBJ_IS_DOWN(obj)) {
    
        obj->physical = &phy_supply_dying;
        obj->flags |= OBJ_FLG_DYING;
        obj->ttl = 3;
    }
}

static void supply_die(hsObject obj) {

    fire_turret(obj, &turret_supply_1);
    fire_turret(obj, &turret_supply_2);
    fire_turret(obj, &turret_supply_3);
    
    obj->physical = &phy_supply_dying;
    obj->ttl = 3;
}

#pragma argsused
static void supply_hit(hsObject obj) {

//    obj->physical = &phy_octo_hit;
}

const sObjType ot_supply = {
    &phy_supply,
    OBJTYPE_NAT_FOE_OBJ,
    10,
    supply_behave,
    supply_die,
    supply_hit,
    NULL
};
