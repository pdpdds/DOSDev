
#define  CR      13    /* Ð”w Ç¡· ´¯aÇ¡ Å¡—a       */
#define  ESC     27
#define  SPACE   32
#define  F1     315    /* Ð”w Ç¡· ÑÂ¸wÇ¡ Å¡—a + 256 */
#define  F2     316
#define  F3     317
#define  F4     318
#define  F5     319
#define  F6     320
#define  F7     321
#define  F8     322
#define  F9     323
#define  F10    324
#define  HOME   327
#define  UP     328
#define  PGUP   329
#define  LEFT   331
#define  RIGHT  333
#define  END    335
#define  DOWN   336
#define  PGDN   337
#define  INS    338
#define  DEL    339

/***************************************************/
/* Ðq®ˆt : ·©¤eÇ¡ -> Ç¡· ´¯aÇ¡ Å¡—a             */
/*          Ëb®Ç¡ -> Ç¡· ÑÂ¸wÇ¡ Å¡—a+256         */
/***************************************************/
int get_key()
    {
    unsigned   key,lo,hi;

    key = bioskey(0);         /* ’‰a»¥ Ç¡· Å¡—a  */
    lo  = key & 0x00FF;       /* Å¡—a· Ða¶á ¤a·¡Ëa*/
    hi  =(key & 0xFF00) >> 8; /* Å¡—a· ¬w¶á ¤a·¡Ëa*/

    if (lo==0)
	return  hi+256;     /* Ëb®Ç¡ˆa ’‰a»¥ ‰w¶*/
    else
	return  lo;         /* ·©¤eÇ¡ˆa ’‰a»¥ ‰w¶*/
    }

/***************************************************/
/* Ðq®ˆt : Ç¡ˆa ’‰a»¥ ‰w¶   -> Ç¡· Å¡—a        */
/*          ’‰a»¡»¡ ´g·e ‰w¶ -> 0                */
/***************************************************/
int in_key()
    {
    return( ( kbhit() ) ? get_key() : kbhit() );
    }

/***************************************************/
/* (xµi, yÐ—) ·¡ÒµA msgŸi Îa¯¡Ða‰¡ ¢…¸aµi·i       */
/* strµA ·³b¤h“e”a.                               */
/***************************************************/
void xy_gets(int x, int y, char *msg, char *str)
    {
    gotoxy(x, y);
    cputs(msg);
    cscanf("%s", str);
    }

/***************************************************/
/* Ðq®ˆt : (xµi, yÐ—)µA Îa¯¡–A´á ·¶“e ¢…¸a· Å¡—a */
/***************************************************/
int xy_vgetch(int x, int y)
    {
    unsigned  v_s;              /* §¡•Aµ¡ ­A‹a åËa */

    if ( peekb(0, 0x449) == 7 )	v_s = 0xB000;
    else                       	v_s = 0xB800;

    return ( peekb(v_s, 160*(y-1)+2*(x-1)) );
    }

/***************************************************/
/* (xµi, yÐ—) ·¡ÒµA Îa¯¡–A´á ·¶“e ¢…¸aµi·i        */
/* strµA ŠÐe”a(¢…¸aµi ‹©·¡“e n).                  */
/***************************************************/
void xy_vgets(int x, int y, int n, char *str)
    {
    int       i;
    unsigned  v_s;              /* §¡•Aµ¡ ­A‹a åËa */

    if ( peekb(0, 0x449) == 7 )	v_s = 0xB000;
    else                        v_s = 0xB800;

    for (i=x ; i<x+n ; i++)
	{
	*str = peekb(v_s, 160*(y-1)+2*(i-1));
        if (  *str==0x9  )
	    break;
	str++;
	}
    *str = '\0';                /* ¢…¸aµi· {¢…¸a */
    }

/*

main()
    {
    char  str[80];

    xyvgets(1,1, 80, str);
    printf("%s\n", str);
    }
*/
