/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          play()                                              */
/*   FILENAME           play.c                                              */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* If the source text is compiled in any other memory models */
/* except for the large model, issues a fatal error message  */

#include <stdlib.h>
#include <bios.h>
#include <time.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

#define MAX_OCTAVE        7
#define MAX_LENGTH       64
#define MAX_TEMPO       255
#define MAX_NOTE         84
#define MAX_VARIANCE  10000

enum
{
   INVALID_COMMAND = 1,
   INVALID_OCTAVE,
   INVALID_LENGTH,
   INVALID_TEMPO,
   INVALID_NUM_NOTE,
   INVALID_NOTE,
   INVALID_VARIANCE,
   INVALID_PLAY_MODE,
   OCTAVE_NOT_SPECIFIED,
   LENGTH_NOT_SPECIFIED,
   TEMPO_NOT_SPECIFIED,
   NUM_NOTE_NOT_SPECIFIED,
   VARIANCE_NOT_SPECIFIED
};

#define MUSIC_NORMAL  0
#define MUSIC_LONG    1
#define MUSIC_ITER    2
#define MUSIC_FANTASY 3

/****************************************************************************/
/*                   Tables(to be referenced by play()                      */
/****************************************************************************/

const int note_freq[(MAX_OCTAVE+1)*12] =
{     65,   69,   74,   78,   82,   87,   93,   98,  104,  110,  117,  123,
     131,  139,  149,  156,  165,  175,  185,  196,  208,  220,  233,  247,
     262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
     523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
    1046, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1857, 1976,
    2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3714, 3952,
    4186, 4434, 4698, 4978, 5274, 5588, 5920, 6272, 6644, 7040, 7428, 7904,
    8372, 8868, 9396, 9956,10548,11176,11840,12544,13288,14080,14856,15808
};

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int play(char *string)
{
   static unsigned char octave = 3, b_length = 4,
		   tempo = 120, music_mode = MUSIC_NORMAL;
   static mod_freq = 4;
   char token, note, length;
   unsigned number, duration, frequency;
   int index = 0;

   strlwr(string);
   while((token = string[index++]) != 0)
   {
      if(token == 'm')
      {
         switch(string[index++])
         {
            case 'n' : music_mode = MUSIC_NORMAL;  break;
            case 'l' : music_mode = MUSIC_LONG;    break;
            case 'i' : music_mode = MUSIC_ITER;    break;
            case 'f' : music_mode = MUSIC_FANTASY; break;
	    default  : nosound(); return(INVALID_PLAY_MODE);
         }
         continue;
      }

      if(token == ' ')
	 continue;
      else if(token == '>')
      {
	 octave++;
         continue;
      }
      else if(token == '<')
      {
         octave--;
         continue;
      }
      if(token == 'o' || token == 'l' || token == 't' || token == 'n' || token == 'v')
      {
         for(number = 0; string[index] >= '0' && string[index] <= '9'; index++)
         {
            number *= 10;
            number += string[index] - '0';
         }
	 switch(token)
         {
            case 'o' : if(number > MAX_OCTAVE)
                       {
			  nosound();
                          return(INVALID_OCTAVE);
                       }
                       if(number == 0)
                       {
                          nosound();
                          return(OCTAVE_NOT_SPECIFIED);
                       }
                       octave = number;
                       continue;
            case 'l' : if(number > MAX_LENGTH)
                       {
                          nosound();
                          return(INVALID_LENGTH);
		       }
                       if(number == 0)
                       {
                          nosound();
                          return(LENGTH_NOT_SPECIFIED);
		       }
                       b_length = number;
                       continue;
            case 't' : if(number > MAX_TEMPO)
                       {
                          nosound();
                          return(INVALID_TEMPO);
                       }
                       if(number == 0)
                       {
                          nosound();
                          return(TEMPO_NOT_SPECIFIED);
                       }
		       tempo = number;
                       continue;
            case 'v' : if(number > MAX_VARIANCE)
                       {
                          nosound();
                          return(INVALID_VARIANCE);
		       }
                       if(number == 0)
                       {
                          nosound();
                          return(VARIANCE_NOT_SPECIFIED);
                       }
                       mod_freq = number;
                       continue;
            case 'n' : if(number > MAX_NOTE)
                       {
                          nosound();
                          return(INVALID_NUM_NOTE);
		       }
                       if(number == 0)
                       {
                          nosound();
                          return(NUM_NOTE_NOT_SPECIFIED);
                       }
                       note = number;
		       length = b_length;
                       break;
         }
      }
      else if((token >= 'a' && token <= 'g') || token == 'p')
      {
         if(token != 'p')
         {
            note = ((token < 'c') ? octave*12 + ((token - 'a') << 1) + 9 :
            octave*12 + ((token - 'c') << 1));
            if(token > 'e') note--;
	    if(string[index] == '#' || string[index] == '+')
            {
               index++;
               note++;
            }
            else if(string[index] == '-')
            {
               index++;
	       note--;
            }
         }
         for(number = 0; string[index] >= '0' && string[index] <= '9'; index++)
         {
            number *= 10;
            number += string[index] - '0';
         }
         if(number > MAX_LENGTH)
         {
	    nosound();
            return(INVALID_LENGTH);
         }
         length = (number == 0) ? b_length : number;
      }
      else
      {
         nosound();
         return(INVALID_COMMAND);
      }

      duration = ((480. * 500.) / (float) (length * tempo));
      if(string[index] == '.')
      {
         duration += duration >> 1;
         index++;
      }

      if(token == 'p')
      {
         nosound();
         delay(duration);
         continue;
      }

      if(note < 0 || note > (MAX_OCTAVE+1)*12 - 1)
      {
         nosound();
	 return(INVALID_NOTE);
      }
      frequency = note_freq[note];
      switch(music_mode)
      {
         case MUSIC_NORMAL  : 
	    sound(frequency);
            delay(duration - (duration >> 3));
	    nosound();
            delay(duration >> 3);
            break;
         case MUSIC_LONG    : 
		    sound(frequency);
            delay(duration);
            break;
         case MUSIC_ITER    : 
	    for(number = 0; number < duration / 100; number++)
            {
               sound(frequency);
	       delay(90);
               nosound();
               delay(10);
            }
            sound(frequency);
            delay(duration % 90);
            nosound();
	    break;
         case MUSIC_FANTASY :
            for(number = 0; number < duration / 30; number++)
            {
               sound(frequency + mod_freq / 2  - number % mod_freq);
               delay(30);
            }
            sound(frequency);
            delay(duration % 30);
            break;
      }
   }

   if(music_mode == MUSIC_LONG || music_mode == MUSIC_FANTASY) nosound();
   return(0);
}
