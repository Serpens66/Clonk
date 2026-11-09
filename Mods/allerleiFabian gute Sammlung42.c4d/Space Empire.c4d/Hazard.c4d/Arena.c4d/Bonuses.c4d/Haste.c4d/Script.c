/*-- Haste --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,255,nil)); }

public func Activate(object obj) {
  if(!AddEffect("HasteBonus",obj,60,5,obj))
    return;
  Sound("Speed", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  return(true);
}

global func FxHasteBonusStart(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  pTarget->SetPhysical("Walk",(pTarget->GetPhysical("Walk") ?? 0) + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",(pTarget->GetPhysical("Jump") ?? 0) + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",(pTarget->GetPhysical("Scale") ?? 0) + 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",(pTarget->GetPhysical("Hangle") ?? 0) + 10000,PHYS_StackTemporary);
}

global func FxHasteBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  CreateParticle("NoGravSpark",RandomX(-5,5),RandomX(-6,6),nil,nil,35,RGB(250,250,Random(20)),nil,Random(2));
  if((iEffectTime ?? 0) > (BONUS_Duration ?? 0))
    return(-1);
}

global func FxHasteBonusStop(object pTarget, int iEffectNumber, int tmp) {
  iEffectNumber ??= 0;
  tmp ??= 0;
  if(tmp)
    return;
  pTarget->SetPhysical("Walk",(pTarget->GetPhysical("Walk") ?? 0) - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",(pTarget->GetPhysical("Jump") ?? 0) - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",(pTarget->GetPhysical("Scale") ?? 0) - 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",(pTarget->GetPhysical("Hangle") ?? 0) - 10000,PHYS_StackTemporary);
}

global func FxHasteBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "HasteBonus"))
  {
    return(-1);
  }
}
