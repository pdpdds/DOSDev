#define IGNORE  0
#define RETRY   1
#define ABORT   2

static char *herr_msg[] = {
		"³a‹¡ ¤w»¡–Q",
		"¡¡Ÿa“e ¸wÃ¡·±",
		"º…§¡ˆa ´e–Q",
		"¡¡Ÿa“e ¡ww·±",
		"•A·¡Èa µAœá·±",
		"ËiŸ¥ ·w”s·±",
		"ˆñ¬‚ µAœá·±",
		"¡¡Ÿa“e ¸wÃ¡·±",
		"­BÈáŸi ¡µÀx·q",
    "¹··¡ ´ô·q",
		"³a‹¡ µAœá",
		"·ª‹¡ µAœá",
		"¸wÃ¡ µAœá",
		"¥¡Ñ¡",
		"¥¡Ñ¡",
    "—aœa·¡§a ËiŸ±"
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
			vErrorMsgfDlg(0, 5, "¸wÃ¡");
			hardretn(ABORT);
			return ABORT;
	 }
	 drive = ax & 0x00FF;
	 errorno = di & 0x00FF;
   sprintf(msg, "%c µA %s, ¸¯¡•¡?", 'A' + drive, herr_msg[errorno]);

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
