/*-- Fundament --*/

#strict 3
#include BAS7

private func BasementID(...) { return(B556);
}
private func BasementWidth(...) { return(80);
}
func IsBasement(){return(nil);}  //einziges Fundament, welches zerstörbar und reparierbar ist, weil einzeln baubar



/* Fertigstellung */

protected func Initialize()
{
  if(!Equal(GetID(), BasementID()) ) return(nil);
  SetAction("Consolidate");
  MoveOutClonks();
  SetPosition(GetX(this()),(GetY(this()) ?? 0)+8,this());
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
  if(!Equal(GetID(), BasementID())) return(nil);
  while(And(Stuck(pObj), Inside((GetY(pObj) ?? 0)-(GetY() ?? 0),-15,+5)))
    SetPosition(GetX(pObj),(GetY(pObj) ?? 0)-1,pObj);
}
