/*-- Ritter --*/

#strict 3

#include CLNK

/* Lokale Variablen */

local iOrder, pOrdrTarget;
local ordrData1, ordrData2;
local iLastAimAngle;
local fArmored, idArmorType;
local idShield;
local waartezeit;

// Vom König ueberladbar, gibt Anzahl an Standard-Portraits an
public func StdPortraitCount() { return(3); }

/* Bei dem Beitritt zum Spieler Portraits prüfen */

public func Recruitment(int player)
{
  // Zur Netzwerk-Sicherheit
  player ??= 0;
  var iRandomPortrait = Random(StdPortraitCount());
  // Mal schauen, was fuer ein Portrait wir haben
  var portrait = GetPortrait(this(), nil, 1);
  var portrait_id = GetPortrait(this(), 1, 1);
  // Ist es ein eigenes Portrait? Dann lassen wir es.
  if(!!SEqual(portrait, "custom")) return(_inherited() );
  // Ist eine ID angegeben?
  if(And(portrait_id, !Equal(portrait_id, NONE)))
  {
    // Kein Ritterportrait?
    if(!Equal(portrait_id, GetID()) )
    {
      SetPortrait(Format("%d", 1 + (iRandomPortrait ?? 0)), this(), GetID(), true, nil);
      return(_inherited() );
    }
  }
  // Es handelt sich um ein Ritterportrait. Aber ein richtiges?
  if(!WildcardMatch(portrait, "?") )
  {
    // Nein, das war ein Ruestungsportrait oder sowas. Wir wollen aber lieber ein normales.
    SetPortrait(Format("%d", 1 + (iRandomPortrait ?? 0)), this(), GetID(), true, nil);
  }
  return(_inherited(player) );
}

/*--------------------------------------------- Status -------------------------------------------------*/

//! Welche Objekte kann dieser Clonk einsetzen?
public func CanUse(def)
{
  // Schwert
  if (Equal(def, SWOR)) return(1);
  // Axt
  if (Equal(def, AXE1)) return(1);
  // Bogen 
  if (Equal(def, BOW1)) return(1);
  // Schild 
  if (Equal(def, SHIE)) return(1);
  // Speer 
  if (Equal(def, SPER)) return(1);
  // Lanze 
  if (Equal(def, LANC)) return(1);
  // Rüstung 
  if (Equal(def, ARMR)) return(1);
  // Schwarze Rüstung 
  if (Equal(def, BRMR)) return(1);
  // Unbekannt 
  return(nil);
} 
 
//! Bevorzugte Waffentypen für selbständige Bewaffnung (zur Zeit werden genau zwei Waffentypen erwartet). 
public func GetFightWeaponType(index) 
{ 
  if (Equal(index, nil)) return(SWOR);
  if (Equal(index, 1)) return(AXE1);
  return(nil);
} 
 
//! Dieser Clonk ist ein Ritter
public func IsKnight()  
{  
  return(1);  
} 
 
//! Reitet der Clonk gerade? 
private func IsRiding() 
{
  return (WildcardMatch(GetAction(), "Ride*"));
} 
 
//! Zielt der Clonk gerade mit dem Bogen? 
private func IsAiming() 
{ 
  return (Or(!!SEqual(GetAction(), "AimBow"), !!SEqual(GetAction(), "RideAimBow")));
} 
 
//! Lädt der Clonk gerade den Bogen nach? 
private func IsLoading() 
{ 
  return (Or(!!SEqual(GetAction(), "LoadBow"), !!SEqual(GetAction(), "RideLoadBow")));
} 
 
//! Legt der Clonk gerade eine Rüstung an? 
private func IsPuttingOnArmor() 
{ 
  return (!!SEqual(GetAction(), "PutOnArmor"));
} 
 
//! Der Clonk gilt als bewaffnet, wenn er mindestens ein Schild oder eine Rüstung nebst einer Handwaffe trägt.
private func IsFullyArmed() 
{ 
  return (And(HasFightWeapon(), (Or(HasShield(), HasArmor()))));
} 
 
//! Nicht voll bewaffnet. 
public func IsNotFullyArmed()  
{  
  return (!IsFullyArmed());  
} 
 
//! Trägt der Clonk einen Bogen und hat Pfeile dabei? 
private func IsBowEquipped() 
{ 
  // Bogen dabei? 
  if (!FindContents(BOW1)) return(nil);
  // Pfeile dabei? 
  if (!GetArrowCount()) return(nil);
  // Bogen und Pfeile dabei 
  return(1); 
} 
 
//! Liefert den Schutzfaktor. 
public func IsShielded(fArrow) 
{ 
  var iShielded = nil, iShieldFactor;
  // Schild 
  var pShield; 
  if (pShield = HasShield()) 
    // Benutzerdefiniertes Schild 
    if (iShieldFactor = pShield->~ShieldFactor(fArrow)) 
      S42MigrationAssign(iShielded, "+=", (iShieldFactor ?? 0));
    // Normales Schild 
    else 
      // Das Standardschild hat einen Bonus gegen Pfeile 
      if (fArrow) 
        S42MigrationAssign(iShielded, "+=", 20);
      else 
        S42MigrationAssign(iShielded, "+=", 10);
  // Rüstung                    
  if (HasArmor()) 
    S42MigrationAssign(iShielded, "+=", (DefinitionCall(idArmorType, "ShieldFactor") ?? 0));
  // Wert zurückliefern 
  return(iShielded); 
} 
 
//! Prüft ob pObj (oder dieser Clonk) ein Schild trägt. 
public func HasShield(pObj)  
{  
  if (!pObj) pObj = this();
  if(LocalN("idShield", pObj)) return(LocalN("idShield", pObj));
  return (FindObject(nil,nil,nil,nil,nil,nil, "Shield", pObj));
}
 
//! Prüft ob pObj (oder dieser Clonk) eine Lanze trägt. 
public func HasLance(pObj)  
{  
  if (!pObj) pObj = this();  
  return (FindObject(LNCA, nil,nil,nil,nil,nil, "Lance", pObj));
} 
 
//! Prüft ob pObj (oder dieser Clonk) eine Rüstung trägt. 
public func HasArmor(pObj)  
{ 
  if (pObj) return(pObj->~HasArmor());  
  return(fArmored);  
} 
 
