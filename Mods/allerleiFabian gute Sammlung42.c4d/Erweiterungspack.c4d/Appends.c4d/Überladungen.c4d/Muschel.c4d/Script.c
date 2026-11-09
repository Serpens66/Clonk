/*-- Muschel --*/

#strict

protected func Initialize()
{
  SetAction("Auf");
}

protected func Hit()
{
  Sound("MetalHit*");
  if(GetAction() eq "Auf" && !Random(15)) Activate();
}

protected func Activate()
{
  // CreateObject(PR9I,0,0,-1);
  CreateContents(PR9I,Contained(this()));
  ChangeDef(GM9I);
}

private func Close()
{
  if(!Random(3)) Bubble();
  if(!Random(2)) Bubble();
  if(!Random(10)) return(SetAction("Schliessen"));
  if(!InLiquid()) return(SetAction("Schliessen"));
  if(FindObject(0,-50,-50,100,100,OCF_CrewMember()) && !Random(2)) SetAction("Schliessen");
}

private func Open()
{
  if(!Random(3)) Bubble();
  if(!InLiquid()) return(0);
  if(!FindObject(0,-50,-50,100,100,OCF_CrewMember()) && !Random(3)) SetAction("Oeffnen");
}