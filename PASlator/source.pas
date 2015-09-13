uses crt;

procedure frame(x1: integer;y1: integer; x2: integer; y2: integer);
var
  i: integer;
begin;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  for i:= x1 to x2 do
  begin
    gotoxy(i, y1);
    writeln('#');
    gotoxy(i, y2);
    writeln('#');
  end;
  for i:= y1 to y2 do
  begin
    gotoxy(x1, i);
    write('#');
    gotoxy(x2, i);
    write('#');
  end;
end;

const
  x1 = 6;
  y1 = 4;
  x2 = 74;
  y2 = 19;

var
  x:integer; y: integer;
  c:char;
begin
  clrscr;
  textcolor(red);
  frame(x1 - 1, y1 - 1, x2 + 1, y2+1);
  x := 10;
  y := 10;
  textcolor(green);
  gotoxy(x, y);
  write('*');
  c:=readkey;
  while ord(c) <> 27 do
  begin
    if (c = 'w') then
      if (y>y1) then
        y:=y-1;
    if (c = 'a') then
      if (x>x1) then
        x:=x-1;
    if (c = 's') then
      if (y<y2) then
        y:=y+1;
    if (c = 'd') then
      if (x<x2) then
        x:=x+1;

    gotoxy(x, y);
    write('*');
    c:=readkey;
  end;
end.

  dfgfd