int choose_gr_mode(void)
{
   int key, mode;

   puts("Graphics Library mode setup");
   puts("----------------------------------------------");
   puts("       0. Hercules 0 page (640x400x2)");
   puts("       1. Hercules 1 page (640x400x2)");
   puts("       2. Hercules 0 page (720x348x2)");
   puts("       3. Hercules 1 page (720x348x2)");
   puts("       4. CGA             (640x200x2)");
   puts("       5. CGA             (320x200x4)");
   puts("       6. EGA             (320x200x16)");
   puts("       7. EGA             (640x350x16)");
   puts("       8. EGA             (640x200x16)");
   puts("       9. VGA             (640x480x16)");
   puts("       A. VGA             (640x400x16)");
   puts("       B. VGA             (640x480x2)");
   puts("       C. SVGA            (640x480x256)");
   puts("       D. SVGA            (640x400x256)");
   puts("       E. SVGA            (800x600x16)");
   puts("       F. SVGA            (800x600x256)");
   puts("       G. SVGA            (1024x768x16)");
   puts("       H. SVGA            (1024x768x256)");
   puts("       Q. Quit");
   puts("-----------------------------------------");
   cputs("Choose graphics mode -- ");

   do
   {
      key = getch();
      if(key >= '0' && key <= '9')
	 mode = key - '0';
      else if(key >= 'A' && key <= 'H')
	 mode = key - 'A' + 10;
      else if(key >= 'a' && key <= 'h')
	 mode = key - 'a' + 10;
      else if(key == 'Q' || key == 'q' || key == 27)
	 mode = -1;
      else
	 continue;
   }
   while(0);

   return(mode);
}
