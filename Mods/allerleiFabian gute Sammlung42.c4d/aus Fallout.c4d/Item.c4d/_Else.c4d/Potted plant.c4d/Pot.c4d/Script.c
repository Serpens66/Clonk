/*--- Holz ---*/

#strict

protected func Hit(xDir, yDir)
{
  Sound("WoodHit*");
  CastObjects(EF8Z,3,5);
  var obj = Contents();
  if (obj) {
    Exit(obj);
    if(GetID(obj) == CY8Z) obj->Activate();
    obj->~Hit();
  }
  return(RemoveObject());
}

protected func Activate(pObj) {
  if (Contents()) return(Message("This pot contains a %v.",this,GetName(Contents())));
  CreateMenu(EJ8Z,pObj,0,0,"No items nearby.");
  for(var pItem in FindObjects (Find_Distance(20), Find_Category(C4D_Object), Find_Exclude(this),Find_NoContainer ()))  
    AddMenuItem (GetName(pItem), "PutIn",GetID(pItem), pObj,0,pItem, "Put it in the pot"); 
  return(1);
}

protected func PutIn(idT, pObj) {
  Enter(this,pObj);
  Sound("Scream");
}