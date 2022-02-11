#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "GAMECFG.H"

extern int levmin[20];
extern int levsec[20];
extern int cyclecolors[3];

char* gameStrings[GAME_CONFIG_IDENTIFIERS];

// Identifiers for externalised game data.
static const char* gameConfigIdentifiers[GAME_CONFIG_IDENTIFIERS] =
{
    "Intro1",
    "Intro2",
    "Intro3",
    "Intro4",
    "Intro5",
    "Intro6",
    "Intro7",

    "DoHelpText1",
    "DoHelpText2",
    "DoHelpText3",
    "DoHelpText4",
    "DoHelpText5",
    "DoHelpText6",
    "DoHelpText7",
    "DoHelpText8",
    "DoHelpText9",
    "DoHelpText10",
    "DoHelpText11",

    "DebugMemory1",
    "DebugMemory2",
    "DebugMemory3",
    "DebugMemory4",
    "DebugMemory5",
    "DebugMemory6",
    "DebugMemory7",

    "DebugKeys1",
    "DebugKeys2",
    "DebugKeys3",

    "CheckKeys1",
    "CheckKeys2",
    "CheckKeys3",

    "main1",
    "main2",
    "main3",

    "GameOver1",
    "GameOver2",
    "GameOver3",
    "GameOver4",
    "GameOver5",
    "GameOver6",
    "GameOver7",

    "Victory1",
    "Victory2",
    "Victory3",
    "Victory4",
    "Victory5",
    "Victory6",
    "Victory7",
    "Victory8",
    "Victory9",
    "Victory10",
    "Victory11",
    "Victory12",
    "Victory13",

    "BaseScreen1",
    "BaseScreen2",
    "BaseScreen3",
    "BaseScreen4",
    "BaseScreen5",
    "BaseScreen6",
    "BaseScreen7",
    "BaseScreen8",

    "PlayGame1",
    "PlayGame2",
    "PlayGame3",

    "CalibrateJoy1",
    "CalibrateJoy2",
    "CalibrateJoy3",
    "CalibrateJoy4",
    "CalibrateJoy5",
    "CalibrateJoy6",

    "calibratekeys1",
    "calibratekeys2",
    "calibratekeys3",
    "calibratekeys4",
    "calibratekeys5",
    "calibratekeys6",
    "calibratekeys7",
    "calibratekeys8",
    "calibratekeys9",
    "calibratekeys10",

    // as: Enemy stats
    "BaseScreen9",
    "BaseScreen10",

    "levnames0",
    "levnames1",
    "levnames2",
    "levnames3",
    "levnames4",
    "levnames5",
    "levnames6",
    "levnames7",
    "levnames8",
    "levnames9",
    "levnames10",
    "levnames11",
    "levnames12",
    "levnames13",
    "levnames14",
    "levnames15",
    "levnames16",
    "levnames17",
    "levnames18",
    "levnames19",

    "levtext0",
    "levtext1",
    "levtext2",
    "levtext3",
    "levtext4",
    "levtext5",
    "levtext6",
    "levtext7",
    "levtext8",
    "levtext9",
    "levtext10",
    "levtext11",
    "levtext12",
    "levtext13",
    "levtext14",
    "levtext15",
    "levtext16",
    "levtext17",
    "levtext18",
    "levtext19",
};

#define CONFIG_BUFFER_LENGTH (128)
#define CONFIG_LINE_LENGTH (80)
#define CONFIG_STRING_LENGTH (512)

static struct
{
    int handle;

    char *pBuffer;
    int bufferIndex;
    int bufferCount;

    char *pLine;
    int chrIndex;
    int chrCount;

    int lineNumber;

    int integer;
    char *pString;

} gameConfig;

static int ConfigBuffer(void)
{
    gameConfig.bufferIndex = 0;
    gameConfig.bufferCount = read(gameConfig.handle, gameConfig.pBuffer, CONFIG_BUFFER_LENGTH);
    return gameConfig.bufferCount;
}

