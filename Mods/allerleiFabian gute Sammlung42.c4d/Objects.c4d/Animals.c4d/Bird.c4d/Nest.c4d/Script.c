/*-- Nest --*/

#strict 3

local pTree;

protected func Initialize()
{
  SetAction("Be");
  return(1);
}

private func Check4Tree()
{
  // Baum noch da?	
  if(pTree)	
   if(!OnFire(pTree))  
    if(!((GetCategory(pTree) ?? 0) & (C4D_Vehicle ?? 0)))
      return(1);      	    
  SetAction("Decay");
  SetCategory(C4D_Vehicle);
  // Ei benachrichtigen
  var pEgg;
  if(pEgg = FindObject(BEGG, nil, nil, nil, nil, nil, nil, this())) pEgg->SetAction("Idle");
  return(1);
}

protected func Incineration()
{
  SetAction("Decay");
  SetCategory(C4D_Vehicle);
  // Ei benachrichtigen
  var pEgg;
  if(pEgg = FindObject(BEGG, nil, nil, nil, nil, nil, nil, this())) pEgg->SetAction("Idle");
  return(1);
}

public func RejectCollect(pObj, pID)
{
  if(Equal(pID, BIRD)) return(nil);
  return(1);
}

public func Breed()
{
  var pBird = FindContents(BIRD);
  if (pBird) pBird->LayEgg(); else SetAction("Be");
}

public func Ejection()
{
  SetAction("Be");	
}

private func Hit()
{
  if (!Equal(GetAction(), "Decay"))  SetAction("Decay");
  return(1);
}

protected func ActivateEntrance()
{
  SetEntrance(1);
  return(1);
}

private func Remove()
{
  return(RemoveObject());
}
