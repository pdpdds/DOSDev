#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include "files.h"


#define CAMPAIGN_MAGIC    690304
#define CAMPAIGN_VERSION  6


int ScanCampaign( const char *filename, char *title, int *missions )
{
  int f;
  int i;
  TCampaignSetting setting;

  f = open( filename, O_RDONLY | O_BINARY);
  if (f >= 0)
  {
    read( f, &i, sizeof( i));
    if (i != CAMPAIGN_MAGIC)
    {
      close( f);
      return CAMPAIGN_BADFILE;
    }

    read( f, &i, sizeof( i));
    if (i != CAMPAIGN_VERSION)
    {
      close( f);
      return CAMPAIGN_VERSIONMISMATCH;
    }

    read( f, setting.title, sizeof( setting.title));
    read( f, setting.author, sizeof( setting.author));
    read( f, setting.description, sizeof( setting.description));
    read( f, &setting.missionCount, sizeof( setting.missionCount));
    strcpy( title, setting.title);
    *missions = setting.missionCount;
    close( f);
    return CAMPAIGN_OK;
  }
  return CAMPAIGN_BADPATH;
}

int LoadCampaign( const char *filename, TCampaignSetting *setting, int max_missions, int max_characters )
{
  int f;
  int i;

  f = open( filename, O_RDONLY | O_BINARY);
  if (f >= 0)
  {
    read( f, &i, sizeof( i));
    if (i != CAMPAIGN_MAGIC)
    {
      close( f);
      return CAMPAIGN_BADFILE;
    }

    read( f, &i, sizeof( i));
    if (i != CAMPAIGN_VERSION)
    {
      close( f);
      return CAMPAIGN_VERSIONMISMATCH;
    }

    read( f, setting->title, sizeof( setting->title));
    read( f, setting->author, sizeof( setting->author));
    read( f, setting->description, sizeof( setting->description));

    read( f, &setting->missionCount, sizeof( setting->missionCount));

    if (max_missions <= 0)
    {
      i = setting->missionCount * sizeof( struct Mission);
      setting->missions = malloc( i);
      memset( setting->missions, 0, i);
      max_missions = setting->missionCount;
    }
    else if (setting->missionCount < max_missions)
      max_missions = setting->missionCount;

    for (i = 0; i < max_missions; i++)
    {
      read( f, &setting->missions[i], sizeof( struct Mission));
    }

    read( f, &setting->characterCount, sizeof( setting->characterCount));

    if (max_characters <= 0)
    {
      i = setting->characterCount * sizeof( TBadGuy);
      setting->characters = malloc( i);
      memset( setting->characters, 0, i);
      max_characters = setting->characterCount;
    }
    else if (setting->characterCount < max_characters)
      max_characters = setting->characterCount;

    for (i = 0; i < max_characters; i++)
    {
      read( f, &setting->characters[i], sizeof( TBadGuy));
    }
    close( f);
    return CAMPAIGN_OK;
  }
  return CAMPAIGN_BADPATH;
}

int SaveCampaign( const char *filename, TCampaignSetting *setting )
{
  int f;
  int i;

  f = open( filename,
            O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,
            S_IRUSR | S_IWUSR);
  if (f >= 0)
  {
    i = CAMPAIGN_MAGIC;
    write( f, &i, sizeof( i));

    i = CAMPAIGN_VERSION;
    write( f, &i, sizeof( i));

    write( f, setting->title, sizeof( setting->title));
    write( f, setting->author, sizeof( setting->author));
    write( f, setting->description, sizeof( setting->description));

    write( f, &setting->missionCount, sizeof( setting->missionCount));
    for (i = 0; i < setting->missionCount; i++)
    {
      write( f, &setting->missions[i], sizeof( struct Mission));
    }

    write( f, &setting->characterCount, sizeof( setting->characterCount));
    for (i = 0; i < setting->characterCount; i++)
    {
      write( f, &setting->characters[i], sizeof( TBadGuy));
    }
    close( f);
    return CAMPAIGN_OK;
  }
  return CAMPAIGN_BADPATH;
}

