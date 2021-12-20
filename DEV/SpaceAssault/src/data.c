#include <stdlib.h>

#include "shooter.h"
#include "objpool.h"
#include "data.h"

// ---------------------------------------------------------------------------
// scenario callbacks
// ---------------------------------------------------------------------------

#pragma argsused
void scpt_emit_at_pos(hcsScenarioPoint scpoint, unsigned char repeat) {

    hsObject emit;
    t_pos_world x;

    if ((emit = objpool_alloc(scpoint->emit_objtype)) == NULL)
        return;
        
    emit->pos.x = scpoint->emit_pos.x;
    emit->pos.y = scpoint->emit_pos.y;
    
    x = grid2world(VIEWGRID_WIDTH - emit->physical->dim.x);
    if (emit->pos.x > x)
        emit->pos.x = x;
}

#pragma argsused
void scpt_emit_random_x(hcsScenarioPoint scpoint, unsigned char repeat) {

    hsObject emit;

    if ((emit = objpool_alloc(scpoint->emit_objtype)) == NULL)
        return;
        
    emit->pos.x = grid2world(rand() % (VIEWGRID_WIDTH - emit->physical->dim.x));
    emit->pos.y = scpoint->emit_pos.y;
}

#pragma argsused
void scpt_emit_octo(hcsScenarioPoint scpoint, unsigned char repeat) {

    hsObject obj;

    if ((obj = objpool_alloc(&ot_octo)) == NULL)
        return;
    
    obj->pos.x = scpoint->emit_pos.x;
    obj->pos.y = scpoint->emit_pos.y;
    
    octo_init(obj, OCTO_ST_CLW_GO_RIGHT);
}

#pragma argsused
void scpt_emit_flank_left(hcsScenarioPoint scpoint, unsigned char repeat) {

    hsObject obj;

    if ((obj = objpool_alloc(&ot_flank)) == NULL)
        return;
    
    obj->pos.x = scpoint->emit_pos.x;
    obj->pos.y = scpoint->emit_pos.y;
    
    flank_init(obj, FLANK_GO_LEFT_DOWN);
}

#pragma argsused
void scpt_emit_flank_right(hcsScenarioPoint scpoint, unsigned char repeat) {

    hsObject obj;

    if ((obj = objpool_alloc(&ot_flank)) == NULL)
        return;
    
    obj->pos.x = scpoint->emit_pos.x;
    obj->pos.y = scpoint->emit_pos.y;
    
    flank_init(obj, FLANK_GO_RIGHT_DOWN);
}

// ---------------------------------------------------------------------------
// scenario
// ---------------------------------------------------------------------------

static const sDisplayText text_intro_0[] = {
    { { 26, 10 }, 0x07, "ROMAN HOCKE" },
    { { 22, 12 }, 0x07, "presentes  (c) 2017" },
    { { 6, 16 },  0x04, "   __  __  __  __  __     __  __  __  __         __ " },
    { { 6, 17 },  0x0c, "  /_  /_/ /_/ /   /_     /_/ /_  /_  /_/ / / /   /  " },
    { { 6, 18 },  0x08, " __/ /   / / /_  /_     / / __/ __/ / / /_/ /_  /   " },
};
static const sDisplayText text_intro_1[] = {
    { { 28, 10 }, 0x0f, "CONGRATS!" }, 
    { { 21, 13 }, 0x07, "You solved the puzzle!" }, 
    { { 26, 17 }, 0x07, "NOW SOLVE THIS" },
    { { 29, 10 }, 0x07, "Clever!" }
};
static const sDisplayText text_bible_boss[] = {
    { { 28, 10 }, 0x07, "now face" },
    { { 24, 13 }, 0x0f, "THE ALIEN BIBLE" },
    { { 28, 10 }, 0x07, "now face" },
    { { 24, 13 }, 0x0f, "THE ALIEN BIBLE" },
    { { 29, 16 }, 0x0f, "VOL. 2" },
    { { 2, 25 },  0x07, "with glossary..." },
    { { 38, 25 }, 0x07, "... and appendix" }
};
static const sDisplayText text_final_showdown[] = {
    { { 28, 10 }, 0x07, "Beware!" },
    { { 19, 13 }, 0x07, "Something HUGE is coming!" },
};
static const sDisplayText text_outro[] = {
    { { 15, 10 }, 0x07, "This is the end, brave HERO!" },
    { { 19, 13 }, 0x07, "The space is safe now." },
    { { 20, 19 }, 0x07, "...but for how long?" },
    
    { { 20, 10 }, 0x07, "Buy the DLC and see!" },
    { { 16, 13 }, 0x07, "Just kidding, fuck DLCs :-)" },
    
    { { 16, 10 }, 0x07, "Author will work hard..."},
    { { 12, 13 }, 0x07, "... to bring You more enemies ..."},
    { { 16, 16 }, 0x07, "... deadlier weapons ..."},
    { { 16, 19 }, 0x07, "... in an EPIC scenario!"},
    
    { { 24, 10 }, 0x07, "All for FREE" },
    { { 23, 13 }, 0x07, "On dosheaven.eu" },
    
    { { 27, 16 }, 0x07, "THE END" }
};

