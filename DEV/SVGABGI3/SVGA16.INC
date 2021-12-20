(************************************************)
(* 						*)
(*	  SuperVGA 16 BGI driver defines	*)
(*		Copyright (c) 1991		*)
(*	    Jordan Hargraphix Software		*)
(*						*)
(************************************************)

type DacPalette16 = array[0..15] of array[0..2] of Byte;

(* These are the currently supported modes *)
const
  SVGA320x200x16	= 0;	(* 320x200x16 Standard EGA/VGA *)
  SVGA640x200x16	= 1;	(* 640x200x16 Standard EGA/VGA *)
  SVGA640x350x16	= 2;	(* 640x350x16 Standard EGA/VGA *)
  SVGA640x480x16	= 3;	(* 640x480x16 Standard VGA *)
  SVGA800x600x16	= 4;	(* 800x600x16 SuperVGA/VESA *)
  SVGA1024x768x16	= 5;	(* 1024x768x16 SuperVGA/VESA *)

  XNOR_PUT		= 5;
  NAND_PUT		= 6;
  NOR_PUT		= 7;

(* Setvgapalette sets the entire 16 color palette *)
(* PalBuf contains RGB values for all 16 colors   *)
(* R,G,B values range from 0 to 63	           *)
procedure SetVGAPalette16(PalBuf : DacPalette16);
var
  Reg : Registers;

begin
  reg.ax := $1012;
  reg.bx := 0;
  reg.cx := 16;
  reg.es := Seg(PalBuf);
  reg.dx := Ofs(PalBuf);
  intr($10,reg);
end;



