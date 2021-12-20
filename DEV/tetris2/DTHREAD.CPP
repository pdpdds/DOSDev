#include <dos.h>
#include "dthread.h"

int DThread::numberOfThreads = 0;
int DThread::interruptVectors[maximumThreads] = {0x1c, 0x08};
DThread * DThread::owners[maximumThreads] = {0, 0};
void interrupt (* DThread::runNewHandlers[maximumThreads])(...) = {runNewHandler0, runNewHandler1};

DThread::DThread()
{
	if (numberOfThreads < maximumThreads)
	{
		if (owners[0] == 0)
		{
			id = 0;
			owners[id] = this;
			runOldHandler = getvect(interruptVectors[id]);
			canStart = true;

			numberOfThreads++;
		}
		else if (owners[1] == 0)
		{
			id = 1;
			owners[id] = this;
			runOldHandler = getvect(interruptVectors[id]);
			canStart = true;

			numberOfThreads++;
		}
		else
		{
			id = 0;
			canStart = false;
		}
	}
	else
	{
		id = 0;
		canStart = false;
	}

	alive = false;
}

DThread::~DThread()
{
	if (alive == true && owners[id] != 0)
	{
		disable();
		setvect(interruptVectors[id], runOldHandler);
		enable();

		owners[id] = 0;
	}

	numberOfThreads--;
}

void DThread::start()
{
	if (alive == true || canStart == false)
		return;

	disable();
	setvect(interruptVectors[id], runNewHandlers[id]);
	enable();

	alive = true;
}

void DThread::sleep(long millis)
{
	delay(millis);
}

void interrupt runNewHandler0(...)
{
	if (DThread::owners[0])
	{
		DThread::owners[0]->run();
		DThread::owners[0]->runOldHandler();
	}
}

void interrupt runNewHandler1(...)
{
	if (DThread::owners[1])
	{
		DThread::owners[1]->run();
		DThread::owners[1]->runOldHandler();
	}
}