#define SCPOINT_SUB_TEXT(t, c)      (t), (c)
#define SCPOINT_SUB_TEXT_NONE       NULL, 0

//                                          type,                       delay,  repeat, tick,   text,                   emit,   pos
#define SCPOINT_MACRO_TEXT(t, c, d)         { SCPOINT_TYPE_NONE,        (d),    0,      NULL,   SCPOINT_SUB_TEXT(t, c), NULL,   { 0, 0 } }
#define SCPOINT_MACRO_DELAY(d)              { SCPOINT_TYPE_NONE,        (d),    0,      NULL,   SCPOINT_SUB_TEXT_NONE,  NULL,   { 0, 0 } }
#define SCPOINT_MACRO_WAIT_DEAD(d)          { SCPOINT_TYPE_WAIT_DEAD,   (d),    1,      NULL,   SCPOINT_SUB_TEXT_NONE,  NULL,   { 0, 0 } }
#define SCPOINT_MACRO_WAIT_DEAD_TEXT(t, c)  { SCPOINT_TYPE_WAIT_DEAD,   1,      1,      NULL,   SCPOINT_SUB_TEXT(t, c), NULL,   { 0, 0 } }
#define SCPOINT_MACRO_END                   { SCPOINT_TYPE_END,         1,      1,      NULL,   SCPOINT_SUB_TEXT_NONE,  NULL,   { 0, 0 } }

#define SCPOINT_EPOS(x, y)                  { grid2world(x), grid2world(y) }
#define SCPOINT_EPOS_LEFT(y)                { 0, grid2world(y) }
#define SCPOINT_EPOS_RIGHT(y)               { -1, grid2world(y) }

