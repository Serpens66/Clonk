/*-- AmmoPack --*/

#strict 3

public func IsBonus() { return(true); }

func Color() { return(RGB(90,90,90)); }

func Activate(object obj) {
  var i,id;
  while(id = GetDefinition(S42MigrationStep(i, 1, true)))
    DoAmmo(id,(DefinitionCall(id,"MaxAmmo") ?? 0)/5,obj);
  
  AddEffect("AmmoBonus",obj,1,5);
  Sound("AmmoPack", 1, nil,nil, (GetOwner(obj) ?? 0)+1);

  return(true);
}

global func FxAmmoBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  CreateParticle("XSpark",(GetX(pTarget) ?? 0)+(Sin((EffectVar(nil,pTarget,iEffectNumber) ?? 0)*60,12) ?? 0),(GetY(pTarget) ?? 0)+(Cos((EffectVar(nil,pTarget,iEffectNumber) ?? 0)*60,12) ?? 0),nil,nil,40,RGB(90,90,90),pTarget);
  if(S42MigrationStep(EffectVar(nil,pTarget,iEffectNumber), 1) > 6) {
    CastParticles("XSpark",5,15,GetX(pTarget),GetY(pTarget),40,60,RGB(90,90,90),nil,pTarget);
    return(-1);
  }
}
