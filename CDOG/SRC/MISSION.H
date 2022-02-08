#define TileItemIsObjective(f)   (((f) & TILEITEM_OBJECTIVE) != 0)
#define ObjectiveFromTileItem(f) ((((f) & TILEITEM_OBJECTIVE) >> OBJECTIVE_SHIFT)-1)
#define ObjectiveToTileItem(o)   (((o)+1) << OBJECTIVE_SHIFT)

struct EditorInfo
{
  int itemCount;
  int pickupCount;
  int keyCount;
  int doorCount;
  int exitCount;
  int rangeCount;
};

int  SetupBuiltinCampaign( int index );
int  SetupBuiltinDogfight( int index );
void ResetCampaign( void );
void SetupMission( int index, int buildTables );
int  CheckMissionObjective( int flags );
int  MissionCompleted( void );


// Intended for use with the editor only

void SetupMissionCharacter( int index, const TBadGuy *b );
void GetEditorInfo( struct EditorInfo *info );
const char *RangeName( int index );

