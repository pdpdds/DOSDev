# MegaBreak

To mark its imminent 20 year anniversary, I'm doing a little revisit of my first big programming project. This repo collects all the source code for the game, as well as discussing some aspects of its creation.

I started this project way back in 1995, at the age of 15. My goal was to create a fast, "arcadey" type of game. Since this was my first attempt at creating a full game, I also figured it would be smart to keep things relatively simple. I eventually decided to create a Breakout/Arkanoid type of game, since I felt this wouldn't be too complicated to implement, while at the same time presenting plenty of technical challenges. In addition, the amount of graphical assets needed were low enough that I could do it all by myself. Since I was just starting out with PC programming at this point, taking on this project also meant creating lots of low level graphics and I/O functions, as well as some basic tools to help me realise my vision. I went with C++ (though I mostly used pure C) for the main implementation, which I had been learning from a couple of books for a year or so. I also had been learning some basic assembler programming and thus decided to do the low level graphics functions in assembly. The project took quite a while to finish, and was finally released in 1997 as free software. It gained some recognition after it's release, including a feature in scandinavian amateur computer magazine "Komputer for alle" where I was interviewed briefly about the game. It was subsequently distributed on their cover CD for the next couple of years or so.

![MegaBreak gameplay](https://github.com/carrierdown/megabreak/raw/master/megabreak-first-two-levels.gif "Megabreak gameplay: first two levels")

Note: This GIF does not accurately depict the actual frame rate of the game, which is a lot higher ;)

## Graphics

The graphics for the game were all created using Deluxe Paint II, which originally hails from the Amiga 500, and which I'd previously used extensively with my Atari ST. The PC version was not up to the same standards as the Atari and Amiga versions, but was nonetheless one of the few PC applications that made pixel based graphics creation easy. All art assets were saved as PCX-files, which I mainly chose because it was relatively easy to parse. However, I did not like the idea of having lots of PCX-files distributed alongside my game, so I wrote some tools to enable me to store the raw picture data from each PCX-file sequentially to a data file. Since all graphics were made with the same palette, this information was also removed from each image and only stored in the data file once.

![MegaBreak sprite sheets](https://github.com/carrierdown/megabreak/raw/master/megabreak-asset-sheet-x2.png "Megabreak sprite sheets")

## Sound

Given the amount of time and effort I've put into [music and sound](http://upland.no) for the past 15+ years of my life, it's a bit ironic that MegaBreak features no sound or music at all. The reason for this is however purely technical. Implementing sound in a DOS environment was not trivial, and although there were some libraries available at the time to ease the process, I was unable to get any of these to work together with my choice of compiler and memory model (see the next paragraph for more on this), although I don't remember the details.

## Compiler and memory model

I used the Watcom C compiler to build the game. The main reason for choosing this particular compiler was that it made it easy to incorporate the DOS4GW protected mode extender, which removed the 640k limit and segmented memory model normally imposed on DOS programs. With DOS4GW, memory could be addressed linearly using 32-bit registers rather than the conventional segmented model.

Although I haven't tested this, it should be possible to build the game using an open source compiler and the DOS/32 extender, which is essentially an open source version of DOS4GW.

## Level editor

MegaBreak features its own level editor, which can be used to quickly create levels for the game. The editor is invoked from the command line with a filename, which opens a graphical editor where you can draw a level using the different brick types. The output is written into a 260 byte .MAP file, which contains the level data. Starting the megabreak executable itself with the option -mylevels enables you to run the game using any self-designed .MAP files residing in the main directory of the game.

![Level editor](https://github.com/carrierdown/megabreak/raw/master/level-editor.png "Level editor")

## Mock advertising

I also created some mock advertising for the game, including a banner ad typical for the time, and a parody of the widely used "Netscape Now!" and similar buttons. 

![MegaBreak mock ad](https://github.com/carrierdown/megabreak/raw/master/megabreak-mock-ad.gif)

![MegaBreak Now!](https://github.com/carrierdown/megabreak/raw/master/megabreak-now.gif)

## Sequel plans

I did plan a sequel for a while following the release of the first game. I teamed up with a friend for this project, and our intent was to get the game running on Windows using DirectX. Due to various reasons and shifting priorities, the project never materialized. However, I did create some preliminary graphics for it, including a mock game screen, some sprites, different brick types, and a basic font. This was done around 1998-99.

![MegaBreak 2 mock screenshot](https://github.com/carrierdown/megabreak/raw/master/megabreak-2-mock-screenshot.png "Megabreak 2 mock screenshot")

![MegaBreak 2 bat sprite](https://github.com/carrierdown/megabreak/raw/master/mb2-bat-animated.gif "MegaBreak 2 bat sprite")

![MegaBreak 2 assorted graphics](https://github.com/carrierdown/megabreak/raw/master/mb2-art.png)
