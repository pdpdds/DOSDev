#ifndef	__GAMECFG__
#define	__GAMECFG__

#define STR_Intro1 0
#define STR_Intro2 1
#define STR_Intro3 2
#define STR_Intro4 3
#define STR_Intro5 4
#define STR_Intro6 5
#define STR_Intro7 6
#define STR_DoHelpText1 7
#define STR_DoHelpText2 8
#define STR_DoHelpText3 9
#define STR_DoHelpText4 10
#define STR_DoHelpText5 11
#define STR_DoHelpText6 12
#define STR_DoHelpText7 13
#define STR_DoHelpText8 14
#define STR_DoHelpText9 15
#define STR_DoHelpText10 16
#define STR_DoHelpText11 17
#define STR_DebugMemory1 18
#define STR_DebugMemory2 19
#define STR_DebugMemory3 20
#define STR_DebugMemory4 21
#define STR_DebugMemory5 22
#define STR_DebugMemory6 23
#define STR_DebugMemory7 24
#define STR_DebugKeys1 25
#define STR_DebugKeys2 26
#define STR_DebugKeys3 27
#define STR_CheckKeys1 28
#define STR_CheckKeys2 29
#define STR_CheckKeys3 30
#define STR_main1 31
#define STR_main2 32
#define STR_main3 33
#define STR_GameOver1 34
#define STR_GameOver2 35
#define STR_GameOver3 36
#define STR_GameOver4 37
#define STR_GameOver5 38
#define STR_GameOver6 39
#define STR_GameOver7 40
#define STR_Victory1 41
#define STR_Victory2 42
#define STR_Victory3 43
#define STR_Victory4 44
#define STR_Victory5 45
#define STR_Victory6 46
#define STR_Victory7 47
#define STR_Victory8 48
#define STR_Victory9 49
#define STR_Victory10 50
#define STR_Victory11 51
#define STR_Victory12 52
#define STR_Victory13 53
#define STR_BaseScreen1 54
#define STR_BaseScreen2 55
#define STR_BaseScreen3 56
#define STR_BaseScreen4 57
#define STR_BaseScreen5 58
#define STR_BaseScreen6 59
#define STR_BaseScreen7 60
#define STR_BaseScreen8 61
#define STR_PlayGame1 62
#define STR_PlayGame2 63
#define STR_PlayGame3 64
#define STR_CalibrateJoy1 65
#define STR_CalibrateJoy2 66
#define STR_CalibrateJoy3 67
#define STR_CalibrateJoy4 68
#define STR_CalibrateJoy5 69
#define STR_CalibrateJoy6 70
#define STR_calibratekeys1 71
#define STR_calibratekeys2 72
#define STR_calibratekeys3 73
#define STR_calibratekeys4 74
#define STR_calibratekeys5 75
#define STR_calibratekeys6 76
#define STR_calibratekeys7 77
#define STR_calibratekeys8 78
#define STR_calibratekeys9 79
#define STR_calibratekeys10 80

// as: Enemy stats
#define STR_BaseScreen9 81
#define STR_BaseScreen10 82

// The base index for levnames
#define STR_levnames0 83
#define STR_levtext0 103

#define GAME_CONFIG_IDENTIFIERS (STR_levnames0 + 20 + 20)

extern char* gameStrings[GAME_CONFIG_IDENTIFIERS];

void GameConfigLoad(void);

#endif