//! Prüft ob der Clonk eine Handwaffe trägt, die er verwenden kann. 
public func HasFightWeapon(pClonk) 
{  
  // Anderer Clonk 
  if (pClonk)  
    return (pClonk->~HasFightWeapon()); 
  // Dieser Clonk 
  var pObj; 
  for (var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if (And(pObj->~IsFightWeapon(), CanUse(GetID(pObj))))
      return(1); 
  // Keine Waffe 
  return(nil);
} 
 
//! Liefert die erste gefundene Handwaffe, die der Clonk verwenden kann. 
public func GetFightWeapon(pClonk) 
{  
  // Anderer Clonk 
  if (pClonk)  
    return (pClonk->~GetFightWeapon()); 
  // Dieser Clonk 
  var pObj; 
  for (var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if (And(pObj->~IsFightWeapon(), CanUse(GetID(pObj))))
      return(pObj); 
  // Keine Waffe 
  return(nil);
} 
 
//! Kann der Clonk aus der aktuellen Aktion den Bogen oder eine Wurfwaffe einsetzen? 
public func ReadyToFire() 
{ 
  // Nur beim Laufen - und neu auch beim Reiten... 
  if (Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Ride")), !!SEqual(GetAction(), "RideStill"))) return(1);
  return(nil);
} 
 
//! Der Ritter braucht einen Sattel zum Reiten. 
public func NeedSaddle()  
{  
  return(1);  
} 
 
/*--------------------------------------------- Befehle ------------------------------------------------*/   
 
private func OrderNone()       { return(nil);   }
private func OrderAttack()     { return(101); } 
private func OrderDefend()     { return(102); } 
private func OrderGuard()      { return(103); } 
private func OrderArm()        { return(104); } 
private func OrderThrowSpear() { return(105); } 
 
/* Befehlsausführung */ 

protected func ExecuteOrder() // TimerCall 
{ 
  // Tot: nix mehr Befehle 
  if (!GetAlive()) return;
  // Kommando: Zielen abbrechen 
  if (GetCommand()) StopAiming(); 
  // Kommandoausführung (außer Warten) 
  if (GetCommand())  
    if (!SEqual(GetCommand(), "Wait"))
      return;
  // Angriff   
  if (Equal(iOrder, OrderAttack())) ExecuteOrderAttack();
  // Verteidigung 
  if (Equal(iOrder, OrderDefend())) ExecuteOrderDefend();
  // Bewachen 
  if (Equal(iOrder, OrderGuard())) ExecuteOrderGuard();
  // Bewaffnen 
  if (Equal(iOrder, OrderArm())) ExecuteOrderArm();
  // Speer werfen 
  if (Equal(iOrder, OrderThrowSpear())) ExecuteOrderThrowSpear();
  // Fertig 
  return(1); 
} 
 
private func ExecuteOrderDefend() 
{ 
  // Distanz von der Verteidigungsposition ermitteln 
  var iDx=Abs((GetX() ?? 0)-(ordrData1 ?? 0)), iDy=Abs((GetY() ?? 0)-(ordrData2 ?? 0));
  // Kein Ziel: zurück zur Verteidigungsposition 
  if (!pOrdrTarget) 
    if (Or((iDx ?? 0)>20, (iDy ?? 0)>50))
      return(SetCommand(this(),"MoveTo",nil,ordrData1,ordrData2));
  // Zu weit weg von Verteidigungsposition 
  if (Or((iDx ?? 0)>150, (iDy ?? 0)>150))
    return(S42MigrationFirst(1, pOrdrTarget=nil));
  // Keine feindliche Einheit in Reichweite 
  pOrdrTarget=FindEnemyUnit(); 
  if (!pOrdrTarget) return(ExecuteWatch()); 
  if ((ObjectDistance(pOrdrTarget) ?? 0) > 350) return(ExecuteWatch());
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return(1); 
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return(1); 
  // Direkter Angriff (mit kurzfristiger Laufzeit) 
  StopAiming(); 
  AddCommand(this(),"Attack",pOrdrTarget,nil,nil,nil,2);
  // Weiter 
  return(1); 
} 
   
private func ExecuteOrderGuard() 
{ 
  // Distanz von der Verteidigungsposition ermitteln 
  var iDx=Abs((GetX() ?? 0)-(ordrData1 ?? 0)), iDy=Abs((GetY() ?? 0)-(ordrData2 ?? 0));
  // Zurück zur Verteidigungsposition 
  if (Or((iDx ?? 0)>10, (iDy ?? 0)>20))
    return(SetCommand(this(),"MoveTo",nil,ordrData1,ordrData2));
  // Keine feindliche Einheit in Reichweite 
  pOrdrTarget=FindEnemyUnit(); 
  if (!pOrdrTarget) return(ExecuteWatch()); 
  if ((ObjectDistance(pOrdrTarget) ?? 0) > 350) return(ExecuteWatch());
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return(1); 
  // Direkter Angriff (mit kruzfristiger Laufzeit) wenn Ziel ganz nah 
  StopAiming(); 
  if ((ObjectDistance(pOrdrTarget) ?? 0) < 30)
    return(AddCommand(this(),"Attack",pOrdrTarget,nil,nil,nil,2));
  // Position halten 
  return(1); 
} 
   
private func ExecuteWatch() 
{ 
  if (Random(3)) return(1); 
  if (!!SEqual(GetAction(), "Walk")) SetDir(Random(2));
  if (!Random(500)) { 
    var now = Random(24); 
    if (!Random(500)) now = 42; 
    var state = "$MsgWatchStateWell$"; 
    if ((GetEnergy() ?? 0) < 10)
      state = "$MsgWatchStateBad$"; 
    Message("$MsgWatch$", this(), now, state); 
  } 
  return(1); 
} 
   
private func ExecuteOrderAttack() 
  { 
  // Kein Zielobjekt 
  if (!pOrdrTarget) return(OrderFailure("$MsgNoEnemy$")); // "Kein Angriffsziel!" 
  // Ziel ist tot 
  if (!GetAlive(pOrdrTarget)) 
    return(OrderSuccess("$MsgEnemyDeath$",GetName(pOrdrTarget))); // "%s erledigt." 
  // Ziel nicht mehr feindlich 
  if (!Hostile(GetOwner(pOrdrTarget),GetOwner())) 
    return(OrderSuccess("$MsgEnemyHostChanged$",GetName(pOrdrTarget))); // "%s gehört|jetzt zu uns." 
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return(1); 
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return(1); 
  // Direkter Angriff (mit kruzfristiger Laufzeit) 
  StopAiming(); 
  AddCommand(this(),"Attack",pOrdrTarget,nil,nil,nil,2);
  // Weiter 
  return(1); 
  } 
 
private func ExecuteAttackBow(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite? 
  if (Or((Abs((iTx ?? 0)-(GetX() ?? 0)) ?? 0)>300, (Abs((iTy ?? 0)-(GetY() ?? 0)) ?? 0)>300))
    // Nicht? Mögliche Abschußposition einnehmen 
    return( SetCommand(this(),"MoveTo",nil, BoundBy(GetX(),(iTx ?? 0)-290,(iTx ?? 0)+290),GetY()) );
  // Bogen auswählen 
  ShiftContents(nil, nil, BOW1);
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!ReadyToFire()) 
      return(nil);
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return(nil);
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),(GetY() ?? 0)-20, (iTy ?? 0)-20 ))
      return(nil);
  // Schießen, aber immer kurz warten, weil sonst viel zu schnell
  if(waartezeit) return(nil);
  FireBowAt(iTx,iTy, nil); waartezeit=1;
  ScheduleCall(nil,"waartezeitbeenden",30); // nach 30 Frames kann wieder geschossen werden
  // Weitermachen   
  return(1); 
  } 

func waartezeitbeenden() {waartezeit=nil;}
 
private func ExecuteAttackSpear(pTarget) 
  { 
  // Speer noch vorhanden? 
  var pSpear = FindSpearContents(); if (!pSpear) return;
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  var iDx = (iTx ?? 0)-(GetX() ?? 0);
  // Schon zu nah 
  if ((Abs(iDx) ?? 0)>50) return(nil);
  // Ziel in Reichweite? 
  if (Or((Abs(iDx) ?? 0)>120, (Abs((iTy ?? 0)-(GetY() ?? 0)) ?? 0)>40))
      // Mögliche Abschußposition einnehmen 
      return(SetCommand( this(),"MoveTo",nil, BoundBy(GetX(),(iTx ?? 0)-100,(iTx ?? 0)+100),GetY() ));
  // Speer auswählen 
  ShiftContents(nil,nil,GetID(pSpear));
  // Kann leider von hier aus nicht werfen 
  if (!ReadyToFire()) return(nil);
  // Ziel nicht im Freien 
  if (Contained(pTarget)) return(nil);
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),(GetY() ?? 0)-10, iTx,(iTy ?? 0)-10 ))
      if (!PathFree( GetX(),(GetY() ?? 0)-20, iTx,(iTy ?? 0)-20 ))
        return(nil);
  // Richtung  
  if ((iDx ?? 0)>0) SetDir(DIR_Right); else SetDir(DIR_Left);
  // Speer werfen 
  if (IsRiding())
    SetAction("RideThrowSpear"); 
  else
    SetAction("ThrowSpear"); 
  // Weitermachen   
  return(1); 
  } 
 
