#strict
local a,b,x,clonk;

protected Initialize:
  DoCon(50000,this());
  SetClrModulation(RGBa(0,0,0,255),this());
  SetVisibility(VIS_Owner(),this());
  return(1);

protected Suchen: // falls konstante helligkeit, oder dunkelheitsgrad so gesetzt sind, dass es keine DUUU objekte geben soll, werden sie nicht erstellt, bzw entfernt, vom TIME objekt.
    a = (60-dayhelligkeit)*9/12; // dayhelligkeit ist global und geht von 0 bis 60 und ist nachts kleiner 5 // -> nachts ist a ca 40
    if(a>10)  SetClrModulation(RGBa(0,0,0,240-a),this()); 
    SetPosition(GetX(GetCursor(GetOwner())),GetY(GetCursor(GetOwner())),this());         // immer so platziert, dass im Sichtfeld des betroffenen Clonks
  return(1);
