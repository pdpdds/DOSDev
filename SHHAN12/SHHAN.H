#ifndef __LARGE__
#error this library must compile in large model
#endif

void shhan(int x,int y,char *str,int fcolor,int bcolor
            ,int attr,int put);
void shset(int gd,int gm);
void shmode(int mode);
void shget(int x,int y,char *str,int fcolor,int bcolor
            ,int limit,int keep);
