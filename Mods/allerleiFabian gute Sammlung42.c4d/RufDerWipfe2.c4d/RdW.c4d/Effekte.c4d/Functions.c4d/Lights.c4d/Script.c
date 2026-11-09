#strict

public func AddLightLayer(sGraph,iLayer,idDef)
{
  // Log("AddLightLayer(%v,%v)",sGraph,iLayer);
  // if(!iLayer)  return Log("AddLightLayer(%v,%v)::LightLayer not possible",sGraph,iLayer);
  if(!idDef)idDef=GetID();
  SetGraphics(sGraph,this(),idDef,iLayer+1,GFXOV_MODE_ExtraGraphics,0,GFX_BLIT_Additive);
  AddEffect("LightPulse",this(),40,RandomX(4,10),this(),0,iLayer+1);
  return iLayer;
}

public func DeleteLightLayer(iLayer)
{
  SetGraphics(0,this(),0,iLayer+1);
  var i = GetEffectCount("LightPulse",this()),iEffect;
  while (i--)
    if (iEffect = GetEffect("LightPulse", this(), i))
      if(EffectVar(3,this(),iEffect)==iLayer+1)
        RemoveEffect(0, this(), iEffect);
}

protected func FxLightPulseStart(pTarget, iEffectNumber, iTemp, iLayer)
{
  if(!iTemp)
  {
    EffectVar(1,pTarget,iEffectNumber)=80;
    EffectVar(2,pTarget,iEffectNumber)=0;
    EffectVar(3,pTarget,iEffectNumber)=iLayer;
    EffectCall(pTarget,iEffectNumber,"Timer");
  }
}

protected func FxLightPulseTimer(pTarget, iEffectNumber, iEffectTime)
{
  var Layer=EffectVar(3,pTarget,iEffectNumber);
  var Dir=BoundBy(EffectVar(2,pTarget,iEffectNumber)+RandomX(-1,1),-7,7);
  var Alpha=BoundBy(EffectVar(1,pTarget,iEffectNumber)+RandomX(-5,5)+RandomX(Dir),10,110);
  if(Alpha>80) Dir-=Random(2);
  if(Alpha<40) Dir+=Random(2);
  SetClrModulation(RGB(255-Alpha,255-Alpha,255-Alpha),pTarget,Layer);
  EffectVar(1,pTarget,iEffectNumber)=Alpha;
  EffectVar(2,pTarget,iEffectNumber)=Dir;
}

protected func FxLightPulseEffect(szNewEffectName, pTarget, iEffectNumber, iNewEffectNumber, iLayer)
{
  if(szNewEffectName ne "LightPulse") return(0);
  if(EffectVar(3,pTarget,iEffectNumber) == iLayer)
    return(-1);
}
