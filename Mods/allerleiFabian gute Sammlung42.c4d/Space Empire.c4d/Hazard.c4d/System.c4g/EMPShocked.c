/*-- EMP Shocked--*/

#strict 3

global func EMPShockEffect(int iTime) {
  iTime ??= 0;
  AddEffect("EMPDamaged", this(), 190, 5, nil, nil,iTime);
}

global func EMPShocked() {
  return GetEffect("EMPDamaged",this());
}

global func FxEMPDamagedStart(object pTarget, int iEffectNumber, int iTemp, int iTime)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iTime ??= 0;
  EffectVar(nil, pTarget, iEffectNumber) = iTime;
  EffectVar(1, pTarget, iEffectNumber) = pTarget->GetClrModulation();
  pTarget->SetClrModulation(RGB(200,200,200));
}

global func FxEMPDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if((EffectVar(nil, pTarget, iEffectNumber) ?? 0) < (iEffectTime ?? 0))
    return -1;
  // effects
  if(!(pTarget->~Contained())) {
    var wdt = GetDefWidth(GetID(pTarget));
    var hgt = GetDefHeight(GetID(pTarget));
    Sparks(2,nil,(GetX(pTarget) ?? 0)+(RandomX(-(wdt ?? 0)/2,(wdt ?? 0)/2) ?? 0),(GetY(pTarget) ?? 0)+(RandomX(-(hgt ?? 0)/2,(hgt ?? 0)/2) ?? 0));
    if(!Random(3)) {
		var pLaser = CreateObject(LASR, (GetX(pTarget) ?? 0)+(RandomX(-(wdt ?? 0)/4,(wdt ?? 0)/4) ?? 0), (GetY(pTarget) ?? 0)+(RandomX(-(hgt ?? 0)/4,(hgt ?? 0)/4) ?? 0), -1);
		pLaser->SetClrModulation(RGBa(255, 200, nil, 64));
		pLaser->Set(Random(360), 8, Sqrt((wdt ?? 0)*(wdt ?? 0)/4+(hgt ?? 0)*(hgt ?? 0)/4), 3);
	}
  }
}

global func FxEMPDamagedAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iTime) {
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = Max(EffectVar(nil,pTarget,iEffectNumber),iTime);
}

global func FxEMPDamagedEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(S42MigrationEqual(szNewEffectName, "EMPDamaged"))
    return -2;
}

global func FxEMPDamagedStop(object pTarget, int iEffectNumber) {
  iEffectNumber ??= 0;
  pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber));
  pTarget->~EMPShockEnd();
}