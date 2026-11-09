#strict 3

public func AddLightLayer(sGraph,iLayer,idDef)
{
  // Log("AddLightLayer(%v,%v)",sGraph,iLayer);
  // if(!iLayer)  return Log("AddLightLayer(%v,%v)::LightLayer not possible",sGraph,iLayer);
  if(!idDef)idDef=GetID();
  SetGraphics(sGraph,this(),idDef,(iLayer ?? 0)+1,GFXOV_MODE_ExtraGraphics,nil,GFX_BLIT_Additive);
  AddEffect("LightPulse",this(),40,RandomX(4,10),this(),nil,(iLayer ?? 0)+1);
  return iLayer;
}

public func DeleteLightLayer(iLayer)
{
  SetGraphics(nil,this(),nil,(iLayer ?? 0)+1);
  var i = GetEffectCount("LightPulse",this()),iEffect;
  while (S42MigrationStep(i, -1, true))
    if (iEffect = GetEffect("LightPulse", this(), i))
      if(Equal(EffectVar(3,this(),iEffect), (iLayer ?? 0)+1))
        RemoveEffect(nil, this(), iEffect);
}

protected func FxLightPulseStart(pTarget, iEffectNumber, iTemp, iLayer)
{
  if(!iTemp)
  {
    EffectVar(1,pTarget,iEffectNumber)=80;
    EffectVar(2,pTarget,iEffectNumber)=nil;
    EffectVar(3,pTarget,iEffectNumber)=iLayer;
    EffectCall(pTarget,iEffectNumber,"Timer");
  }
}

protected func FxLightPulseTimer(pTarget, iEffectNumber, iEffectTime)
{
  var Layer=EffectVar(3,pTarget,iEffectNumber);
  var Dir=BoundBy((EffectVar(2,pTarget,iEffectNumber) ?? 0)+(RandomX(-1,1) ?? 0),-7,7);
  var Alpha=BoundBy((EffectVar(1,pTarget,iEffectNumber) ?? 0)+(RandomX(-5,5) ?? 0)+(RandomX(Dir) ?? 0),10,110);
  if((Alpha ?? 0)>80) S42MigrationAssign(Dir, "-=", (Random(2) ?? 0));
  if((Alpha ?? 0)<40) S42MigrationAssign(Dir, "+=", (Random(2) ?? 0));
  SetClrModulation(RGB(255-(Alpha ?? 0),255-(Alpha ?? 0),255-(Alpha ?? 0)),pTarget,Layer);
  EffectVar(1,pTarget,iEffectNumber)=Alpha;
  EffectVar(2,pTarget,iEffectNumber)=Dir;
}

protected func FxLightPulseEffect(szNewEffectName, pTarget, iEffectNumber, iNewEffectNumber, iLayer)
{
  if(!SEqual(szNewEffectName, "LightPulse")) return(nil);
  if(Equal(EffectVar(3,pTarget,iEffectNumber), iLayer))
    return(-1);
}
