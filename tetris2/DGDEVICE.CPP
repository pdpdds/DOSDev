#include <stdio.h>
#include <stdlib.h>
#include "dgdevice.h"

DGraphicDevice::DGraphicDevice()
{
	driver = DETECT;
//	registerbgidriver(EGAVGA_driver);
	initgraph(&driver, &mode, ".\\");
	error = graphresult();

	if (error == 0)
		cleardevice();
	else
	{
		printf("BGI Error: Can't initialize graphics.\n");
		exit(1);
	}
}

DGraphicDevice::~DGraphicDevice()
{
	closegraph();
}

void DGraphicDevice::clear()
{
	if (error == 0)
		cleardevice();
}

int DGraphicDevice::getErrorCode()
{
	return error = graphresult();
}
