#include "data.h"
#include <stdlib.h>

// ---------------------------------------------------------------------------
// special bullets
// ---------------------------------------------------------------------------

static const sPhysical phy_letter_k = PHYSICAL_1X1('K', 0x74);
static const sPhysical phy_letter_i = PHYSICAL_1X1('I', 0x74);
static const sPhysical phy_letter_l = PHYSICAL_1X1('L', 0x74);

static const sObjType ot_letter_l = {
    &phy_letter_l,
    OBJTYPE_NAT_FOE_BULLET,
    25,
    NULL,
    NULL,
    NULL,
    &cb_deal_die
};

static const sObjType ot_letter_k = {
    &phy_letter_k,
    OBJTYPE_NAT_FOE_BULLET,
    25,
    NULL,
    NULL,
    NULL,
    &cb_deal_die
};

static const sObjType ot_letter_i = {
    &phy_letter_i,
    OBJTYPE_NAT_FOE_BULLET,
    25,
    NULL,
    NULL,
    NULL,
    &cb_deal_die
};

// ---------------------------------------------------------------------------
// images
// ---------------------------------------------------------------------------

static const sPhysical phy_alien_bible = {
    { 11, 5 },
    {
        '_',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,   ' ',    0x07,   ' ',    0x07,   ' ',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,
        'k',    0x78,   '@',    0x78,   'l',    0x78,   'l',    0x78,   ' ',    0x78,   '|',    0x78,   ' ',    0x78,   '#',    0x78,   'i',    0x78,   'l',    0x78,   'l',    0x78,   
        'a',    0x78,   'l',    0x78,   '%',    0x78,   ' ',    0x78,   ' ',    0x78,   '|',    0x78,   ' ',    0x78,   'a',    0x78,   'l',    0x78,   'l',    0x78,   ' ',    0x78,
        '*',    0x78,   'u',    0x78,   'm',    0x78,   'n',    0x78,   ' ',    0x78,   '|',    0x78,   ' ',    0x78,   'h',    0x78,   '%',    0x78,   'm',    0x78,   '.',    0x78,   
        '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   'U',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06
    }
};
static const sPhysical phy_alien_bible_dead = {
    { 11, 5 },
    {
        ' ',    0x07,   ' ',    0x07,   '_',    0x07,   '_',    0x07,   ' ',    0x07,   ' ',    0x07,   ' ',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,   ' ',    0x07,
        '#',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   ' ',    0x08,   '|',    0x08,   ' ',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   
        '#',    0x08,   '#',    0x08,   '#',    0x08,   ' ',    0x08,   ' ',    0x08,   '|',    0x08,   ' ',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   ' ',    0x08,
        '#',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   ' ',    0x08,   '|',    0x08,   ' ',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   '#',    0x08,   
        '-',    0x06,   '-',    0x06,   '-',    0x06,   '-',    0x06,   '-',    0x06,   'U',    0x06,   '-',    0x06,   '-',    0x06,   '-',    0x06,   '-',    0x06,   '-',    0x06
    }
};
static const sPhysical phy_alien_bible_hit = {
    { 11, 5 },
    {
        '_',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,   ' ',    0x07,   ' ',    0x07,   ' ',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,   '_',    0x07,
        'k',    0x74,   '@',    0x74,   'l',    0x74,   'l',    0x74,   ' ',    0x74,   '|',    0x78,   ' ',    0x74,   '#',    0x74,   'i',    0x74,   'l',    0x74,   'l',    0x74,   
        'a',    0x74,   'l',    0x74,   '%',    0x74,   ' ',    0x74,   ' ',    0x74,   '|',    0x78,   ' ',    0x74,   'a',    0x74,   'l',    0x74,   'l',    0x74,   ' ',    0x74,
        '*',    0x74,   'u',    0x74,   'm',    0x74,   'n',    0x74,   ' ',    0x74,   '|',    0x78,   ' ',    0x74,   'h',    0x74,   '%',    0x74,   'm',    0x74,   '.',    0x74,   
        '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '^',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06,   '=',    0x06
    }
};

static const sTurret turret_bible_0 = TURRET_GRID(1, 5, -2, 12, &ot_letter_k);
static const sTurret turret_bible_1 = TURRET_GRID(3, 5, -1, 12, &ot_letter_i);
static const sTurret turret_bible_2 = TURRET_GRID(7, 5, +1, 12, &ot_letter_l);
static const sTurret turret_bible_3 = TURRET_GRID(9, 5, +2, 12, &ot_letter_l);

// ---------------------------------------------------------------------------
// object types
// ---------------------------------------------------------------------------

void cb_bible_behave(hsObject obj) {

    obj->speed.y = adjust(obj->speed.y + 2 - (rand() % 5), -4, 4);
    obj->speed.x = adjust(obj->speed.x + 4 - (rand() % 9), -12, 12);

    if (obj->ttl > 20) {
    
        obj->speed.x /= 2;
        obj->speed.y /= 2;
    }
    
    if (!(obj->flags & OBJ_FLG_DYING)) {
    
        obj->physical = &phy_alien_bible;

        if (obj->ttl == 12)
            fire_turret(obj, &turret_bible_0);
        else
        if (obj->ttl == 9)
            fire_turret(obj, &turret_bible_1);
        else
        if (obj->ttl == 6)
            fire_turret(obj, &turret_bible_2);
        else
        if (obj->ttl == 3)
            fire_turret(obj, &turret_bible_3);
        else    
        if (obj->ttl == 0)
            obj->ttl = 32;
    }
}

static void cb_bible_hit(hsObject obj) {

    obj->physical = &phy_alien_bible_hit;
}

static void cb_bible_die(hsObject obj) {

    obj->physical = &phy_alien_bible_dead;
    obj->ttl = 10;
}

const sObjType ot_alien_bible = {
    &phy_alien_bible,
    OBJTYPE_NAT_FOE_OBJ,
    80,
    cb_bible_behave,
    cb_bible_die,
    cb_bible_hit,
    NULL
};
