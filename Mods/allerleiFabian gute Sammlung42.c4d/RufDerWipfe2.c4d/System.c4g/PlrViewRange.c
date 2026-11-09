/*-- PlrViewRange --*/

#strict 3

global func GetPlrViewRange(pObj)
{
  if(!pObj)
   pObj=this();
  return(GetObjPlrViewRange(pObj));
}

/*--- FxFadePlrViewRange ---*/

global func FadePlrViewRange(iRange,pObj,iSpeed)
{
  if(!pObj)
    pObj=this();
  if(!iSpeed)
    iSpeed=1;
  if(!Equal(GetPlrViewRange(pObj), iRange))
    AddEffect("FadePlrViewRange",pObj,100,iSpeed,nil,nil,iRange);
  return(1);
}

global func FxFadePlrViewRangeStart(pTarget, iEffectNumber, iTemp, iValue)
{
  if(!iTemp)
    EffectVar(nil,pTarget,iEffectNumber)=iValue;
  return(1);
}

protected func FxFadePlrViewRangeEffect(szNewEffect, pTarget, iEffectNumber)
{
  if (!!SEqual(szNewEffect, "FadePlrViewRange")) return (-3);
  // Ansonsten ignorieren
  return;
}

protected func FxFadePlrViewRangeAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iNewVal)
{
  EffectVar(nil,pTarget,iEffectNumber)=iNewVal;
  // Fertig
  return(1);
}

global func FxFadePlrViewRangeTimer(pObj, iEffectNumber)
{
  var iValue=EffectVar(nil,pObj,iEffectNumber);
  if(Equal(GetPlrViewRange(pObj), iValue))
    return(-1);
  var Modifer;
  if((iValue ?? 0)<(GetPlrViewRange(pObj) ?? 0))
    Modifer=-3;
  else
    Modifer=3;
  SetPlrViewRange((GetPlrViewRange(pObj) ?? 0)+(Modifer ?? 0),pObj);
  return(1);
}

global func FxFadePlrViewRangeStop(pObj, iEffectNumber, iTemp)
{
  var iValue=EffectVar(nil,pObj,iEffectNumber);
  if(!iTemp)
    SetPlrViewRange(iValue,pObj);
  return(1);
}
