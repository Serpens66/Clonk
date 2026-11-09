/*-- Medipack --*/

#strict 3

func Activate(object pByObj) {
  if((pByObj->GetEnergy() ?? 0) >= (pByObj->GetPhysical("Energy") ?? 0) / 1000) return(nil);

  if(GetEffect("*Heal*", pByObj)) return(nil);

  var healrate = HealRate();
  if(!ObjectSetAction(pByObj, "Heal")) S42MigrationAssign(healrate, "*=", 4);

  AddEffect("MedipackHeal",pByObj,200,1,nil,GetID(),HealAmount(),healrate);
  Sound("Medi");
  RemoveObject();
  return(1);
}

func HealRate() { return(2); }
func HealAmount() { return(80); }

func FxMedipackHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iHealAmount ??= 0;
  iHealRate ??= 0;
  if(iTemp)
    return;

  EffectVar(nil,pTarget,iEffectNumber) = iHealAmount; //Heilung Insgesamt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //1 Punkt Heilung alle iHealRate-Frames
}

func FxMedipackHealTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  // Abbruch
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(!SEqual(pTarget->GetAction(), "Heal")) return(-1);

//Log("*heil*");
//Effekt
  var width= GetDefCoreVal("Width",nil,GetID(pTarget)),
      height = GetDefCoreVal("Height",nil,GetID(pTarget));
  S42MigrationAssign(width, "-=", (width ?? 0)/3);
  S42MigrationAssign(height, "-=", (height ?? 0)/3);
  S42MigrationAssign(width, "/=", 2);
  S42MigrationAssign(height, "/=", 2);
  CreateParticle("PxSpark",(GetX(pTarget) ?? 0)+(RandomX(-(width ?? 0), width) ?? 0),(GetY(pTarget) ?? 0)+(RandomX(-(height ?? 0),height) ?? 0),nil,-10,(Random(25) ?? 0)+10,RGBa(255,nil,nil,50));
//*Heile mach*
  if(!((iEffectTime ?? 0) % (EffectVar(1,pTarget,iEffectNumber) ?? 0)))
  {
    DoEnergy(1,pTarget);
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

func FxMedipackHealDamage(object pTarget, int iEffectNumber, int iChange)
{
  iEffectNumber ??= 0;
  iChange ??= 0;
  if((iChange ?? 0) >= 0) return(iChange);

  RemoveEffect("MedipackHeal", pTarget);

  return(iChange);
}

func FxMedipackHealStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if(fTemp) return;

  if(!!SEqual(pTarget->GetAction(), "Heal")) ObjectSetAction(pTarget, "Walk");
}

public func IsEquipment() { return(true); }

// KI-Behandlung
protected func AI_Inventory(object pClonk)
{
  // Benutzen, wenn der Clonk weniger als 2/3 Leben hat
  if((pClonk->GetEnergy() ?? 0) < (pClonk->GetPhysical("Energy") ?? 0) * 2/3 / 1000)
    // Benutzen (verzögert einsetzen)
    ScheduleCall(this, "Activate", 1, nil, pClonk);
  // übernehmen wir
  return(1);
}
