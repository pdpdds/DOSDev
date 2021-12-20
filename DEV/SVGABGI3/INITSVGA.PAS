{ Sample program that initializes the SuperVGA driver}
Program Test256;

Uses Graph,Crt,Dos;

{$i svga16.inc}
{$i svga256.inc}

var
  GraphMode, GraphDriver : integer;
  Ky : Char;
  Drv : Integer;

{$F+}
function DetectVGA256 : Integer;
var Vid : Integer;

begin
  Writeln('Which video mode would you like to use?');
  Writeln('  0) 320x200x256');
  Writeln('  1) 640x400x256');
  Writeln('  2) 640x480x256');
  Writeln('  3) 800x600x256');
  Writeln('  4) 1024x768x256');
  Write('> ');
  Readln(Vid);
  DetectVGA256 := Vid;
end;

function DetectVGA16 : Integer;
var Vid : Integer;

begin
  Writeln('Which video mode would you like to use? ');
  Writeln('  0) 320x200x16');
  Writeln('  1) 640x200x16');
  Writeln('  2) 640x350x16');
  Writeln('  3) 640x480x256');
  Writeln('  4) 800x600x16');
  Writeln('  5) 1024x768x16');
  Writeln('>');
  Readln(Vid);
  DetectVGA16 := Vid;
end;

function DetectTwk256 : Integer;
var Vid : Integer;

begin
  Writeln('Which video mode would you like to use?');
  Writeln('  0) 320x400x256');
  Writeln('  1) 320x480x256');
  Writeln('  2) 360x480x256');
  Writeln('  3) 376x564x256');
  Writeln('  4) 400x564x256');
  Writeln('  5) 400x600x256');
  Write('> ');
  Readln(Vid);
  DetectTwk256 := Vid;
end;

function DetectTwk16 : Integer;
var Vid : Integer;

begin
  Writeln('Which video mode would you like to use? ');
  Writeln('  0) 704x528x16');
  Writeln('  1) 720x540x16');
  Writeln('  2) 736x552x16');
  Writeln('  3) 752x564x256');
  Writeln('  4) 768x576x16');
  Writeln('  5) 784x588x16');
  Writeln('  6) 800x600x16');
  Writeln('>');
  Readln(Vid);
  DetectTwk16 := Vid;
end;
{$F-}
 
begin
  Writeln('Which driver would you like to use?');
  Writeln('  0) Svga256');
  Writeln('  1) Svga16');
  Writeln('  2) Tweak256');
  Writeln('  3) Tweak16');
  Write('>');
  Readln(Drv);
  if (Drv = 0) then
    GraphDriver := InstallUserDriver('SVGA256',@DetectVGA256)
  else if (Drv = 1)
    GraphDriver := InstallUserDriver('SVGA16',@DetectVGA16)
  else if (Drv = 2)
    GraphDriver := InstallUserDriver('Twk256',@DetectTwk256)
  else if (Drv = 3)
    GraphDriver := InstallUserDriver('Twk16',@DetectTwk16);
  GraphDriver := Detect;
  InitGraph(GraphDriver,GraphMode,'');
  setcolor(15);
  line(0,0,GetMaxX,GetMaxY);
  line(0,GetMaxY,GetMaxX,0);
  Ky := ReadKey;
  CloseGraph;
end.