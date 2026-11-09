/*-- Fundament --*/

#strict 3
#include BAS7

/* B070 stellt die Grundfunktionalität für alle Fundament-Objekte sowie 
   abgeleitete Gebäude-Objekte zur Verfügung. */

private func BasementID() { return(B070); }
private func BasementWidth() { return(73); }
func IsBasement(){return(1);}  

/*
//Fundament-Objekte 

// Fertigstellung 

protected func Initialize()
{
  if(GetID() != BasementID() )
  {
    AddEffect("DestrCheck",this,10,0,this);
    return(_inherited());
  }
  return(_inherited() );
}

public func FxDestrCheckDamage(pTarget,iNum,iDmg,iCause)
{
  if(GetCon()<5)
    return Split2Components();
  if(GetCon()<10)
    if(!OnFire())
      return iDmg+Incinerate();
  if(FX_Call_DmgFire==iCause)
  {
    // Message ausgeben und sich selbst löschen
 //   EventMessage("Destruct",GetName(),GetID(),SetBit(0,GetOwner()+1,true));
    RemoveEffect(0,pTarget,iNum);
  }
  return iDmg;
}
   */
//Gebäude-Objekte 

//Fundament 

protected func Destruction()
{
  if(Equal(GetID(), BasementID()) ) return(nil);
  if(basement) RemoveObject(basement);
  return(_inherited() );
}

private func MoveOutClonk(object pObj) {
  if(!Equal(GetID(), BasementID())) return(nil);
  while(And(Stuck(pObj), Inside((GetY(pObj) ?? 0)-(GetY() ?? 0),-35,+5)))
    SetPosition(GetX(pObj),(GetY(pObj) ?? 0)-1,pObj);
}



//Fenster-Kruscht 
private func AddWindowLight(idDef,iX,iY,iPhase)
{
}

public func AddWindowLightEx(idDef,iX,iY,iPhase)
{
}
