/*-- Phoenixei --*/

#strict 3

local zehler;

/* TimerCall */

private func Breed()
{
  if (Or(Or(Or(Or(Or(Or(Or(Equal(GetMaterial(nil,3), Material("Ashes")), Equal(GetMaterial(nil,3), Material("FlyAshes"))),  Equal(GetMaterial(nil,-3), Material("Ashes"))), Equal(GetMaterial(nil,-3), Material("FlyAshes"))), Equal(GetMaterial(3,nil), Material("Ashes"))), Equal(GetMaterial(3,nil), Material("FlyAshes"))),  Equal(GetMaterial(-3,nil), Material("Ashes"))), Equal(GetMaterial(-3,nil), Material("FlyAshes"))))
  S42MigrationStep(zehler, 1, true);
  if ((zehler ?? 0)>=30) DoBreak(); //wenn es auf Asche liegt, schlüpft der Phoenix nach 30 sekunden von alleine
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
  if (!ActIdle()) return(nil);
  if (GBackSolid()) return(nil);
  Sound("EggBreak");
  SetAction("Break");
  var pMonster = CreateConstruction(PHNX,nil,nil,-1,40);
  pMonster->~Birth();
  return(1);
}
  
private func Destroy()
{
  RemoveObject();
  return(1);
}