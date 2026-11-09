/*-- Phönix --*/

#strict 3

public func IsPossessible() { return 1; }
public func IsThreat() {return(true);}

public func SearchRange() { return 150; }

local pTarget;
local pMaster;

public func Birth()
{
  SetAction("Fly");
  if (Random(2)) SetComDir(COMD_Left);
  else SetComDir(COMD_Right);
  return(1);
}

/* Initialisierung */

/*
public func Purchase(int iByPlr, object BuyObject)
{
	var pCultSite = FindObject2(Find_ID(CLST), Find_Allied(iByPlr));
	if(pCultSite)
	{
		pCultSite->Summon(GetID(), BuyObject, 1);
		DrawParticleLine("MSpark", GetX(pCultSite)-GetX(), GetY(pCultSite)-GetY(), GetX(BuyObject)-GetX(), GetY(BuyObject)-GetY(), 20, 100, RGBa(255,255,255,50), RGBa(200,255,255,100), -5);
		Message("", BuyObject, GetName());
		RemoveObject(this);
	}
}
*/

protected func Initialize()
{
  SetAction("Fly");
  AddEffect("IntFire", this, 1, 1, this);
  var iColor;
  iColor = RGBa(235,150,10,50);
  AddLight3(800, iColor, this());
  if (Random(2)) return SetComDir(COMD_Right);
  return SetComDir(COMD_Left);
}

// Feuereffekt
func FxIntFireTimer(pTarget, iNumber, iTime) 
{
  var iDir = -1+2*(GetDir() ?? 0);
  var iX = -10*(iDir ?? 0);
  if(S42MigrationEqual(GetAction(), "Turn")) iX =-(Cos((GetPhase() ?? 0)*18, iX) ?? 0);
  if(!GBackLiquid())
    for(var i = nil; (i ?? 0)<2; S42MigrationStep(i, 1, true))
      CreateParticle("Fire2",RandomX(nil,iX),RandomX(nil, 4),-(Random(15) ?? 0)*(-1+2*(GetDir() ?? 0)),RandomX(2, -2),RandomX(40,60),RGB(210,120,110), this, 1);
  if(!Random(3)) CreateParticle("Fire2",(RandomX(10,-5) ?? 0)*(iDir ?? 0),RandomX(nil, 4),-(Random(15) ?? 0)*(-1+2*(GetDir() ?? 0)),RandomX(2, -2),RandomX(40,60),RGB(210,120,110), this);
  var pObj;
  if(!Mod(iTime, 6))
  {
    var aEnemy = Find_Not(Find_Owner(-1));
    if((GetOwner() ?? 0)>=0) aEnemy = Find_Hostile(GetOwner());
    for(pObj in FindObjects(Find_Distance(20), Find_OCF(OCF_Prey), Find_Container(Contained()), Find_OCF(OCF_Alive),aEnemy))
    {
      Sound("Inflame");
      DoEnergy(-1, pObj);
    }
  }
}

func FxIntFireEffect(string szNewEffectName, object pTarget) {
  // nicht anbrennen lassen
  if(WildcardMatch(szNewEffectName,"*Fire*")) {
    return -1;
  }
}

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Nur wenn noch lebt
  if(!GetAlive()) return;
  // Geräusche machen
