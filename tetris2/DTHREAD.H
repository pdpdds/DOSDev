// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : DThread
// Platform: DOS System
// Purpose : Thread Model Using DOS Interrupts (Supports Only Two Threads)
// License : GNU General Public License        ---------------------------
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - DThread()
//        Allocates a new DOS interrupt for this thread.
// Method Summary:
// - virtual void start()
//        Starts this thread to begin execution. The run method of this
//        thread is called without hesitation.
// - virtual void run()
//        Thread's execution body.
// - boolean isAlive()
//        Tests if this thread is alive.
// - static void sleep(long millis)
//        Sleeps the currently executing thread for the number of
//        milliseconds.
// --------------------------------------------------------------------------

#ifndef _DTHREAD_
#define _DTHREAD_

#include "boolean.h"

const int maximumThreads = 2;

class DThread
{
	private:
		int id;
		boolean canStart;
		boolean alive;
		void interrupt (* runOldHandler)(...);
		static int numberOfThreads;
		static int interruptVectors[maximumThreads];
		static DThread * owners[maximumThreads];
		static void interrupt (* runNewHandlers[maximumThreads])(...);
		friend void interrupt runNewHandler0(...);
		friend void interrupt runNewHandler1(...);
	public:
		DThread();
		virtual ~DThread();
		virtual void start();
		virtual void run();
		boolean isAlive();
		static void sleep(long millis);
};

inline void DThread::run()
{

}

inline boolean DThread::isAlive()
{
	return alive;
}

#endif
