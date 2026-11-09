/*-- Samen --*/

#strict 3

local idTree;

protected func Hit()
{
  Sound("WoodHit*");
}

protected func Activate(pObj)
{
  var ID= idTree,i= Random(4);
  if(Equal(ID, RMMG))
    {
    if(Equal(i, nil)) ID= TRE1;
    if(Equal(i, 1)) ID= TRE2;
    if(Equal(i, 2)) ID= TRE3;
    if(Equal(i, 3)) ID= TRE4;
    }
  // Ist noch Platz für einen Baum?
  var iSize= DefinitionCall(ID,"ReproductionAreaSize"),iOffset= (iSize ?? 0)/ -2;
  if(!((pObj-> ObjectCount(nil,iOffset,iOffset,iSize,iSize,OCF_Chop) ?? 0)< (DefinitionCall(ID,"MaxTreeCount") ?? 0)))
   return(Message("Hier hat kein Baum mehr Platz!",pObj));
  ObjectSetAction(pObj,"Bridge");
  pObj-> CreateConstruction(ID,nil,+15,-1,5);
  RemoveObject();
  return(1);
}