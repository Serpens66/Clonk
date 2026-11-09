/*-- Invisible --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(200,nil,255)); }

public func Activate(object obj) {
  if(!AddEffect("InvisBonus",obj,190,1,obj))
    return;
  Sound("Invisible", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(true);
}

global func FxInvisBonusStart(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  //Wegen den Layern machen wir es einfach mit einem Hilfsobjekt. :)
  pTarget->SetObjectBlitMode(1);
  pTarget->SetClrModulation(RGBa(255,255,255,180));
}

global func FxInvisBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  SetPosition(GetX(pTarget),GetY(pTarget),EffectVar(nil,pTarget,iEffectNumber));
  if((iEffectTime ?? 0) > (BONUS_Duration ?? 0))
    return(-1);
}

global func FxInvisBonusStop(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  pTarget->SetClrModulation();
  pTarget->SetObjectBlitMode();
}

global func FxInvisBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "BerserkerBonus"))
  {
    return(-1);
  }
}
