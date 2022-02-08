/*************************************************************/
/* menu.h : x_menu(), y_menu(), dialog_box() Ðq® ¸÷·       */
/*************************************************************/

#include  "chulryuk.h"   /* normal(), reverse(), set_attrib()*/
#include  "ibryuk.h"     /* get_key(), xy_vgets()            */

/*************************************************************/
/* Ðq®ˆt : ¬åÈ‚–E ¡A“AµA ”Ðe ®•¸a(Àõ¤å¼->0, –¤å¼->2,...)*/
/* Esc Ç¡       : Ðq® ¹·ža, Ðq®ˆt => -1                    */
/* flag[0]=='L' : LEFT/RIGHT Ç¡Ÿi ’Ÿe‰w¶ ¯©Ð—¹·ža.         */
/*                Ðq®ˆt·e LEFT/RIGHT(331/333)               */
/* flag[0]!='L' : LEFT/RIGHT Ç¡Ÿi ’Ÿe‰w¶ «¢ ­¡Ÿ¡ e ¶‰Ÿ¥”a  */
/* flag[1]=='C' : Ðq® ¯©Ð— ¹·ža¯¡ ¡A“AÌe·i »¡¶…”a(Clear)    */
/* flag[1]!='C' : Ðq® ¯©Ð— ¹·ža¯¡ ¡A“AÌe·i »¡¶»¡ ´g“e”a    */
/*************************************************************/
int y_menu(int x, int y, int suntaek, char *flag, char **menu, int chuga)
    {
    int    n     = 0;    /* menu· Ðw¡¢ ˆ®ˆa ŠÐ»¥”a      */
    int    chdae = 0;    /* menu· Ðw¡¢º— ÂA” ‹©·¡· ¢…¸a® */
    char   buf[4000];    /* ¡A“AÌe ¦¦…· ·¡¸å ¶w ¥¡‰Å¶w   */
    int    i;

       set_cursor(32,0);
    
    /*  menu Ðw¡¢· ˆ•®(n), ÂA” ‹©·¡(chdae)Ÿi ŠÐe”a */
    while (menu[n][0] != '.')
	{
	if (strlen(menu[n]) > chdae)
	    chdae = strlen(menu[n]);
	n++;
	}

    /* ¡A“AÌe·a¡ ¬a¶wÐi ¬aˆbÑw ¦· ¹·¸å ¶w·i ¥¡‰Å */
    gettext(x, y, x+chdae+6,y+2+n, buf);

    /* ¡A“AÌe·a¡ ¬a¶wÐi ¬aˆbÑw ¦ e·i »¡¶…”a */
    window(x+2, y+1, x+chdae+6,y+2+n);
    clrscr();
    window(x ,y ,x+chdae+3,y+1+n);
    textattr(0x70);
    clrscr();
    window(1,1, 80,25);
    single_box(x, y, x+chdae+3,y+1+n);
   
   for (i=0 ; i<n ; i++)
        xy_puts(x+3, y+1+i, menu[i]);

    /* ·¡¸åµA ¬åÈ‚–E menu Ðw¡¢·i ¤e¸å ¬wÈ¡ Îa¯¡ */
    xy_puts(x+3, y+1+suntaek, menu[suntaek]);
    set_attrib(x+1, x+chdae+2, y+1+suntaek, 0x2f);

    /* µEÈá/ESC Ç¡Ÿi ’Ÿa¡e ¹·ža–E”a */
    while ( 1 )
	{
	  if(chuga==0 && suntaek==0)
           xy_puts(5,25,"¢Ñ  ¸íÇÔÆÄÀÏ(person2.dat)ÀÇ ³¡¿¡ »õ·Î¿î ¸íÇÔÀ» Ãß°¡");
	   else if(chuga==0 && suntaek==1)
            xy_puts(5,25,"¢Ñ  ¼ºÇÔÀüÃ¼·Î °Ë»ö,¼öÁ¤                           ");
	    else if(chuga==0 && suntaek==2)
             xy_puts(5,25,"¢Ñ  ·¹ÄÚµå¹øÈ£(°¢ ·¹ÄÚµåÀÇ ¼ø¹ø)·Î »èÁ¦            ");
	     else if(chuga==0 && suntaek==3)
              xy_puts(5,25,"¢Ñ  ¿øÇÏ´Â ·¹ÄÚµå µÚ¿¡ »õ·Î¿î ¸íÇÔ »ðÀÔ            ");
	      else if(chuga==0 && suntaek==4)
               xy_puts(5,25,"¢Ñ  A: µå¶óÀÌºê·Î ¸íÇÔÆÄÀÏ(person2.dat)À» »ª¾÷     ");
	       else if(chuga==0 && suntaek==5)
                xy_puts(5,25,"¢Ñ  The End                                        ");
          if(chuga==1 && suntaek==0)
           xy_puts(5,25,"¢Ñ  µî·ÏµÈ ¸ðµç ¸íÇÔÀ» º¸¿©ÁÝ´Ï´Ù.    ");
          else if(chuga==1 && suntaek==1)
           xy_puts(5,25,"¢Ñ  ³²ÀÚ¸¸  º¸¿©ÁÝ´Ï´Ù.               ");  
           else if(chuga==1 && suntaek==2)
            xy_puts(5,25,"¢Ñ  ¿©ÀÚ¸¸ º¸¿©ÁÝ´Ï´Ù.                ");

          if(chuga==2 && suntaek==0)
           xy_puts(5,25,"¢Ñ  ¼ºÇÔÀ¸·Î °Ë»öÇØÁÝ´Ï´Ù.                                ");
           else if(chuga==2 && suntaek==1)
            xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, ÀÌ¸§ÀÌ '±æµ¿'ÀÎ »ç¶÷À» Ã£´Â´ÙµçÁö             ");
            else if(chuga==2 && suntaek==4)
             xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, 'Ä£±¸'¸¸ °Ë»öÇÑ´ÙµçÁö, '°í°´'¸¸ °Ë»öÇÑ´ÙµçÁö");
             else if(suntaek==7)
              xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, '¼­¿ï'¿¡ »ç´Â »ç¶÷¸¸ °Ë»öÇÒ ¶§              ");
              else if(suntaek==8)
               xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, '¼ººÏ±¸'¿¡ »ç´Â »ç¶÷¸¸ °Ë»öÇÒ ¶§            ");
	       else if(suntaek==11)
                xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, Á÷ÀåÀÌ '¼­¿ï'ÀÎ »ç¶÷¸¸ °Ë»öÇÒ ¶§            ");
                else if(suntaek==12)
                 xy_puts(5,25,"¢Ñ  ¿¹¸¦µé¸é, Á÷ÀåÀÌ '¼ººÏ±¸'ÀÎ »ç¶÷¸¸ °Ë»öÇÒ ¶§          ");
                 else if(chuga==2)
                  xy_puts(5,25,"¢Ñ  À¶Åë¼ºÀÖ´Â °Ë»öÀ» ÇØÁÝ´Ï´Ù.                            ");
          if(chuga==3 && suntaek==0)
           xy_puts(5,25,"¢Ñ  ÇÑ¹ø ÀÐ¾î º¸¼¼¿ä.");
           else if(chuga==3 && suntaek==1)
            xy_puts(5,25,"¢Ñ  Àâ¼Ò¸®           ");

        switch ( get_key() )
	    {
	    case UP   : set_attrib(x+1,x+chdae+2, y+1+suntaek, 0x70);
			if (suntaek == 0)   suntaek = n-1;
			else                suntaek--;
                        set_attrib(x+1,x+chdae+2, y+1+suntaek, 0x2f);
			break;
	    case DOWN : set_attrib(x+1,x+chdae+2, y+1+suntaek, 0x70);
			if (suntaek == n-1) suntaek = 0;
			else                suntaek++;
                        set_attrib(x+1,x+chdae+2, y+1+suntaek, 0x2f);
			break;
	    case LEFT : if (flag[0] == 'L')
			    {
			    if (flag[1] == 'C')
				puttext(x, y,x+chdae+6,y+2+n, buf);
			    return ( LEFT );
			    }
			putch('\a'); break;
	    case RIGHT: if (flag[0] == 'L')
			    {
			    if (flag[1] == 'C')
				puttext(x, y,x+chdae+6,y+2+n, buf);
			    return ( RIGHT );
			    }
			putch('\a'); break;
	    case ESC  : if (flag[1] == 'C')
			    puttext(x, y,x+chdae+6,y+2+n, buf);

			return ( -1 );
	    case CR   : if (flag[1] == 'C')
			    puttext(x, y,x+chdae+6,y+2+n, buf);
			normal();
			set_cursor(12,13);
			return ( suntaek );
            default   :  break;
	    }
	}
    }

