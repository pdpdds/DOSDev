@echo Insert Graphics Library Diskette in drive B.
@pause
cd b:\pcx
copy decopcx.c b:
copy pcxhgc.c b:
copy pcxvga.c b:

copy pcx?.lib b:
copy pcx.h b:

copy viewpcx.* b:
copy *.bat b:
cd b:\
