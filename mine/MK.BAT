@bcc -c -O2 -ml miner.cpp button.cpp mouse.cpp
@bcc -ml -eminer.exe *.obj graphics.lib
@dir *.exe