const sScenarioPoint g_scenario[] = {

    // intro screen
    SCPOINT_MACRO_DELAY(1),
    SCPOINT_MACRO_TEXT(text_intro_0, 2, 5),
    SCPOINT_MACRO_TEXT(text_intro_0, 5, 1),
    SCPOINT_MACRO_TEXT(text_intro_0, 2, 1),
    SCPOINT_MACRO_TEXT(text_intro_0, 5, 1),
    SCPOINT_MACRO_TEXT(text_intro_0, 2, 1),
    SCPOINT_MACRO_TEXT(text_intro_0, 5, 5),

    // wait for "hitme" under intro text
    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_intro_0, 5), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_intro_0, 5),
    
    // puzzle congrats
    SCPOINT_MACRO_TEXT(text_intro_1, 2, 8),
    SCPOINT_MACRO_DELAY(3),
  
    // wave of grchhrx bees...
    { SCPOINT_TYPE_NONE, 3, 2, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_intro_1 + 2, 1), &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(10) },
    SCPOINT_MACRO_WAIT_DEAD(3),
    SCPOINT_MACRO_TEXT(&text_intro_1[3], 1, 10),

    // wave of octos
    { SCPOINT_TYPE_NONE, 5, 2, scpt_emit_octo, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 10) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // wave of grchhrx bees...
    { SCPOINT_TYPE_NONE, 3, 6, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_RIGHT(15) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // wave of octos
    { SCPOINT_TYPE_NONE, 5, 12, scpt_emit_octo, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 10) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // grchkrx queen introduces...
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // wave of grchhrx bees...
    { SCPOINT_TYPE_NONE, 3, 6, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_RIGHT(15) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // waves of flankers
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_left, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_right, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // some healery
    { SCPOINT_TYPE_NONE, 5, 0, scpt_emit_random_x, SCPOINT_SUB_TEXT_NONE, &ot_supply, SCPOINT_EPOS(10, 1) },
    SCPOINT_MACRO_DELAY(25),

    // wave of grchhrx bees...
    { SCPOINT_TYPE_NONE, 3, 2, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(10) },
    SCPOINT_MACRO_DELAY(15),
    { SCPOINT_TYPE_NONE, 2, 4, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(10) },
    SCPOINT_MACRO_DELAY(15),
    { SCPOINT_TYPE_NONE, 2, 4, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(15) },
    SCPOINT_MACRO_DELAY(15),
    { SCPOINT_TYPE_NONE, 2, 4, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(20) },
    SCPOINT_MACRO_DELAY(15),
    { SCPOINT_TYPE_NONE, 2, 4, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(25) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // the alien bible boss
    SCPOINT_MACRO_DELAY(1),
    SCPOINT_MACRO_TEXT(text_bible_boss, 1, 5),
    SCPOINT_MACRO_TEXT(text_bible_boss, 2, 5),
    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_alien_bible, SCPOINT_EPOS(23, 10) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // wave of octos
    { SCPOINT_TYPE_NONE, 5, 12, scpt_emit_octo, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 10) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // queen guarded by bees
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 0) },
    SCPOINT_MACRO_DELAY(3),
    { SCPOINT_TYPE_NONE, 1, 9, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_RIGHT(15) },
    SCPOINT_MACRO_DELAY(9),
    { SCPOINT_TYPE_NONE, 1, 9, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(25) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // waves of flankers
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_right, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_left, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_right, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // queens trio
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 0) },
    SCPOINT_MACRO_DELAY(5),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(6, 0) },
    SCPOINT_MACRO_DELAY(5),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(46, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    { SCPOINT_TYPE_NONE, 3, 2, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_RIGHT(25) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // some healery
    { SCPOINT_TYPE_NONE, 5, 0, scpt_emit_random_x, SCPOINT_SUB_TEXT_NONE, &ot_supply, SCPOINT_EPOS(10, 1) },
    SCPOINT_MACRO_DELAY(25),
    
    // alien bible boss vol. 2
    SCPOINT_MACRO_TEXT(text_bible_boss + 2, 1, 5),
    SCPOINT_MACRO_TEXT(text_bible_boss + 2, 2, 5),
    SCPOINT_MACRO_TEXT(text_bible_boss + 2, 3, 5),
    { SCPOINT_TYPE_NONE, 8, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_bible_boss + 2, 3), &ot_alien_bible, SCPOINT_EPOS(23, 20) },
    SCPOINT_MACRO_TEXT(text_bible_boss + 2, 4, 0),
    { SCPOINT_TYPE_NONE, 8, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_bible_boss + 5, 1), &ot_alien_bible, SCPOINT_EPOS(8, 12) },
    { SCPOINT_TYPE_NONE, 8, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_bible_boss + 5, 2), &ot_alien_bible, SCPOINT_EPOS(38, 12) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // mixed wave of bees and octos
    SCPOINT_MACRO_DELAY(5),
    { SCPOINT_TYPE_NONE, 3, 8, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_bee, SCPOINT_EPOS_LEFT(15) },
    SCPOINT_MACRO_DELAY(15),
    { SCPOINT_TYPE_NONE, 2, 12, scpt_emit_octo, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 20) },
    SCPOINT_MACRO_DELAY(25),
    SCPOINT_MACRO_WAIT_DEAD(3),

    // queen guarded by flanks
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 0) },
    SCPOINT_MACRO_DELAY(3),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_right, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_left, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // some healery
    { SCPOINT_TYPE_NONE, 5, 0, scpt_emit_random_x, SCPOINT_SUB_TEXT_NONE, &ot_supply, SCPOINT_EPOS(10, 1) },
    SCPOINT_MACRO_DELAY(25),

    // final showdown    
    SCPOINT_MACRO_TEXT(text_final_showdown, 1, 5),
    SCPOINT_MACRO_TEXT(text_final_showdown, 2, 5),
    
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(26, 10) },
    SCPOINT_MACRO_DELAY(2),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(6, 15) },
    SCPOINT_MACRO_DELAY(2),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_grchkrx_queen, SCPOINT_EPOS(46, 10) },

    SCPOINT_MACRO_DELAY(15),
    
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_alien_bible, SCPOINT_EPOS(26, 20) },
    SCPOINT_MACRO_DELAY(2),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_alien_bible, SCPOINT_EPOS(6, 25) },
    SCPOINT_MACRO_DELAY(2),
    { SCPOINT_TYPE_NONE, 0, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT_NONE, &ot_alien_bible, SCPOINT_EPOS(46, 20) },

    SCPOINT_MACRO_DELAY(60),
    
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_right, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_DELAY(25),
    { SCPOINT_TYPE_NONE, 1, 2, scpt_emit_flank_left, SCPOINT_SUB_TEXT_NONE, NULL, SCPOINT_EPOS(0, 0) },
    SCPOINT_MACRO_WAIT_DEAD(3),

    // the outro
    SCPOINT_MACRO_DELAY(5),
    SCPOINT_MACRO_TEXT(text_outro, 1, 7),
    SCPOINT_MACRO_TEXT(text_outro, 2, 7),
    SCPOINT_MACRO_TEXT(text_outro, 3, 7),

    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_outro, 3), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_outro, 3),
    
    SCPOINT_MACRO_TEXT(text_outro + 3, 1, 7),

    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_outro + 3, 1), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_outro + 3, 1),

    SCPOINT_MACRO_TEXT(text_outro + 3, 2, 7),

    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_outro + 3, 2), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_outro + 3, 2),

    SCPOINT_MACRO_TEXT(text_outro + 5, 1, 7),
    SCPOINT_MACRO_TEXT(text_outro + 5, 2, 7),
    SCPOINT_MACRO_TEXT(text_outro + 5, 3, 7),
    SCPOINT_MACRO_TEXT(text_outro + 5, 4, 7),

    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_outro + 5, 4), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_outro + 5, 4),

    SCPOINT_MACRO_TEXT(text_outro + 9, 1, 7),
    SCPOINT_MACRO_TEXT(text_outro + 9, 2, 7),

    { SCPOINT_TYPE_NONE, 3, 0, scpt_emit_at_pos, SCPOINT_SUB_TEXT(text_outro + 9, 2), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
    SCPOINT_MACRO_WAIT_DEAD_TEXT(text_outro + 9, 2),

    // the end
    { SCPOINT_TYPE_END, 1, 1, NULL, SCPOINT_SUB_TEXT(text_outro + 11, 1), &ot_btn_hitme, SCPOINT_EPOS(26, 26) },
};
