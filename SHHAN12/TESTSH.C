#include <stdio.h>
#include <graphics.h>
#include "shhan.h"
main()
{
int gd=0,gm,i;
char str[150];
initgraph(&gd,&gm, "");
/* VGA�e gm�� 0���a�� �a�� ���b�i ϩ�a ���q
�a��a herc�e 640*400�� �w���e gm�i 1�� 720*348�� �w��
�e gm�i 0�a�� �x�ẅ�a.  */
if (gd!=9) gm=0;
shset(gd,gm);
setfillstyle(1,1);
bar(0,0,639,479);
shhan(10,10,"�e�i �a���a�១ test",15,1,0,0);
shhan(10,50,"�A�b�a:�wӁ���b�a ���A�� �e���A COM.COM �b�����w",15,1,0,0);
shhan(10,70,"�A�b���:full Assembler",15,1,0,0);
shhan(10,90,"�A�b����:1993�e 9��",15,1,0,0);
shhan(10,110,"���� �a��Ϣ �a�a����:���I���a,VGA,EGA",15,1,0,0);
shhan(10,130,"�������:Turbo C 2.0 , C++",15,1,0,0);
shhan(10,150,"���� memory model:large model",15,1,0,0);
shhan(10,170,"�a���a�១ �a��:ͥ�a �a�A ͡�q 10000 byte",15,1,0,0);
shhan(10,230,"�� �a���a�១�e �Ÿ��e ���� ���a�a ���ᷡ�a�� �a���a ���a�i �� ��",15,1,0,0);
shhan(10,250,"�a�a �w��w ���a�a���� �A�b�A�� ��b�a�� ��a���� �a�w�i �� ���s",15,1,0,0);
shhan(10,270,"���a.������ ���a�a�� �ťb�a�A �����a�a ����,���b�i �� ���s���a.",15,1,0,0);
shhan(10,290,"�e �� �a���a�១ �a�A�i �wΑ�� �i ���e ���s���a.",15,1,0,0);
shhan(10,350,"�a�A�a �A��ǡ ʁ ��ˡ�A ",15,1,0,0);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"shhan library�i �a�w�a�� ��Ё��e shhan.h�i �e�a�� ͡�q���a��",15,1,0,0);
shhan(10,30,"�s���a.",15,1,0,0);
shhan(10,50,"shhan.h�A�e �q���� ���w�� �i�᷶�s���a.",15,1,0,0);
shhan(10,70,"������ �q��:shset",15,1,0,0);
shhan(10,90,"���w: void shset(int gd,int gm);",15,1,0,0);
shhan(10,110,"initgraph�q�� �a�� ��A �� �q���i ѡЁ �������a",15,1,0,0);
shhan(10,130,"gd:�a��Ϣ �a�a����i �����s���a.7���e HGC 9�e VGA�� �����s���a.",15,1,0,0);
shhan(10,150,"gm:�a��Ϣ ���a�i �����s���a.0:720*348  1:640*400",15,1,0,0);
shhan(10,170,"������ �t�i ������ initgraph�� ������ �t�� ��á���a �����a�a�� ",15,1,0,0);
shhan(10,190,"initgraph�A�� ���a�e gd,gm�i �a���� �a�w�i �� ���s���a.",15,1,0,0);
shhan(10,210,"�e HGC�� �w�� gm�t�i ��wӡ ���Ё ���ᴡ �q",15,1,0,0);
shhan(10,250,"�a�w��)",15,1,0,0);
shhan(10,270,"int gd=DETECT,gm;",15,1,0,0);
shhan(10,290,"initgraph(&gd,&gm,\"\");",15,1,0,0);
shhan(10,310,"shset(gd,gm);",15,1,0,0);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"�b �q��:shhan",15,1,0,0);
shhan(10,70,"���w: void shhan(int x,int y,char *str,int fcolor,int bcolor",15,1,0,0);
shhan(10,90,"                  ,int attr,int put",15,1,0,0);
shhan(10,110,"x,y:���a�i �b ��á ����,�a��Ϣ ���a��",15,1,0,0);
shhan(10,130,"str:�b�i ���a�i",15,1,0,0);
shhan(10,150,"fcolor,bcolor:��w��,���w��",15,1,0,0);
shhan(10,170,"attr:���� ����",15,1,0,0);
shhan(10,190,"put:�i�a�i ���e�A ���i���� bit�e�e ����",15,1,0,0);
shhan(10,250,"�a�w��)",15,1,0,0);
shhan(10,270,"shhan(10,30,\"������ �a��\",15,1,0,0);",15,1,0,0);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"��A�e ���a �A�� �a�w ",15,1,0,0);
for(i=20;i<55;i++)
shhan(10+(i-20)*16,20+i,"�w",15,1,0,0);
shhan(10,100,"��w�� ���w�� ����(HGC�e ���w���� ���a)",15,1,0,0);
shhan(10,120,"�wӁ���b�a ���A�� �e���A COM.COM",14,4,0,0);
shhan(10,140,"�wӁ���b�a ���A�� �e���A COM.COM",4,14,0,0);
shhan(10,160,"�wӁ���b�a ���A�� �e���A COM.COM",1,4,0,0);
shhan(10,180,"�wӁ���b�a ���A�� �e���A COM.COM",7,8,0,0);
shhan(10,200,"�wӁ���b�a ���A�� �e���A COM.COM",15,0,0,0);
shhan(10,220,"�wӁ���b�a ���A�� �e���A COM.COM",13,1,0,0);
shhan(10,240,"�wӁ���b�a ���A�� �e���A COM.COM",12,5,0,0);
shhan(10,260,"�wӁ���b�a ���A�� �e���A COM.COM",6,8,0,0);
shhan(10,280,"�wӁ���b�a ���A�� �e���A COM.COM",2,3,0,0);
shhan(10,300,"�wӁ���b�a ���A�� �e���A COM.COM",11,4,0,0);
shhan(10,320,"�a��Ϣ���� ѥ�w",15,1,0,0);
for(i=0;i<200;i+=10)
circle(320,200,i);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"���b �q��:shget",15,1,0,0);
shhan(10,70,"���w: void shget(int x,int y,char *str,int fcolor,int bcolor",15,1,0,0);
shhan(10,90,"                  ,int limit,int keep",15,1,0,0);
shhan(10,110,"x,y:���a�i �b ��á ����,�a��Ϣ ���a��",15,1,0,0);
shhan(10,130,"str:�b�i ���a�i",15,1,0,0);
shhan(10,150,"fcolor,bcolor:��w��,���w��",15,1,0,0);
shhan(10,170,"limit:�A�� ���b ���a��",15,1,0,0);
shhan(10,190,"keep  0:����i ������ ���b �h�q",15,1,0,0);
shhan(10,210,"      1:����i �A���a�� ���b �h�q",15,1,0,0);
shhan(10,250,"�a�w��)",15,1,0,0);
shhan(10,270,"char str[20];",15,1,0,0);
shhan(10,290,"shget(10,310,str,15,1,20,0);",15,1,0,0);
shget(10,310,str,15,1,20,0);
shhan(10,330,str,15,1,0,0);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"���b ���a �A�� �q��:shmode",15,1,0,0);
shhan(10,70,"���w: void shmode(int mode);",15,1,0,0);
shhan(10,110,"mode  0:�e�i ���b ���a",15,1,0,0);
shhan(10,130,"      1:�w�� ���b ���a",15,1,0,0);
shhan(10,150,"���b���A mode�i �a���a�e shift+space�i ���� �������a",15,1,0,0);
shhan(10,170,"�e�i ���a�� ���b�S���a.",15,1,0,0);
shmode(0);
shget(10,190,str,15,1,20,0);
shhan(10,210,"�w�� ���a�� ���b�S���a.",15,1,0,0);
shmode(1);
shget(10,230,str,15,1,20,0);
getch();
cleardevice();
bar(0,0,639,479);

shhan(10,10,"�i �e�i�� ���a�� Ж�e�A �i�� ��Α�� ���A�a",15,1,0,0);
shhan(10,50,"�寡�A�a �s�bЁ�� ���qЁ�� 1.0�i �a�����s���a.",15,1,0,0);
shhan(10,90,"���b���a ���b�� �a���� �{�e�A �w�񷡜a�e �a���a �b��",15,1,0,0);
shhan(10,130,"�a�w�� �e�e�a�a�e �������a.",15,1,0,0);
shhan(10,170,"���e�� ��b�a�e�A�� ���� ���e �a���a�១�i �A�bЁ ���V�s���a.",15,1,0,0);
shhan(10,210,"���a�i ����Ё ���v�a�� �ů����e ���e �e��Ё ������ �a�s���a.",15,1,0,04);
shhan(50,250,"�q�a�s���a.",15,1,0,0);
getch();
closegraph();
}