static OutputCString( FILE *f, const char *s, int indentLevel )
{
  int length;

  for (length = 0; length < indentLevel; length++)
    fputc( ' ', f);

  fputc( '\"', f);
  while (*s)
  {
    switch (*s)
    {
      case '\"':
      case '\\':
        fputc( '\\', f);
      default:
        fputc( *s, f);
    }
    s++;
    length++;
    if (length > 75)
    {
      fputs( "\"\n", f);
      length = 0;
      for (length = 0; length < indentLevel; length++)
        fputc( ' ', f);
      fputc( '\"', f);
    }
  }
  fputc( '\"', f);
}

void SaveCampaignAsC( const char *filename, const char *name, TCampaignSetting *setting )
{
  FILE *f;
  int i, j;

  f = fopen( filename, "w");
  if (f)
  {
    fprintf( f, "TBadGuy %s_badguys[ %d] =\n{\n", name, setting->characterCount);
    for (i = 0; i < setting->characterCount; i++)
    {
      fprintf( f, "  {%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,0x%x}%s\n",
               setting->characters[i].armedBodyPic,
               setting->characters[i].unarmedBodyPic,
               setting->characters[i].facePic,
               setting->characters[i].speed,
               setting->characters[i].probabilityToMove,
               setting->characters[i].probabilityToTrack,
               setting->characters[i].probabilityToShoot,
               setting->characters[i].actionDelay,
               setting->characters[i].gun,
               setting->characters[i].skinColor,
               setting->characters[i].armColor,
               setting->characters[i].bodyColor,
               setting->characters[i].legColor,
               setting->characters[i].hairColor,
               setting->characters[i].health,
               setting->characters[i].flags,
               i < setting->characterCount-1 ? "," : "");
    }
    fprintf( f, "};\n\n");

    fprintf( f, "struct Mission %s_missions[ %d] =\n{\n", name, setting->missionCount);
    for (i = 0; i < setting->missionCount; i++)
    {
      fprintf( f, "  {\n");
      OutputCString( f, setting->missions[i].title, 4);
      fprintf( f, ",\n");
      OutputCString( f, setting->missions[i].description, 4);
      fprintf( f, ",\n");
      fprintf( f, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n",
               setting->missions[i].wallStyle,
               setting->missions[i].floorStyle,
               setting->missions[i].roomStyle,
               setting->missions[i].exitStyle,
               setting->missions[i].keyStyle,
               setting->missions[i].doorStyle,
               setting->missions[i].mapWidth,
               setting->missions[i].mapHeight,
               setting->missions[i].wallCount,
               setting->missions[i].wallLength,
               setting->missions[i].roomCount,
               setting->missions[i].squareCount);
      fprintf( f, "    %d,%d,%d,%d,\n",
               setting->missions[i].exitLeft,
               setting->missions[i].exitTop,
               setting->missions[i].exitRight,
               setting->missions[i].exitBottom);
      fprintf( f, "    %d,\n", setting->missions[i].objectiveCount);
      fprintf( f, "    {\n");
      for (j = 0; j < OBJECTIVE_MAX; j++)
      {
        fprintf( f, "      {\n");
        OutputCString( f, setting->missions[i].objectives[j].description, 8);
        fprintf( f, ",\n");
        fprintf( f, "        %d,%d,%d,%d,0x%x\n",
                 setting->missions[i].objectives[j].type,
                 setting->missions[i].objectives[j].index,
                 setting->missions[i].objectives[j].count,
                 setting->missions[i].objectives[j].required,
                 setting->missions[i].objectives[j].flags);
        fprintf( f, "      }%s\n", j < OBJECTIVE_MAX-1 ? "," : "");
      }
      fprintf( f, "    },\n");

      fprintf( f, "    %d,\n", setting->missions[i].baddieCount);
      fprintf( f, "    {");
      for (j = 0; j < BADDIE_MAX; j++)
      {
        fprintf( f, "%d%s", setting->missions[i].baddies[j],
                            j < BADDIE_MAX-1 ? "," : "");
      }
      fprintf( f, "},\n");

      fprintf( f, "    %d,\n", setting->missions[i].specialCount);
      fprintf( f, "    {");
      for (j = 0; j < SPECIAL_MAX; j++)
      {
        fprintf( f, "%d%s", setting->missions[i].specials[j],
                            j < SPECIAL_MAX-1 ? "," : "");
      }
      fprintf( f, "},\n");

      fprintf( f, "    %d,\n", setting->missions[i].itemCount);
      fprintf( f, "    {");
      for (j = 0; j < ITEMS_MAX; j++)
      {
        fprintf( f, "%d%s", setting->missions[i].items[j],
                            j < ITEMS_MAX-1 ? "," : "");
      }
      fprintf( f, "},\n");

      fprintf( f, "    {");
      for (j = 0; j < ITEMS_MAX; j++)
      {
        fprintf( f, "%d%s", setting->missions[i].itemDensity[j],
                            j < ITEMS_MAX-1 ? "," : "");
      }
      fprintf( f, "},\n");

      fprintf( f, "    %d,0x%x,\n",
               setting->missions[i].baddieDensity,
               setting->missions[i].weaponSelection);
      OutputCString( f, setting->missions[i].song, 4);
      fprintf( f, ",\n");
      OutputCString( f, setting->missions[i].map, 4);
      fprintf( f, ",\n");
      fprintf( f, "    %d,%d,%d,%d\n",
               setting->missions[i].wallRange,
               setting->missions[i].floorRange,
               setting->missions[i].roomRange,
               setting->missions[i].altRange);
      fprintf( f, "  }%s\n", i < setting->missionCount ? "," : "");
    }
    fprintf( f, "};\n\n");

    fprintf( f, "struct CampaignSetting %s_campaign =\n{\n", name);
    OutputCString( f, setting->title, 2);
    fprintf( f, ",\n");
    OutputCString( f, setting->author, 2);
    fprintf( f, ",\n");
    OutputCString( f, setting->description, 2);
    fprintf( f, ",\n");
    fprintf( f, "  %d, %s_missions, %d, %s_badguys\n};\n",
             setting->missionCount, name,
             setting->characterCount, name);

    fclose( f);
  }
};

