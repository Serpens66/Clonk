#strict

public func Activate()
{
  if(SetVar(1,FindObject2(Find_Not(Find_Func("IsBasement")),Find_AtRect(-5,-5,10,20),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)))))
  {CreateObject(_ATW,0,12,GetOwner(Var(1))); Sound("Connect"); RemoveObject();};
  Message("Kein Gebäude zum Einbau eines Schildes gefunden",this());
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}