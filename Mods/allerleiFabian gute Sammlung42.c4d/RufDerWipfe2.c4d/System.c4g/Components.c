/*-- Components --*/

#strict

global func GetComponentCount(idComponent, bNoCon, pObject)
{
  if(!pObject)pObject=this();
  if(!pObject)return 0;
  var cnt=0,id=GetID(pObject);
  for(var i = 0;; i++)
  {
    var id = GetDefCoreVal("Components", "DefCore", id, i*2);
    if(!id) break;
    if(id==idComponent)
    {
     var cnt = GetDefCoreVal("Components", "DefCore", id, i*2+1);
     break;
    }
  }
  if(bNoCon)
   return cnt;
  return cnt*GetCon(pObject)/100;
}
