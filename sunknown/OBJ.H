// OBJ.H

#ifndef _OBJ_H
#define _OBJ_H

#include "rcglobal.h"

struct obj_S
{
	int size,skx,sky,dzpos,dxpos,dypos,cup,vfac,vand,spno,maxspno,minspno,rot,
			zpos,cdown,origcup;
	long xpos,ypos;
	word angle,callb;
	byte status,bounce;
	obj_S::obj_S();
};

class oliste_C
{
public:
	obj_S *obj;
	int antall;
	oliste_C();
	~oliste_C();
	void kopier(obj_S *f, obj_S *t);
	void nyttobj(obj_S *ny);
	void fjern(byte no);
};

#endif


