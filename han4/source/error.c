/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          error_exit_han()                                    */
/*   FILENAME           error.c                                             */
/*   DATE & TIME        03/11/92(WED) 19:09                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

char *error_exit_msg;

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void error_exit_han(char *func_name, int exit_code)
/* print error message and return to DOS  */
{
   char *msg = "Ðq®µA¬á ”a·q‰Á ˆ{·e µAœáˆa ¤i¬—Ð–¯s“¡”a";
   int msg_width, box_x;

   msg_width = strlen(func_name) + strlen(msg) + 1;
   if(strlen(error_exit_msg) > msg_width) msg_width = strlen(error_exit_msg);
   box_x = (abs_maxx_han + 1 - msg_width - 4) >> 1;

   if(msg_width > abs_maxx_han - 3)
   {
      close_han();
      puts("Too long error message in calling error_exit_han().");
      exit(exit_code);
   }

   aclear_text_han(box_x + 1, 11, box_x + msg_width + 2, 14, attr_han);
   draw_box_han(box_x,     10, box_x + msg_width + 3, 15, BOX_H2V2);
   draw_box_han(box_x + 1, 13, box_x + msg_width + 2, 13, BOX_H1V1);
   put_eng_font(box_x, 13, 199);
   put_eng_font(box_x + msg_width + 3, 13, 182);
   xputs_han(box_x + 2,                         11, func_name);
   xputs_han(box_x + 2 + strlen(func_name) + 1, 11, msg);
   xputs_han(box_x + 2,                         12, error_exit_msg);
   xputs_han((abs_maxx_han + 1 - 16) / 2,       14, "´a¢ Ç¡a Ã¡­A¶a");

   if(!getch()) getch();
   close_han();
   exit(exit_code);
}
