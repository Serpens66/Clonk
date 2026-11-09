/*-- Phoenixei --*/

#strict

local zehler;

/* TimerCall */

private func Breed()
{
  if (GetMaterial(0,3) == Material("Ashes") || GetMaterial(0,3) == Material("FlyAshes")  ||  GetMaterial(0,-3) == Material("Ashes") || GetMaterial(0,-3) == Material("FlyAshes") || GetMaterial(3,0) == Material("Ashes") || GetMaterial(3,0) == Material("FlyAshes")  ||  GetMaterial(-3,0) == Material("Ashes") || GetMaterial(-3,0) == Material("FlyAshes"))
  zehler++;
  if (zehler>=30) DoBreak(); //wenn es auf Asche liegt, schlüpft der Phoenix nach 30 sekunden von alleine
  return(1);
}
  
/* Einwirkungen */  

protected func Hit()
{
  return(1); // kein schlüpfen durch aufschlag oder schaden
}

protected func Damage()
{
  return(1);
}

/* Aktionen */
  
private func DoBreak()
{
  if (!ActIdle()) return(0);
  if (GBackSolid()) return(0);
  Sound("EggBreak");
  SetAction("Break");
  var pMonster = CreateConstruction(PHNX,0,0,-1,40);
  pMonster->~Birth();
  return(1);
}
  
private func Destroy()
{
  RemoveObject();
  return(1);
}