//  if (!Random(25)) Sound("Raven*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  // Gegner suchen
  if(!pTarget && !GetCommand())
  {
    if(S42MigrationEqual(GetOwner(), -1)) pTarget = FindObject2(Find_Distance(SearchRange()), Find_OCF((OCF_CrewMember ?? 0) | (OCF_Prey ?? 0)), Find_NoContainer());
    else pTarget = FindObject2(Find_Distance(SearchRange()), Find_Hostile(GetOwner()), Find_OCF((OCF_CrewMember ?? 0) | (OCF_Prey ?? 0)), Find_NoContainer());//FindObject(0, -SearchRange(), -SearchRange(), SearchRange()*2, SearchRange()*2, OCF_CrewMember | OCF_Prey, 0, 0, NoContainer());
    if(pTarget) if(S42MigrationEqual(GetAction(), "Hang")) SetAction("Fly");
  }
  
  // Beute am Maul beißen
  if (Attack()) return;
  
  // Angreifen
  if(!pMaster || (ObjectDistance(pMaster) ?? 0)<300)
  if(pTarget)
  {
    // Wenn Gegner im Haus, tot oder zu weit weg abbrechen    
    if(Contained(pTarget)) { SetCommand(this, "None"); return pTarget=nil; }
    if(!GetAlive(pTarget)) { SetCommand(this, "None"); return pTarget=nil; }
    if((ObjectDistance(pTarget) ?? 0)>(SearchRange() ?? 0)) { SetCommand(this, "None"); return pTarget=nil; }
    // sonst zum Gegner fliegen
    var iFromSide = +1; if ((GetX(pTarget) ?? 0)>(GetX() ?? 0)) iFromSide = -1;
    SetCommand(this, "MoveTo", nil, (GetX(pTarget) ?? 0) + 13 * (iFromSide ?? 0), GetY(pTarget));
    if((iFromSide ?? 0)>0 && GetDir()) TurnLeft();
    if((iFromSide ?? 0)<0 && !GetDir()) TurnRight();
    return;
  }
  if(pMaster)
  {
    if(!GetAlive(pMaster)) return pMaster = nil;
    if((ObjectDistance(pMaster) ?? 0)>150)
    {
      CastParticles("PxSpark", 10,20,nil,nil,50,80,RGB(200,100,nil), RGB(200,100,nil));
      SetPosition(GetX(pMaster), GetY(pMaster));
      CastParticles("PxSpark", 10,20,nil,nil,50,80,RGB(200,100,nil), RGB(200,100,nil));
    }
    // zum Meister fliegen
    var iFromSide = +1; if ((GetX(pMaster) ?? 0)>(GetX() ?? 0)) iFromSide = -1;
    SetCommand(this, "MoveTo", nil, (GetX(pMaster) ?? 0) + 13 * (iFromSide ?? 0), GetY(pMaster));
    if((iFromSide ?? 0)>0 && GetDir()) TurnLeft();
    if((iFromSide ?? 0)<0 && !GetDir()) TurnRight();
    return;
  }
  
  // Nichts machen
  if (Random(2) || !S42MigrationEqual(GetAction(), "Fly")) return 1;

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if (Random(2)) SetComDir(COMD_Down);

  // Nichts machen
  if (!Random(4)) return 1;

  // Umdrehen
  if (Random(2)) return TurnRight();
  return TurnLeft();
}

protected func HitCheck()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;
}

public func Attack(object pAttack)
{
  // Während des Fliegens kann alle 2 Sekunden einmal gebissen werden
  if (GetEffect("IntAttackDelay", this) || !S42MigrationEqual(GetAction(), "Fly")) return;

  // Beute suchen
  var aEnemy = Find_Not(Find_Owner(-1));
  if((GetOwner() ?? 0)>=0) aEnemy = Find_Hostile(GetOwner());
  pAttack = FindObject2(Find_AtPoint(-10+20*(GetDir() ?? 0)), Find_OCF(OCF_Prey), Find_NoContainer(), aEnemy);

  if (!pAttack) return;

  if(GetAlive(pAttack))
  {
    // Schaden machen und schleudern      
    Punch(pAttack);
    if((GetX(pAttack) ?? 0) < (GetX(this) ?? 0))
      Fling(pAttack, -1, -1);
    else
      Fling(pAttack, 1, -1);
  }
  
  // Reinbeißen!
  SetAction("Attack");
  Sound("Raven*");
  AddEffect("IntAttackDelay", this, 1, 70);
  return 1;
}

