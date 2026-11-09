/*-- Fenster --*/

#strict

public func Start(pTarget)
{
  if(FindObject2(Find_ID(GetID()),Find_Exclude(),Find_AtPoint()))
   return !RemoveObject();
  SetAction("Fensterln");
  SetObjectBlitMode(1);
  AddEffect("LightPulse",this(),40,RandomX(6,9),this());
  return(1);
}

protected func FxLightPulseStart(pTarget, iEffectNumber, iTemp, iLayer)
{
  EffectVar(1,pTarget,iEffectNumber)=70;
  EffectVar(2,pTarget,iEffectNumber)=0;
}

protected func FxLightPulseTimer(pTarget, iEffectNumber, iEffectTime)
{
  var Layer=EffectVar(3,pTarget,iEffectNumber);
  var Dir=BoundBy(EffectVar(2,pTarget,iEffectNumber)+RandomX(-1,1),-7,7);
  var Alpha=BoundBy(EffectVar(1,pTarget,iEffectNumber)+RandomX(-5,5)+RandomX(Dir),20,160);
  if(Alpha>150) Dir-=Random(2);
  if(Alpha<30) Dir+=Random(2);
  SetClrModulation(RGBa(255,255,255,Alpha),pTarget,Layer);
  EffectVar(1,pTarget,iEffectNumber)=Alpha;
  EffectVar(2,pTarget,iEffectNumber)=Dir;
}
