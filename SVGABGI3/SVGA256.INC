(************************************************)
(* 						*)
(*	 SuperVGA 256 BGI driver defines	*)
(*		Copyright (c) 1991		*)
(*	    Jordan Hargraphix Software		*)
(*						*)
(************************************************)

type DacPalette256 = array[0..255] of array[0..2] of Byte;

(* These are the currently supported modes *)
const
  SVGA320x200x256	= 0;	(* 320x200x256 Standard VGA *)
  SVGA640x400x256	= 1;	(* 640x400x256 Svga *)
  SVGA640x480x256	= 2;	(* 640x480x256 Svga *)
  SVGA800x600x256	= 3;	(* 800x600x256 Svga *)
  SVGA1024x768x256	= 4;	(* 1024x768x256 Svga *)

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