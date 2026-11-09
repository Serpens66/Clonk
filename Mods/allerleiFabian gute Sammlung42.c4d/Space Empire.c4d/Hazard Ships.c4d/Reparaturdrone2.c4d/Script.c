/*-- Support Drone --*/

#strict 3

local bArmed;
local pGuard;
local curTarget;

func Initialize()
{
  DoEnergy(100);

  AddEffect("ShowWeapon",this(),1,1,this(),GetID());  
  SetGraphics(nil,this(),GetID(),3,5,nil,nil,this());
  
  SetAction("Idle"); 
  
  if (Random(2)) return(SetComDir(COMD_Right));
  return(SetComDir(COMD_Left));
}

// Kann per Script aufgerufen werden, um die Drohne an einen Clonk zu heften
public func ArmToClonk(object pClonk, object pWeapon)
{
  if(Or(!pClonk, !pWeapon)) return;
  pGuard = pClonk;
  SetOwner(GetOwner(pGuard));
  return(GetArmed(nil, pWeapon));
}

public func GetGuardedClonk() { return(pGuard); }

public func WeaponAt(&x, &y, &r) {
  x = nil;
  y = 8000;
  r = Abs(AimAngle());
  if((r ?? 0) >= 180) {
    r = Abs(180-(r ?? 0));
    S42MigrationAssign(r, "-=", 90);
    S42MigrationAssign(r, "*=", -1);
  }
  else
    S42MigrationAssign(r, "-=", 90);
  return(1);
}
/*
public func WeaponEnd(&x, &y) {
  x = Sin(AimAngle(), 10);
  y = -Cos(AimAngle(), 5);
  return(1);
}*/

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  x = (EffectVar(4, this(), number) ?? 0)/1000;
  y = (EffectVar(5, this(), number) ?? 0)/1000;
}

