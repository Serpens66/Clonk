/*-- Monsterei --*/

#strict 3

/* TimerCall */

protected func Breed()
{
  if (!Random(2000)) DoBreak();
}
  
/* Einwirkungen */  

protected func Hit()
{
  if (Random(2)) DoBreak();
}

protected func Damage()
{
  return(DoBreak());
}

/* Aktionen */
  
private func DoBreak()
{
  if (!ActIdle()) return(nil);
  if (GBackSolid()) return(nil);
  Sound("EggBreak");
  SetAction("Break");
  var pChild = CreateConstruction(MONS, nil, nil, -1, 40);
  SetOwner(-1, pChild);
  pChild->~Birth();
}
  
private func Destroy()
{
  RemoveObject();
}
