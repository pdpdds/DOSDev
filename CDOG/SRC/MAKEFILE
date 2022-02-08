#
# Makefile for C-Dogs
#
.ERASE

wlink_options = system dos4g
wcc_options = /mf /wx /onetr /d__32__=1 /zq /zp1 /5r

.c.obj: .AUTODEPEND
  wcc386 $[* $(wcc_options)

cdogs.exe : cdogs.obj pics.obj draw.obj joystick.obj blit.obj &
             actors.obj map.obj sounds.obj defs.obj sprcomp.obj &
             grafx.obj text.obj objs.obj gamedata.obj ai.obj &
             hiscores.obj automap.obj mission.obj game.obj &
             triggers.obj input.obj prep.obj keyboard.obj &
             mainmenu.obj password.obj files.obj menu.obj
  wlink $(wlink_options) @cdogs

cdogsed.exe : cdogsed.obj pics.obj draw.obj joystick.obj blit.obj &
              actors.obj map.obj sounds.obj defs.obj sprcomp.obj &
              grafx.obj text.obj objs.obj gamedata.obj &
              hiscores.obj automap.obj mission.obj &
              triggers.obj input.obj keyboard.obj files.obj &
              game.obj ai.obj charsed.obj events.obj
  wlink $(wlink_options) @cdogsed

blit.obj : blit.asm
  wasm /mf /5p blit.asm
