/*-- Lore --*/

#strict
#include LORY
#appendto LORY

public func RejectCollect(id idObj,object pObj)
{
  if(ContentsCount() < MaxContents())
    {
    // Wenn sich ein Objekt nicht aufsammel lässt, ertönt kein Ton-> Logisch. :D
    if(!ProtectedCall(pObj,"RejectEntrance",this()))
     if(GetProcedure(pObj) ne "ATTACH")
      Sound("Clonk");
    return(0);
    }
  if(Contained(pObj)) return(Message("$TxtLorryisfull$", this()));
  if(Abs(GetXDir(pObj))>6) SetYDir(-5,pObj);
  // Wenn sich ein Objekt nicht aufsammel lässt, ertönt kein Ton.
  if(!ProtectedCall(pObj,"RejectEntrance",this()))
   if(GetProcedure(pObj) ne "ATTACH")
    Sound("WoodHit*");
  return(1);
}