static int ConfigReadLine(void)
{
    char chr;
    int result;

    gameConfig.chrIndex = 0;
    gameConfig.chrCount = 0;

    do
    {
        while(gameConfig.bufferIndex < gameConfig.bufferCount)
        {
            if((chr = gameConfig.pBuffer[gameConfig.bufferIndex++]) == '\n')
            {
                gameConfig.pLine[gameConfig.chrCount] = '\0';
                
                gameConfig.lineNumber++;

                return gameConfig.chrCount;
            }

            if(gameConfig.chrCount >= CONFIG_LINE_LENGTH)
            {
                printf("Line %d too long!\n", gameConfig.lineNumber);
                return -1; // Line too long
            }

            gameConfig.pLine[gameConfig.chrCount++] = chr;
        }

        if((result = ConfigBuffer()) < 0)
            return result;

        if(!result)
            break; // End of file

    } while(1);

    gameConfig.lineNumber++;

    gameConfig.pLine[gameConfig.chrCount] = '\0';

    return gameConfig.chrCount;
}

static int IsLetter(char chr)
{
    return ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z'));
}

static int IsDigit(char chr)
{
    return (chr >= '0' && chr <= '9');
}

static int IsLetterOrDigit(char chr)
{
    return (IsLetter(chr) || IsDigit(chr));
}

static int IsWhiteSpace(char chr)
{
    return (chr == ' ' || chr == '\t');
}

static int ParseIdentifier(void)
{
    char chr;

    // A legal option identifier is like a C# namespace
    // identifier = <letter or underscore>[letter, underscore or digit]
    // option_identifier = identifier ['.' identifier]+
    // i.e. "_blah", "Example42" or "Window.Left"

    chr = gameConfig.pLine[gameConfig.chrIndex];
    if(chr != '_' && !IsLetter(chr))
    {
        printf("Expecting an option identifier, on line %d!\n", gameConfig.lineNumber);
        return -1;
    }

    while(++gameConfig.chrIndex < gameConfig.chrCount)
    {
        chr = gameConfig.pLine[gameConfig.chrIndex];

        if(chr != '_' && !IsLetterOrDigit(chr))
        {   // End of identifier
            if(chr == '.')
            {
                if(gameConfig.pLine[gameConfig.chrIndex - 1] == '.')
                {
                    printf("Illegal option identifier, on line %d!\n", gameConfig.lineNumber);
                    return -1;
                }

                continue;
            }
            else if(!IsWhiteSpace(chr))
            {
                printf("Illegal option identifier, on line %d!\n", gameConfig.lineNumber);
                return -1;
            }

            break;
        }
    }

    if(gameConfig.chrIndex == gameConfig.chrCount)
    {
        printf("Unexpected end of line, expecting the option's value, on line %d!\n", gameConfig.lineNumber);
        return -1;
    }

    if(gameConfig.pLine[gameConfig.chrIndex - 1] == '.')
    {
        printf("Illegal option identifier, on line %d!\n", gameConfig.lineNumber);
        return -1;
    }

    return gameConfig.chrIndex;
}

static int SkipWhiteSpace(void)
{
    while(gameConfig.chrIndex < gameConfig.chrCount)
    {
        if(!IsWhiteSpace(gameConfig.pLine[gameConfig.chrIndex]))
            break;

        gameConfig.chrIndex++;
    }

    return (gameConfig.chrIndex < gameConfig.chrCount);
}

static int ParseInteger(void)
{
    char chr;

    if(!SkipWhiteSpace())
    {
        printf("Unexpected end of line, on line %d!", gameConfig.lineNumber);
        return -1;
    }

    chr = gameConfig.pLine[gameConfig.chrIndex];
    if(!IsDigit(chr))
    {
        printf("Expecting an integer, at column %d on line %d!", gameConfig.chrIndex + 1, gameConfig.lineNumber);
        return -1;
    }

    gameConfig.integer = chr - '0';
    gameConfig.chrIndex++;

    while(gameConfig.chrIndex < gameConfig.chrCount)
    {
        chr = gameConfig.pLine[gameConfig.chrIndex++];

        if(!IsDigit(chr))
            break;

        gameConfig.integer *= 10;
        gameConfig.integer += chr - '0';
    }

    return 0;
}

