#ifndef __DATA_H
#define __DATA_H

#include "objpool.h"

#define dimof(x) (sizeof(x) / sizeof(x[0]))

#define TURRET_NONE                     { { 0, 0 }, { 0, 0 }, NULL }
#define TURRETS_NONE                    { TURRET_NONE, TURRET_NONE, TURRET_NONE, TURRET_NONE }
#define TURRET_GRID(x, y, dx, dy, t)    { { grid2world(x), grid2world(y) }, { (dx), (dy) }, (t) }

hsObject fire_turret(hsObject obj, hcsTurret turret);
int adjust(int value, int min, int max);
void cb_deal_die(hsObject bullet, hsObject target);

extern const sTurret turret_player_bullet;
extern const sTurret turret_player_megablast;
extern const sTurret turret_player_lleft;
extern const sTurret turret_player_lright;
extern const sTurret turret_player_llleft;
extern const sTurret turret_player_llright;

extern const sObjType ot_player;
extern const sObjType ot_btn_hitme;
extern const sObjType ot_supply;
extern const sObjType ot_foe_bullet;
extern const sObjType ot_foe_laser;
extern const sObjType ot_foe_ball;
extern const sObjType ot_octo;
extern const sObjType ot_flank;
extern const sObjType ot_grchkrx_bee;
extern const sObjType ot_grchkrx_queen;
extern const sObjType ot_alien_bible;

#define OCTO_ST_CLW_GO_RIGHT            0
#define OCTO_ST_CLW_GO_DOWN             1
#define OCTO_ST_CLW_GO_LEFT             2
#define OCTO_ST_CLW_GO_UP               3

#define FLANK_GO_LEFT_DOWN              0
#define FLANK_GO_RIGHT_UP               1
#define FLANK_GO_RIGHT_DOWN             2
#define FLANK_GO_LEFT_UP                3

#define PHYSICAL_1X1(ascii, attr)       { { 1, 1 }, { (ascii), (attr) } }

#endif
