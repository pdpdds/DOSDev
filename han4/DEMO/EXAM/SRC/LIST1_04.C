#include <time.h>

void main(void)
{
   clock_t start, end;
   long i;

   puts("Wait a moment...");
   start = clock();
   for(i = 0; i < 10000000L; i++) ;
   end = clock();

   printf("Elapsed time = %5d clock ticks.\n", end - start);
}
