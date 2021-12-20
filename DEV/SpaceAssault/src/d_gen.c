#include "data.h"
#include <stdlib.h>

// ---------------------------------------------------------------------------
// images
// ---------------------------------------------------------------------------

static const sPhysical phy_player_blue = {
    { 3, 2 }, 
    {
        '\0',   0x00,   '^',    0x09,   '\0',   0x00,
        '<',    0x01,   '-',    0x09,   '>',    0x01
    }
};
static const sPhysical phy_player_dead = {
    { 3, 2 }, 
    {
        '\0',   0x00,   '*',    0x08,   '\0',   0x00,
        '*',    0x08,   '*',    0x08,   '*',    0x08
    }
};
static const sPhysical phy_shield = {
    { 9, 1 },
    {
        '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d,   '-',    0x5d
    }
};
static const sPhysical phy_hitme = {
    { 7, 2 },
    {
        '_',    0x0a,   '_',    0x0a,   '_',    0x0a,   '_',    0x0a,   '_',    0x0a,   '_',    0x0a,   '_',    0x0a,
        ':',    0x20,   ' ',    0x20,   'H',    0x20,   'I',    0x20,   'T',    0x20,   ' ',    0x20,   ':',    0x20
    }
};

static const sPhysical phy_megablast = {
    { 1, 3 },
    {
        '*',    0x0d,
        '|',    0x05,
        ':',    0x05
    }
};

static const sPhysical phy_megablast_2 = {
    { 1, 3 },
    {
        '*',    0x0d,
        ':',    0x05,
        '|',    0x05
    }
};

static const sPhysical phy_bullet_simple    = PHYSICAL_1X1('.', 0x0f);
static const sPhysical phy_bullet_torpedo   = PHYSICAL_1X1('!', 0x0f);
static const sPhysical phy_bullet_laser     = PHYSICAL_1X1('|', 0x0a);
static const sPhysical phy_laser_left       = PHYSICAL_1X1('\\', 0x0a);
static const sPhysical phy_laser_right      = PHYSICAL_1X1('/', 0x0a);

static const sPhysical phy_alien_laser      = PHYSICAL_1X1('|', 0x0c);
static const sPhysical phy_alien_ball       = PHYSICAL_1X1('*', 0x0f);

// ---------------------------------------------------------------------------
// object types
// ---------------------------------------------------------------------------

void cb_deal_die(hsObject bullet, hsObject target) {

    bullet->flags |= OBJ_FLG_DESTROY;
    target->damage_now += bullet->type->hp;
}

const sObjType ot_ally_simple = {
    &phy_bullet_simple,
    OBJTYPE_NAT_ALLY_BULLET,
    6,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

static const sTurret turret_megablast_sw    = TURRET_GRID(-2, +2, -6, +6, &ot_ally_simple);
static const sTurret turret_megablast_w     = TURRET_GRID(-2, +0, -8, +0, &ot_ally_simple);
static const sTurret turret_megablast_nw    = TURRET_GRID(-2, -2, -6, -6, &ot_ally_simple);
static const sTurret turret_megablast_ne    = TURRET_GRID(+2, -2, +6, -6, &ot_ally_simple);
static const sTurret turret_megablast_e     = TURRET_GRID(+2, +0, +8, +0, &ot_ally_simple);
static const sTurret turret_megablast_se    = TURRET_GRID(+2, +2, +6, +6, &ot_ally_simple);

void cb_deal_megablast(hsObject bullet, hsObject target) {

    fire_turret(bullet, &turret_megablast_sw);
    fire_turret(bullet, &turret_megablast_w);
    fire_turret(bullet, &turret_megablast_nw);
    fire_turret(bullet, &turret_megablast_ne);
    fire_turret(bullet, &turret_megablast_e);
    fire_turret(bullet, &turret_megablast_se);

    bullet->flags |= OBJ_FLG_DESTROY;
    target->damage_now += bullet->type->hp;
}

void cb_behave_megablast(hsObject obj) {

    if (obj->ttl == 0) {
    
        obj->physical = &phy_megablast;
        obj->ttl = 4;
    }
    else
    if (obj->ttl == 2) {

        obj->physical = &phy_megablast_2;
    }
}

const sObjType ot_ally_bullet = {
    &phy_bullet_torpedo,
    OBJTYPE_NAT_ALLY_BULLET,
    6,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_ally_laser = {
    &phy_bullet_laser,
    OBJTYPE_NAT_ALLY_BULLET,
    2,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_ally_laser_left = {
    &phy_laser_left,
    OBJTYPE_NAT_ALLY_BULLET,
    2,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_ally_laser_right = {
    &phy_laser_right,
    OBJTYPE_NAT_ALLY_BULLET,
    2,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_ally_megablast = {
    &phy_megablast,
    OBJTYPE_NAT_ALLY_BULLET,
    12,
    cb_behave_megablast,
    NULL,
    NULL,
    cb_deal_megablast
};

const sObjType ot_shield = {
    &phy_shield,
    OBJTYPE_NAT_ALLY_OBJ,
    40,
    NULL,    
    NULL,    
    NULL,    
    NULL
};

const sTurret turret_player_bullet      = TURRET_GRID(1, -1,  0, -10, &ot_ally_bullet);
const sTurret turret_player_megablast   = TURRET_GRID(1, -1,  0,  -8, &ot_ally_megablast);
const sTurret turret_player_lleft       = TURRET_GRID(0, -1,  0, -12, &ot_ally_laser);
const sTurret turret_player_lright      = TURRET_GRID(2, -1,  0, -12, &ot_ally_laser);
const sTurret turret_player_llleft      = TURRET_GRID(0, 0,  -8, -12, &ot_ally_laser_left);
const sTurret turret_player_llright     = TURRET_GRID(2, 0,  +8, -12, &ot_ally_laser_right);

void cb_player_die(hsObject obj) {

    obj->physical = &phy_player_dead;
    obj->ttl = 1;
}

const sObjType ot_player = {
    &phy_player_blue,
    OBJTYPE_NAT_ALLY_OBJ,
    100,
    NULL,
    cb_player_die,
    NULL,
    NULL
};

const sObjType ot_foe_bullet = {
    &phy_bullet_simple,
    OBJTYPE_NAT_FOE_BULLET,
    5,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_foe_laser = {
    &phy_alien_laser,
    OBJTYPE_NAT_FOE_BULLET,
    2,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_foe_ball = {
    &phy_alien_ball,
    OBJTYPE_NAT_FOE_BULLET,
    10,
    NULL,
    NULL,
    NULL,
    cb_deal_die
};

const sObjType ot_btn_hitme = {
    &phy_hitme,
    OBJTYPE_NAT_FOE_OBJ,
    1,
    NULL,
    NULL,
    NULL,
    NULL
};
