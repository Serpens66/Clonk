/*-- Support Drone --*/

#strict

local bArmed;
local pGuard;
local curTarget;

func Initialize()
{
  DoEnergy(100);

  AddEffect("ShowWeapon",this(),1,1,this(),GetID());  
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
  
  SetAction("Idle"); 
  
  if (Random(2)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));
}

// Kann per Script aufgerufen werden, um die Drohne an einen Clonk zu heften
public func ArmToClonk(object pClonk, object pWeapon)
{
  if(!pClonk || !pWeapon) return();
  pGuard = pClonk;
  SetOwner(GetOwner(pGuard));
  return(GetArmed(0, pWeapon));
}

public func GetGuardedClonk() { return(pGuard); }

public func WeaponAt(&x, &y, &r) {
  x = 0;
  y = 8000;
  r = Abs(AimAngle());
  if(r >= 180) {
    r = Abs(180-r);
    r -= 90;
    r *= -1;
  }
  else
    r -= 90;
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
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func OnDmg(int iDamage, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Melee)
    return(-30);
  if(iType & DMG_Projectile)
    return(20);
  if(iType & DMG_Fire)
    return(30);
  if(iType & DMG_Explosion)
    return(-10);
  if(iType & DMG_Energy)
    return(-40);
  if(iType & DMG_Bio)
    return(50);
}

public func Death()
{
  TurnOff();
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(10); else FakeExplode(10,0,0,10);
  return();
}

