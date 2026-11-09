/*-- Fundament --*/

#strict

#include BAS7

local hatch;

/* Zerstörung */

protected func Destruction()
{
  // Bodenklappe löschen
  if(hatch) RemoveObject(hatch);
  // Basisfunktion aufrufen
  return(_inherited() );
}

/* In ein Fundament mit Loch (für die Bodenklappe) verwandeln */
public func ChangeToHatch(object pHatch)
{
  // Kein entsprechendes Bodenklappe-Fundament? Geht hier nicht.
  if(!HatchID() ) return(0);
  // Bodenklappe speichern
  hatch = pHatch;
  // Umwandeln
  ChangeDef(HatchID() );
}

private func BasementID() { return(B4S8); }
private func BasementWidth() { return(80); }
private func HatchID() { return(BSO3); }

func IsBasement(){return(0);}  //einziges Fundament, welches zerstörbar und reparierbar ist, weil einzeln baubar

/* Fertigstellung */

protected func Initialize()
{
  if(GetID() != BasementID() ) return(0);
  SetAction("Consolidate");
  MoveOutClonks();
  SetPosition(GetX(this()),GetY(this())+8,this());
  return(_inherited() );
}

/* Fundament */

protected func Construction()
{
  SetCategory(1);
  return(_inherited());
}

/* Feststeckende Clonks und andere Objekte rauswerfen */

private func MoveOutClonks() {
/*  var pObj;
  if(GetID()!=BasementID()) return(0);
  while(pObj=FindObject(0,-BasementWidth()/2-10,-15,BasementWidth()+20,25,OCF_Collectible() | OCF_Grab() | OCF_Alive(),0,0,NoContainer(),pObj))
    if(!DefinitionCall(GetID(pObj),"IsBridge"))
      if (~GetCategory(pObj) & C4D_Structure)
        MoveOutClonk(pObj); */
}
  
private func MoveOutClonk(object pObj) {
  if(GetID()!=BasementID()) return(0);
  while(Stuck(pObj) && Inside(GetY(pObj)-GetY(),-15,+5))
    SetPosition(GetX(pObj),GetY(pObj)-1,pObj);  
}




