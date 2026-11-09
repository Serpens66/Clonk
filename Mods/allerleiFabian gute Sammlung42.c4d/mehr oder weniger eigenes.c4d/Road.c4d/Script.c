/*-- Fundament --*/

// unzerstörbare Fundamentteile, die als Straße fungieren

#strict 3

local basement;

private func BasementID() { return(_ROA); }
private func BasementWidth() { return(72); }
func IsBasement(){return(nil);}  //einziges Fundament, welches zerstörbar und reparierbar ist, weil einzeln baubar


/* Fertigstellung */

protected func Initialize()
{
  if(!Equal(GetID(), BasementID()) ) return(nil);
  SetAction("Consolidate");
  MoveOutClonks();
  return(_inherited() );
}

private func Consolidated()
{
  if(!Equal(GetID(), BasementID()) ) return(nil);
  
  // Das Fundament ist fertig und abgesunken: in den Hintergrund (fixieren)
  SetCategory(1);
}

public func Consolidate()
{
  return(SetCategory(1));
}

/* Schaden / Zerstörung */
/*
protected func Damage(iChange, iByPlayer)
{
  if(GetID() != BasementID() ) return(_inherited(iChange, iByPlayer) );
  if(GetDamage() < 150) return(_inherited(iChange, iByPlayer) );

  CastObjects(ROCK,4,15,0,-5);
  RemoveObject();
  
  return(_inherited(iChange, iByPlayer) );
}
*/
  
/* Feststeckende Clonks und andere Objekte rauswerfen */

private func MoveOutClonks() {
  var pObj;
  if(!Equal(GetID(), BasementID())) return(nil);
  while(pObj=FindObject(nil,-(BasementWidth() ?? 0)/2-10,-15,(BasementWidth() ?? 0)+20,25,(OCF_Collectible ?? 0) | (OCF_Grab ?? 0) | (OCF_Alive ?? 0),nil,nil,NoContainer(),pObj))
    if(!DefinitionCall(GetID(pObj),"IsBridge"))
      MoveOutClonk(pObj);
}
  
private func MoveOutClonk(object pObj) {
  if(!Equal(GetID(), BasementID())) return(nil);
  while(And(Stuck(pObj), Inside((GetY(pObj) ?? 0)-(GetY() ?? 0),-15,+5)))
    SetPosition(GetX(pObj),(GetY(pObj) ?? 0)-1,pObj);
}
  
/*-- Gebäude-Objekte --*/

/* Fundament */

protected func Construction()
{
  if(Equal(GetID(), BasementID()) ) return(nil);
  // Nur wenn noch kein Fundament vorhanden: Damit bei Überladung von
  // Fundamentobjekten nicht Fundamente mehrfach erstellt werden.
  if(!basement) basement = CreateObject(BasementID(), nil, 8, GetOwner() );
  return(_inherited() );
}
  
protected func Destruction()
{
  if(Equal(GetID(), BasementID()) ) return(nil);
  CastObjects(ROCK,1,15,nil,-5);
  if(basement) RemoveObject(basement);
  return(_inherited() );
}

