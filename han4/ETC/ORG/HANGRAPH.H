/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          header file - graphics functions(5)                 */
/*   FILENAME           hangraph.h                                          */
/*   DATE & TIME        12/18/92 (FRI) 15:30                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* �a�� ���A�� �A���� ���I�� ���a���A�e �A�� �A�����i �b�e ҁ */
/* ���a���i ���e�a���� �e�a.                                    */

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

unsigned imagesize_han(int left, int top, int right, int bottom);
void getimage_han(int left, int top, int right, int bottom, char *bitmap);
void putimage_han(int left, int top, char *bitmap);

void draw_horiz_line(int left, int top, int width, int color);
void draw_vert_line(int left, int top, int height, int color);

