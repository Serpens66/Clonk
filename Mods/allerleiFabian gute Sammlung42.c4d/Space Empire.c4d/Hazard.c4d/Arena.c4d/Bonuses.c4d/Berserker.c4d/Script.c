/*-- Berserker --*/

#strict 3

//Dauer: die halbe * die ganze Antwort!
static const BONUS_Duration = 882;

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,100,nil)); }

public func Activate(object obj) {
  if(!AddEffect("BerserkerBonus",obj,190,5,obj))
    return;
  Sound("Berserk", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(true);
}

global func FxBerserkerBonusStart(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  SetClrModulation(RGB(255,50,nil),pTarget);
}

global func FxBerserkerBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  CreateParticle("PxSpark",RandomX(-3,3),-8,nil,-10,35,RGB(250,50+(Random(100) ?? 0),Random(20)),pTarget,1);
  if((iEffectTime ?? 0) > (BONUS_Duration ?? 0))
    return(-1);
}

global func FxBerserkerBonusStop(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  SetClrModulation(RGB(255,255,255),pTarget);
}

global func FxBerserkerBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "BerserkerBonus"))
  {
    return(-1);
  }
}

global func FxBerserkerBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Damage*2!
  iEffectNumber ??= 0;
  iData ??= 0;
  if(Equal(iData, FM_Damage)) return((value ?? 0)*2);
  return(value);
}
