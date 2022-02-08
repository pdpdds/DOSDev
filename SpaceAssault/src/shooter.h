#ifndef __SHOOTER_H
#define __SHOOTER_H

// ---------------------------------------------------------------------------
// coordinate systems
// ---------------------------------------------------------------------------

// playground size
#define VIEWGRID_WIDTH                  60
#define VIEWGRID_HEIGHT                 48
#define VIEWGRID_DIVIDER                44

// position tests
#define OBJ_IS_RIGHT(obj)               ((world2grid((obj)->pos.x) + (obj)->physical->dim.x) >= VIEWGRID_WIDTH)
#define OBJ_IS_LEFT(obj)                (world2grid((obj)->pos.x) == 0)
#define OBJ_IS_DOWN(obj)                ((world2grid((obj)->pos.y) + (obj)->physical->dim.y) >= VIEWGRID_DIVIDER)

#define OBJ_POS_LEFT(obj)               (0)
#define OBJ_POS_RIGHT(obj)              (grid2world(VIEWGRID_WIDTH - (obj)->physical->dim.x))

// position coordinate in the grid (screen characters) and world (precise with fractions)
typedef unsigned char       t_pos_grid;
typedef unsigned char       t_delta_grid;
typedef unsigned int        t_pos_world;
typedef signed int          t_delta_world;

// conversions between grid and world coordinates
#define GRID_RATIO          32
#define grid2world(x)       ((x) * GRID_RATIO)
#define world2grid(x)       ((x) / GRID_RATIO)

// point in grid coordinates
typedef struct sPosGrid {

    t_pos_grid              x;
    t_pos_grid              y;

} sPosGrid, *hsPosGrid;
typedef const sPosGrid *hcsPosGrid;

// vector in grid coordinates
typedef struct sVecGrid {

    t_delta_grid            x;
    t_delta_grid            y;

} sVecGrid, *hsVecGrid;
typedef const sVecGrid *hcsVecGrid;

// point in world coordinates
typedef struct sPosWorld {

    t_pos_world             x;
    t_pos_world             y;

} sPosWorld, *hsPosWorld;
typedef const sPosWorld *hcsPosWorld;

// vector in world coordinates
typedef struct sVecWorld {

    t_delta_world           x;
    t_delta_world           y;

} sVecWorld, *hsVecWorld;
typedef const sVecWorld *hcsVecWorld;

// ---------------------------------------------------------------------------
// static data
// ---------------------------------------------------------------------------

// needed forward declarations
struct sObjType; 
typedef const struct sObjType *hcsObjType;

struct sObject;
typedef struct sObject *hsObject;

struct sScenarioPoint;
typedef const struct sScenarioPoint *hcsScenarioPoint;

// physical properties of object (visual, damage)
typedef struct sPhysical {

    sVecGrid                dim;            // object dimensions
    char                    image[128];     // image bitmap (char, color, char, color...)

} sPhysical, *hsPhysical;
typedef const sPhysical *hcsPhysical;

// object's "turrets"
typedef struct sTurret {

    sVecWorld               pos;
    sVecWorld               speed;
    hcsObjType              type;

} sTurret, *hsTurret;
typedef const sTurret *hcsTurret;

// object "natures"
#define OBJTYPE_NATMASK_ALLY                0x01
#define OBJTYPE_NATMASK_FOE                 0x02
#define OBJTYPE_NATMASK_BULLET              0x04
#define OBJTYPE_NATMASK_OBJ                 0x08

#define OBJTYPE_NAT_NEUTRAL                 0x00
#define OBJTYPE_NAT_ALLY_OBJ                (OBJTYPE_NATMASK_ALLY | OBJTYPE_NATMASK_OBJ)
#define OBJTYPE_NAT_ALLY_BULLET             (OBJTYPE_NATMASK_ALLY | OBJTYPE_NATMASK_BULLET)
#define OBJTYPE_NAT_FOE_OBJ                 (OBJTYPE_NATMASK_FOE | OBJTYPE_NATMASK_OBJ)
#define OBJTYPE_NAT_FOE_BULLET              (OBJTYPE_NATMASK_FOE | OBJTYPE_NATMASK_BULLET)

// callback to implement object's tick behaviour
typedef void cbObject(hsObject obj);
typedef void cbObjectObject(hsObject obj1, hsObject obj2);

// objects classes
typedef struct sObjType {

    hcsPhysical             physical;       // default physical properties (visual, damage)
    char                    nature;         // one of OBJTYPE_NAT_*
    unsigned int            hp;             // total hitpoints / bullet damage

    cbObject                *cb_behave;     // callback for object's daily routine
    cbObject                *cb_die;        // gets called at the moment object's damage exceeds it's HP
    cbObject                *cb_hit;        // called whenever the object is hit
    cbObjectObject          *cb_deal;       // called whenever bullet hits target

} sObjType, *hsObjType;
typedef const sObjType *hcsObjType;

// text to display
typedef struct sDisplayText {

    sPosGrid                pos;
    char                    color;
    const char              *text;

} sDisplayText, *hsDisplayText;
typedef const sDisplayText *hcsDisplayText;

// scenario event point types
#define SCPOINT_TYPE_NONE               1
#define SCPOINT_TYPE_WAIT_DEAD          2   // wait until all objects are dead
#define SCPOINT_TYPE_END                3   // total end

// callback to implement scenarion point's tick behaviour
typedef void cbScenarioPoint(hcsScenarioPoint scpoint, unsigned char index);

// scenario sequence event
typedef struct sScenarioPoint {

    char                    type;           // SCPOINT_TYPE_*
    unsigned char           delay;          // delay after performing the point in 1/4 secs
    unsigned char           repeat;         // how many times the point goes around (after delay)

    cbScenarioPoint         *cb_tick;       // called once every "delay", total 1 + "repeat" times
    
    hcsDisplayText          ds_text;        // start of texts to display
    unsigned char           ds_count;       // number of texts to display
    
    hcsObjType              emit_objtype;   // object type to be emitted
    sPosWorld               emit_pos;       // position of emmited object

} sScenarioPoint, *hsScenarioPoint;

// ---------------------------------------------------------------------------
// dynamic data
// ---------------------------------------------------------------------------

#define OBJ_FLG_DESTROY     0x01            // mark object to be garbage collected
#define OBJ_FLG_INACTIVE    0x02            // object was just added and is not yet to be considered
#define OBJ_FLG_DYING       0x04            // object already dead, just in death animation

// game object instance
typedef struct sObject {

    hcsObjType              type;
    char                    flags;          // OBJ_FLG_*
    hcsPhysical             physical;       // current physical properties
    
    sPosWorld               pos;            // current position
    sVecWorld               speed;          // current speed vector, added to position every tick
    
    unsigned char           damage_now;     // total damage taken in last tick
    unsigned int            damage_total;   // total damage taken
    
    unsigned int            ttl;            // remaining ticks until action end (for certain actions)
    char                    state[4];
    
} sObject, *hsObject;

#endif
    