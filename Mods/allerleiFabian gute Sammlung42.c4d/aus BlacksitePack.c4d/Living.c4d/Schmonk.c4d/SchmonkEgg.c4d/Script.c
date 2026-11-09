/*-- Monsterei --*/

#strict

/* TimerCall */

protected func Breed()
{
  if (!Random(2000)) DoBreak();
}
  
/* Einwirkungen */  

protected func Hit()
{
  if (!Random(10)) DoBreak();
}

protected func Damage()
{
  return(RemoveObject());
}

/* Aktionen */
  
private func DoBreak()
{
  if (!ActIdle()) return(0);
  if (GBackSolid()) return(0);
  Sound("EggBreak");
  SetAction("Break");
  var pChild = CreateConstruction(SHMK, 0, 0, -1, 40);
  SetOwner(-1, pChild);
  pChild->~Birth();
}
  
private func Destroy()
{
  RemoveObject();
}
