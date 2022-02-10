#define IGNORE  0
#define RETRY   1
#define ABORT   2

static char *herr_msg[] = {
		"�a�� �w���Q",
		"���a�e �wá��",
		"�����a �e�Q",
		"���a�e �w�w��",
		"�A���a �A�ᷱ",
		"�i�� �w�s��",
		"�� �A�ᷱ",
		"���a�e �wá��",
		"�B��i ���x�q",
    "���� ���q",
		"�a�� �A��",
		"���� �A��",
		"�wá �A��",
		"��ѡ",
		"��ѡ",
    "�a�a���a �i��"
};

#pragma warn -par


int iErrorHandler(int errval,int ax,int bp,int si)
{
	 static char msg[80];
	 unsigned di;
	 int drive;
	 int errorno;
	 int err_flag=RETRY;

	 di= _DI;

	 if (ax < 0)
	 {
			vErrorMsgfDlg(0, 5, "�wá");
			hardretn(ABORT);
			return ABORT;
	 }
	 drive = ax & 0x00FF;
	 errorno = di & 0x00FF;
   sprintf(msg, "%c �A %s, ������?", 'A' + drive, herr_msg[errorno]);

	 err_flag=ihUserChoiceDlg(msg)+1;
	 if(err_flag==RETRY) {
		 hardresume(RETRY);
		 return RETRY;
	 }
	 else {
		 hardretn(ABORT);
		 return ABORT;
	 }

}

#pragma warn +par

int iCBreakHandler(void)
{
	return(1);
}

void vInitErrorHandler(void)
{
	ctrlbrk(iCBreakHandler);
	harderr(iErrorHandler);
}
