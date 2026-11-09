/*-- AmmoExpert --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(30,70,255)); }

public func Activate(object obj) {
  if(!AddEffect("AmmoExpBonus",obj,190,2,obj))
    return;
  Sound("AmmoExpert", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(true);
}

global func FxAmmoExpBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  CreateParticle("PSpark",Sin((iEffectTime ?? 0)*3,10),Cos((iEffectTime ?? 0)*3,10),nil,nil,30,RGB(30,130-(Sin((iEffectTime ?? 0)*2,100) ?? 0),250),pTarget);
  CreateParticle("PSpark",Sin((iEffectTime ?? 0)*3+180,10),Cos((iEffectTime ?? 0)*3+180,10),nil,nil,30,RGB(30,130-(Sin((iEffectTime ?? 0)*2,100) ?? 0),250),pTarget,true);
  if((iEffectTime ?? 0) > (BONUS_Duration ?? 0)/1+(BONUS_Duration ?? 0))
    return(-1);
}

global func FxAmmoExpBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "AmmoExpBonus"))
  {
    return(-1);
  }
}

global func FxAmmoExpBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  iEffectNumber ??= 0;
  iData ??= 0;
  if(Equal(iData, FM_AmmoRate)) return((value ?? 0)*2);
  return(value);
}