public func OnDmg(int iDamage, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDamage ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Melee ?? 0))
    return(-30);
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(20);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(30);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(-10);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(-40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

public func Death()
{
  TurnOff();
  Explode(10);
  return;
}

public func Activate(object byObj)
{
  //Checken, ob der Clonk nicht schon eine hat. :Ooo
  var pDrone;

  for(pDrone in FindObjects( Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(byObj->GetOwner())))
  {
    if(!Equal(pDrone->GetGuardedClonk(), byObj)) continue;
    PlayerMessage(byObj->GetOwner(), "$JustOneDrone$", byObj);
    Sound("Error");
    return(1);
  }

  //Kaputte Drohne?
  if(EMPShocked())
  {
    PlayerMessage(byObj->GetOwner(), "$Damaged$", byObj);
    Sound("Error");
    return(1);
  }
  
  // Menü erzeugen
  CreateMenu(DRSU, byObj, this(),nil,"$NoWeap$");
  
  //Inventar des Clonks durchgehen
  for(var i = nil; (i ?? 0) < (byObj->ContentsCount() ?? 0); S42MigrationStep(i, 1))
  {
    var pInvObj = byObj->Contents(i);
    //Wenn Waffe, dem Menü hinzufügen
    //if(pInvObj->~GetFMData(FM_Aim)) continue; // Aber keine Waffen, die zielen müssen
    if(pInvObj->~IsWeapon())
      AddMenuItem("$ArmWith$", "GetArmed", pInvObj->GetID(), byObj, nil, pInvObj);
  } 
  
  //Schützling zuweisen
  pGuard = byObj;
  SetOwner(GetOwner(pGuard));
  return(1);
}

public func GetArmed(id idItem, object pItem)
{
  //Waffe iwie verschwunden? Abbrechen!
  if(! pItem) return(nil);

  //Aus Clonk raus, in Drone rein.
  pItem->Exit();
  pItem->Enter(this());

  //Aktion setzen
  SetAction("Fly");
  //Effekt zum Folgen setzen
  AddEffect("Guard", this(), 1, 5, this(), GetID(), pGuard, pItem);
  //Besitzer setzen
  SetOwner(pGuard->GetOwner());
  bArmed = true;

  // Meldung
  HelpMessage(pGuard->GetOwner(),"$ArmedWith$",pGuard,GetName(pItem));
  return(1);
}

public func FxGuardStart(object pTarget, int iEffectNumber, int iTemp, object pGClonk, object pWeapon)
{
  //Rausgehen
  iEffectNumber ??= 0;
  iTemp ??= 0;
  pTarget->Exit();
  //Speichern
  EffectVar(nil, pTarget, iEffectNumber) = pGClonk;   //Clonk
  EffectVar(1, pTarget, iEffectNumber) = pWeapon;   //Waffe
  //Drone kurzzeitig einem Besitzer geben (FF)
  pTarget->SetOwner(pGClonk->GetOwner());
  //Es lebt!11 haraahara
  SetAlive(true);
}

public func FxGuardTimer(object pTarget, int iEffectNumber, int iTemp)
{
  //Lebt der Clonk noch? Oder noch da? :o
  iEffectNumber ??= 0;
  iTemp ??= 0;
  if( !EffectVar(nil, pTarget, iEffectNumber) ) return(pTarget->TurnOff());
  if( !EffectVar(nil, pTarget, iEffectNumber)->GetAlive()) return(pTarget->TurnOff());
  
  //Commands killen, die komischerweise vom Follow aufgerufen werden
  if(Or(!Equal(pTarget->GetCommand(), "Follow"), !Equal(pTarget->GetCommand(), "MoveTo")))
    pTarget->SetCommand(nil, "None");
    
  //Wenn gerade keinen Command folgen wir dem zu Beschützenden
  if(!pTarget->GetCommand())
  {
    StopAutoGuns(EffectVar(1, pTarget, iEffectNumber));
    pTarget->AddCommand(nil, "Follow", EffectVar(nil, pTarget, iEffectNumber));
  }

  //Ausrichtung der Drone
  if((pTarget->GetXDir() ?? 0) > 0) pTarget->SetDir(DIR_Right);
  else
    pTarget->SetDir(DIR_Left);
  
  //Wenn Abstand zu gross müssen wir uns zum Beschützendem beamen
  if( (ObjectDistance(pTarget, EffectVar(nil, pTarget, iEffectNumber)) ?? 0) > 400)
  {
     Sound("DeEnergize", nil, pTarget);
     Beam(pTarget, EffectVar(nil, pTarget, iEffectNumber));
  }
  
  //ggf. Gegner angreifen
  var pEnemy;
  for(pEnemy in FindObjects(Find_Distance(250), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Or(Find_Owner(-1),Find_Hostile(GetController(pTarget))), Find_Func("IsBulletTarget",nil,nil,this()), Find_Exclude(EffectVar(nil, pTarget, iEffectNumber))))
  {
    if( !PathFree( pTarget->GetX(), pTarget->GetY(), pEnemy->GetX(), pEnemy->GetY()))
      continue;
      
    if(!CheckHostility(pEnemy))
      continue;
    
    if(! Inside(pTarget->GetY(), (pEnemy->GetY() ?? 0) - 15, (pEnemy-> GetY() ?? 0) + 15))
    {
      //Hinbewegen
      SetCommand(pTarget, "MoveTo", nil, pTarget->GetX(), pEnemy->GetY(), nil, nil, 2);
      // Viel zu nah dran?
      if( Inside(pTarget->GetX(), (pEnemy->GetX() ?? 0) - 15, (pEnemy->GetX() ?? 0) + 15))
        SetCommand(pTarget, "MoveTo", nil, 2*((pEnemy->GetX() ?? 0) - (pTarget->GetX() ?? 0)), pEnemy->GetY(), nil, nil, 2);
    }
    if( !WayFree(pTarget, pEnemy))
    {
      SetCommand(pTarget, "MoveTo", nil, pEnemy->GetX(), pEnemy->GetY(), nil, nil, 2);
      Sound("BipBipBip");
    }
    else 
    {
      //Zum Gegner drehen
      if((pEnemy->GetX() ?? 0) > (pTarget->GetX() ?? 0)) pTarget->SetDir(DIR_Right);
      else                                 pTarget->SetDir(DIR_Left);
      
      //Schießen!11
      curTarget = pEnemy;
      if(!EffectVar(1, pTarget, iEffectNumber)->IsShooting())
        EffectVar(1, pTarget, iEffectNumber)->ControlThrow(this());
    }        
  }
  // Kein Gegner?
  if(!pEnemy)
    // wir ballern?
    if(EffectVar(1, pTarget, iEffectNumber)->IsShooting())
      // aufhören.
      EffectVar(1, pTarget, iEffectNumber)->StopAutoFire();

  var pAnimal;
  for(pAnimal in FindObjects(Find_Distance(150), Find_OCF(OCF_Alive), Find_OCF(OCF_Living), Find_Func("IsAnimalAim"), Find_Owner(-1)))  
  {
    if( !PathFree( pTarget->GetX(), pTarget->GetY(), pAnimal->GetX(), pAnimal->GetY()))
      continue;
      
    if(! Inside(pTarget->GetY(), (pAnimal->GetY() ?? 0) - 15, (pAnimal-> GetY() ?? 0) + 15))
    {
      //Hinbewegen
      SetCommand(pTarget, "MoveTo", nil, pTarget->GetX(), pAnimal->GetY(), nil, nil, 2);
      // Viel zu nah dran?
      if( Inside(pTarget->GetX(), (pAnimal->GetX() ?? 0) - 15, (pAnimal->GetX() ?? 0) + 15))
        SetCommand(pTarget, "MoveTo", nil, 2*((pAnimal->GetX() ?? 0) - (pTarget->GetX() ?? 0)), pAnimal->GetY(), nil, nil, 2);
    }
    if( !WayFree(pTarget, pAnimal))
    {
      SetCommand(pTarget, "MoveTo", nil, pAnimal->GetX(), pAnimal->GetY(), nil, nil, 2);
      Sound("BipBipBip");
    }
    else 
    {
      //Zum Gegner drehen
      if((pAnimal->GetX() ?? 0) > (pTarget->GetX() ?? 0)) pTarget->SetDir(DIR_Right);
      else                                 pTarget->SetDir(DIR_Left);
      
      //Schießen!11
      curTarget = pAnimal;
      if(!EffectVar(1, pTarget, iEffectNumber)->IsShooting())
        EffectVar(1, pTarget, iEffectNumber)->ControlThrow(this());
    }        
  }
  // Kein Gegner?
  if(!pAnimal)
    // wir ballern?
    if(EffectVar(1, pTarget, iEffectNumber)->IsShooting())
      // aufhören.
      EffectVar(1, pTarget, iEffectNumber)->StopAutoFire();

  //ggf. Jäger reparieren
  local repa;
  repa=FindObject(PRE_,-20,-20,40,40, nil, "Gelandet", nil, NoContainer());
  if(GetDamage(repa)) if(!Random(10)) Reparatur();
  repa=FindObject(PRL_,-20,-20,40,40, nil, "Gelandet", nil, NoContainer());
  if(GetDamage(repa)) if(!Random(10)) Reparatur();  
  //Cougar
  repa=FindObject(CGC_,-20,-20,40,40, nil, "Gelandet", nil, NoContainer());
  if(GetDamage(repa)) if(!Random(10)) Reparatur();  
  //Tucan
  repa=FindObject(TCN_,-20,-20,40,40, nil, "Gelandet", nil, NoContainer());
  if(GetDamage(repa)) if(!Random(10)) Reparatur(); 
  //Concordia
  repa=FindObject(CCD_,-20,-20,40,40, nil, "Gelandet", nil, NoContainer());
  if(GetDamage(repa)) if(!Random(10)) Reparatur(); 

}


public func Reparatur()
{
  DoDamage(-3,repa);
  CastParticles("PxSpark",5,20,nil,10,30,40, RGB(187, 214, 224), RGB(200, 224, 250));
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  SetXDir(nil);
  SetYDir(nil);
  return(1);
}

public func FxGuardStop(object pTarget, int iEffectNumber)
{
  //Waffe fallen lassen und Aktion ändern
  iEffectNumber ??= 0;
  EffectVar(1, pTarget, iEffectNumber)->Exit();
  pTarget->SetAction("Idle");
  bArmed = nil;
  //Owner wieder wechseln
  pTarget->SetOwner(-1);
  //Variablen killen
  pGuard = nil;
  curTarget = nil;
  SetAlive(nil);
  StopAutoGuns(EffectVar(1, pTarget, iEffectNumber));
}

public func WayFree(object pCaller, object pEnemy)
{
  //Schauen, ob das Schussfeld frei ist.
  var iDir, pFriend, iX;
  if( (pEnemy->GetX() ?? 0) < (pCaller->GetX() ?? 0))
  {
    iX = (pCaller->GetX() ?? 0) - (pEnemy->GetX() ?? 0);
    iDir = 1;
  }
  else
    iX = (pEnemy->GetX() ?? 0) - (pCaller->GetX() ?? 0);
  
  for(pFriend in FindObjects( Find_InRect(-(iX ?? 0) * (iDir ?? 0), -10, iX, 20), Find_OCF( OCF_CrewMember), Find_Not(Find_Hostile(pCaller->GetOwner()))))
    if(pFriend)
    {
      return(nil);
    }
  
  return(true);
}

// Prüft ob ein Objekt von dieser Drohne abgeknallt wird
public func CheckHostility(object pTarget)
{
  // Ohne IsBulletTarget ist das ganze sinnlos
  if(!(pTarget->~IsBulletTarget(nil, nil, this()))) return(nil);
  // Ich bin besitzerlos
  if(Equal(GetOwner(), NO_OWNER)) return(true);
  // pTarget ist besitzerlos
  if(Equal(pTarget->GetOwner(), NO_OWNER))
  {
    // OCF_CrewMember bleiben verschont
    if((pTarget->GetOCF() ?? 0) & (OCF_CrewMember ?? 0)) return(nil);
    // Ansonsten alle Lebewesen
    return(true);
  }
  // Feindlich?
  if(Hostile(GetOwner(), pTarget->GetOwner())) return(true);
  // Kein Feind
  return(nil);
}

public func StopAutoGuns(object pWeapon)
{  
  if( pWeapon->~GetFMData(FM_Auto))
  { 
    if( Equal(pWeapon->LocalN("stopauto"), nil))
    {
      pWeapon->StopAutoFire();
      //pWeapon->LocalN("stopauto") = true;
      //pWeapon->~OnAutoStop(pWeapon->LocalN("firemode"));
    }  
  }
}

public func TurnOff()
{
  if( GetEffect("Guard", this()))
    RemoveEffect("Guard", this());
}

public func AimAngle(int iMaxAngle, int iRange, bool mode)
{
  iMaxAngle ??= 0;
  iRange ??= 0;
  mode ??= false;
  var angle;

  angle = 90*((GetDir() ?? 0)*2-1);

  if(curTarget)
    angle = Angle(GetX(), GetY(), curTarget->GetX(), curTarget->GetY());
  
  if(And((angle ?? 0) > 0, (angle ?? 0) < 180))
    return(BoundBy(angle, 90, 150));
  else
    return(BoundBy(angle,210,270));
}

public func AmmoStoring()
{
  //Keinen, von dem wir klauen können? :C
  if(! pGuard) return(nil);
  //Clonks Ammobag zurückgeben (falls er einen hat)
  return(pGuard->LocalN("ammobag"));
}

public func EMPShock()
{
  TurnOff();
  EMPShockEffect(15*35);
  return(1);
}

public func FxDamagedStart(object pTarget, int iEffectNumber, int iTemp, int iTime)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iTime ??= 0;
  EffectVar(nil, pTarget, iEffectNumber) = iTime;
  EffectVar(1, pTarget, iEffectNumber) = pTarget->GetClrModulation();
  pTarget->SetClrModulation(RGB(200,200,200));
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if((EffectVar(nil, pTarget, iEffectNumber) ?? 0) < (iEffectTime ?? 0))
    return(-1);
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

public func FxDamagedAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iTime) {
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = Max(EffectVar(nil,pTarget,iEffectNumber),iTime);
}

public func FxDamagedEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "Damaged"))
    return(-2);
}

public func FxDamagedStop(object pTarget, int iEffectNumber) {
  iEffectNumber ??= 0;
  pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber));
  pTarget->~EMPShockEnd();
}

global func EMPShockEffect(int iTime) {
  iTime ??= 0;
  AddEffect("Damaged", this(), 190, 5, nil, DRSU,iTime);
}

global func EMPShocked() {
  return(GetEffect("Damaged",this()));
}

public func RejectEntrance()
{
  return(bArmed);
}

public func ReadyToFire()
{
  if(!!SEqual(GetAction(), "Fly"))
    return(1);
}

public func UpdateCharge() { return(1); }

public func IsBulletTarget(id ID,object obj,object shooter) {
	if(shooter)
		if(Equal(shooter, pGuard))
			return(nil);
	return(bArmed);
}
public func IsDrone() { return(1); }
public func IsEquipment() { return(1); }
public func IsMachine() { return(1); }
public func IsRefineryProduct() { return(1); }

public func IsVehicleProduct(...) { return(1);
}

func CheckInBuilding()
{
  var pObj;
  pObj = Contained(this());
  if(!Equal(GetID(pObj), C4Id("HZCK")))
	Exit(this());
}