private func ExecuteOrderThrowSpear() 
{ 
  // Kein Speer? 
  if (Or(!pOrdrTarget, (And(!ObjectCall(pOrdrTarget, "IsSpear"), !Equal(GetID(pOrdrTarget), SPER)))))
    return(OrderFailure("$MsgNoSpear$"));
  // Speer nicht dabei: einsammeln 
  if (!Equal(Contained(pOrdrTarget), this()))
    return(SetCommand(this(),"Get",pOrdrTarget)); 
  // Ziel in Reichweite? 
  if (Or((Abs((ordrData1 ?? 0)-(GetX() ?? 0)) ?? 0)>120, (Abs((ordrData2 ?? 0)-(GetY() ?? 0)) ?? 0)>40))
    // Mögliche Abschußposition einnehmen 
    return(SetCommand(this(),"MoveTo",nil, BoundBy(GetX(),(ordrData1 ?? 0)-100,(ordrData1 ?? 0)+100),GetY() ));
  // Speer auswählen 
  ShiftContents(nil,nil,GetID(pOrdrTarget));
  // Kann leider von hier aus nicht werfen 
  if (!ReadyToFire()) return(nil);
  // Richtung  
  if ((ordrData1 ?? 0)>(GetX() ?? 0))
    SetDir(DIR_Right);
  else 
    SetDir(DIR_Left);
  // Speer werfen 
  if (IsRiding())
    SetAction("RideThrowSpear"); 
  else
    SetAction("ThrowSpear"); 
  // Fertig 
  return(OrderSuccess()); 
} 
 
private func ExecuteOrderArm() 
  { 
  // Schild dabei: anlegen 
  var pWeapon; 
  if (!HasShield()) 
    if (pWeapon=FindContents(SHIE)) 
      return(ProtectedCall(pWeapon,"Activate",this())); 
    else 
      // Noch kein Schild: suchen 
      if (pWeapon=FindAvailableWeapon(SHIE)) 
        return(SetCommand(this(),"Get",pWeapon)); 
  // Noch keine Handwaffe: suchen 
  if (!HasFightWeapon()) 
    { 
    var idWp1,idWp2; 
    if (Random(2))  
      { idWp1=GetFightWeaponType(nil); idWp2=GetFightWeaponType(1); }
    else  
      { idWp1=GetFightWeaponType(1); idWp2=GetFightWeaponType(nil); }
    pWeapon = FindAvailableWeapon(idWp1); 
    if (!pWeapon) pWeapon = FindAvailableWeapon(idWp2); 
    if (pWeapon) return(SetCommand(this(),"Get",pWeapon)); 
    } 
  // Wenn's geht noch einen Bogen suchen 
  if (!FindContents(BOW1)) 
    { 
    if (pWeapon=FindAvailableWeapon(BOW1)) 
      return(SetCommand(this(),"Get",pWeapon)); 
    } 
  else 
    // Hat einen Bogen: noch Pfeile dazu suchen 
    if ((GetArrowCount() ?? 0)<21)
      if (pWeapon=FindAvailableArrow())
        return(SetCommand(this(),"Get",pWeapon));   
  // Wenn's geht noch einen Speer suchen 
  if (!FindContents(SPER)) 
    if (pWeapon=FindAvailableWeapon(SPER)) 
      return(SetCommand(this(),"Get",pWeapon)); 
  // Fehlgeschlagen 
  if (!IsFullyArmed()) return(OrderFailure("$MsgNoWeapons$")); // "Keine Waffen." 
  // Fertig   
  return(OrderSuccess("$MsgArmed$")); // "Bewaffnet." 
  } 
 
// Verfügbare Waffe suchen 
private func FindAvailableWeapon(idOfType) 
  { 
  var pWeapon = FindObject(idOfType,nil,nil,-1,-1,OCF_Available);
  if ((ObjectDistance(pWeapon) ?? 0) < 500) return(pWeapon);
  return(nil);
  } 
  
private func FindAvailableArrow()
{
  // Brandpfeile > Sprengpfeile > Pfeile
  var weapon = FindAvailableWeapon(FARP);
  if(!weapon) weapon = FindAvailableWeapon(XARP);
  if(!weapon) weapon = FindAvailableWeapon(ARWP);
  return(weapon);
}
  
/*------------------------------------------ Befehle geben ---------------------------------------------*/   
 
private func SetOrder(iToOrder, pTarget) { iOrder=iToOrder; pOrdrTarget=pTarget; return(1); } 
private func GetOrder() { return(iOrder); } 
private func SetData(pTarget) { pOrdrTarget=pTarget; return(1); } 
private func GetData() { return(pOrdrTarget); } 
 
public func SetOrderNone() 
  { 
  SetOrder(OrderNone()); 
  return(1); 
  } 
 
private func OrderFailure(szMessage) 
  { 
  Sound("KnightFailure"); 
  SetOrderNone(); 
  SetCommand(this(),"None"); 
  SetComDir(COMD_Stop);
  Message(szMessage,this()); 
  return(1); 
  } 
 
private func OrderSuccess(szMessage, p1, p2, p3) 
  { 
  Sound("KnightDone"); 
  SetOrderNone(); 
  SetCommand(this(),"None"); 
  SetComDir(COMD_Stop);
  Message(szMessage,this(), p1, p2, p3); 
  return(1); 
  } 
 
public func SetOrderAttack(pTarget) 
  { 
  SetOrder( OrderAttack(), pTarget ); 
  Sound("KnightConfirm*"); 
  return(1); 
  } 
 
public func SetOrderDefend(iX, iY) 
  { 
  SetOrder( OrderDefend() ); 
  ordrData1=iX; ordrData2=iY; 
  Sound("KnightConfirm*"); 
  return(1); 
  } 
 
public func SetOrderGuard(iX, iY) 
  { 
  SetOrder( OrderGuard() ); 
  ordrData1=iX; ordrData2=iY; 
  Sound("KnightConfirm*"); 
  return(1); 
  } 
 
public func SetOrderArm() 
  { 
  SetOrder( OrderArm() ); 
  ordrData1=ordrData2=nil;
  Sound("KnightConfirm*"); 
  return(1); 
  } 
 
public func SetOrderThrowSpear(pSpear, iX, iY) 
  { 
  SetOrder( OrderThrowSpear(), pSpear ); 
  ordrData1=iX; ordrData2=iY; 
  return(1); 
  } 
 
 
/*------------------------------------------- Steuerung ------------------------------------------------*/   
 
/* An Inhalt weitergeben, sonst internen Befehl ausführen. */ 
/* Direkte Steuerung löscht Befehle. */ 
 
/* Steuerung Werfen */ 
protected func ControlThrow(object pByObject)
{ 
  SetOrderNone();
  // Bei vorherigem Doppel-Stop nur Ablegen   
  if (GetPlrDownDouble(GetOwner())) return(nil);
  // Steuerung an Inhalt/Fahrzeug weitergeben 
  if (Control2Contents("ControlThrow")) return(1); 
  if (Control2Vehicle("ControlThrow")) return(1); 
  // Sonst internen Throw-Befehl ausführen 
  return(_inherited(pByObject)); 
} 
 