/*************************************************************/
/* Ðq®ˆt : ¬åÈ‚–E ¡A“AµA ”Ðe ®•¸a(Àõ¤å¼->0, –¤å¼->2,...)*/
/* Esc Ç¡       : Ðq® ¹·ža, Ðq®ˆt => -1                    */
/* flag[0]=='D' : DOWN Ç¡Ÿi ’Ÿe‰w¶ ¯©Ð— ¹·ža.              */
/* flag[0]!='D' : DOWN Ç¡Ÿi ’Ÿe‰w¶ «¢ ­¡Ÿ¡ e ¶‰Ÿ¥”a        */
/* flag[1]=='S' : ¡A“AÌe·i Îa¯¡Ò ¡A“AŸi ¬åÈ‚Ða‰A Ðe”a       */
/* flag[1]!='S' : ¡A“AÌe·i Îa¯¡ e ÐeÒ ¯©Ð— ¹·ža             */
/*************************************************************/
int x_menu(int x, int y, int suntaek, char *flag, char **menu)
    {
    int    n     = 0;        /* menu· Ðw¡¢ ˆ®ˆa ŠÐ»¥”a  */
    int    chdae = 0;        /* menu· Ðw¡¢º— ÂA” ‹©·¡      */
    int    x_jwapyo[20];     /* ˆb menu Ðw¡¢·i Îa¯¡Ði x ¹ÁÎa */
    int    pok;              /* ˆb Ðw¡¢· ¡A“AÐ— ¬wµA¬á· Í¢ */
    int    i, giri;

     set_cursor(32,0);

    /* menu Ðw¡¢· ˆ•®(n), ÂA” ‹©·¡(chdae)Ÿi ŠÐe”a */
    while (menu[n][0] != '.')
	{
	if ( (giri=strlen(menu[n])) > chdae )
	    chdae = giri;
	n++;
	}
    pok = chdae+1;

    /* ¡A“AÐ—µA £»º‰·i ‹aŸ¥”a */



    /* ˆb ¡A“A Ðw¡¢·i Îa¯¡ */
    for (i=0 ; i<n ; i++)
	{
	giri        = strlen(menu[i]);
	x_jwapyo[i] = x+pok*i+(pok-giri)/2;
	xy_puts(x_jwapyo[i], y, menu[i]);
	}

    if (suntaek == -1)  return (-1);

    /* ·¡¸åµA ¬åÈ‚–E menuÐw¡¢·i ¤e¸å ¬wÈ¡ Îa¯¡ */
    textattr(0xcf);
    xy_puts(x_jwapyo[suntaek], y, menu[suntaek]);
    normal();

    if (flag[1] != 'S')
      { set_cursor(12,13); return ( suntaek ); }

    /* µEÈá/ESC Ç¡Ÿi ’Ÿa¡e ¹·ža–E”a */
    while ( 1 )
	{
	  textattr(0x70);
          if(suntaek==0)
            xy_puts(5,25,"¢Ñ  ¸íÇÔÀ» µî·Ï,°Ë»ö,»èÁ¦,»ðÀÔ        ");
          else if(suntaek==1)
            xy_puts(5,25,"¢Ñ  Ã¢À» ¶ç¿ö¼­ ¸íÇÔÀ» º¼ ¼ö ÀÖ½À´Ï´Ù.");
          else if(suntaek==2)
            xy_puts(5,25,"¢Ñ  °¢ ÇÊµåº°·Î À¶Åë¼ºÀÖ´Â °Ë»ö       ");
          else if(suntaek==3)
            xy_puts(5,25,"¢Ñ  »ç¿ë¹ý°ú Àâ¼Ò¸®                   ");
            
	   normal();
	  switch ( get_key() )
	    {
	    case LEFT : textattr(0x70);
			xy_puts(x_jwapyo[suntaek], y,menu[suntaek]);
			if (suntaek==0)   suntaek = n-1;
			else              suntaek--;
                        textattr(0xcf);
			xy_puts(x_jwapyo[suntaek], y,menu[suntaek]);
			normal();
			break;
            case RIGHT: textattr(0x70);
			xy_puts(x_jwapyo[suntaek], y,menu[suntaek]);
			if (suntaek==n-1) suntaek = 0;
			else              suntaek++;
                        textattr(0xcf);
			xy_puts(x_jwapyo[suntaek], y,menu[suntaek]);
			normal();
			break;
	    case DOWN : if (flag[0] == 'D')
			    return (suntaek);
			putch('\a');
			break;
            case CR   : normal(); return suntaek;
            case ESC  : set_cursor(12,13); normal(); return  -1    ;
            default   :  break;
	    }
	}
    }

