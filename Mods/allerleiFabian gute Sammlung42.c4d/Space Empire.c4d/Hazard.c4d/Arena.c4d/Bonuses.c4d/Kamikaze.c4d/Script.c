/*-- Kamikaze --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(20,20,20)); }

public func Activate(object obj) {
  if(!Random(10))
    Explode(50,CreateObject(TIM2,GetX(obj),GetY(obj),-1));
  else if(!AddEffect("KamikazeBonus",obj,190,5,obj))
    return(nil);
  Sound("Kamikaze", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(1);
}

global func FxKamikazeBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(!((iEffectTime ?? 0)%1500))
    if(!Random(10))
      return(-1);
}

global func FxKamikazeBonusStop(object pTarget, int iEffectNumber, int iReason, bool tmp) {
  iEffectNumber ??= 0;
  iReason ??= 0;
  tmp ??= false;
  if(tmp)
    return;
  Explode(50,CreateObject(TIM2));
}

global func FxKamikazeBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "KamikazeBonus"))
  {
    return(-1);
  }
}
