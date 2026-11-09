/*-- PlrViewRange --*/

#strict

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
  if(GetPlrViewRange(pObj)!=iRange)
    AddEffect("FadePlrViewRange",pObj,100,iSpeed,0,0,iRange);
  return(1);
}

global func FxFadePlrViewRangeStart(pTarget, iEffectNumber, iTemp, iValue)
{
  if(!iTemp)
    EffectVar(0,pTarget,iEffectNumber)=iValue;
  return(1);
}

protected func FxFadePlrViewRangeEffect(szNewEffect, pTarget, iEffectNumber)
{
  if (szNewEffect eq "FadePlrViewRange") return (-3);
  // Ansonsten ignorieren
  return();
}

protected func FxFadePlrViewRangeAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iNewVal)
{
  EffectVar(0,pTarget,iEffectNumber)=iNewVal;
  // Fertig
  return(1);
}

global func FxFadePlrViewRangeTimer(pObj, iEffectNumber)
{
  var iValue=EffectVar(0,pObj,iEffectNumber);
  if(GetPlrViewRange(pObj)==iValue)
    return(-1);
  var Modifer;
  if(iValue<GetPlrViewRange(pObj))
    Modifer=-3;
  else
    Modifer=3;
  SetPlrViewRange(GetPlrViewRange(pObj)+Modifer,pObj);
  return(1);
}

global func FxFadePlrViewRangeStop(pObj, iEffectNumber, iTemp)
{
  var iValue=EffectVar(0,pObj,iEffectNumber);
  if(!iTemp)
    SetPlrViewRange(iValue,pObj);
  return(1);
}
