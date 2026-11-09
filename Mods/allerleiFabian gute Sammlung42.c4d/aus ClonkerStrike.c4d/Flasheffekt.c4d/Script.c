#strict
local a,b,x,clonk,fast;

protected Initialize:
  DoCon(5000,this());
  //SetVisibility(VIS_Owner(),this());
  clonk=Par(1);
  if(clonk)
  if(clonk->~IsAI()) clonk->~Istnungeblendet();  
  fast=Par();
  return(1);

protected Suchen:
  //if(Global(111)==1)return(RemoveObject(this()));
  SetClrModulation(RGBa(255,255,255,x),this());           // sorgt dafür, dass es nach Zeit durchsichtiger wird
  if(GetCursor(GetOwner(this()))==clonk)SetVisibility(VIS_Owner(),this());  // wenn betroffener Clonk aktiv ist, dann objekt sichtbar
  if(Not(GetCursor(GetOwner(this()))==clonk))SetVisibility(VIS_None(),this()); // wenn anderer Clonk aktiv ist, dann nicht sichtbar
  SetPosition(GetX(clonk),GetY(clonk),this());         // immer so platziert, dass im Sichtfeld des betroffenen Clonks
  ++b;
  if(fast==1)x=x+2;
  if(b>150 && fast==0)++x;
  if(x>255) {if(clonk) if(clonk->~IsAI()) clonk->~Nichtmehrgeblendet(); return(RemoveObject(this()));}
  return(1);
