/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          CGA detecting function                              */
/*   FILENAME           iscga.c                                             */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_cga(void)
/* This routine detects the presence of CGA by looking for the CGA's 6845 */
/* CRTC at I/O port 03d4h						  */
{
   return(is_6845(0x03d4));
}

