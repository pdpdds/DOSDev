#include "byte.h"
#include "dos.h"

void smemtosmem(dest,sorc,rep_num)
BYTE *dest,*sorc;
unsigned rep_num;
{
    memtomem(FP_SEG(dest),FP_OFF(dest),FP_SEG(sorc),FP_OFF(sorc),rep_num,1);
}
