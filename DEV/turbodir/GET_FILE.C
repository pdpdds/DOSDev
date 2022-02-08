 #include <dos.h>
 #include <dir.h>
 #include <string.h>
 #include <stdio.h>
 #include <conio.h>
 #include <stdlib.h>

 char far *base_mem;
 static struct dirfile {
	char filename[13];
	int  attrib;
} FILE_dir [300];

static int total, first;


 #include "key.h"
 #include "vid_mode.c"
 #include "vxy_cls.c"
 #include "vsavescr.c"
 #include "vrestscr.c"
 #include "vputchar.c"
 #include "vput_str.c"
 #include "vxy_box.c"
 #include "goto_xy.c"
 #include "get_key.c"
 #include "vget_str.c"
 #include "vputattr.c"
 #include "scroll.c"
 #include "vput_spc.c"
 #include "find_dir.c"
 #include "get_dir.c"

 int dir_box(void);

 void main(void)
 {
	video_mode();
	dir_box();
 }

 int dir_box(void)
 {
	unsigned char *buf;
	static char path[80];  /* 최대경로명 */
	char *str;
	int rtn;

	getcwd(path,68); /* 드라이브명과 경로명을 읽어서 path */

	buf=save_screen(19,10,62,12,7<<4|3);
	if(!buf) exit(1);

	xy_box(19,10,62,12,7<<4|0);
	put_str(31,10," 읽을 파일의 이름은? ",7<<4|1);
	put_attr(21,11,40,3<<4|0);
	for(;;) {
		strcat(path,"\\*.*");
		rtn=get_str(21,11,path,40,3<<4|0);
		if(rtn==ENTER || rtn==ESC)
			break;
	}
	restore_screen(19,10,62,12,buf);
	if(rtn==ESC) return ESC;

	str=strupr(path);		/* 문자열을 대문자로 변환 */
	strcpy(path,str);

	get_directory(path);
 }
