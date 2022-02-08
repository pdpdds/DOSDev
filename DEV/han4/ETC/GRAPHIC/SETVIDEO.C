/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          video mode setting function               	    */
/*   FILENAME           setvideo.c                                          */
/*   DATE & TIME        12/21/92 (MON) 11:35                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_video_mode(int mode)
/* This routine calls INT 10H to set video mode */
{
   asm	mov	ax, mode
   asm 	xor 	ah, ah		/* calls INT 10H/00H */
   asm	int	10h
}
