/*-- Components --*/

#strict 3

global func GetComponentCount(idComponent, bNoCon, pObject)
{
  if(!pObject)pObject=this();
  if(!pObject)return nil;
  var cnt=nil,id=GetID(pObject);
  for(var i = nil;; S42MigrationStep(i, 1, true))
  {
    var id = GetDefCoreVal("Components", "DefCore", id, (i ?? 0)*2);
    if(!id) break;
    if(Equal(id, idComponent))
    {
     var cnt = GetDefCoreVal("Components", "DefCore", id, (i ?? 0)*2+1);
     break;
    }
  }
  if(bNoCon)
   return cnt;
  return (cnt ?? 0)*(GetCon(pObject) ?? 0)/100;
}