void AddFileEntry( struct FileEntry **list, const char *name, const char *info, int data )
{
  struct FileEntry *entry;

  while (*list && strcmp( (*list)->name, name) < 0)
    list = &(*list)->next;

  printf( "Adding [%s]\n", name);
  entry = malloc( sizeof( struct FileEntry));
  strcpy( entry->name, name);
  strcpy( entry->info, info);
  entry->data = data;
  entry->next = *list;
  *list = entry;
}

struct FileEntry *GetFilesFromDirectory( const char *directory )
{
  DIR *dir;
  struct dirent *d;
  struct FileEntry *list = NULL;

  dir = opendir( directory);
  if (dir != NULL)
  {
    while ((d = readdir( dir)) != NULL)
      AddFileEntry( &list, d->d_name, "", 0);
    closedir( dir);
  }
  return list;
}

void FreeFileEntries( struct FileEntry *entries )
{
  struct FileEntry *tmp;

  while (entries)
  {
    tmp = entries;
    entries = entries->next;
    free( tmp);
  }
}

void GetCampaignTitles( struct FileEntry **entries )
{
  int i;
  struct FileEntry *tmp;
  char s[10];

  while (*entries)
  {
    if (ScanCampaign( (*entries)->name, &(*entries)->info, &i) == CAMPAIGN_OK)
    {
      sprintf( s, " (%d)", i);
      strcat( (*entries)->info, s);
      entries = &((*entries)->next);
    }
    else // Bad campaign
    {
      tmp = *entries;
      *entries = tmp->next;
      free( tmp);
    }
  }
}

