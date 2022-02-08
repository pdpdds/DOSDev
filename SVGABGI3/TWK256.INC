(************************************************)
(* 						*)
(*	  Tweaked 256 BGI driver defines	*)
(*		Copyright (c) 1991		*)
(*	    Jordan Hargraphix Software		*)
(*						*)
(************************************************)

type DacPalette256 = array[0..255] of array[0..2] of Byte;

(* These are the currently supported modes *)
const
  TWK320x400x256	= 0;
  TWK320x480x256	= 1;
  TWK360x480x256	= 2;
  TWK376x564x256	= 3;
  TWK400x564x256	= 4;
  TWK400x600x256	= 5;

  TRANS_COPY_PIX	= 8;

(* Setvgapalette sets the entire 256 color palette *)
(* PalBuf contains RGB values for all 256 colors   *)
(* R,G,B values range from 0 to 63	           *)
procedure SetVGAPalette256(PalBuf : DacPalette256);
var
  Reg : Registers;

begin
  reg.ax := $1012;
  reg.bx := 0;
  reg.cx := 256;
  reg.es := Seg(PalBuf);
  reg.dx := Ofs(PalBuf);
  intr($10,reg);
end;


