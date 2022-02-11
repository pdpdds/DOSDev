// TIMER_C

#ifndef _TIMER_H
#define _TIMER_H

class timer_C
{
public:
  timer_C();
  ~timer_C();
  long readtimer(void);
  long elapsed(unsigned long start,unsigned long stop);
};

#endif

