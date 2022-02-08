test.exe : test.obj hanio.lib
   @tcc -ml test.obj hanio.lib graphics.lib
test.obj : test.c
   @tcc -c -ml test
