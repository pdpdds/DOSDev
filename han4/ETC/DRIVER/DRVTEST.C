/****************************************************************************/
/*   TITLE              Test Program                                        */
/*   SUB-TITLE          How to call driver function?                        */
/*   FILENAME           drvtest.c                                           */
/*   DATE & TIME        12/06/92(SUN) 12:11                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <alloc.h>

/* function pointers for driver functions */
void (*bell)(void);
void (*put_char)(char ascii);
int  (*add)(int x, int y);

/* function prototype */
void set_function(char *buffer, int length);

void main(void)
{
   FILE *driver_file;
   char *driver_buff;
   int  length, x;

   /* 1. open driver file */
   if((driver_file = fopen("driver.bin", "rb")) == NULL)
   {
      puts("driver.bin not found.");
      exit(1);
   }

   /* 2. allocate memory to driver buffer */
   length = filelength(fileno(driver_file));
   if((driver_buff = malloc(length)) == NULL)
   {
      puts("memory is not enough.");
      exit(2);
   }

   /* 3. read driver file */
   if(fread(driver_buff, length, 1, driver_file) != 1)
   {
      puts("can't read driver.bin");
      exit(3);
   }

   /* 4. close driver file */
   fclose(driver_file);

   /* 5. set function pointer */
   set_function(driver_buff, length);

   /* 6. free allocated memory */
   free(driver_buff);

   /* example of calling driver functions(not different from */
   /* any other C functions                                  */
   bell();
   put_char('T');
   put_char('\n');
   put_char('\r');
   x = add(1, 2);
   printf("1 + 2 = %d\n", x);
   getch();
}

void set_function(char *buffer, int length)
/* Set function pointer with information in driver buffer. */
/* set_function() enables driver function to be called  in */
/* familiar C style                                        */
{
   unsigned seg, off;

   /* get the segment and offset of driver buffer in far heap */
   seg = FP_SEG(buffer);
   off = FP_OFF(buffer);

   /* set function pointer with <seg>, <off> and add */
   /* the offset of the procedure in driver file     */
   (long) (*bell)     = ((long) seg << 16) + off + *((unsigned *) buffer + 0);
   (long) (*put_char) = ((long) seg << 16) + off + *((unsigned *) buffer + 1);
   (long) (*add)      = ((long) seg << 16) + off + *((unsigned *) buffer + 2);
}
