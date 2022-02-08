


 ¸÷ÑeÈ‚   (helpme  )
[!] 733¤å ·¡œå‰w¶µA •¡¶‘·¡ –A´ö·a¡e...      11/23 00:08   152 line

  ´aœ ¢…¬á ÑÁ·©·i ·ª´á¬á ÑÁ¡eµA¬á Îa¯¡Ða‰¡ 
  
  ¯¼·e¦…·¡ ·¶‹¡µA •¡¶‘·¡ –A´ö·a¡e Ða‰¡ µ©Ÿ±“¡”a.
  
  ´aœ Î‰‹aŸ±·¡ µÅ¥bÐi»¡“e ¸i¡¡Ÿa‰V»¡ e ´a a
  
  ‹aœâ¸áœâ ¸b•··¡ –IˆáµA¶a.. Ó¡...
  
  Àq, ‹aŸ¡‰¡ Ìa·© ‹©·¡ˆa 98Line·¡ ñ·i ‰a¶µA“e
  
  MAX_LINE·i ®¸÷Ða¯¡‰¡¶a ÐeLine· ‹©·¡ˆa 100¸a »b
  
  Ðe‹i·e 50¸aŸi ñ·i ‰w¶µA“e MAX_CHAR·i ‰¡Áaº­A¶a.
  
  ‹aœñ, ¸i ¬a¶wÐa¯¡‹¡Ÿi...

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #include "hginit.h"
 #include "hghlib.h"
 #include "key.h"
 #include "hgvir.h"

 #define MAX_LINE 100
 #define MAX_CHAR 100

void move_str(char *tmp,char *in,int st,int len )
{   int i,j;

    if (strlen(in)<1)
    {  strcpy(tmp,""); return; }
    for (i=0;i<len;i++)
        *(tmp+i) = *(in+i+st);
    *(tmp+i) = '\0';
}

void read_me( char *fn )
{
   FILE *fi_na;
   int x1=0,y1=5*16,x2=79*8,y2=25*16;
   char tmp[MAX_CHAR+2],*ip[MAX_LINE];
   int i,j,count,k=0,ch,with,rt=0,xp=0;
   VIMAGE *pic;

   if ( (fi_na=fopen(fn,"rb"))==NULL ) return;

   hgGetImageVIRTUAL( x1,y1,x2,y2,&pic );
   hgPrtWindowXy( x1,y1,x2,y2,"Read Me" );

   i=0;
   while (1)
   {   if ( fgets( tmp,MAX_CHAR,fi_na )==NULL ) break;
       ip[i] = (char *)malloc(strlen(tmp)+2);
       if (i>MAX_LINE-2) break;
       if (ip[i]==NULL)
       {  free(ip[i-1]); i--; break;
       }
       tmp[strlen(tmp)-2] = '\0';
       if (rt<strlen(tmp)) rt=strlen(tmp);
       strcpy(ip[i++],tmp);
   }
   ip[i] = (char *)malloc(3);
   strcpy(ip[i],"");
   fclose( fi_na );

   count=(y2-y1-48)/16;
   if (i<count) count=i;
   with =(x2-x1-29)/8;
   rt -= with;
   if (rt<0) rt=0;
   for(j=0;j<count;j++)
   {  strcpy(tmp,ip[j]);
      if ( strlen(tmp)>with ) tmp[with]='\0';
      hgOutTextXy( x1+16,y1+(j+2)*16+3,tmp );
   }

   while(1)
   {   ch = inkey(WAIT);
       if (ch==UP)
       {  k--;
          if (k<0) k=0;
          else
          {  hgScrDown(x1+16,y1+35,x2-13,y2-16,16,WHITE);
             move_str(tmp,ip[k],xp,with);
             if ( strlen(tmp)>with ) tmp[with]='\0';
             hgOutTextXy( x1+16,y1+2*16+3,tmp );
          }
       }
       else if (ch==DOWN)
       {  k++;
          if (k>(i-count)) k=i-count;
          else
          {  hgScrUp(x1+16,y1+35,x2-13,y2-16,16,WHITE);
             move_str(tmp,ip[k+count-1],xp,with);
             if ( strlen(tmp)>with ) tmp[with]='\0';
             hgOutTextXy( x1+16,y1+(count+1)*16+3,tmp );
          }
       }
       else if (ch==LEFT)
       {  xp--;
          if (xp<0) xp=0;
          else
          {  hgBoxFill(x1+16,y1+35,x2-13,y2-16,WHITE);
             for(j=0;j<count;j++)
             {  move_str(tmp,ip[j+k],xp,with);
                if ( strlen(tmp)>with ) tmp[with]='\0';
                hgOutTextXy( x1+16,y1+(j+2)*16+3,tmp );
             }
          }
       }
       else if (ch==RIGHT)
       {  xp++;
          if (xp>rt) xp=rt;
          else
          {  hgBoxFill(x1+16,y1+35,x2-13,y2-16,WHITE);
             for(j=0;j<count;j++)
             {  move_str(tmp,ip[j+k],xp,with);
                if ( strlen(tmp)>with ) tmp[with]='\0';
                hgOutTextXy( x1+16,y1+(j+2)*16+3,tmp );
             }
          }
       }
       else if (ch==HOME)
       {  k=0;
          hgBoxFill(x1+16,y1+35,x2-13,y2-16,WHITE);
          for(j=0;j<count;j++)
          {  move_str(tmp,ip[j],xp,with);
             if ( strlen(tmp)>with ) tmp[with]='\0';
             hgOutTextXy( x1+16,y1+(j+2)*16+3,tmp );
          }
       }
       else if (ch==END)
       {  k=i-count;
          hgBoxFill(x1+16,y1+35,x2-13,y2-16,WHITE);
          for(j=0;j<count;j++)
          {  move_str(tmp,ip[j+k],xp,with);
             if ( strlen(tmp)>with ) tmp[with]='\0';
             hgOutTextXy( x1+16,y1+(j+2)*16+3,tmp );
          }
       }
       else if(ch==ESC) break;
   }

   for( j=0;j<i+1;j++ )
      free( ip[j] );
   hgPutImageVIRTUAL( x1,y1,x2,y2,pic );
   hgFreeVIMAGE ( &pic );

}
.
 PRINTER/CAPTUREŸi OFF Ða¯¡‰¡ EnterŸi ’Ÿa¯³¯¡µ¡.
