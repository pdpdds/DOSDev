#define ASSEMBLING  	 0
#define ASSEMBLED   	 1
#define ASSEMBLE_ERROR  -1

//#define MAX_JAPAN_SORI  206

#define	JAPANLOC	(15)
typedef unsigned char  BYTE;
typedef unsigned short WORD;

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "japan_s.h"


typedef struct {
	int x;           
	int y;           
	BYTE lx;          
	BYTE ly;          
	int cx;
	int cy;
	int  cc;
	WORD cr;
	WORD cp;
	WORD total;
	WORD bl;
	BYTE modify;
	BYTE active;
	BYTE h_e;
	BYTE ins;
	BYTE scr_c;
	BYTE tab[30];

	BYTE word_gap; // gap between words;
	BYTE up_s;
	BYTE down_s;
	BYTE left_s;   // free space of the vLeftKey
	BYTE repeat;         // how many repeat printing
	BYTE prn_range[11];  // SNode one BYTE is index '0'= Full range '1'= Part
	BYTE prn_dens;

	char headline[80];
} text_window;

text_window tw[30];

BYTE cur_w;

static char japan_in_buf[4];
static char *japan_loc_p = japan_in_buf;

static int iSearchSori(BYTE *key, int count)
{
	int cond;
	int low = 0, high = MAX_JAPAN_SORI, mid;

  while (low < high) {
		mid = (low + high) / 2;

		cond = strncmp((const char*)key, JapanSoriTbl[mid].s, count);

		if (cond < 0) high = mid;
		else if (cond > 0) low = mid + 1;
		else return mid;
	}

	return -1;
}

void vJapanSetStart(void)
{
	memset(japan_in_buf, (int)NULL, sizeof(japan_in_buf));
	japan_loc_p = japan_in_buf;
}

int iJapanSoriAutomata(BYTE key, BYTE *value1, BYTE *value2)
{
	int n;

	if(key<65 || (key>90 && key<97) || key>122) {
		vJapanSetStart();
		return key;
	}
	*japan_loc_p++ = toupper(key);
	*japan_loc_p=0;
	*value1 = *value2 = 0;

	n = iSearchSori((BYTE*)japan_in_buf, strlen(japan_in_buf));
	if (n == -1) {
		vJapanSetStart();
		return ASSEMBLE_ERROR;
	}

	if (strlen(japan_in_buf) != strlen(JapanSoriTbl[n].s)) return ASSEMBLING;
	else if(tw[cur_w].h_e==JAPANLOC) {
			 *value1 = (int) JapanSoriTbl[n].c[0];
			 *value2 = (int) JapanSoriTbl[n].c[1];
	}
	else {
		*value1 = ((int) JapanSoriTbl[n].c[0]);
		if(value1==0) (*value1) += 83;
		*value2 = ((int) JapanSoriTbl[n].c[1]) + 83;
	}

	vJapanSetStart();
	return (JapanSoriTbl[n].c[0]) ? 2 : 1;
}

void vJapanInputState(void)
{
//	vWriteString(tw[cur_w].x+20, tw[cur_w].y+9, "   ", 0);
//	vWriteString(tw[cur_w].x+20, tw[cur_w].y+9, japan_in_buf, 0);
}
