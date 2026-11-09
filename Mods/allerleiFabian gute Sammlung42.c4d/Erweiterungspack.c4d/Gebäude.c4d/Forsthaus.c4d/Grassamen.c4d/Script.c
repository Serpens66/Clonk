/*-- Samen --*/

#strict 3

local idTree;

protected func Hit()
{
  Sound("WoodHit*");
}

protected func Activate(pObj)
{
  var ID= GRA1;
  // Ist noch Platz für einen Baum?
  var iSize= DefinitionCall(ID,"ReproductionAreaSize"),iOffset= (iSize ?? 0)/ -2;
  if(!((pObj-> ObjectCount(nil,iOffset,iOffset,iSize,iSize,OCF_Chop) ?? 0)< (DefinitionCall(ID,"MaxTreeCount") ?? 0)))
   return(Message("Hier hat kein Grasbüschel mehr Platz!",pObj));
  ObjectSetAction(pObj,"Bridge");
  pObj-> CreateConstruction(ID,10,+15,-10,15);
  pObj-> CreateConstruction(ID,-10,+15,-10,15);
  RemoveObject();
  return(1);
}