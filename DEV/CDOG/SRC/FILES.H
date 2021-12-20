#include "gamedata.h"


#define CAMPAIGN_OK                0
#define CAMPAIGN_BADFILE          -1
#define CAMPAIGN_VERSIONMISMATCH  -2
#define CAMPAIGN_BADPATH          -3


struct FileEntry
{
  char name[13];
  char info[80];
  int  data;
  struct FileEntry *next;
};

int  LoadCampaign( const char *filename, TCampaignSetting *setting,
                   int max_missions, int max_characters );
int  SaveCampaign( const char *filename, TCampaignSetting *setting );
void SaveCampaignAsC( const char *filename, const char *name, TCampaignSetting *setting );

void AddFileEntry( struct FileEntry **list, const char *name, const char *info, int data );
struct FileEntry *GetFilesFromDirectory( const char *directory );
void FreeFileEntries( struct FileEntry *entries );
void GetCampaignTitles( struct FileEntry **entries );

