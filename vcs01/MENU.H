/*************************************************************/
/* menu.h : x_menu(), y_menu(), dialog_box() �q�� ����       */
/*************************************************************/

#include  "chulryuk.h"   /* normal(), reverse(), set_attrib()*/
#include  "ibryuk.h"     /* get_key(), xy_vgets()            */

/*************************************************************/
/* �q���t : ��Ȃ�E �A�A�A ���e ���a(���弁->0, ���弁->2,...)*/
/* Esc ǡ       : �q�� ���a, �q���t => -1                    */
/* flag[0]=='L' : LEFT/RIGHT ǡ�i ���e�w�� ��З���a.         */
/*                �q���t�e LEFT/RIGHT(331/333)               */
/* flag[0]!='L' : LEFT/RIGHT ǡ�i ���e�w�� �� �����e �����a  */
/* flag[1]=='C' : �q�� ��З ���a�� �A�A�e�i �����a(Clear)    */
/* flag[1]!='C' : �q�� ��З ���a�� �A�A�e�i ������ �g�e�a    */
/*************************************************************/
int y_menu(int x, int y, int suntaek, char *flag, char **menu, int chuga)
    {
    int    n     = 0;    /* menu�� �w�� �����a ��Ё���a      */
    int    chdae = 0;    /* menu�� �w���� �A�� ������ ���a�� */
    char   buf[4000];    /* �A�A�e ������ ���� ���w ���Ŷw   */
    int    i;

       set_cursor(32,0);
    
    /*  menu �w���� ����(n), �A�� ����(chdae)�i ���e�a */
    while (menu[n][0] != '.')
	{
	if (strlen(menu[n]) > chdae)
	    chdae = strlen(menu[n]);
	n++;
	}

    /* �A�A�e�a�� �a�w�i �a�b�w ������ ���� ���w�i ���� */
    gettext(x, y, x+chdae+6,y+2+n, buf);

    /* �A�A�e�a�� �a�w�i �a�b�w �����e�i �����a */
    window(x+2, y+1, x+chdae+6,y+2+n);
    clrscr();
    window(x ,y ,x+chdae+3,y+1+n);
    textattr(0x70);
    clrscr();
    window(1,1, 80,25);
    single_box(x, y, x+chdae+3,y+1+n);
   
   for (i=0 ; i<n ; i++)
        xy_puts(x+3, y+1+i, menu[i]);

    /* ����A ��Ȃ�E menu �w���i �e�� �wȁ�� �a�� */
    xy_puts(x+3, y+1+suntaek, menu[suntaek]);
    set_attrib(x+1, x+chdae+2, y+1+suntaek, 0x2f);

    /* �E��/ESC ǡ�i ���a�e ���a�E�a */
    while ( 1 )
	{
	  if(chuga==0 && suntaek==0)
           xy_puts(5,25,"��  ��������(person2.dat)�� ���� ���ο� ������ �߰�");
	   else if(chuga==0 && suntaek==1)
            xy_puts(5,25,"��  ������ü�� �˻�,����                           ");
	    else if(chuga==0 && suntaek==2)
             xy_puts(5,25,"��  ���ڵ��ȣ(�� ���ڵ��� ����)�� ����            ");
	     else if(chuga==0 && suntaek==3)
              xy_puts(5,25,"��  ���ϴ� ���ڵ� �ڿ� ���ο� ���� ����            ");
	      else if(chuga==0 && suntaek==4)
               xy_puts(5,25,"��  A: ����̺�� ��������(person2.dat)�� ����     ");
	       else if(chuga==0 && suntaek==5)
                xy_puts(5,25,"��  The End                                        ");
          if(chuga==1 && suntaek==0)
           xy_puts(5,25,"��  ��ϵ� ��� ������ �����ݴϴ�.    ");
          else if(chuga==1 && suntaek==1)
           xy_puts(5,25,"��  ���ڸ�  �����ݴϴ�.               ");  
           else if(chuga==1 && suntaek==2)
            xy_puts(5,25,"��  ���ڸ� �����ݴϴ�.                ");

          if(chuga==2 && suntaek==0)
           xy_puts(5,25,"��  �������� �˻����ݴϴ�.                                ");
           else if(chuga==2 && suntaek==1)
            xy_puts(5,25,"��  �������, �̸��� '�浿'�� ����� ã�´ٵ���             ");
            else if(chuga==2 && suntaek==4)
             xy_puts(5,25,"��  �������, 'ģ��'�� �˻��Ѵٵ���, '����'�� �˻��Ѵٵ���");
             else if(suntaek==7)
              xy_puts(5,25,"��  �������, '����'�� ��� ����� �˻��� ��              ");
              else if(suntaek==8)
               xy_puts(5,25,"��  �������, '���ϱ�'�� ��� ����� �˻��� ��            ");
	       else if(suntaek==11)
                xy_puts(5,25,"��  �������, ������ '����'�� ����� �˻��� ��            ");
                else if(suntaek==12)
                 xy_puts(5,25,"��  �������, ������ '���ϱ�'�� ����� �˻��� ��          ");
                 else if(chuga==2)
                  xy_puts(5,25,"��  ���뼺�ִ� �˻��� ���ݴϴ�.                            ");
          if(chuga==3 && suntaek==0)
           xy_puts(5,25,"��  �ѹ� �о� ������.");
           else if(chuga==3 && suntaek==1)
            xy_puts(5,25,"��  ��Ҹ�           ");

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
/* �q���t : ��Ȃ�E �A�A�A ���e ���a(���弁->0, ���弁->2,...)*/
/* Esc ǡ       : �q�� ���a, �q���t => -1                    */
/* flag[0]=='D' : DOWN ǡ�i ���e�w�� ��З ���a.              */
/* flag[0]!='D' : DOWN ǡ�i ���e�w�� �� �����e �����a        */
/* flag[1]=='S' : �A�A�e�i �a��ҁ �A�A�i ��Ȃ�a�A �e�a       */
/* flag[1]!='S' : �A�A�e�i �a���e �eҁ ��З ���a             */
/*************************************************************/
int x_menu(int x, int y, int suntaek, char *flag, char **menu)
    {
    int    n     = 0;        /* menu�� �w�� �����a ��Ё���a  */
    int    chdae = 0;        /* menu�� �w���� �A�� ����      */
    int    x_jwapyo[20];     /* �b menu �w���i �a���i x ���a */
    int    pok;              /* �b �w���� �A�AЗ �w�A�᷁ ͢ */
    int    i, giri;

     set_cursor(32,0);

    /* menu �w���� ����(n), �A�� ����(chdae)�i ���e�a */
    while (menu[n][0] != '.')
	{
	if ( (giri=strlen(menu[n])) > chdae )
	    chdae = giri;
	n++;
	}
    pok = chdae+1;

    /* �A�AЗ�A �����i �a���a */



    /* �b �A�A �w���i �a�� */
    for (i=0 ; i<n ; i++)
	{
	giri        = strlen(menu[i]);
	x_jwapyo[i] = x+pok*i+(pok-giri)/2;
	xy_puts(x_jwapyo[i], y, menu[i]);
	}

    if (suntaek == -1)  return (-1);

    /* ����A ��Ȃ�E menu�w���i �e�� �wȁ�� �a�� */
    textattr(0xcf);
    xy_puts(x_jwapyo[suntaek], y, menu[suntaek]);
    normal();

    if (flag[1] != 'S')
      { set_cursor(12,13); return ( suntaek ); }

    /* �E��/ESC ǡ�i ���a�e ���a�E�a */
    while ( 1 )
	{
	  textattr(0x70);
          if(suntaek==0)
            xy_puts(5,25,"��  ������ ���,�˻�,����,����        ");
          else if(suntaek==1)
            xy_puts(5,25,"��  â�� ����� ������ �� �� �ֽ��ϴ�.");
          else if(suntaek==2)
            xy_puts(5,25,"��  �� �ʵ庰�� ���뼺�ִ� �˻�       ");
          else if(suntaek==3)
            xy_puts(5,25,"��  ������ ��Ҹ�                   ");
            
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
/* �����w�a�i �a���a�� data[i].str�� �t�i ���b�h�e�a              */
/* �q���t : ���w���a=>1,  Esc ǡ�� ���a=>-1                       */
/* (x, y) : �����w�a ���b��, n : �A���� ����, pok : �A���� �A��͢ */
/******************************************************************/
#define  MSG_POK  18          /* �A�A���� ͢                      */

struct  daehwa {              /* �����w�a���A�᷁ ���b�w �����A */
	char   *msg;          /*                    �a���i �A�A�� */
	char    str[30];      /*                  ���b�h�i �A���� */
      };
/*----------------------------------------------------------------*/
int dialog_box(int x,int y,int n, int pok, struct daehwa *data)
    {
    int     wichi   = 0;   /* ���a ��á�e ϩ�a ��ѡ(0,1,...n-1) */
    int     munjasu = 0;   /* �A���� ���b ϩ�a�A ���b�E ���a��    */
    char    buf[4000];     /* �����w�a ������ ���� ���w ���Ŷw    */
    int     i, key;

    /* �����w�a�� �a�w�i �a�b�w ������ ���� ���w�i ���� */
    gettext(x,y, x+MSG_POK+pok+5,y+n*2+6, buf);

    /* �����w�a�� �a�w�i �a�b�w �����e�i �����a */
    window( x,y, x+MSG_POK+pok+5,y+n*2+6);
    normal(); clrscr();
    window( 1,1, 80,25);

    /* �����w�a�i �a�� */
    form_window(x,y, x+MSG_POK+pok+5, y+n*2+6, n*2+2, "�A���ᷳ�b");
    xy_puts(x+2+pok/2, y+n*2+3, "��� ���� : ���i�a ǡ");
    xy_puts(x+2+pok/2, y+n*2+4, "���b ���a : Enter  ǡ");
    xy_puts(x+2+pok/2, y+n*2+5, "��З �᭡ : Esc    ǡ");

    for (i=0 ; i<n ; i++)             /* �A�A�� / ���b ϩ�a  �a�� */
	{
	xy_puts(x+4, y+2+i*2, data[i].msg);
	set_attrib(x+MSG_POK, x+MSG_POK+pok-1, y+2+i*2, 0x70);
	}

    /* �A����i ���b�h�e�a */
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

    /* ���b�e �A����i ���e�w�A��  */
    for (i=0 ; i<n ; i++)
	xy_vgets(x+MSG_POK,y+2+i*2, pok, data[i].str);

    /* �����w�a ������ ������ ���w�i ���� */
    puttext(x,y, x+MSG_POK+pok+5,y+n*2+6, buf);
    return 1;
    }
