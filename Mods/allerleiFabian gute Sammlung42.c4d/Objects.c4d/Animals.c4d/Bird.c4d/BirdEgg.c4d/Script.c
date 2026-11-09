/*-- Vogelei --*/

#strict 3

local iBreedTime;

/* TimerCall */

protected func Breed()
{
  if(!!SEqual(GetAction(), "Nest")) S42MigrationStep(iBreedTime, 1, true);
  if((iBreedTime ?? 0)>=50) DoBreak();
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

protected func Entrance()
{
  SetAction("Idle");
  SetActionTargets();
  iBreedTime=nil;
  if(FindContents(BIRD)) RemoveObject(FindContents(BIRD));
}

/* Aktionen */
  
private func DoBreak()
{
  if (!!SEqual(GetAction(), "Break")) return(nil);
  Sound("EggBreak");
  SetAction("Break");
  if((iBreedTime ?? 0)<25) return(nil); // Noch nicht richtig ausgebildeter Vogel
  if (GBackSolid()) return(nil);
  var pChild = CreateConstruction(BIRD, nil, nil, -1, 40);
  SetOwner(-1, pChild);
  pChild->~Birth();
}
  
private func Destroy()
{
  RemoveObject();
}

public func Activate(object pClonk)		// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(25);
    pClonk->DoBreath(100);
    RemoveObject();
    return(1);
}

public func GetFeedValue() { return(25); }
