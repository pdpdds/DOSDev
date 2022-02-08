/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          current video mode detecting function               */
/*   FILENAME           getvideo.c                                          */
/*   DATE & TIME        12/21/92 (MON) 11:35                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_video_mode(void)
/* This routine calls INT 10H to detect current video mode */
{
   asm 	mov	ah, 0fh
   asm	int	10h
   asm	xor	ah, ah		/* AX = display mode */
}
