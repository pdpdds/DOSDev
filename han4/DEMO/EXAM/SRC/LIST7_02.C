#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   pulldown_menu_struct menu_data =
   {
      10, 1,        /* ���A�A�� �����e 10������ �A���A�e 1�� �A�A�a ��Ȃ�E�a.*/
      { 1, 7, 15, 21, 27, 33, 39, 45, 51, 61 },      /* ���A�A 10���� x ���a */
      { " <�e> ", " ��A�� ", " �e�� ", " ���b ", " �x�� ",
        " ���e ", " �b ", " ���e ", " �b�����w ", " ��Ȃ�a�w ",
      },
      /* �� �����A�ᦁ�� ���A�A 10 ���a �����E�a. */
      { /* 0 �� �A�A(<�e>) */
        { 6, 0, 1, 1,                      /* (1, 1)�A 6���a�� ���A�A�i ���� */
          { " <�e> ���e    ",
            " �����i    F1 ",
            " �a�e���i @F1 ",
            " �i�b         ",
            " ���e��       ",
            " ������ѡ��   ",
          }
        },
        /* 1 �� �A�A(��A��) */
        { 7, 0, 7, 1,                      /* (7, 1)�A 7���a�� ���A�A�i ���� */
          { "�� �i       @N",
            "���ᵡ��    @O",
            "��w�a��    @S",
            "�����q�a��  @V",
            "�w���a����    ",
            "���a�w�w�� @F0",
            "�{          @X",
          }
        },
        /* 2 �� �A�A(�e��) */
        { 7, 0, 15, 1,                    /* (15, 1)�A 7���a�� ���A�A�i ���� */
          { " ��������   ^Z ",
            " ���a����   ^X ",
            " ���a       ^C ",
            " ������     ^V ",
            " ������     ^E ",
            " �i�� ���� ^KR ",
            " �i�� �a�� ^KW ",
          }
        },
        /* 3 �� �A�A(���b) */
        { 7, 0, 21, 1,                    /* (21, 1)�A 7���a�� ���A�A�i ���� */
          { " ���a      @F2 ",
            " �i�a����   @L ",
            " ���e����   @T ",
            " ��a����   @D ",
            " �w�w����З @I ",
            " ���a������ @B ",
            " �a��審З @Z ",
          }
        },
        /* 4 �� �A�A(�x��) */
        { 6, 0, 27, 1,                    /* (27, 1)�A 6���a�� ���A�A�i ���� */
          { " �x��     ^QF ",
            " �a����   ^GA ",
            " З�a��   ^QG ",
            " �A����    @G ",
            " ���e���a  ^G ",
            " ���a���A ^QS ",
          }
        },
        /* 5 �� �A�A(���e) */
        { 7, 0, 33, 1,                    /* (33, 1)�A 7���a�� ���A�A�i ���� */
          { " �១�i      ^NH ",
            " �����i      ^NF ",
            " �b��        ^NN ",
            " �a��        ^NI ",
            " �b�� ���a   ^NS ",
            " �A���� ���� ^NP ",
            " �A���� �a��  ^J ",
          }
        },
        /* 6 �� �A�A(�b) */
        { 2, 0, 39, 1,                    /* (39, 1)�A 2���a�� ���A�A�i ���� */
          { " ����        @P ",
            " �a���� ��� ^P ",
          }
        },
        /* 7 �� �A�A(���e) */
        { 7, 0, 45, 1,                    /* (45, 1)�A 7���a�� ���A�A�i ���� */
          { " �����a���� ^WV ",
            " ���w�a���� ^WH ",
            " ���e�h��   ^WC ",
            " �a�e���e    @W ",
            " �����a���� ^WX ",
            " �a������   ^WS ",
            " ���e��   ^WZ ",
          }
        },
        /* 8 �� �A�A(�b�����w) */
        { 3, 0, 51, 1,                    /* (51, 1)�A 3���a�� ���A�A�i ���� */
          { " ���a�e�i�� ",
            " �A�� �ỡ  ",
            " �� �a      ",
          }
        },
        /* 9 �� �A�A(��Ȃ�a�w) */
        { 8, 0, 61, 1,                    /* (61, 1)�A 8���a�� ���A�A�i ���� */
          { " �a�� ������   ",
            " ���q ������   ",
            "  \x14  ������    ",
            " ��w �e�b  30 ",
            " ȓ���        ",
            " ���w��        ",
            " ��  ��        ",
            " ���� �e�a��   ",
          }
        },
      }
   };
   char key;                                           /* �a���b�A ���a�� ǡ */
   int  main_menu, sub_menu;                       /* ��Ȃ�E ���A�A�� ���A�A */

   if(init_han() != OK_HAN) exit(1);

   aputs_han(33, 19, "�A�A�i  ��Ȃ�a�A�a", FONT_SHADE);
   while(1)
   {
      /* �� �����A�� Ή�a�� �A�A�a �����E�a. */
      key = pulldown_han(&menu_data);

      if(key == ESC) break;                  /* <Esc> ǡ�i ���ᕡ �{�� �e�a. */
      /* ��Ȃ�E ���A�A�e main_menu �A, ���A�A�e sub_menu�A ��w�e�a. */
      main_menu = menu_data.current;
      sub_menu  = menu_data.sub_menu[main_menu].current;

      /* �� �����A�� '�{' ���A�� �A�A�� �q�A �១Ё ���e �E�a. */
      switch(main_menu)
      {
         case 1 : switch(sub_menu)
                  {
                     /* ���A�A�e 1, ���A�A�e 0 ('�{' �A�A)���e �{�i ���a.*/
                     case 6 : close_han();
                              exit();
                  }
      /* case 2 : switch(sub_menu)
           :
      */
      }
      nprintf_han(20, 20, 40, "<%s> �A�A�i ��Ȃ�a�v�s���a",
                  menu_data.sub_menu[main_menu].string[sub_menu]);
   }

   close_han();
}