protected func ControlUp(object pByObject)
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlUp") ) return(1); 
  if ( Control2Contents("ControlUp") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlUpDouble(object pByObject)
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlUpDouble") ) return(1); 
  if ( Control2Contents("ControlUpDouble") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlDown(object pByObject)
{ 
  SetOrderNone(); 
  if(IsFighting() ) 
  { 
    StopFighting(); 
    return(1); 
  }
  if ( Control2Vehicle("ControlDown") ) return(1); 
  if ( Control2Contents("ControlDown") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlDownSingle(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlDownSingle") ) return(1); 
  if ( Control2Contents("ControlDownSingle") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlDownDouble(object pByObject)
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlDownDouble") ) return(1); 
  if ( Control2Contents("ControlDownDouble") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlLeft(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlLeft") ) return(1); 
  if ( Control2Contents("ControlLeft") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlRight(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlRight") ) return(1); 
  if ( Control2Contents("ControlRight") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlLeftDouble(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlLeftDouble") ) return(1); 
  if ( Control2Contents("ControlLeftDouble") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlRightDouble(object pByObject)
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlRightDouble") ) return(1); 
  if ( Control2Contents("ControlRightDouble") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlDigDouble(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlDigDouble") ) return(1); 
  if ( Control2Contents("ControlDigDouble") ) return(1); 
  return(_inherited(pByObject)); 
} 
protected func ControlDig(object pByObject) 
{ 
  SetOrderNone();
  if ( Control2Vehicle("ControlDig") ) return(1); 
  if ( Control2Contents("ControlDig") ) return(1); 
  return(_inherited(pByObject)); 
}

protected func ControlUpdate(object pByObject, int comdir, bool dig, bool throw)
{
  // Clear orders (if there is a comdir set)
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if (comdir) SetOrderNone();
  if( Control2Vehicle("ControlUpdate", comdir, dig, throw)) return(1);
  if( Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  return(_inherited(pByObject, comdir, dig, throw));
}
 
// Kontextmenü

public func IsBuckled()
{
  return(Or(Or(FindAttachedItems(), HasArmor()), HasShield()) );
}

public func ContextUnbuckle()
{
  [$CtxUnbuckleDesc$|Image=KMSU]//|Condition=IsBuckled]
  ShowUnbuckleMenu();
  return(1);
}

private func ShowUnbuckleMenu()
{
  var obj;
  CreateMenu(KMSU, nil, nil, nil, "$CtxUnbuckleDesc$", nil, 1);
  while(obj = FindAttachedItems(obj) )
    AddMenuItem(Format("$CtxUnbuckleItem$", GetName(nil, obj->UnbuckledID()) ), "Unbuckle", GetID(obj), nil, nil, obj );
  if(idShield) AddMenuItem(Format("$CtxUnbuckleItem$", GetName(nil, idShield)), "RemoveShield", idShield);
  if(HasArmor()) AddMenuItem(Format("$CtxUnbuckleItem$", GetName(nil, idArmorType)), "RemoveArmor",idArmorType);
}

private func Unbuckle(id dummy, object obj)
{
  obj->~Unbuckle();
}
 
public func ContextAttack() 
{                           
  [$CtxAttackDesc$|Image=MI_A] 
  // Nächstbeste feindliche Einheit angreifen 
  SetOrderAttack(FindEnemyUnit()); 
  return(1); 
} 
 
public func ContextDefend() 
{ 
  [$CtxDefendDesc$|Image=MI_D] 
  // Diese Position verteidigen 
  SetOrderDefend(GetX(),GetY()); 
  return(1); 
} 
 
public func ContextGuard() 
{ 
  [$CtxGuardDesc$|Image=MI_A] 
  // Diese Position bewachen 
  SetOrderGuard(GetX(),GetY()); 
  return(1); 
} 
 
public func ContextArm() 
{  
  [$CtxArmDesc$|Image=KMIA|Condition=IsNotFullyArmed]  
  // Waffen suchen und anlegen 
  SetOrderArm(); 
  return(1); 
} 

// Damit Aufrufe keinen Fehler verursachen, wenn bestimmte Pakete nicht aktiviert sind
global func MMStartPraying(par1,par2,par3){return(_inherited(par1,par2,par3));}
global func MMStopPraying(par1,par2,par3){return(_inherited(par1,par2,par3));}
global func MMGetChurch(par1,par2,par3){return(_inherited(par1,par2,par3));}

private func PrayingAllowed()
{
 // Zum ueberpruefen, ob entsprechendes vorhanden ist
 if(Or(!GetName(nil,CHCS), !GetName(nil,KRME)))return(nil);
 if(!GetActMapVal("Length","Pray",GetID()))return(nil);
 return(And(And(!ObjectCount(NPRY), !Contained()), MMGetChurch(GetOwner())));
}

public func ContextPray()
{
  [$CtxPrayDesc$|Image=CHCS|Condition=PrayingAllowed]
  if(!!SEqual(GetAction(), "Walk"))
  SetAction("Pray");
  else if(!!SEqual(GetAction(), "Pray"))
  SetAction("KneelUp");
  return(1);
}

protected func StartPraying()
{
  // Wenn das Objektpaket nicht geladen ist, einfach nicht einmischen
  if(Or(!GetName(nil,CHCS), !GetName(nil,KRME)))return(nil);
  
  var iAmount = 5;
  if(FindObject2(Find_ID(SNHP), Find_AtPoint())) iAmount = 10;
  else if(FindObject2(Find_ID(ALTR), Find_AtPoint())) iAmount = 7;
  if(!MMStartPraying(iAmount)) SetAction("Walk");
}

protected func StopPraying()
{
  // Wenn das Objektpaket nicht geladen ist, einfach nicht einmischen
  if(Or(!GetName(nil,CHCS), !GetName(nil,KRME)))return(nil);
  
  if(!!SEqual(GetAction(), "Pray")) return;
  MMStopPraying();
}
 
/*------------------------------------------ Kommandosteuerung ------------------------------------------------*/   
 
protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{ 
  // Context -> Keine Sonderbehandlung 
  if(!!SEqual(szCommand, "Context")) return(nil);
  // Bisherige Befehle löschen  
  SetOrderNone();  
  // Kommando MoveTo an Pferd weiterleiten. 
  if (IsRiding()) 
    { 
    if (!!SEqual(szCommand, "MoveTo"))
      return(ObjectCall(GetActionTarget(), "ControlCommand", szCommand, pTarget, iTx, iTy)); 
    // Anderes Kommando beim Reiten: absteigen 
    SetComDir(COMD_Stop, GetActionTarget());
    ObjectCall(GetActionTarget(), "ControlDownDouble", this()); 
    } 
  // Kommando MoveTo in Bogenschießen umwandeln. 
  if (!!SEqual(szCommand, "MoveTo"))
    { 
    if (IsAiming()) 
      { FireBowAt(iTx, iTy, true); return(1); } 
    // Der Spieler war etwas schnell... 
    if (IsLoading()) 
      return(1); 
    } 
  // Kommando Attack abfangen und in eigenen OrderAttack umwandeln. 
  if (!!SEqual(szCommand, "Attack"))
    return(SetOrderAttack(pTarget)); 
  // Kommando Throw mit Speer in OrderThrowSpear umwandeln. 
  if (!!SEqual(szCommand, "Throw"))
    if (Or(Equal(GetID(pTarget), SPER), ObjectCall(pTarget, "IsSpear")))
      return(SetOrderThrowSpear(pTarget, iTx, iTy)); 
  // Sonst nicht abfangen 
  return(_inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data) ); 
} 
 
/* Spezialtaste: Inventarwechsel */ 
protected func ControlSpecial() 
{ 
  [$CtrlInventoryDesc$|Image=CXIV]
  // Zielaktion abbrechen 
  StopAiming();  
  // Inventory verschieben 
  return (ShiftContents(nil,nil,nil,1));
} 
 
// Inventarauswahl 
protected func ControlContents() 
{ 
  // Zielaktion abbrechen 
  StopAiming();  
  // Standardaktivität 
  return(nil);
} 
 
/* Leitet die Steuerung an das erste Inhaltsobjekt weiter.  
   Rückgabewert 1 wenn erfolgreich */ 
private func Control2Contents(szControl)
{ 
  // Getragenes Objekt hat spezielle Steuerungsauswertung 
  // aber keine hazardwaffen
  if (Contents(nil))
  if (Contents(nil)->~IsWeapon()) return(nil);
  if (ObjectCall(Contents(),szControl, this(), ...) ) 
    return(1); 
  // Keine Inhaltssteuerung 
  return(nil);
} 
     
/* Leitet die Steuerung an ein ggf. gesteuertes Fahrzeug weiter. 
   Rückgabewert 1 wenn erfolgreich */ 
 
private func Control2Vehicle(szControl) 
{ 
  // Reiten und Werfen - an dieser Stelle doppelt gemoppelt, weil schon im normalen Clonk-Script.
  /*if (IsRiding()) 
    if (szControl eq "ControlThrow") 
      if (Contents()) 
      { 
        SetAction("RideThrow"); // Bemerke: ActionTarget wird beibehalten 
        return(); 
      }*/
  // Reiten: Steuerung an Pferd weiterleiten 
  if (IsRiding()) 
    if (ObjectCall(GetActionTarget(),szControl, this(), ...)) 
      return(1); 
  // Reiten und Links/Rechts: Steuerung der Lanze 
  var pLance; 
  if (IsRiding()) 
    if (Or(!!SEqual(szControl, "ControlLeft"), !!SEqual(szControl, "ControlRight")))
      if (pLance = HasLance())    
        if (pLance->LNCA::ControlTargetHeight(this())) 
          return(1);       
  // Keine Fahrzeugsteuerung 
  return(nil);
} 
 
/*-------------------------------------------- Pfeile / Pakete -----------------------------------------*/ 
 
/* Zielaktion abbrechen */ 
public func StopAiming() 
{ 
  // Zielt gar nicht: nix machen 
  if (!IsAiming()) return(nil);
  // Stop 
  SetComDir(COMD_Stop);
  // Basisaktion setzen 
  if (IsRiding()) 
    SetAction("Ride"); 
  else 
    SetAction("Walk"); 
  // Sound 
  Sound("Grab"); 
  return(1); 
} 
 
/* Überprüft ob der übergebene Typ ein Pfeil ist  */ 
private func CheckArrow(idDef) { return(idDef->~IsArrow()); } 
 
/* Überprüft ob der übergebene Typ ein Pfeilpaket ist */ 
private func CheckArrowPack(idDef) { return(idDef->~IsArrowPack()); } 
 
private func MaxContentsCount() { if (Or(HasArmor(), IsPuttingOnArmor())) return(2); else return(3); }
 
 
/* Aufnahme */
protected func Collection2(pObj) 
  { 
  // Übergeordnete Aufnahme 
  _inherited(pObj); 
  // Nicht mehr enthalten (oder gelöscht): Abbrechen 
  if (!Equal(Contained(pObj), this())) return(1);
  // Bogen nicht mehr am ersten Inventarplatz?
  if (And(Contents(), IsAiming()))
    if (And(!Contents()->~IsAiming(), !Equal(GetID(pCont), BOW1)))
      {
      // Erstmal versuchen, den Bogen wieder anzuwählen...
      var i,pCont;
      while (pCont=Contents(S42MigrationStep(i, 1))) if (Or(pCont->~IsAiming(), Equal(GetID(pCont), BOW1))) break;
      if (pCont)
        ShiftContents(nil, nil, GetID(pCont));
      else
        // Neuanwahl erfolglos (warum auch immer): Zielen abbrechen
        StopAiming();
      }
  return(1); 
  }
  
public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Der Ritter kann zusätzlich noch 30 Pfeile tragen
  if(Equal(iIndex, nil)) { if(fAmount) return(30); return("IsArrow"); }
}
 
protected func GetObject2Drop(pCollectObj) 
  { 
  var iArrowPriority; 
  // Wenn ein Pfeil oder Pfeilpaket aufgenommen werden soll... 
  if (pCollectObj) if (Or(ObjectCall(pCollectObj, "IsArrow"), ObjectCall(pCollectObj, "IsArrowPack")))
    // Pfeilen und Pfeilpaketen eine niedrigere Priorität geben 
    iArrowPriority=-500; 
  // Das schlechteste Objekt wegwerfen 
  var pBestDropObj, iBestDropPrio=1000; 
  var pObj, iPrio; 
  var idCollectObj = GetID(pCollectObj); 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if (((iPrio = GetContentsPriority(pObj, iArrowPriority, idCollectObj)) ?? 0) < (iBestDropPrio ?? 0))
      { 
      iBestDropPrio = iPrio; 
      pBestDropObj = pObj; 
      } 
  // Dieses Objekt zurückgeben 
  return(pBestDropObj); 
  } 
 
private func GetContentsPriority(pContents, iArrowPriority, idCollectObj) 
  { 
  // Pfeil? Normalerweise hohe Priorität, wenn nicht gerade ein anderes Pfeilpaket eingesammelt werden soll 
  if (pContents->~IsArrow()) return(302+(iArrowPriority ?? 0));
  // Pfeilpakete sind etwas geringer brauchbar, damit sie eher abgelegt werden (geht schneller...) 
  if (pContents->~IsArrowPack()) return(301+(iArrowPriority ?? 0));
  // Objekt ist eine Handwaffe? 
  var idCnt = GetID(pContents); 
  if (pContents->~IsFightWeapon()) 
    { 
    // Es soll eine Handwaffe eingesammelt werden 
    if (DefinitionCall(idCollectObj, "IsFightWeapon")) 
      // Hat schon eine Handwaffe 
      if (HasFightWeapon()) 
        // Sehr niedrige Priorität 
        return(1); 
    // Ansonsten relativ hohe Priorität 
    return(220); 
    } 
  // Bogen? 
  if (Equal(idCnt, BOW1))
    { 
    // Noch ein Bogen vorhanden? Muss nicht sein 
    if (Or(Equal(idCollectObj, BOW1), (ContentsCount(BOW1) ?? 0) > 1)) return(1);
    // Ansonsten fast so wichtig wie die Handwaffe (für die KI...) 
    return(216); 
    } 
  // Speer? 
  if (Equal(idCnt, SPER)) return(212);
  // Der Rest kann kaum gebraucht werden; ablegen nach Wert 
  return(GetValue(pContents)); 
  } 
 
 
/*---------------------------------------------- Aktionen ------------------------------------------------*/ 
  
private func Throwing(pObj) 
{            
  // Ggf. zu werfendes Objekt suchen 
  if (!pObj) 
  { 
    pObj = Contents(nil);
    if (!Contents()) return(nil);
  } 
  // Wurfparameter berechnen 
  var iX, iY, iR, iXDir, iYDir, iRDir; 
  if(And(!Equal(GetID(pObj), SPER), !pObj->~IsSpear()))
  { 
    // Standardwurf   
    iX = nil; if (!GetDir()) iX = -(iX ?? 0);
    iY = -10; 
    iR = Random(360); 
    iXDir = (GetPhysical("Throw") ?? 0) / 25000; if(!GetDir()) iXDir = -(iXDir ?? 0);
    iYDir = -(GetPhysical("Throw") ?? 0) / 25000;
    iRDir = (Random(40) ?? 0) - 20;
  } 
  else 
  { 
    // Speerwurf 
    iX = 10; if (!GetDir()) iX = -(iX ?? 0);
    iY = -6; 
    iR = 150; if(!GetDir()) iR = 180 - (iR ?? 0);
    iXDir = 5; if(!GetDir()) iXDir = -(iXDir ?? 0);
    iYDir = -2; 
    iRDir = 4; if(!GetDir()) iRDir = -(iRDir ?? 0);
  } 
  // Reitet? Eigengeschwindigkeit addieren 
  if (GetActionTarget()) if (IsRiding()) 
  {
    S42MigrationAssign(iXDir, "+=", (GetXDir(GetActionTarget()) ?? 0) / 10);
    // Steigungen beim Gehen werden leider nicht in die Y-Dir eingerechnet. Bei Steigungen
    // kann es daher vorkommen, dass der Wurf den Reiter vom Pferd haut. Was dagegen tun..?
    S42MigrationAssign(iYDir, "+=", (GetYDir(GetActionTarget()) ?? 0) / 10);
  } 
  // Werfen! 
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir); 
  return(1);   
} 
 
/* Speer werfen */ 
private func ExitSpear() 
{ 
  return(Throwing(FindSpearContents())); 
} 
 
/* Speer im Inventar finden */ 
private func FindSpearContents() 
  { 
  var pObj,i; 
  // SPER suchen 
  if (pObj = FindContents(SPER)) return (pObj); 
  // Objekte mit IsSpear() suchen 
  while (pObj=Contents(S42MigrationStep(i, 1, true))) if (pObj->~IsSpear()) return (pObj);
  // Nichts gefunden 
  return;
  } 
 
// Prüft, ob der Clonk gerade kaempft. 
private func IsFighting() 
{ 
  if (WildcardMatch(GetAction(),"*Fight") ) return(true); 
  if (WildcardMatch(GetAction(),"*Strike") ) return(true); 
  if (WildcardMatch(GetAction(),"*Punch*") ) return(true); 
  if (WildcardMatch(GetAction(),"*Thrust") ) return(true); 
  return(nil);
} 
 
// Erst 25 Frames nach dem ControlDown-Kommando 
private func StopFighting() 
{ 
  // ComDir auf NONE setzen, damit der Clonk nach dem Kampfabbruch nicht weiterlaeuft. 
  SetComDir(COMD_None );
  // StopFighting-Effekt anfuegen, der den Kampf nach 25 Frames abbricht, wenn nicht schon vorhanden. 
  if(!GetEffect("IntKNIGStopFighting",this(),nil,1) ) AddEffect("IntKNIGStopFighting", this(), 1, 25, this() );
} 
 
protected func FxIntKNIGStopFightingStop() 
{ 
  // Kaempfen wir noch? 
  if(IsFighting() ) 
  { 
    // Letzten Gegner fuer weitere 25 Frames speichern 
    AddEffect("IntKNIGLastEnemy", this(), 1, 25, this(), nil, GetActionTarget() );
    // Kampf abbrechen 
    SetAction("Walk"); 
  } 
} 
 
protected func FxIntKNIGLastEnemyStart(pTarget, iEffect, pEnemy) 
{ 
  EffectVar(nil, pTarget, iEffect) = pEnemy;
} 
 
protected func RejectFight(object pEnemy) 
{ 
  // Ist ein letzter Gegner gespeichert? 
  if(GetEffect("IntKNIGLastEnemy", this(), nil, 1) )
    // Ist der letzte Gegner der, mit dem wir hier kaempfen wuerden? 
    if(Equal(pEnemy, EffectVar(nil, this(), GetEffect("IntKNIGLastEnemy", this()) )) )
      // Ja, also abbrechen 
      return(1); 
  // Kampf erlauben 
  return(nil);
} 
 
/* Während des Kämpfens */ 
private func Fighting(pEnemy) 
{ 
  // Lanze verlieren 
  var pLance; 
  if (pLance = HasLance())  
    pLance->LNCA::Unbuckle();   
  // Kampf mit Handwaffe 
  var pWeapon; 
  if (pWeapon = GetFightWeapon()) 
  { 
    // Erstmal Kampfaktion setzen 
    if (!SEqual(GetAction(), pWeapon->~GetFightAction()))
      return (SetAction(pWeapon->~GetFightAction(), pEnemy)); 
    // Je nach Waffengeschwindigkeit Schlag durchführen 
    if (!Random(pWeapon->~GetStrikeTime())) 
      return (SetAction(pWeapon->~GetStrikeAction())); 
    // Weiterkämpfen 
    return(1); 
  } 
  // Keine Waffe: Gegner schlagen 
  if (!Random(4))  
    SetAction("Punch"); 
  // Weiterkämpfen 
  return(1); 
} 

protected func Death(int iKiller, a,b,c,d,e)
{
  // Angelegtes Zeugs ablegen
  iKiller ??= 0;
  var obj;
  while(obj = FindAttachedItems(obj) ) ProtectedCall(obj, "AttachTargetLost");
  // Schild ablegen
  RemoveShield();
  // Rüstung ablegen
  ArmorRemoved(); while (Contents()) Contents()->Exit();
  // Basisfunktion aufrufen
  return(_inherited(iKiller, a,b,c,d,e) );
}
 
/* Schlag mit Handwaffe */ 
private func Striking() 
{ 
  // Waffe bestimmen 
  var pWeapon; 
  if (!(pWeapon = GetFightWeapon())) return(nil);
  // Sound 
  pWeapon->~StrikeSound(); 
  // Kein Schaden 
  if (!Random(2)) return(1); 
  // Schaden ausrechnen 
  var pEnemy = GetActionTarget(); 
  var iDamage = pWeapon->~GetStrikeDamage(); 
  iDamage = (iDamage ?? 0) * (100 - (pEnemy->~IsShielded() ?? 0)) / 100;
  // Schlagen! 
  Punch(pEnemy, iDamage); 
  return(1); 
} 
 
protected func QueryCatchBlow(pObj) 
{ 
  // Schutzfaktor: lässt Objekte zurückprallen 
  var iPower = (BoundBy(GetMass(pObj), nil, 50) ?? 0) * (Distance(GetXDir(pObj), GetYDir(pObj)) ?? 0) * (Distance(GetXDir(pObj), GetYDir(pObj)) ?? 0);   if((Random(50000) ?? 0) + (iPower ?? 0) / 3 < (IsShielded() ?? 0) * 1000)
  {           
    // Projektil abprallen lassen 
    if((GetXDir(pObj) ?? 0) < 0) if((GetX(pObj) ?? 0) < (GetX() ?? 0) + 6) SetPosition((GetX() ?? 0) + 9, (GetY(pObj) ?? 0) - (GetYDir(pObj) ?? 0) / 3, pObj);
    if((GetXDir(pObj) ?? 0) > 0) if((GetX(pObj) ?? 0) > (GetX() ?? 0) - 6) SetPosition((GetX() ?? 0) - 9, (GetY(pObj) ?? 0) - (GetYDir(pObj) ?? 0) / 3, pObj);
    SetXDir(BoundBy(-(GetXDir(pObj) ?? 0) / 3, -10, 10), pObj); SetYDir(nil, pObj);
    // Sounds dazu 
    if(And((GetMass(pObj) ?? 0) >= 10, HasArmor())) Sound("ClonkHit*");
    if(And((GetMass(pObj) ?? 0) < 10, HasArmor())) Sound("ArrowRicochet*");
    ProtectedCall(pObj,"Hit");       
    return(1); 
  }            
  return(_inherited(pObj,...)); 
} 
 
protected func CatchBlow(iLevel, pObj) 
{ 
  // Zusätzlicher Schutzfaktor: Weniger Energieverlusst 
  DoEnergy(-(iLevel ?? 0) * (IsShielded() ?? 0) / 100);
  return(_inherited((iLevel ?? 0) * (100 - (IsShielded() ?? 0)) / 100, pObj));
} 
 
/* Fallschaden */ 
protected func Hit3() 
{ 
  if (HasArmor()) 
  { 
    DoEnergy(-10); 
    Sound("ClonkHit*");   
  }     
  return(_inherited(...)); 
} 
 
static const KNIG_ShieldLayer=10; // Der Layer der Schildgrafik

/* Schild anziehen */ 
public func WearShield(pObj, szGraphic)
{
  // Nur ein Schild gleichzeitig
  if(HasShield()) return;
  // Kein Objekt oder keine Grafik angegeben?
  if(Or(!pObj, !szGraphic)) return;
  // Grafik nicht vorhanden?
  if(!SetGraphics(szGraphic, this(), GetID(), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics)) return;
  // Angezogen? dann ID speichern und Objekt löschen
  idShield = GetID(pObj);
  RemoveObject(pObj);
  // Sound dazu!
  Sound("Connect");
  return(1);
}

/* Schild ausziehen */ 
public func RemoveShield() 
{ 
  // Kein Schild an? 
  if (!idShield) return(nil);
  // Sound
  Sound("Connect");
  // Grafikoverlay entfernen
  SetGraphics(nil, this(), nil, KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics);
  // Schildobjekt erzeugen und einstecken
  Collect(CreateObject(idShield,nil,10,GetOwner()),this());
  idShield=nil;
  return(1); 
}    

/* Rüstung anziehen */ 
public func WearArmor(pArmor) 
{ 
  // Kein Rüstungsobjekt übergeben? 
  if (!pArmor) {return(nil); }
  // Schon eine Rüstung an? 
  if (fArmored) {return(nil); }
  // Nicht am Boden?
  if (!SEqual(GetAction(), "Walk")) {return(nil);}
  // Neue Grafik setzen 
  if (!SetGraphics(pArmor->~GetGraphics())) 
    // Grafik nicht vorhanden: Sicher ein abgeleitetes Objekt, das keine Rüstung tragen kann 
    {return(nil); }
  // Mit dem Anziehen beginnen 
  // SetAction("PutOnArmor");    // animation des anziehens weglassen, weil AI sonst abbricht
  SetComDir(COMD_None);
  SetXDir(nil);
  // Rüstungstyp speichern und Rüstungsobjekt verbrauchen 
  idArmorType = GetID(pArmor); 
  RemoveObject(pArmor); 
  // Sound 
  Sound("Connect"); 
  return(1); 
} 
 
/* Rüstung ausziehen */ 
public func RemoveArmor() 
{ 
  // Keine Rüstung an? 
  if (!fArmored) return(nil);
  // Nur im laufen, sonst wird evtl. ein Sprung abgebrochen
  if (!SEqual(GetAction(), "Walk")) return(nil);
  // Sound 
  Sound("Connect"); 
  // Mit dem Ausziehen beginnen 
  SetAction("PutOffArmor");  
  SetComDir(COMD_None);
  SetXDir(nil);
  return(1); 
}              
 
/* Callback: Rüstung fertig ausgezogen bzw. Anziehen abgebrochen */ 
protected func ArmorRemoved() 
{ 
  // Rüstungsobjekt neu erstellen  
  CreateContents(idArmorType);   
  // Ausziehen 
  SetArmored(nil);
}          
 
/* Callback: Rüstung fertig angezogen */ 
protected func ArmorPutOn() 
{ 
  // Angezogen 
  SetArmored(1, idArmorType); 
} 
 
/* Rüstungsstatus setzen */ 
public func SetArmored(fnArmored, idnArmorType) 
{ 
  // Rüstung angelegt 
  if (fArmored = fnArmored) 
  {                   
    // Kein Rüstungstyp angegeben! 
    if (!idnArmorType) { fArmored = nil; return(nil); }
    // Rüstungstyp speichern 
    idArmorType = idnArmorType;     
    // Grafik setzen 
    var strArmorType = DefinitionCall(idArmorType, "GetGraphics"); 
    SetGraphics(strArmorType); 
    // Beschränkte Physicals ...könnte man noch von der jeweiligen Rüstung abhängig machen 
    SetPhysical("Jump", (GetPhysical("Jump") ?? 0) * 3 / 4, 2); // Sprungkraft temporär vermindern
    SetPhysical("Swim", (GetPhysical("Swim") ?? 0) / 2, 2); // Schwimmen klappt auch nicht mehr so richtig
    SetPhysical("CanScale", nil, 2);
    SetPhysical("CanHangle", nil, 2);
    // Ruestungsportrait 
    var iPortraitCount = 3; 
    if (!!SEqual(strArmorType, "BlackKnight")) iPortraitCount = 1; // Ein GetPortraitCount("Name") wäre nett...
    SetPortrait(Format("%s%d", strArmorType, 1 + (Random(iPortraitCount) ?? 0)), this(), GetID(), nil, nil);
  } 
  else 
  { 
    // Rüstungstyp löschen 
    idArmorType = nil;
    // Normale Grafik 
    SetGraphics("");   
    // Normale Physicals 
    ResetPhysical(); 
    // Normales Portrait 
    SetPortrait(GetPortrait(this(), nil, 1), this(), GetPortrait(this(), 1, 1), nil, nil);
  } 
  // Ok 
  return(1); 
}    
 
protected func CalcValue() 
{ 
  // Standardwert 
  var iValue = _inherited(); 
  // Wert der Rüstung hinzurechnen 
  if (fArmored) 
    S42MigrationAssign(iValue, "+=", (GetDefValue(idArmorType) ?? 0));
  // Wert zurückliefern 
  return(iValue);   
} 
 
private func Scaling() 
  { 
  var szDesiredAction; 
  if ((GetYDir() ?? 0)>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (!SEqual(GetAction(), szDesiredAction)) SetAction(szDesiredAction);
  return(1);    
  } 
 
protected func CheckStuck() 
{                    
  // Verhindert Festhängen am Mittelvertex 
  if(!GetXDir()) if((Abs(GetYDir()) ?? 0) < 5)
    if(GBackSolid(nil, 3))
      SetPosition(GetX(), (GetY() ?? 0) + 1);
} 
 
     
/* Suchfunktionen ======================================================== */ 
 
private func FindEnemyUnit() 
{ 
  var pObj; 
  // Nächstgelegene Mannschaftsobjekte überprüfen 
  while (pObj = FindObject(nil, nil,nil,-1,-1, OCF_CrewMember, nil, nil, nil, pObj))
    // Verfeindet? 
    if (Hostile(GetOwner(),GetOwner(pObj))) 
      // Feind gefunden 
      return(pObj); 
  // Keine feindliche Einheit gefunden 
  return(nil);
} 
 
private func FindClosestBase() 
{ 
  // Nächstgelegene Heimatbasis suchen 
  var i, pObj, pClosestBase = nil;
  for(var i = nil; pObj = FindBase(GetOwner(), i); S42MigrationStep(i, 1, true))
    if (Or(!pClosestBase, (ObjectDistance(pObj) ?? 0) < (ObjectDistance(pClosestBase) ?? 0)))
      pClosestBase = pObj; 
  return(pClosestBase); 
}

protected func FindAttachedItems(object after)
{
  var obj = after;
  while(obj = FindObject(nil, nil, nil, nil, nil, nil, nil, this(), nil, obj) )
    if(!!SEqual(GetProcedure(obj), "ATTACH"))
      // ein Ritterding
      if(obj->~UnbuckledID())
        return(obj);
  return(nil);
}
 
/* Bogenschießen ========================================================== */ 
 
private func AimBowToTarget(iXOffset, iYOffset) 
{ 
  // Richtung 
  if ((iXOffset ?? 0) > 0) SetDir(DIR_Right);
  if ((iXOffset ?? 0) < 0) SetDir(DIR_Left);
  // Zielrichtung 
  var iAngle = Angle(nil,nil,Abs(iXOffset),iYOffset);
  SetPhase(BoundBy( (iAngle ?? 0) / 23, nil, 5));
  // Höher zielen für größere Distanz 
  if ((GetPhase() ?? 0) > 1)
    SetPhase(BoundBy((GetPhase() ?? 0) - (Abs(iXOffset) ?? 0) / 150, 1, 5));
  // Fertig 
  return(1); 
} 
 
private func AimBowToAngle(iAngle) 
  { 
  // Winkel anpassen 
  while ((iAngle ?? 0) > 180) S42MigrationAssign(iAngle, "-=", 360);
  // Richtung 
  if ((iAngle ?? 0) > 0 ) SetDir(DIR_Right); else SetDir(DIR_Left);
  // Zielrichtung 
  SetPhase(BoundBy( ((Abs(iAngle) ?? 0)+8)/16, nil,7));
  // Fertig 
  return(1); 
  } 

private func SimBow(int iAngle, int iTargetX)
{
  // Startwerte
  iAngle ??= 0;
  iTargetX ??= 0;
  var iX, iY, iR, iXDir, iYDir, iRDir;
  GetBowOut(nil, iAngle, nil, iX, iY, iR, iXDir, iYDir, iRDir);
  // Koordinaten umrechnen
  S42MigrationAssign(iX, "+=", (GetX() ?? 0)); S42MigrationAssign(iY, "+=", (GetY() ?? 0));
  // Zeit, die das Geschoss fliegen muss
  var iTime = ((iTargetX ?? 0) - (iX ?? 0)) * 10 / (iXDir ?? 0);
  // Simulieren
  return(SimFlight(iX, iY, iXDir, iYDir, 1, 100, iTime));
}

private func FireBowAt(int iX, int iY, bool fForce) 
{ 
  iX ??= 0;
  iY ??= 0;
  fForce ??= false;
  var v = 10, // 10 px / tick
      f = 200, // Fixpunktfaktor
      g = -(GetGravity() ?? 0) * (f ?? 0) * (f ?? 0) / 1000 / (v ?? 0), // 0,2 px / tick²
      x = ((iX ?? 0) - (GetX() ?? 0)) * (f ?? 0) / (v ?? 0),
      y = -((iY ?? 0) - (GetY() ?? 0)) * (f ?? 0) / (v ?? 0) /* Korrektur: */ - (Abs(x) ?? 0) * (GetGravity() ?? 0) / 2000,
      d = (y ?? 0) * (y ?? 0) + (x ?? 0) * (x ?? 0),
      k = (y ?? 0) + (f ?? 0) * (f ?? 0) * (f ?? 0) / 2 / (g ?? 0),
      w = (k ?? 0) * (k ?? 0) - (d ?? 0);
  // Außer Reichweite?
  if(And((w ?? 0) < 0, !fForce)) return;
  // Flugzeiten und Winkel berechnen
  var t1 = Sqrt( ((k ?? 0) + (Sqrt(w) ?? 0)) * ((f ?? 0) * (f ?? 0) * (f ?? 0) / (g ?? 0)) ),
      t2 = Sqrt( ((k ?? 0) - (Sqrt(w) ?? 0)) * ((f ?? 0) * (f ?? 0) * (f ?? 0) / (g ?? 0)) ),
      phi1 = ArcCos(x, t1),
      phi2 = ArcCos(x, t2);
  if((y ?? 0) < (g ?? 0) * (t1 ?? 0) / (f ?? 0) * (t1 ?? 0) / (f ?? 0) / (f ?? 0)) phi1 = -(phi1 ?? 0);
  if((y ?? 0) < (g ?? 0) * (t2 ?? 0) / (f ?? 0) * (t2 ?? 0) / (f ?? 0) / (f ?? 0)) phi2 = -(phi2 ?? 0);
    // Winkel umrechnen
    phi1 = (270 - (phi1 ?? 0)) % 360 - 180; phi2 = 90 - (phi2 ?? 0);
  // Erste Lösung außerhalb der Grenzen?
    if(And(!Inside(phi1, -120, 120), !fForce)) return;
    phi1=BoundBy(phi1, -120, 120);
    // Zielwinkel probieren
  var iAngle;
  if(!SimBow(phi1, iX))
    iAngle = phi1;
  // Zweite Schusslösung?
  else if(And((t2 ?? 0) < (t1 ?? 0) * 3, !SimBow(phi2, iX)))
    iAngle = phi2;
  else if(fForce)
    iAngle = phi1;      
    else
    return;
  // Zielt gar nicht: erst mal laden 
  if (!IsAiming())
    {
    LoadBow(FindContents(BOW1), 1);  
    return;
    }
  // Zielen 
  AimBowToAngle(iAngle); 
  // Feuern 
  return(Contents()->~FireAngle(iAngle));
} 

/* Beginnt den Zielvorgang */  
public func AimBow() 
{ 
  // Gibt den Rückgabewert von SetAction zurück 
  if (IsRiding()) 
    return(SetAction("RideAimBow")); 
  else 
    return(SetAction("AimBow"));   
}  
  
/* Beginnt den Ladevorgang */  
public func LoadBow(pBow, fFirstTime) 
  { 
  // Nur mit Pfeil! 
  if (!GetArrowCount()) 
    { 
    // Kein Pfeil fürs erste Zielen: Meldung ausgeben 
    if (fFirstTime) 
      { 
      Sound("KnightFailure"); 
      PlayerMessage(GetController(), "$MsgNoArrow$", this()); // Kein Pfeil 
      } 
    return(1); 
    } 
  // Bogen lieber wieder auswählen (GetArrow wählt gerne ab...) 
  ShiftContents(nil,nil,BOW1);
  // Schon beim Nachladen? 
  if (IsLoading()) return(1); 
  // War gerade beim Zielen: letzten Winkel sichern 
  if (IsAiming())  
    iLastAimAngle = GetPhase();  
  else  
    iLastAimAngle = 4; 
  // Muss den Rückgabewert von SetAction zurückgeben, da das Setzen durch Überladungen fehlgeschlagen sein kann 
  if (IsRiding()) 
    return(SetAction("RideLoadBow")); 
  else 
    return(SetAction("LoadBow")); 
  } 
 
protected func BeginAiming() { return(SetPhase(iLastAimAngle)); } 

/* Winkel, in den der Ritter momentan zielt */
public func GetBowAimAngle()
  { 
  // Richtung ermitteln 
  var iOutDir; if (!GetDir()) iOutDir=-1; else iOutDir=1; 
  // Winkel (von oben) ermitteln
  return((iOutDir ?? 0)*(GetPhase() ?? 0)*115/7);
  }

/* Zielabweichung des Ritters. Überladbar. */
public func GetAimingDeviation(pBow)
  {
  return(5);
  }
  
/* Ausgangsgeschwindigkeit und -position, wenn der Ritter mit dem Bogen in angegebene Richtung schießt */
public func GetBowOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
  {
  // Zufallsabweichung
  if(fDeviate) 
    { 
    var iDeviation = GetAimingDeviation(pBow);
    S42MigrationAssign(iAngle, "+=", (Random((iDeviation ?? 0) * 2 + 1) ?? 0) - (iDeviation ?? 0));
    }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 70);
  iOutYDir = -(Cos(iAngle, 70) ?? 0);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * (GetCon() ?? 0) / 100);
  iOutY = -(Cos(iAngle, 25 * (GetCon() ?? 0) / 100) ?? 0);
  // Pfeildrehung 
  iOutR = iAngle; 
  if((iAngle ?? 0) > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return(1);
  } 

private func ChangeBowAimAngle(iChange) 
  { 
  var iOldAngle; 
  // Derzeitigen Winkel ermitteln 
  if (IsAiming()) iOldAngle = GetPhase(); 
  if (IsLoading()) iOldAngle = iLastAimAngle; 
  // Am Anschlag? 
  if (!Inside((iOldAngle ?? 0) + (iChange ?? 0), nil, 7)) return;
  // Neuen Winkel setzen 
  if (IsAiming()) SetPhase((iOldAngle ?? 0) + (iChange ?? 0));
  if (IsLoading()) S42MigrationAssign(iLastAimAngle, "+=", (iChange ?? 0));
  } 
 
public func BowControlUp(pBow, &rVal) 
  { 
  ChangeBowAimAngle(-1); 
  // Funktion überladen 
  return(1); 
  } 
 
public func BowControlDown(pBow, &rVal) 
  { 
  ChangeBowAimAngle(+1); 
  // Funktion überladen 
  return(1); 
  } 


/* Aufwertung: Nur erlauben, wenn der Clonk noch kein Ritter (z.B.: König) ist */

public func IsRevaluationBlocked(object target) { return(target->~IsKnight()); }

public func OnRevaluation()
  {
  // Der Magier bekommt bei Aufwertung Mana - Der Ritter also Waffen
  var i = Random(3);
       if (!S42MigrationStep(i, -1, true)) { CreateContents(BOW1); CreateContents(ARWP); }
  else if (!S42MigrationStep(i, -1, true)) CreateContents(SWOR);
  else if (!S42MigrationStep(i, -1, true)) { CreateContents(AXE1); CreateContents(SHIE); }
  }
  
