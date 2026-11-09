/*-- Rapid Fire --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(nil,255,nil)); }

public func Activate(object obj) {
  if(!AddEffect("RapidBonus",obj,190,3,obj))
    return(nil);
  Sound("Rapidfire", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(1);
}

global func FxRapidBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var bar = EffectVar(nil,pTarget,iEffectNumber);
  var foo = (12*(bar ?? 0))/180,
      off=4,
      v = And(90 < (bar ?? 0), (bar ?? 0) < 270);
  if((bar ?? 0) > 180) {
    foo = -(13-(foo ?? 0));
    off=-7;
  }
  CreateParticle("PSpark",Sin(bar,12),(off ?? 0)+(Cos(bar,foo) ?? 0),nil,nil,35,RGBa(50,255,50,20),pTarget,v);
  CreateParticle("PSpark",Sin(bar,-12),((off ?? 0)+(Cos(bar,foo) ?? 0)),nil,nil,35,RGBa(50,255,50,20),pTarget,v);

  S42MigrationAssign(EffectVar(nil,pTarget,iEffectNumber), "+=", 15);
  if((EffectVar(nil,pTarget,iEffectNumber) ?? 0) > 360)
    EffectVar(nil,pTarget,iEffectNumber) = nil;
  
  if((iEffectTime ?? 0) > (BONUS_Duration ?? 0))
    return(-1);
}

global func FxRapidBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "RapidBonus"))
  {
    return(-1);
  }
}

global func FxRapidBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Log("Rapidfire");
  iEffectNumber ??= 0;
  iData ??= 0;
  if(Equal(iData, FM_Recharge)) return(Max(1,(value ?? 0)/2));
  if(Equal(iData, FM_Reload)) return(Max(1,(value ?? 0)/2));
  return(value);
}
