/*-- Graphics --*/
// Musste leider sein (erhöht die Performance [um 14% !])

#strict

static const GRAPH_Save=55;

global func SetGraphics(gfxname,pObj,idSrcDef,iOverlay,a,b,c,d,e)
{
  var ret;
  if(ret=_inherited(gfxname,pObj,idSrcDef,iOverlay,a,b,c,d,e))
    if(!iOverlay)
      Local(GRAPH_Save,pObj)=gfxname;
  return ret;
}

global func GetGraphics(pObj)
{
  if(Local(GRAPH_Save,pObj))
    return Local(GRAPH_Save,pObj);
  else
    return "";
}