public func Activate(object byObj)
{
  //Checken, ob der Clonk nicht schon eine hat. :Ooo
  var pDrone;

  for(pDrone in FindObjects( Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(byObj->GetOwner())))
  {
    if(pDrone->GetGuardedClonk() != byObj) continue;
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
  CreateMenu(DRSU, byObj, this(),0,"$NoWeap$");
  
  //Inventar des Clonks durchgehen
  for(var i = 0; i < byObj->ContentsCount(); ++i)
  {
    var pInvObj = byObj->Contents(i);
    //Wenn Waffe, dem Menü hinzufügen
    //if(pInvObj->~GetFMData(FM_Aim)) continue; // Aber keine Waffen, die zielen müssen
    if(pInvObj->~IsWeapon())
      AddMenuItem("$ArmWith$", "GetArmed", pInvObj->GetID(), byObj, 0, pInvObj);
  } 
  
  //Schützling zuweisen
  pGuard = byObj;
  SetOwner(GetOwner(pGuard));
  return(1);
}

public func GetArmed(id idItem, object pItem)
{
  //Waffe iwie verschwunden? Abbrechen!
  if(! pItem) return(0);

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
  pTarget->Exit();
  //Speichern
  EffectVar(0, pTarget, iEffectNumber) = pGClonk;   //Clonk
  EffectVar(1, pTarget, iEffectNumber) = pWeapon;   //Waffe
  //Drone kurzzeitig einem Besitzer geben (FF)
  pTarget->SetOwner(pGClonk->GetOwner());
  //Es lebt!11 haraahara
  SetAlive(true);
}

public func FxGuardTimer(object pTarget, int iEffectNumber, int iTemp)
{
  //Lebt der Clonk noch? Oder noch da? :o
  if( !EffectVar(0, pTarget, iEffectNumber) ) return(pTarget->TurnOff());
  if( !EffectVar(0, pTarget, iEffectNumber)->GetAlive()) return(pTarget->TurnOff());
  
  //Commands killen, die komischerweise vom Follow aufgerufen werden
  if(pTarget->GetCommand() != "Follow" || pTarget->GetCommand() != "MoveTo")
    pTarget->SetCommand(0, "None");
    
  //Wenn gerade keinen Command folgen wir dem zu Beschützenden
  if(!pTarget->GetCommand())
  {
    StopAutoGuns(EffectVar(1, pTarget, iEffectNumber));
    pTarget->AddCommand(0, "Follow", EffectVar(0, pTarget, iEffectNumber));
  }

  //Ausrichtung der Drone
  if(pTarget->GetXDir() > 0) pTarget->SetDir(DIR_Right);
  else
    pTarget->SetDir(DIR_Left);
  
  //Wenn Abstand zu gross müssen wir uns zum Beschützendem beamen
  if( ObjectDistance(pTarget, EffectVar(0, pTarget, iEffectNumber)) > 400)
  {
     Sound("DeEnergize", 0, pTarget);
     Beam(pTarget, EffectVar(0, pTarget, iEffectNumber));
  }
  
  //ggf. Gegner angreifen
  var pEnemy;
  for(pEnemy in FindObjects(Find_Distance(250), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Or(Find_Owner(-1),Find_Hostile(GetController(pTarget))), Find_Func("IsBulletTarget",0,0,this()), Find_Exclude(EffectVar(0, pTarget, iEffectNumber))))  
  {
    if( !PathFree( pTarget->GetX(), pTarget->GetY(), pEnemy->GetX(), pEnemy->GetY()))
      continue;
      
    if(!CheckHostility(pEnemy))
      continue;
    
    if(! Inside(pTarget->GetY(), pEnemy->GetY() - 15, pEnemy-> GetY() + 15))
    {
      //Hinbewegen
      SetCommand(pTarget, "MoveTo", 0, pTarget->GetX(), pEnemy->GetY(), 0, 0, 2);
      // Viel zu nah dran?
      if( Inside(pTarget->GetX(), pEnemy->GetX() - 15, pEnemy->GetX() + 15))
        SetCommand(pTarget, "MoveTo", 0, 2*(pEnemy->GetX() - pTarget->GetX()), pEnemy->GetY(), 0, 0, 2);
    }
    if( !WayFree(pTarget, pEnemy))
    {
      SetCommand(pTarget, "MoveTo", 0, pEnemy->GetX(), pEnemy->GetY(), 0, 0, 2);
      Sound("BipBipBip");
    }
    else 
    {
      //Zum Gegner drehen
      if(pEnemy->GetX() > pTarget->GetX()) pTarget->SetDir(DIR_Right);
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
      
    if(! Inside(pTarget->GetY(), pAnimal->GetY() - 15, pAnimal-> GetY() + 15))
    {
      //Hinbewegen
      SetCommand(pTarget, "MoveTo", 0, pTarget->GetX(), pAnimal->GetY(), 0, 0, 2);
      // Viel zu nah dran?
      if( Inside(pTarget->GetX(), pAnimal->GetX() - 15, pAnimal->GetX() + 15))
        SetCommand(pTarget, "MoveTo", 0, 2*(pAnimal->GetX() - pTarget->GetX()), pAnimal->GetY(), 0, 0, 2);
    }
    if( !WayFree(pTarget, pAnimal))
    {
      SetCommand(pTarget, "MoveTo", 0, pAnimal->GetX(), pAnimal->GetY(), 0, 0, 2);
      Sound("BipBipBip");
    }
    else 
    {
      //Zum Gegner drehen
      if(pAnimal->GetX() > pTarget->GetX()) pTarget->SetDir(DIR_Right);
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
}

public func FxGuardStop(object pTarget, int iEffectNumber)
{
  //Waffe fallen lassen und Aktion ändern
  EffectVar(1, pTarget, iEffectNumber)->Exit();
  pTarget->SetAction("Idle");
  bArmed = false;
  //Owner wieder wechseln
  pTarget->SetOwner(-1);
  //Variablen killen
  pGuard = 0;
  curTarget = 0;
  SetAlive(false);
  StopAutoGuns(EffectVar(1, pTarget, iEffectNumber));
}

public func WayFree(object pCaller, object pEnemy)
{
  //Schauen, ob das Schussfeld frei ist.
  var iDir, pFriend, iX;
  if( pEnemy->GetX() < pCaller->GetX())
  {
    iX = pCaller->GetX() - pEnemy->GetX();
    iDir = 1;
  }
  else
    iX = pEnemy->GetX() - pCaller->GetX();
  
  for(pFriend in FindObjects( Find_InRect(-iX * iDir, -10, iX, 20), Find_OCF( OCF_CrewMember()), Find_Not(Find_Hostile(pCaller->GetOwner()))))
    if(pFriend)
    {
      return(false);
    }
  
  return(true);
}

// Prüft ob ein Objekt von dieser Drohne abgeknallt wird
public func CheckHostility(object pTarget)
{
  // Ohne IsBulletTarget ist das ganze sinnlos
  if(!(pTarget->~IsBulletTarget(0, 0, this()))) return(false);
  // Ich bin besitzerlos
  if(GetOwner() == NO_OWNER) return(true);
  // pTarget ist besitzerlos
  if(pTarget->GetOwner() == NO_OWNER)
  {
    // OCF_CrewMember bleiben verschont
    if(pTarget->GetOCF() & OCF_CrewMember) return(false);
    // Ansonsten alle Lebewesen
    return(true);
  }
  // Feindlich?
  if(Hostile(GetOwner(), pTarget->GetOwner())) return(true);
  // Kein Feind
  return(false);
}

public func StopAutoGuns(object pWeapon)
{  
  if( pWeapon->~GetFMData(FM_Auto))
  { 
    if( pWeapon->LocalN("stopauto") == false)
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
  var angle;

  angle = 90*(GetDir()*2-1);

  if(curTarget)
    angle = Angle(GetX(), GetY(), curTarget->GetX(), curTarget->GetY());
  
  if(angle > 0 && angle < 180)
    return(BoundBy(angle, 90, 150));
  else
    return(BoundBy(angle,210,270));
}

public func AmmoStoring()
{
  //Keinen, von dem wir klauen können? :C
  if(! pGuard) return(0);
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
  EffectVar(0, pTarget, iEffectNumber) = iTime;
  EffectVar(1, pTarget, iEffectNumber) = pTarget->GetClrModulation();
  pTarget->SetClrModulation(RGB(200,200,200));
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  
  if(EffectVar(0, pTarget, iEffectNumber) < iEffectTime)
    return(-1);
  // effects
  if(!(pTarget->~Contained())) {
    var wdt = GetDefWidth(GetID(pTarget));
    var hgt = GetDefHeight(GetID(pTarget));
    Sparks(2,0,GetX(pTarget)+RandomX(-wdt/2,wdt/2),GetY(pTarget)+RandomX(-hgt/2,hgt/2));
    if(!Random(3)) {
		var pLaser = CreateObject(LASR, GetX(pTarget)+RandomX(-wdt/4,wdt/4), GetY(pTarget)+RandomX(-hgt/4,hgt/4), -1);
		pLaser->SetClrModulation(RGBa(255, 200, 0, 64));
		pLaser->Set(Random(360), 8, Sqrt(wdt*wdt/4+hgt*hgt/4), 3);
	}
  }
}

public func FxDamagedAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iTime) {
  EffectVar(0,pTarget,iEffectNumber) = Max(EffectVar(0,pTarget,iEffectNumber),iTime); 
}

public func FxDamagedEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName eq "Damaged")
    return(-2);
}

public func FxDamagedStop(object pTarget, int iEffectNumber) {
  pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber));
  pTarget->~EMPShockEnd();
}

global func EMPShockEffect(int iTime) {
  AddEffect("Damaged", this(), 190, 5, 0, DRSU,iTime);
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
  if(GetAction() eq "Fly")
    return(1);
}

public func UpdateCharge() { return(1); }

public func IsBulletTarget(id ID,object obj,object shooter) {
	if(shooter)
		if(shooter == pGuard)
			return(false);
	return(bArmed);
}
public func IsDrone() { return(1); }
public func IsEquipment() { return(1); }
public func IsMachine() { return(1); }
public func IsRefineryProduct() { return(1); }
public func IsThreat() {return(1);}

func CheckInBuilding()
{
  var pObj;
  pObj = Contained(this());
  if(GetID(pObj) != C4Id("HZCK"))
	Exit(this());
}