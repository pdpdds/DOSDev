/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          read_han_font(), read_eng_font(), read_spc_font(),  */
/*                      set_font(), free_font(), return_to_org_font()       */
/*   FILENAME           fontmng.c                                           */
/*   DATE & TIME        07/03/92(FRI) 02:25                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int read_han_font(char **font_ptr, char *han_font_file)
/* �A�����i �i�w�h�e ҁ �e�i ͥ�a �����i ���e�a. */
{
   FILE *stream;

   /* �A�����i �i�w�h�e�a. */
   if((*font_ptr = (char *) malloc( (NO_BUL1*(NO_HAN1+1)*HAN_FONT_SIZE) +
				    (NO_BUL2*(NO_HAN2+1)*HAN_FONT_SIZE) +
				    (NO_BUL3*(NO_HAN3+1)*HAN_FONT_SIZE) )) == NULL)
      return(MEMORY_NOT_ENOUGH);

   /* ͥ�a �����i ���e�a. */
   if((stream = fopen(han_font_file, "rb")) == NULL) return(HAN_FONT_NOT_FOUND);
   if(fread(*font_ptr, (NO_BUL1*(NO_HAN1+1)*HAN_FONT_SIZE) +
		       (NO_BUL2*(NO_HAN2+1)*HAN_FONT_SIZE) +
		       (NO_BUL3*(NO_HAN3+1)*HAN_FONT_SIZE), 1, stream) != 1)
   {
      free(*font_ptr);
      fclose(stream);
      return(HAN_FONT_READ_ERROR);
   }
   fclose(stream);

   return(OK_HAN);
}

int read_eng_font(char **font_ptr, char *eng_font_file)
/* �A�����i �i�w�h�e ҁ �w�� ͥ�a �����i ���e�a. */
{
   FILE *stream;

   /* �A�����i �i�w�h�e�a. */
   if((*font_ptr = (char *) malloc(NO_ENG*ENG_FONT_SIZE)) == NULL) return(MEMORY_NOT_ENOUGH);

   /* ͥ�a �����i ���e�a. */
   if((stream = fopen(eng_font_file, "rb")) == NULL) return(ENG_FONT_NOT_FOUND);
   if(fread(*font_ptr, NO_ENG*ENG_FONT_SIZE, 1, stream) != 1)
   {
      free(*font_ptr);
      fclose(stream);
      return(ENG_FONT_READ_ERROR);
   }
   fclose(stream);

   return(OK_HAN);
}

int read_spc_font(char **font_ptr, char *spc_font_file)
/* �A�����i �i�w�h�e ҁ �b�� ���a ͥ�a �����i ���e�a. */
{
   FILE *stream;

   /* �A�����i �i�w�h�e�a. */
   if((*font_ptr = (char *) malloc(NO_ENG*HAN_FONT_SIZE)) == NULL) return(MEMORY_NOT_ENOUGH);

   /* ͥ�a �����i ���e�a. */
   if((stream = fopen(spc_font_file, "rb")) == NULL) return(SPC_FONT_NOT_FOUND);
   if(fread(*font_ptr, NO_ENG*HAN_FONT_SIZE, 1, stream) != 1)
   {
      free(*font_ptr);
      fclose(stream);
      return(SPC_FONT_READ_ERROR);
   }
   fclose(stream);

   return(OK_HAN);
}

void set_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr)
/* �e�� �a�w���� ͥ�a�i �A�����A ���ᖅ ͥ�a�� �a���a. */
{
   if(han_font_ptr != NULL)     /* �e�i ͥ�a�i �a���a. */
   {
      (char*)han1_font = han_font_ptr;
      han2_font = han1_font+ NO_BUL1;
      han3_font = han2_font+ NO_BUL2;
   }
   if(eng_font_ptr != NULL)     /* �w�� ͥ�a�i �a���a. */
      (char*)eng_font = eng_font_ptr;
   if(spc_font_ptr != NULL)
      (char*)spc_font = spc_font_ptr;  /* �b�� ���a ͥ�a�i �a���a. */
}

void free_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr)
/* ͥ�a�i �a��ǡ�e ͡���� ???_font_ptr �A �i�w�E �A�����i ���ᯡǥ�a. */
{
   if(han_font_ptr != NULL) free(han_font_ptr);
   if(eng_font_ptr != NULL) free(eng_font_ptr);
   if(spc_font_ptr != NULL) free(spc_font_ptr);
}

void return_to_org_font(int return_han, int return_eng, int return_spc)
/* �e�� �a�w���� ͥ�a�i init_han() �� ѡ�A���i �� ���� �i�� ͥ�a�� �a���a. */
{
   if(return_han)
   {
      han1_font = org_han1_font;
      han2_font = org_han2_font;
      han3_font = org_han3_font;
   }
   if(return_eng) eng_font = org_eng_font;
   if(return_spc) spc_font = org_spc_font;
}
