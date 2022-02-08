/***************************************************/
/* 16 color pcx file�i ����� graphic ���e�A �b  */
/* �a���A�� �A�b�E readpcx16p()�q���� �a ���� �q�� */
/* �i�e �a�e program�A �a�a �a�ᕡ �a���� �������� */
/* �a�w�i �� ������ �e�i���q.                      */
/* �A�b �e��:1993�e 9��. ���� ����:1994�e 5�� 10�� */
/* �wӁ���b�a ���A�� �e���A COM.COM 3�� �� �w �w . */
/***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>

void readpcx16p(char *,int,int);
void readline16p(FILE *,int,int,int);
void readpalette16(char *);

typedef struct {
   char manufacturer;   /* pcx file �·� 0x0a */
   char version;        /* ����ѡ 0x05�a 5.0 256�i�a */
   char encoding;       /* ��š�� �w�� 0x01-RLL ��š�� */
   char bpp;            /* Ϣ�I�w bit��   */
   int xmin,ymin;       /* �a���� �w�a���� �a�� */
   int xmax,ymax;
   int hres;            /* Ё�w��-���a �����a ���q */
   int vres;
   char palette[48];    /* �i�A�a ���� */
   char reserved;       /* ����        */
   char colour_planes;  /* �i�A�� ���� HGC=1,VGA=4 */
   int bpl;             /* ���w byte �� */
   int palette_type;
   char filler[58];     /* ���� �w�b */
} PCXHEAD;

void readpcx16p(char *filename,int x,int y)
{
   FILE *fp;
   PCXHEAD hd;
   unsigned int width,depth;
   unsigned int bytes;
   char palette[768];
   int i,j;

   fp=fopen(filename,"rb");

   if (fp==NULL){
       printf("file open Error\n");
       exit(0);
   }

   fseek(fp,-769l,SEEK_END);

   if(fgetc(fp)==0x0c && fread(palette,1,768,fp)== 768) {
      printf("not 16 color,but 256 color\n");
      exit(0);
   }

   fseek(fp,0l,SEEK_SET);
   fread(&hd,1,128,fp);

   if(hd.manufacturer==0x0a && hd.bpp==0x01) {
     if (hd.version!=0x03) readpalette16(hd.palette);
     fseek(fp,128,SEEK_SET);
     for(i=0;i<hd.ymax-hd.ymin+1;i++) readline16p(fp,hd.bpl,x,y+i);
   }
   return;
}

void readpalette16(char *pal_buf)
{
   int i;
   struct REGPACK reg;

   for(i=0;i<48;i++) pal_buf[i]>>=2;
   for(i=0;i<16;i++) {
      reg.r_ax=0x1000;
      reg.r_bx=i*256+i;
      intr(0x10,&reg);
   }
   reg.r_ax=0x1012;
   reg.r_bx=0;
   reg.r_cx=16;
   reg.r_es=FP_SEG(pal_buf);
   reg.r_dx=FP_OFF(pal_buf);
   intr(0x10,&reg);
   return;
}

void readline16p(FILE *fp,int len,int x,int y)
{
   int i,j,k,n;
   char temp[1040];
   char col[1040];

   n=0;
   do {
      i=fgetc(fp);
      if((i&0xc0)==0xc0) {
	 i=i&0x3f;
	 j=fgetc(fp);
	 while(i--) temp[n++]=j;
      } else temp[n++]=i;
   } while (n<len*4);

   for(i=0;i<640;i++) col[i]=0;
   for(i=0;i<4;i++)
      for(j=0;j<len;j++)
	 for(k=7;k>=0;k--)
	    if(temp[j+i*len]&(1<<k)) col[j*8+7-k]|=(1<<i);
   for (i=0;i<len*8;i++) putpixel(x++,y,col[i]);
   return;
}

