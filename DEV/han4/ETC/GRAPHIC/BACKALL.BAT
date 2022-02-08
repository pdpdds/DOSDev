@echo Insert Graphics Library Diskette in drive B.
@pause
cd b:\source
copy is6845.c b:
copy ismda.c b:
copy ishgc.c b:
copy iscga.c b:
copy isega.c b:
copy isvga.c b:
copy iset3000.c b:
copy iset4000.c b:
copy iswond.c b:
copy iswond1.c b:
copy iswond2.c b:
copy detadapt.c b:
copy detsvga.c b:
copy getvideo.c b:
copy setvideo.c b:
copy setgrhgc.c b:
copy setgrcga.c b:
copy setgrega.c b:
copy setgrvga.c b:
copy setgrsv.c b:
copy setgr.c b:
copy choosegr.c b:
copy getstd.c b:
copy globvar.c b:
copy initgr.c b:
copy closegr.c b:
copy setrgb.c b:
copy *.bat b:

cd b:\lib
copy graph?.lib b:
copy graph.h b:

cd b:\example
copy detect.c b:
copy *.prj b:
