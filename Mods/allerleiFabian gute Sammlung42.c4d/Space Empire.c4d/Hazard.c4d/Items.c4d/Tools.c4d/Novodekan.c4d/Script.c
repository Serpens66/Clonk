/*-- Novodekan --*/

#strict 3

func Activate(object pByObj) {
  if((pByObj->GetEnergy() ?? 0) >= (pByObj->GetPhysical("Energy") ?? 0) / 1000) return(nil);

  if(GetEffect("*Heal*", pByObj)) return(nil);

  AddEffect("NovodekanHeal",pByObj,20,1,nil,GetID(),HealAmount(),HealRate());
  Sound("Medi");
  RemoveObject();
  return(1);
}

func HealRate() { return(10); }
func HealAmount() { return(10000); }

func FxNovodekanHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iHealAmount ??= 0;
  iHealRate ??= 0;
  if(iTemp)
    if((GetPhysical("Walk", 2, pTarget) ?? 0) <= (GetPhysical("Walk", 1, nil, GetID(pTarget)) ?? 0)*5/10)
      return;
  //Log("*HeilStart*");
  EffectVar(nil,pTarget,iEffectNumber) = iHealAmount; //Heilung pro Frame
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //Zeitdauer der Heilung

  // Lähmung
  EffectVar(2, pTarget, iEffectNumber) = (GetPhysical("Walk", 1, nil, GetID(pTarget)) ?? 0)*6/10;
  EffectVar(3, pTarget, iEffectNumber) = (GetPhysical("Jump", 1, nil, GetID(pTarget)) ?? 0)*3/10;
  EffectVar(4, pTarget, iEffectNumber) = (GetPhysical("Scale", 1, nil, GetID(pTarget)) ?? 0)*3/10;
  EffectVar(5, pTarget, iEffectNumber) = (GetPhysical("Hangle", 1, nil, GetID(pTarget)) ?? 0)*3/10;

  SetPhysical("Walk", (GetPhysical("Walk", nil, pTarget) ?? 0)-(EffectVar(2, pTarget, iEffectNumber) ?? 0), 2, pTarget);
  SetPhysical("Jump", (GetPhysical("Jump", nil, pTarget) ?? 0)-(EffectVar(3, pTarget, iEffectNumber) ?? 0), 2, pTarget);
  SetPhysical("Scale", (GetPhysical("Scale", nil, pTarget) ?? 0)-(EffectVar(4, pTarget, iEffectNumber) ?? 0), 2, pTarget);
  SetPhysical("Hangle", (GetPhysical("Hangle", nil, pTarget) ?? 0)-(EffectVar(5, pTarget, iEffectNumber) ?? 0), 2, pTarget);
}

func FxNovodekanHealTimer(object pTarget, int iEffectNumber, int iEffectTime) {
//Log("*heil*");
//Effekt
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var width= GetDefCoreVal("Width",nil,GetID(pTarget)),
      height = GetDefCoreVal("Height",nil,GetID(pTarget));
  S42MigrationAssign(width, "-=", (width ?? 0)/3);
  S42MigrationAssign(height, "-=", (height ?? 0)/3);
  S42MigrationAssign(width, "/=", 2);
  S42MigrationAssign(height, "/=", 2);
  CreateParticle("PxSpark",(GetX(pTarget) ?? 0)+(RandomX(-(width ?? 0), width) ?? 0),(GetY(pTarget) ?? 0)+(RandomX(-(height ?? 0),height) ?? 0),nil,-10,(Random(25) ?? 0)+10,RGBa(255,nil,255,50));
//*Heile mach*
  if(!((iEffectTime ?? 0) % (EffectVar(1, pTarget, iEffectNumber) ?? 0)))
  {
    DoEnergy(1, pTarget);
    S42MigrationStep(EffectVar(nil,pTarget,iEffectNumber), -1, true);
  }
  //Schon voll geheilt?
  if((GetEnergy(pTarget) ?? 0) >= (GetPhysical("Energy",nil,pTarget) ?? 0)/1000) {
    //Log("Geheilt!");
    return(-1); //Ja, Ende
  }
  //Och, schon aus? :(
  if(!EffectVar(nil,pTarget,iEffectNumber)) {
    //Log("Timeout!");
    return(-1); //Heute ist nicht alle Tage, ich komm wieder, keine Frage!
  }
}

public func FxNovodekanHealStop(target, no, reason, temp)
{
  SetPhysical("Walk", (GetPhysical("Walk", nil, target) ?? 0)+(EffectVar(2, target, no) ?? 0), 2, target);
  SetPhysical("Jump", (GetPhysical("Jump", nil, target) ?? 0)+(EffectVar(3, target, no) ?? 0), 2, target);
  SetPhysical("Scale", (GetPhysical("Scale", nil, target) ?? 0)+(EffectVar(4, target, no) ?? 0), 2, target);
  SetPhysical("Hangle", (GetPhysical("Hangle", nil, target) ?? 0)+(EffectVar(5, target, no) ?? 0), 2, target);
}

public func IsEquipment() { return(true); }

// KI-Behandlung
protected func AI_Inventory(object pClonk)
{
  // Benutzen, wenn der Clonk weniger als 2/3 Leben hat
  if((pClonk->GetEnergy() ?? 0) < (pClonk->GetPhysical("Energy") ?? 0) * 2/3 / 1000)
    // Benutzen (verzögert)
    ScheduleCall(this, "Activate", 1, nil, pClonk);
  // übernehmen wir
  return(1);
}
