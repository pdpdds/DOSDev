#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <alloc.h>

void main(int argc, char **argv)
{
   FILE *stream;
   char file_name[80];
   unsigned char *ptr;
   int length, i, j;

   if(argc < 2)
   {
      puts("Usage : fnt2txt <file name>");
      puts("        ex) fnt2txt hangul");
      exit(1);
   }

   strcpy(file_name, argv[1]);
   strcat(file_name, ".fnt");
   if((stream = fopen(file_name, "rb")) == NULL)
   {
      puts("File not found.");
      exit(1);
   }
   length = filelength(fileno(stream));
   if((ptr = malloc(length)) == NULL)
   {
      puts("Memory not enough.");
      fclose(stream);
      exit(1);
   }
   fread(ptr, length, 1, stream);
   fclose(stream);

   strcpy(file_name, argv[1]);
   strcat(file_name, ".txt");
   if((stream = fopen(file_name, "wt")) == NULL)
   {
      puts("Can't write on disk.");
      exit(1);
   }
   printf("   %% completed..");
   for(i = 0; i < length-1; i += 15)
   {
      gotoxy(1, wherey());
      printf("%3d", (int) (i * 100. / length));
      for(j = 0; j < 15 && i+j < length-1; j++)
	 fprintf(stream, "%3d, ", ptr[i+j]);
      if(j == 15) fprintf(stream, "\n");
   }
   fprintf(stream, "%3d", ptr[i+j]);
   fclose(stream);
   free(ptr);
   gotoxy(1, wherey());
   puts("\aSuccessfully converted..");
}
