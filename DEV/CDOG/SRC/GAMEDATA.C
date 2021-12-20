#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gamedata.h"
#include "actors.h"
#include "defs.h"
#include "keyboard.h"
#include "input.h"


struct PlayerData gPlayer1Data =
{
  "Player 1", 0, SHADE_BLUE, SHADE_BLUE, SHADE_BLUE, 0, 0,
  3, { GUN_SHOTGUN, GUN_MG, GUN_FRAGGRENADE },
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  KEYBOARD,
  { keyArrowLeft, keyArrowRight, keyArrowUp, keyArrowDown, keyLeftCtrl, keyEnter }
};

struct PlayerData gPlayer2Data =
{
  "Player 2", 1, SHADE_RED, SHADE_RED, SHADE_RED, 2, 0,
  3, { GUN_POWERGUN, GUN_FLAMER, GUN_GRENADE },
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  KEYBOARD,
  { keyKeypad4, keyKeypad6, keyKeypad8, keyKeypad2, keyKeypad0, keyKeypadEnter }
};

struct GameOptions gOptions =
{
  0,
  1,
  0, 0,
  0,
  0,
  0,
  0,
  COPY_REPMOVSD,
  0,
  0,
  0, 0,
  keyTab,
  100,
  100,
  100,
  0
};

struct CampaignOptions gCampaign =
{
  NULL, 0, 0
};

struct MissionOptions gMission;


struct SongDef *gGameSongs = NULL;
struct SongDef *gMenuSongs = NULL;


void AddSong( struct SongDef **songList, const char *path )
{
  struct SongDef *s;

  s = malloc( sizeof( struct SongDef));
  strcpy( s->path, path);
  s->next = *songList;
  *songList = s;
}

void ShiftSongs( struct SongDef **songList )
{
  struct SongDef **h;

  if (!*songList || !(*songList)->next)
    return;

  h = songList;
  while (*h)
    h = &(*h)->next;

  *h = *songList;
  *songList = (*songList)->next;
  (*h)->next = NULL;
}

void FreeSongs( struct SongDef **songList )
{
  struct SongDef *s;

  while (*songList)
  {
    s = *songList;
    *songList = s->next;
    free( s);
  }
}

void LoadSongs( const char *path, struct SongDef **songList )
{
  FILE *f;
  char s[100], *p;

  f = fopen( path, "r");
  if (f)
  {
    while (fgets( s, sizeof( s), f))
    {
      p = s + strlen( s);
      while (p >= s && !isgraph( *p))
        *p-- = 0;
      if (s[0])
        AddSong( songList, s);
    }
    fclose( f);
  }
}
