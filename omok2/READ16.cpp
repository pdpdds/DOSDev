/***************************************************/
/* 16 color pcx file·i ·ª´á¬á graphic ÑÁ¡eµA Â‰b  */
/* µa‹¡µA¬á ¹A¸b–E readpcx16p()Ðq®µÁ ‹a ¦­¢ Ðq® */
/* —i·e ”aŸe programµA ˆa¹a ˆa¬á•¡ ‹a”¡ ®¸÷´ô·¡ */
/* ¬a¶wÐi ® ·¶•¡¢  e—i´ö·q.                      */
/* ¹A¸b ‘e•¡:1993‘e 9¶©. ®¸÷ ·©¯¡:1994‘e 5¶© 10·© */
/* ‰wÓ”ÐbŠa ÄñÏAÈá µeŠÒA COM.COM 3‹¡ ‹± ¬w Ñw . */
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
   char manufacturer;   /* pcx file ÑÂ·¥ 0x0a */
   char version;        /* ¤á¸å¤åÑ¡ 0x05ˆa 5.0 256Äiœa */
   char encoding;       /* ·¥Å¡—· ¤w¤ó 0x01-RLL ·¥Å¡—· */
   char bpp;            /* Ï¢­I”w bit®   */
   int xmin,ymin;       /* ‹aŸ±· ¬wÐa¹Á¶ Ça‹¡ */
   int xmax,ymax;
   int hres;            /* Ð¬w•¡-¸åÑa ·£¡ˆa ´ô·q */
   int vres;
   char palette[48];    /* ÌiAËa ¸÷¥¡ */
   char reserved;       /* µ´¢        */
   char colour_planes;  /* ÏiA·¥ ˆ•® HGC=1,VGA=4 */
   int bpl;             /* º‰”w byte ® */
   int palette_type;
   char filler[58];     /* µ´¢ µwµb */
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

