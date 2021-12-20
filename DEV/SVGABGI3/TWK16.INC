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
 TWK704x528x16		= 0	(* 704x528x16 Tweaked VGA *)
 TWK720x540x16		= 1	(* 720x540x16 Tweaked VGA *)
 TWK736x552x16		= 2	(* 736x552x16 Tweaked VGA *)
 TWK752x564x16		= 3	(* 752x564x16 Tweaked VGA *)
 TWK784x588x16		= 4	(* 784x588x16 Tweaked VGA *)
 TWK800x600x16		= 5	(* 800x600x16 Tweaked VGA *)

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