/******************************************************************/
/* ”ÑÁ¬w¸aŸi Îa¯¡Ða‰¡ data[i].str· ˆt·i ·³b¤h“e”a              */
/* Ðq®ˆt : ¸÷¬w¹·ža=>1,  Esc Ç¡¡ ¹·ža=>-1                       */
/* (x, y) : ”ÑÁ¬w¸a ¯¡¸b¸ñ, n : •A·¡Èá ˆ•®, pok : •A·¡Èá ÂA”Í¢ */
/******************************************************************/
#define  MSG_POK  18          /* ¡A­A»¡· Í¢                      */

struct  daehwa {              /* ”ÑÁ¬w¸aµA¬á· ·³Â‰b¶w Š¹¡ÁA */
	char   *msg;          /*                    Îa¯¡Ði ¡A­A»¡ */
	char    str[30];      /*                  ·³b¤h·i •A·¡Èá */
      };
/*----------------------------------------------------------------*/
int dialog_box(int x,int y,int n, int pok, struct daehwa *data)
    {
    int     wichi   = 0;   /* Äá¬áˆa ¶áÃ¡Ðe Ï©—a ¤åÑ¡(0,1,...n-1) */
    int     munjasu = 0;   /* •A·¡Èá ·³b Ï©—aµA ·³b–E ¢…¸a®    */
    char    buf[4000];     /* ”ÑÁ¬w¸a ¦¦…· ·¡¸å ¶w ¥¡‰Å¶w    */
    int     i, key;

    /* ”ÑÁ¬w¸a¡ ¬a¶wÐi ¬aˆbÑw ¦· ¹·¸å ¶w·i ¥¡‰Å */
    gettext(x,y, x+MSG_POK+pok+5,y+n*2+6, buf);

    /* ”ÑÁ¬w¸a¡ ¬a¶wÐi ¬aˆbÑw ¦ e·i »¡¶…”a */
    window( x,y, x+MSG_POK+pok+5,y+n*2+6);
    normal(); clrscr();
    window( 1,1, 80,25);

    /* ”ÑÁ¬w¸aŸi Îa¯¡ */
    form_window(x,y, x+MSG_POK+pok+5, y+n*2+6, n*2+2, "•A·¡Èá·³b");
    xy_puts(x+2+pok/2, y+n*2+3, "Äá¬á ·¡•· : ÑÁ¬iÎa Ç¡");
    xy_puts(x+2+pok/2, y+n*2+4, "·³b ¹·ža : Enter  Ç¡");
    xy_puts(x+2+pok/2, y+n*2+5, "¯©Ð— Âá­¡ : Esc    Ç¡");

    for (i=0 ; i<n ; i++)             /* ¡A­A»¡ / ·³b Ï©—a  Îa¯¡ */
	{
	xy_puts(x+4, y+2+i*2, data[i].msg);
	set_attrib(x+MSG_POK, x+MSG_POK+pok-1, y+2+i*2, 0x70);
	}

    /* •A·¡ÈáŸi ·³b¤h“e”a */
    gotoxy(x+MSG_POK, y+2);
    reverse();  set_cursor(12,13);
    while( 1 )
	{
	key = get_key();
	switch ( key )
	    {
	    case UP    : if (wichi==0)     wichi=n-1;  else wichi--;
			 gotoxy(x+MSG_POK, y+2+wichi*2);
			 munjasu=0;
			 break;
	    case DOWN  : if (wichi==n-1)   wichi=0;    else wichi++;
			 gotoxy(x+MSG_POK, y+2+wichi*2);
			 munjasu=0;
			 break;
	    case RIGHT : if (munjasu<pok)
			     gotoxy(wherex()+1, wherey()),munjasu++;
			 break;
	    case LEFT  : if (munjasu>0)
			     gotoxy(wherex()-1, wherey()),munjasu--;
			 break;
	    }
        if (key==ESC)
	    {
	    puttext(x,y, x+MSG_POK+pok+5,y+n*2+6, buf);
	    normal(); return -1;
	    }
	if (key==CR)
	    {
	    if (wichi == n-1)
		break;
	    else
		{
		wichi++, munjasu=0,
		gotoxy(x+MSG_POK, y+2+wichi*2);
		}
	    }
	if (key!=CR && key!=UP && key!=DOWN && key!=LEFT && key!=RIGHT)
	    {
	    munjasu++;
	    if (munjasu>pok) putch('\a');
	    else             putch(key);
	    }
	}
    normal();

    /* ·³bÐe •A·¡ÈáŸi ÑÁ¡e¬wµA¬á ÂÂ‰ */
    for (i=0 ; i<n ; i++)
	xy_vgets(x+MSG_POK,y+2+i*2, pok, data[i].str);

    /* ”ÑÁ¬w¸a ¦¦…· ¶¥œ· ¶w·i ¥¢¶¥ */
    puttext(x,y, x+MSG_POK+pok+5,y+n*2+6, buf);
    return 1;
    }