static int ParseIntegerValue(int min, int max)
{
    if(ParseInteger() < 0)
        return -1;

    if(gameConfig.chrIndex != gameConfig.chrCount)
    {
        printf("Illegal option value, on line %d!", gameConfig.lineNumber);
        return -1;
    }

    if(gameConfig.integer < min || gameConfig.integer > max)
    {
        printf("Expecting value in range %d to %d, on line %d!", min, max, gameConfig.lineNumber);
        return -1;
    }

    return 0;
}

static int ParseString(void)
{
    int inEscape, index;
    char chr;

    if(!SkipWhiteSpace())
    {
        printf("Unexpected end of line, on line %d!", gameConfig.lineNumber);
        return -1;
    }

    if(gameConfig.pLine[gameConfig.chrIndex] != '"')
    {
        printf("Expecting a string (opening quote), on line %d!", gameConfig.lineNumber);
        return -1;
    }

    index = 0;
    do
    {
        // Skip quote
        gameConfig.chrIndex++;

        inEscape = 0;
        while(1)
        {
            if(gameConfig.chrIndex == gameConfig.chrCount)
            {
                printf("Unexpected end of line, expecting closing quote, on line %d!", gameConfig.lineNumber);
                return -1;
            }

            chr = gameConfig.pLine[gameConfig.chrIndex++];

            if(index + 1 >= CONFIG_STRING_LENGTH && !inEscape && chr == '"')
            {
                printf("String too long, on line %d!", gameConfig.lineNumber);
                return -1;
            }

            if(inEscape)
            {   // End escape sequence
                switch(chr)
                {
                    case 'n':
                        gameConfig.pString[index++] = '\n';
                        break;

                    case '\\':
                        gameConfig.pString[index++] = '\\';
                        break;

                    case '"':
                        gameConfig.pString[index++] = '"';
                        break;

                    default:
                        printf("Unsupported escape sequence, on line %d!", gameConfig.lineNumber);
                        return -1;
                }

                inEscape = 0;
            }
            else if(chr == '"')
            {   // End of string
                break;
            }
            else if(chr == '\\')
            {   // Begin escape sequence
                inEscape = 1;
            }
            else
            {   // Character value
                gameConfig.pString[index++] = chr;
            }
        }

        if(gameConfig.chrIndex == gameConfig.chrCount)
            break; // End of value

        if(!SkipWhiteSpace())
        {
            printf("Unexpected end of line, expecting a string continuation character, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        if(gameConfig.pLine[gameConfig.chrIndex++] != '+')
        {
            printf("Unexpected end of line, expecting a string continuation character, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        if(SkipWhiteSpace())
        {
            printf("Unexpected characters after string continuation character, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        if(!ConfigReadLine())
        {
            printf("Unexpected end of file, expecting continuation of string value, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        // Empty lines are not allowed between strings
        if(!SkipWhiteSpace())
        {
            printf("Expecting the continuation of a string value, not an empty line, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        if(gameConfig.pLine[gameConfig.chrIndex] != '"')
        {
            printf("Expecting the continuation of a string value, on line %d!", gameConfig.lineNumber);
            return -1;
        }

    } while(1);

    gameConfig.pString[index] = '\0';

    return index;
}

static int allocated = 0;

static int ParseItem(void)
{
    int result, i;
    int identifierIndex = -1;
    char *pString;

    if(gameConfig.chrCount >= 2 && gameConfig.pLine[0] == '/' && gameConfig.pLine[1] == '/')
        return 0; // Comment

    if((result = ParseIdentifier()) < 0)
        return result;

    for(i = 0; i < GAME_CONFIG_IDENTIFIERS; i++)
    {
        if(strncmp(gameConfig.pLine, gameConfigIdentifiers[i], result) == 0)
        {
            identifierIndex = i;
            break;
        }
    }

    if(identifierIndex != -1)
    {   // String replacement
        if((result = ParseString()) < 0)
            return -1;

        //printf("String len = %d\n", result);

        if((pString = (char *) malloc(result + 1)) == NULL)
        {
            printf("Failed to allocate string, on line %d (%d)!\n", gameConfig.lineNumber, allocated);
            return -1;
        }
        allocated += result + 1;

        strcpy(pString, gameConfig.pString);

        //printf("%s\n", pString);

        gameStrings[identifierIndex] = pString;
    }
    else if(strncmp(gameConfig.pLine, "levmin", 6) == 0)
    {   // Level time limit (minutes)
        
        gameConfig.chrIndex = 6;
        if(ParseInteger() < 0)
            return -1;

        if(gameConfig.integer < 0 || gameConfig.integer > 19)
        {
            printf("Unrecognised identifier, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        result = gameConfig.integer;

        if(ParseIntegerValue(0, 9) < 0)
            return -1;

        levmin[result] = gameConfig.integer;
    }
    else if(strncmp(gameConfig.pLine, "levsec", 6) == 0)
    {   // Level time limit (seconds)

        gameConfig.chrIndex = 6;
        if(ParseInteger() < 0)
            return -1;

        if(gameConfig.integer < 0 || gameConfig.integer > 19)
        {
            printf("Unrecognised identifier, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        result = gameConfig.integer;

        if(ParseIntegerValue(0, 59) < 0)
            return -1;

        levsec[result] = gameConfig.integer;
    }
    else if(strncmp(gameConfig.pLine, "cyclecolors", 11) == 0)
    {   // Level warp colors

        gameConfig.chrIndex = 11;
        if(ParseInteger() < 0)
            return -1;

        if(gameConfig.integer < 0 || gameConfig.integer > 2)
        {
            printf("Unrecognised identifier, on line %d!", gameConfig.lineNumber);
            return -1;
        }

        result = gameConfig.integer;

        if(ParseInteger() < 0)
            return -1;

        cyclecolors[result] = gameConfig.integer;
    }
    else if(strncmp(gameConfig.pLine, "barColor", 8) == 0)
    {    // Bar color
        if(ParseIntegerValue(0, 15) < 0)
	        return -1;

        // Not supported
    }
    else
    {
        printf("Unrecognised identifier, on line %d!", gameConfig.lineNumber);
        return -1;
    }

    return result;
}

static void GameConfigParse(void)
{
    int result;

    if(!ConfigBuffer())
    {
        printf("Unexpected EOF parsing game config!\n");
        return;
    }

    if(!ConfigReadLine())
    {
        printf("Unexpected EOF parsing game config!\n");
        return;
    }

    if(strcmp("Hovertank3D.Game", gameConfig.pLine) != 0)
    {
        printf("ParseGame: Expecting header Hovertank3D.Game on line 1!\n");
        return;
    }

    gameConfig.lineNumber = 1;
    while((result = ConfigReadLine()) >= 0)
    {
        if(result < 0)
            break; // Error

        if(gameConfig.chrCount != 0)
        {
            //printf("%d %s\r\n", gameConfig.lineNumber, gameConfig.pLine);
            
            if(ParseItem() < 0)
                return; // Error
        }

        if(gameConfig.bufferCount == 0)
            break;
    }
}

void GameConfigLoad(void)
{
    if((gameConfig.handle = open("GAME.HOV", O_RDONLY | O_TEXT, S_IREAD)) == -1)
        return;

    if((gameConfig.pBuffer = (unsigned char *) malloc(CONFIG_BUFFER_LENGTH)) != NULL)
    {
        if((gameConfig.pLine = (char *) malloc(CONFIG_LINE_LENGTH)) != NULL)
        {
            if((gameConfig.pString = (char *) malloc(CONFIG_STRING_LENGTH)) != NULL)
            {
                GameConfigParse();

                free(gameConfig.pString);
                gameConfig.pString = NULL;
            }

            free(gameConfig.pLine);
            gameConfig.pLine = NULL;
        }

        free(gameConfig.pBuffer);
        gameConfig.pBuffer = NULL;
    }

    close(gameConfig.handle);
    gameConfig.handle = 0;
}
