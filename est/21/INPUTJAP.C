#define ASSEMBLING  0
#define ASSEMBLED   1
#define ASSEMBLE_ERROR    -1

#define MAX_JAPAN_SORI  206

#include "japan_s.h"
static char japan_in_buf[4];
static char *japan_loc_p = japan_in_buf;

static int near iSearchSori(BYTE *key, int count)
{
	int cond;
	int low = 0, high = MAX_JAPAN_SORI, mid;

  while (low < high) {
		mid = (low + high) / 2;

		cond = strncmp(key, JapanSoriTbl[mid].s, count);

		if (cond < 0) high = mid;
		else if (cond > 0) low = mid + 1;
		else return mid;
	}

	return -1;
}

void vJapanSetStart(void)
{
	memset(japan_in_buf, NULL, sizeof(japan_in_buf));
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

	n = iSearchSori(japan_in_buf, strlen(japan_in_buf));
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
	vWriteString(tw[cur_w].x+20, tw[cur_w].y+9, "   ", 0);
	vWriteString(tw[cur_w].x+20, tw[cur_w].y+9, japan_in_buf, 0);
}
