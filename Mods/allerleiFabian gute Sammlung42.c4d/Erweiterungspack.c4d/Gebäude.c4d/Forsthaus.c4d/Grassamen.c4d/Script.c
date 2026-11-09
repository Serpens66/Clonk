/*-- Samen --*/

#strict

local idTree;

protected func Hit()
{
  Sound("WoodHit*");
}

protected func Activate(pObj)
{
  var ID= GRA1;
  // Ist noch Platz für einen Baum?
  var iSize= DefinitionCall(ID,"ReproductionAreaSize"),iOffset= iSize/ -2;
  if(!(pObj-> ObjectCount(0,iOffset,iOffset,iSize,iSize,OCF_Chop())< DefinitionCall(ID,"MaxTreeCount")))
   return(Message("Hier hat kein Grasbüschel mehr Platz!",pObj));
  ObjectSetAction(pObj,"Bridge");
  pObj-> CreateConstruction(ID,10,+15,-10,15);
  pObj-> CreateConstruction(ID,-10,+15,-10,15);
  RemoveObject();
  return(1);
}