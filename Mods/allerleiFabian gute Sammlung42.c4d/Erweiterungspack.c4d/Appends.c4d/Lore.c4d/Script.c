/*-- Lore --*/

#strict 3
#include LORY
#appendto LORY

public func RejectCollect(id idObj,object pObj)
{
  if((ContentsCount() ?? 0) < (MaxContents() ?? 0))
    {
    // Wenn sich ein Objekt nicht aufsammel lässt, ertönt kein Ton-> Logisch. :D
    if(!ProtectedCall(pObj,"RejectEntrance",this()))
     if(!SEqual(GetProcedure(pObj), "ATTACH"))
      Sound("Clonk");
    return(nil);
    }
  if(Contained(pObj)) return(Message("$TxtLorryisfull$", this()));
  if((Abs(GetXDir(pObj)) ?? 0)>6) SetYDir(-5,pObj);
  // Wenn sich ein Objekt nicht aufsammel lässt, ertönt kein Ton.
  if(!ProtectedCall(pObj,"RejectEntrance",this()))
   if(!SEqual(GetProcedure(pObj), "ATTACH"))
    Sound("WoodHit*");
  return(1);
}