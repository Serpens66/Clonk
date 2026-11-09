/*-- Feuermonsterei --*/

#strict 3

/* TimerCall */

private func Breed()
{
  if (!Random(2000)) DoBreak();
  return(1);
}
  
/* Einwirkungen */  

protected func Hit()
{
  if (Random(2)) return(1);
  return(DoBreak());
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
  var pMonster = CreateConstruction(FMNS,nil,nil,-1,40);
  pMonster->Birth();
  return(1);
}
  
private func Destroy()
{
  RemoveObject();
  return(1);
}