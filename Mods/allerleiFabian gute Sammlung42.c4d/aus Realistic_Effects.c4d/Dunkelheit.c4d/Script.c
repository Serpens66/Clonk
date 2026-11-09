#strict 3
local a,b,x,clonk;

protected func Initialize(...) {
  DoCon(50000,this());
  SetClrModulation(RGBa(nil,nil,nil,255),this());
  SetVisibility(VIS_Owner,this());
  return(1);
}

protected func Suchen(...) { // falls konstante helligkeit, oder dunkelheitsgrad so gesetzt sind, dass es keine DUUU objekte geben soll, werden sie nicht erstellt, bzw entfernt, vom TIME objekt.
    a = (60-(dayhelligkeit ?? 0))*9/12; // dayhelligkeit ist global und geht von 0 bis 60 und ist nachts kleiner 5 // -> nachts ist a ca 40
    if((a ?? 0)>10)  SetClrModulation(RGBa(nil,nil,nil,240-(a ?? 0)),this());
    SetPosition(GetX(GetCursor(GetOwner())),GetY(GetCursor(GetOwner())),this());         // immer so platziert, dass im Sichtfeld des betroffenen Clonks
  return(1);
}