protected func Survive()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (InLiquid() || GBackLiquid((GetXDir() ?? 0)*2, (GetYDir() ?? 0)*2) || GBackLiquid((GetXDir() ?? 0)*3, (GetYDir() ?? 0)*3) || FindObject(nil, -20 + (GetXDir() ?? 0) * 2, -20 + (GetYDir() ?? 0) * 2, 40, 40, OCF_OnFire))
   SetComDir(COMD_Up);
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return TurnRight();
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return TurnLeft();
}

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return SetComDir(COMD_Down);
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return SetComDir(COMD_Up);
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || !S42MigrationEqual(GetAction(), "Fly")) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right); 
  return 1;
}

public func TurnLeft()
{
  if (Stuck() || !S42MigrationEqual(GetAction(), "Fly")) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left); 
  return 1;
}

/* Einwirkungen */

protected func CatchBlow(iLevel, pObject)
{
  if (pObject->~IsArrow()) SetAction("Tumble");
  if (!Random(3)) Sound("RavenHurt");
  return 1;
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (S42MigrationEqual(szCommand, "MoveTo"))
  return SetCommand(this,szCommand, pTarget, iTx, iTy);
 return nil;
}

protected func ContainedLeft()
{
 [$TxtMovement$]
 SetCommand(this, "None");
 TurnLeft();
 return 1;
}

protected func ContainedRight()
{
 [$TxtMovement$]
 SetCommand(this, "None");
 TurnRight();
 return 1;
}

protected func ContainedUp()
{
 [$TxtMovement$]
 SetCommand(this, "None");
 SetComDir(COMD_Up);
 return 1;
}

protected func ContainedDown()
{
 [$TxtMovement$]
 SetCommand(this, "None");
 if(S42MigrationEqual(GetAction(), "Hang")) SetAction("Fly");
 SetComDir(COMD_Down);
 return 1;
}

protected func ContainedDig()
{
 [$TxtAttack$]
 Attack();
 return 1;
}

protected func ContainedThrow() { return 1; }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this);
 return 1;
}

public func Summoned(pClonk)
{
  pMaster = pClonk;
  AddEffect("FriendProtection",this,20,nil,this);
}

/* Schutz vor Freunden */

public func FxFriendProtectionStart(pTarget,iEffectNumber)
{
  return 1;
}

public func FxFriendProtectionDamage(pTarget,iEffectNumber,iDmgEngy,iCause,iPlr)
{
 if(GetPlayerName(iPlr))
   if(!Hostile(iPlr,GetOwner(pTarget)))
   {
    if(S42MigrationEqual(iCause, 35)) { Extinguish(pTarget); return; }
    if((iDmgEngy ?? 0) > 0) return iDmgEngy;
    return nil;
   }
 
 return iDmgEngy;
}

public func Death()
{
  GameCallEx("Zaehler13");
  CastPXS("FlyAshes", 300, 20);
  for( var i=5; (i ?? 0)>0; S42MigrationStep(i, -1))
  {
    CreateParticle("Fire", RandomX(-2,2), RandomX(-2,2), (Sin(Random(360), RandomX(50,65)) ?? 0)+(RandomX(-2,2) ?? 0), (Cos(Random(360), RandomX(50,65)) ?? 0)+(RandomX(-2,2) ?? 0), 100+(Random(55) ?? 0), RGBa(Random(255),nil,nil,90));
    CreateParticle("Fire2", RandomX(-2,2), RandomX(-2,2), (Sin(Random(360), RandomX(50,65)) ?? 0)+(RandomX(-2,2) ?? 0), (Cos(Random(360), RandomX(50,65)) ?? 0)+(RandomX(-2,2) ?? 0), 100+(Random(55) ?? 0));
  }
  CreateParticle("Blast", nil,nil,nil,nil, 100*5);
  Sound("Blast3", this);
  CreateObject(P_EG,nil,nil,-1); // ein phoenixei
  RemoveObject();
}

// Kann mit einem Ritual beschworen werden
public func IsRitual()   { return 1; }

// Nur für die Blutkirche
//public func NeedChurch() { return scLightchurch; }

// Kann nicht in der Werkstatt produziert werden
public func RejectWorkshopConstruction(){return true;}
