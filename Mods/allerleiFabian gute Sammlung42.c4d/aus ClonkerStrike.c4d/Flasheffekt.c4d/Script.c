#strict 3
local a,b,x,clonk,fast;

protected func Initialize(...) {
  DoCon(5000,this());
  //SetVisibility(VIS_Owner(),this());
  clonk=Par(1);
  if(clonk)
  if(clonk->~IsAI()) clonk->~Istnungeblendet();  
  fast=Par();
  return(1);
}

protected func Suchen(...) {
  //if(Global(111)==1)return(RemoveObject(this()));
  SetClrModulation(RGBa(255,255,255,x),this());           // sorgt dafür, dass es nach Zeit durchsichtiger wird
  if(Equal(GetCursor(GetOwner(this())), clonk))SetVisibility(VIS_Owner,this());  // wenn betroffener Clonk aktiv ist, dann objekt sichtbar
  if(Not(Equal(GetCursor(GetOwner(this())), clonk)))SetVisibility(VIS_None,this()); // wenn anderer Clonk aktiv ist, dann nicht sichtbar
  SetPosition(GetX(clonk),GetY(clonk),this());         // immer so platziert, dass im Sichtfeld des betroffenen Clonks
  S42MigrationStep(b, 1);
  if(Equal(fast, 1))x=(x ?? 0)+2;
  if(And((b ?? 0)>150, Equal(fast, nil)))S42MigrationStep(x, 1);
  if((x ?? 0)>255) {if(clonk) if(clonk->~IsAI()) clonk->~Nichtmehrgeblendet(); return(RemoveObject(this()));}
  return(1);
}
