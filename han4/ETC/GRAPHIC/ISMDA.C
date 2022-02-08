/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          MDA detecting function                              */
/*   FILENAME           ismda.c                                             */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_mda(void)
/* This routine detects the presence of MDA by looking for the MDA's 6845 */
/* CRTC at I/O port 03b4h						  */
{
   return(is_6845(0x03b4));
}

