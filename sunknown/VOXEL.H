// VOXEL.H

#ifndef _VOXEL_H
#define _VOXEL_H

#include "global.h"

#define CAMZ      4485.0
#define CAMX  		4230.0
#define MAXZ		  3650.0

void voxel(void);
void visvox(byte *page,float camv, word pageseg,int step);

#endif