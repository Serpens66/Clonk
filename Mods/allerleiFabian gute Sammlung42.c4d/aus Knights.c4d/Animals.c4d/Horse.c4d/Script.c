/*-- Pferd --*/

#strict 3

/* Locals */

local iGait; // Gangart: 0 Stehen, 1 Gehen, 2 Trab, 3 Galopp
local idSaddle; // Gesattelt (Satteltyp)
local iGraphics;

/* IDs */

private func ID_Unsaddled() { return(HORS); }
private func ID_Dead()      { return(DHRS); }

private func MaxGait()
{
  if(!!SEqual(GetProcedure(), "PULL")) return(2);
  return(3);
}

/* Initialisierung */

protected func Initialize()
{
  // zufälliges Aussehen (schlägt fehl, wenn keine Zusatzgrafik vorhanden)
  iGraphics = Random(3);
  SetGraphics(Format("%d", iGraphics), this(),GetID());

  SetAction("Walk");
  SetDir(DIR_Left);
  if (Random(2)) SetDir(DIR_Right);
  SetComDir(COMD_Stop);
  // Satteltyp initialisieren
  if (Equal(GetID(this()), HRSS)) idSaddle = SADL;
  if (Equal(GetID(this()), HRSK)) idSaddle = KSDL;
}

public func Redefine(idDef)
{
  ChangeDef(idDef);
  SetAction("Walk");
  // Grafik anpassen (schlägt fehl, wenn keine Zusatzgrafik vorhanden)
  if(this()->~IsHorse()) SetGraphics(Format("%d", iGraphics), this(),GetID());
  return(1);
}

/* TimerCall */

private func Activity()
{
  // Locals übertragen (für alte Savegames)
  if(And(!iGait, Local(nil))) { iGait = ((Local(nil) ?? 0)+1)%5; if(Equal(GetComDir(), COMD_None)) iGait = nil; Local(nil) = nil; }
  if(And(!idSaddle, Local(1))) { idSaddle = Local(1); Local(1) = nil; }
  // Angehalten: Stehen
  if (Equal(GetComDir(), COMD_Stop))
    if (!GetXDir())
      if (Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Trot")), !!SEqual(GetAction(), "Gallop")))
        SetAction("Stand");
  // Bewegungskommando: Aktion anpassen
  if (!!SEqual(GetCommand(), "MoveTo"))
    MoveToAdjustAction();
  // Ggf. Reiterposition anpassen
  if(!SEqual(GetAction(), "Jump"))
    ResetRiderPos();   
  // Pferd steht?
  if(!!SEqual(GetAction(), "Stand")) SetGait(iGait);
  // Anhalten, wenn ohne Reiter
  return(StopRunning());
}

private func MoveToAdjustAction()
{
  // Stehen
  var inGait = -1;
  var maxGait = MaxGait();

  if ((Abs(Distance(GetX(),GetY(),GetCommand(nil,2),GetCommand(nil,3))) ?? 0) < 10)
    inGait = Min(maxGait, nil);
  // Losgehen
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(nil,2),GetCommand(nil,3))), 10, 75))
    inGait = Min(maxGait, 1);
  // Traben
  if (Inside(Abs(Distance(GetX(),GetY(),GetCommand(nil,2),GetCommand(nil,3))), 75, 150))
    inGait = Min(maxGait, 2);
  // Galoppieren
  if ((Abs(Distance(GetX(),GetY(),GetCommand(nil,2),GetCommand(nil,3))) ?? 0) > 150)
    inGait = Min(maxGait, 3);
  if((inGait ?? 0) > (iGait ?? 0)) SetGait((iGait ?? 0) + 1);
  if((inGait ?? 0) < (iGait ?? 0)) SetGait((iGait ?? 0) - 1);
  return(nil);
}

private func StopRunning()
{
  // Richtige Aktion?
  if(And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Trot")), !SEqual(GetAction(), "Gallop"))) return;
  if(!iGait) return;
  // Kein Reiter?
  if(GetRider()) return;
  // Keiner der mich schiebt?
  if(GetPusher()) return;
  // diesmal einfach nicht langsamer werden
  if(Random(3)) return;
  // Geschwindigkeit stufenweise verringern
  SetGait((iGait ?? 0)-1);
}

/* Reiten */

protected func ActivateEntrance(pRider)
{
  // Nur ein Reiter zur Zeit
  if (GetRider()) return(nil);
  // Nur im Stehen, Gehen, Schwimmen, Ziehen
  if (And(And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Stand")), !SEqual(GetAction(), "Swim")), !SEqual(GetAction(), "Pull")))
    return(nil);
  // Reiter kann nur mit Sattel reiten
  if (pRider->~NeedSaddle())
    if (!IsSaddled())
    {
      Message("$MsgNeedSaddle$",pRider,GetName(pRider));
      return(nil);
    }
  // Aufsitzen
  if (!ObjectSetAction(pRider,"Ride",this()))
  {
    NoGoodRider(pRider);
    return(nil);
  }
  // Clonkposition anpassen
  SetPosition(GetX(), (GetY() ?? 0)-5, pRider);
  // Erfolg
  return(1);
}

/* Steuerung */

private func SetControl(int com, object controller)
{
  com ??= 0;
  if(GetEffect("IntControl", this()))
    RemoveEffect("IntControl", this());

  AddEffect("IntControl", this(), 1, 9, this(), nil, com, controller);
}

protected func FxIntControlStart(object target, int number, int temp, int com, object controller)
{
  number ??= 0;
  temp ??= 0;
  com ??= 0;
  if(temp) return;
  EffectVar(nil, target, number) = com;
  EffectVar(1, target, number) = controller;
}

protected func FxIntControlTimer(object target, int number, int time)
{
  // Keine Aenderung waehrend eines Sprungs
  number ??= 0;
  time ??= 0;
  if(Or(!!SEqual(GetAction(), "Jump"), !!SEqual(GetAction(), "Turn"))) return(nil);

  var com = EffectVar(nil, target, number);
  var controller = EffectVar(1, target, number);

  var tick = EffectVar(2, target, number);
  EffectVar(2, target, number) = ((tick ?? 0) + 1) % 4;

  // Ist kein Controller mehr
  // Controller kann auch auf dem Planwagen sitzen. Hm.
  if(!controller)// || controller != GetRider() || (controller->GetAction() ne "Push" && controller->GetActionTarget() != this()))
    return(-1);

  // Bei normaler Beschleunigung oder Abbremsen nur jeden zweiten Timer
  if(Or(Or(And(Equal(com, COMD_Right), Equal(GetComDir(), COMD_Right)), And(Equal(com, COMD_Left), Equal(GetComDir(), COMD_Left))), Equal(com, COMD_Stop)))
    if((tick ?? 0) & 1)
      return(nil);

  // Bei Beschleunigung in Gegenrichtung jeden
  if(Equal(com, COMD_Right)) ComRight(controller);
  if(Equal(com, COMD_Left)) ComLeft(controller);
  if(Equal(com, COMD_Stop)) ComStop(controller);

  // Keine weitere Beschleunigung/Abbremsung noetig
  if(!!SEqual(GetAction(), "Swim")) return(-1);
  // Ziel erreicht
  if(Or((And(!Equal(com, COMD_Stop), Equal(iGait, MaxGait()))), (And(Equal(com, COMD_Stop), Equal(iGait, nil)))))
    return(-1);
}

protected func ComLeft(object by)
{
  // Gehen/Stehen/Ziehen: Wenden
  if((iGait ?? 0) <= 1)
    if(Equal(GetDir(), DIR_Right))
      { TurnLeft(); return(true); }

  // Schwimmen: Richtung bestimmen
  if(!!SEqual(GetAction(), "Swim"))
    { SetComDir(COMD_Left); return(true); }

  // Beschleunigen
  if((iGait ?? 0) < (MaxGait() ?? 0))
    if(And(Equal(GetDir(), DIR_Left), (Or(Or(Equal(by, GetRider()), Equal(iGait, nil)), GetActionTarget()))))
      { SetGait(BoundBy((iGait ?? 0) + 1, nil, MaxGait())); return(true); }

  // Abbremsen
  if(Equal(GetDir(), DIR_Right))
    { SetGait(BoundBy((iGait ?? 0) - 1, nil, MaxGait())); return(true); }

  return(nil);
}

protected func ComRight(object by)
{
  // Gehen/Stehen/Ziehen: Wenden
  if ((iGait ?? 0) <= 1)
    if (Equal(GetDir(), DIR_Left))
      { TurnRight(); return(true); }

  // Schwimmen: Richtung bestimmen
  if (!!SEqual(GetAction(), "Swim"))
    { SetComDir(COMD_Right); return(true); }

  // Beschleunigen
  if((iGait ?? 0) < (MaxGait() ?? 0))
    if(And(Equal(GetDir(), DIR_Right), (Or(Or(Equal(by, GetRider()), Equal(iGait, nil)), GetActionTarget()))))
      { SetGait(BoundBy((iGait ?? 0) + 1, nil, MaxGait())); return(true); }

  // Abbremsen
  if(Equal(GetDir(), DIR_Left))
    { SetGait(BoundBy((iGait ?? 0) - 1, nil, MaxGait())); return(true); }

  return(nil);
}

protected func ComStop(object by)
{
  // Abbremsen
  if(iGait) { SetGait((iGait ?? 0) - 1); return(true); }
  // TODO: COMD_Stop im Wasser?
  return(nil);
}

public func ControlLeft(pByObject)
{ 
  [$CtrlLeadDesc$|Image=HOR1:0]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Aufzug bedienen
  var pElev = GetElev();
  if(And(pElev, !iGait)) if(!SEqual(GetAction(pElev), "Wait")) return(pElev->~ControlLeft(pByObject));
  return(ComLeft(pByObject));
}

public func ControlRight(pByObject)
{
  [$CtrlLeadDesc$|Image=HOR1:1]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Aufzug bedienen
  var pElev = GetElev();
  if(And(pElev, !iGait)) if(!SEqual(GetAction(pElev), "Wait")) return(pElev->~ControlRight(pByObject));
  return(ComRight(pByObject));
}

public func ControlUpdate(object pByObject, int comdir)
{
  // Weitere Beschleunigung bei JumpAndRun-Steuerung
  comdir ??= 0;
  SetControl(comdir, pByObject);
}

public func ControlLeftReleased(pByObject)
{
  ComStop();
}

public func ControlRightReleased(pByObject)
{
  ComStop();
}

public func ControlUp(pByObject)
{
  [$CtrlJumpDesc$]
  // Nur im Gallop
  if (!!SEqual(GetAction(), "Gallop")) { Jump(); return; }
  // Aufzug bedienen
  var pElev = GetElev();
  if(And(pElev, !iGait)) pElev->~ControlUp(pByObject);
  // Kein Sprung
  return(nil);
}

public func ControlDownSingle(pByObject)
{
  [$CtrlStopDesc$|Method=Classic]
  if(!GetPlrJumpAndRunControl(pByObject->GetController()))
  {
    // Direkte Steuerung löscht Kommandos
    ClearCommands(); 
    // Aufzug bedienen
    var pElev = GetElev();
    if(And(pElev, !iGait)) pElev->~ControlDownSingle(pByObject);
    // Abbremsen
    if(iGait) SetGait((iGait ?? 0) - 1);
  }
  // Keine Steuerung
  return(nil);
}

private func GetElev()
{
  var pElev = FindObject(ELEC, nil, 1);
  if(!Equal(GetX(pElev), GetX())) return(nil);
  else return(pElev);
}

public func ControlDownDouble(pByObject)
{
  [$CtrlGetOffDesc$]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Reiter: absitzen
  if (Equal(pByObject, GetRider()))
  {
    ObjectSetAction(pByObject,"Walk");
    SetActionTargets(nil,nil,pByObject);
    var y=GetY(pByObject),i=nil;
    //Stuck?
    while (S42MigrationStep(i, 1, true)<12)
      if (Stuck(pByObject))
        SetPosition(GetX(pByObject), (y ?? 0)+(i ?? 0), pByObject);
      else
        break;
    return(1);
  }
  // Keine Steuerung
  return(nil);
}

public func ControlDigDouble(pByObject)
{
  [$CtrlFastenDesc$|Image=HOR1:2]
  // Direkte Steuerung löscht Kommandos
  ClearCommands();
  // Angespannt: ausspannen
  if (GetActionTarget())
    { DisconnectWagon(); return(1); }
  // Anspannen/Satteln: nur im Stehen oder Gehen
  if ((iGait ?? 0) > 1)
    return(nil);
  // Wagen suchen und anspannen
  var pWagon;
  if (pWagon = FindHorseWagon())
    { ConnectWagon(pWagon); return(1); }
  // Sattel abnehmen
  if (IsSaddled())
    if (Unsaddle())
      return(1);
  // Satteln (Sattelobjekt im Aufrufer aktivieren)
  var pSaddle;
  if (pSaddle = FindSaddleIn(pByObject))
    if (pSaddle->Activate(pByObject))
      return(1);
  // Keine Aktion
  return(nil);
}

public func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  // Kommando MoveTo: gegebenenfalls wenden
  if (!!SEqual(szCommand, "MoveTo"))
    if ((iTx ?? 0) > (GetX() ?? 0))
      TurnRight();
  if (!!SEqual(szCommand, "MoveTo"))
    if ((iTx ?? 0) < (GetX() ?? 0))
      TurnLeft();
  // Kommando setzen
  return(SetCommand(this(), szCommand, pTarget, iTx, iTy));
}

/* Gangart: Actions */

private func SetGait(inGait)
{                                
  iGait = inGait;
  if(And(And(And(And(And(And(And(And(And(!SEqual(GetAction(), "Stand"), !SEqual(GetAction(), "Walk")), !SEqual(GetAction(), "Trot")), !SEqual(GetAction(), "Gallop")),
     !SEqual(GetAction(), "Pull")), !SEqual(GetAction(), "Pull2")), !SEqual(GetAction(), "Pull3")),
     !SEqual(GetAction(), "Scale")), !SEqual(GetAction(), "Scale2")), !SEqual(GetAction(), "Scale3")))
    return;
  var fScaling;
  if(WildcardMatch(GetAction(), "Scale*")) fScaling = 1;
  // Neue Gangart              
  var fPulling = GetActionTarget();
  if(Equal(inGait, nil)) { if(fPulling) SetAction("Pull"); else SetAction("Stand"); }
  if(Equal(inGait, 1)) { if(fPulling) SetAction("Pull"); else if(fScaling) SetAction("Scale"); else SetAction("Walk"); }
  if(Equal(inGait, 2)) { if(fPulling) SetAction("Pull2"); else if(fScaling) SetAction("Scale2"); else SetAction("Trot"); }
  if(Equal(inGait, 3)) { if(fPulling) SetAction("Pull3"); else if(fScaling) SetAction("Scale3"); else SetAction("Gallop"); }
  if(Equal(inGait, nil))
    SetComDir(COMD_Stop);
  else
    if(Equal(GetDir(), DIR_Left)) SetComDir(COMD_Left); else SetComDir(COMD_Right);
  if(fPulling) SetPhase(Random(10));
}

/* Anspannen und satteln */

private func DisconnectWagon()
{
  GetActionTarget()->~Disconnect(this());
  SetActionTargets(); SetGait(iGait);
  return(1);
}

private func ConnectWagon(pWagon)
{
  pWagon->Connect(this());
  SetActionTargets(pWagon); SetGait(iGait);
  return(1);
}

public func Saddle(idSaddledHorse, idnSaddle)
{
  // Bereits gesattelt
  if (IsSaddled()) return(nil);
  // Satteln: nur im Stehen, Gehen, Ziehen
  if (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Stand")), !SEqual(GetAction(), "Pull")))
    return(nil);
  // Nur ohne Reiter
  if (GetRider()) return(nil);
  // Satteltyp speichern
  idSaddle = idnSaddle;
  // Sound
  Sound("Connect");
  // Satteln
  Redefine(idSaddledHorse);
  return(1);
}

public func Unsaddle()
{
  // Nicht gesattelt
  if (!IsSaddled()) return(nil);
  // Nur ohne Reiter
  if (GetRider()) return(nil);
  // Erst Satteltasche leeren
  if (Contents()) { Sound("Error"); Message("$MsgEmptySaddle$",this()); return(nil); }
  // Sattel abnehmen
  CreateObject(idSaddle,nil,+8,GetOwner()); idSaddle=nil;
  Sound("Connect");
  Redefine(ID_Unsaddled());
  return(1);
}

private func TurnRight()
{
  // Wenden aus Gehen, Stehen, Ziehen
  if (Equal(GetDir(), DIR_Left))
    if (Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Stand")), !!SEqual(GetAction(), "Pull")))
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  // Wenden aus Gehen, Stehen, Ziehen
  if (Equal(GetDir(), DIR_Right))
    if (Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Stand")), !!SEqual(GetAction(), "Pull")))
      SetAction("Turn");
  // Richtung
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

private func ClearCommands()
{
  SetCommand(this(), "None");
  return(1);
}

/* Einwirkungen */

protected func Damage(iDamage)
{
  // Starker Treffer
  if ((iDamage ?? 0) > 10)
    SetAction("Rise");
  // Tot
  if ((GetDamage() ?? 0) > 100)
    Death();
  // Wiehern
  Sound("HorseNeigh*");
  return(1);
}
    
private func Collision()
{
  SetAction("Tumble");
  DoDamage(+5);
  return(1);
}

public func Death()
{
  // Inhalt auswerfen
  while (Contents()) Exit(Contents());
  if (GetRider()) DropRider();
  // Gegebenenfalls Sattel verlieren
  Unsaddle();
  // Verwandeln
  var graphics = iGraphics;
  ChangeDef(ID_Dead());
  SetGraphics(Format("%d", graphics), this(),GetID()); //  (schlägt fehl, wenn keine Zusatzgrafik vorhanden)

  SetAction("Dead");
  return(1);
}

public func DropRider()
{
  return(Fling(GetRider(), nil, 1));
}

private func LiquidDamage()
{
  // Material abfragen
  var iMaterial = GetMaterial();
  if (Equal(iMaterial, -1)) return(nil);
  // Säure/Lava (leider hardgecodet)
  if (Or(Or(Equal(iMaterial, Material("Acid")), Equal(iMaterial, Material("Lava"))), Equal(iMaterial, Material("DuroLava"))))
    DoDamage(+8);
  return(1);
} 

/* Kontakt */

protected func ContactLeft()
{
  if((GetX() ?? 0)<28) return(TurnRight());
  if(And(!GBackSolid(-3,nil), !GBackSolid(-8,nil)))
  {
    SetAction("Scale");
    SetPhysical("CanScale",1,2);
    SetComDir(COMD_Up);
    return(1);
  }
  SetPhysical("CanScale",nil,2);
  // Kollision
//  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnRight(); 
  return(1);
}
  
protected func ContactRight()
{
  if((GetX() ?? 0)>(LandscapeWidth() ?? 0)-28) return(TurnLeft());
  if(And(!GBackSolid(3,nil), !GBackSolid(8,nil)))
  {
    SetAction("Scale");
    SetPhysical("CanScale",1,2);
    SetComDir(COMD_Up);
    return(1);
  }
  SetPhysical("CanScale",nil,2);
  // Kollision
//  if (!Inside(GetXDir(),-30,+30)) return(Collision());
  // Wenden
  TurnLeft(); 
  return(1);
}

/* Aktionen */

private func Pulling()
{
  // Zielobjekt verloren
  if (!GetActionTarget())
    return(DisconnectWagon());
  // Wagen wenden
  if (Equal(GetDir(GetActionTarget()), DIR_Left))
    if ((GetX() ?? 0) > (GetX(GetActionTarget()) ?? 0))
      GetActionTarget()->TurnRight();
  if (Equal(GetDir(GetActionTarget()), DIR_Right))
    if ((GetX() ?? 0) < (GetX(GetActionTarget()) ?? 0))
      GetActionTarget()->TurnLeft();
  return(1);  
}

private func Pulling1()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",50000,2);
  Sound("HorseWalk*");
  return(1);  
}
  
private func Pulling2()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",70000,2);
  Sound("HorseTrot*");
  return(1); 
} 
  
private func Pulling3()
{
  Pulling();
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",130000,2);
  Sound("HorseGallop*");
  return(1);  
}
  
private func Walking()
{                            
  // Reiter-Position zurücksetzen
  ResetRiderPos();
  // Vorherige Gangart wieder aufnehmen (nach Sprung oder Wendung)
  if (Or(!Equal(iGait, 1), GetActionTarget()))
    { SetGait(iGait); return; }
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",50000,2);
  SetPhysical("Scale",1000000,2);
  Sound("HorseWalk*");
  return(1);
}
  
private func Turning()
{
  // Geräusch
  Sound("HorseWalk*");
  return(1);  
}
  
private func Trotting()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk", 90000, 2);
  SetPhysical("Scale", 1000000, 2);
  Sound("HorseTrot*");
  // Angehalten (z.B. durch Kollision)
  if (Equal(GetComDir(), COMD_Stop))
    { iGait = nil; SetAction("Stand"); }
  return(1);     
}
  
private func Gallopping()
{
  // Geschwindigkeit und Geräusch
  SetPhysical("Walk",160000,2);
  SetPhysical("Scale",1000000,2);
  Sound("HorseGallop*");
  // Angehalten (z.B. durch Kollision)
  if (Equal(GetComDir(), COMD_Stop)) { iGait = nil; SetAction("Stand"); }
  return(1);
}
  
private func Swimming()
{
  // Unter Wasser: auftauchen
  if (GBackSemiSolid(nil, -5))
    SetComDir(COMD_Up);
  // Gangart zurücksetzen
  iGait = 1;
  // Geräusch
  Sound("HorseSplash*");
  // Schaden durch gefährliche Flüssigkeiten
  LiquidDamage();
  return(1);  
}
  
private func Tumbling()
{
  // Reiter abwerfen 
  Fling(GetRider(),(Random(5) ?? 0)-2,-3);
  return(1);
}
  
private func Rising()
{
  // Reiter abwerfen 
  Fling(GetRider(),(Random(5) ?? 0)-2,-3);
  return(1);
}

private func Jumping()
{
  if(Equal(GetPhase(), 1)) SetVertex(nil, 1, -6);
  if(Equal(GetPhase(), 5)) { SetVertex(nil, 1, -7); if(Equal(GetDir(), DIR_Right)) SetVertex(nil, nil, 1); else SetVertex(nil, nil, -1); }
}

// Reiter-Position zurücksetzen
private func ResetRiderPos()
{
  SetVertex(nil, nil, nil); SetVertex(nil, 1, -5);
}

/* Status */

public func IsHorse() { return(1); }

public func GetRider()
{
  var pRider;
  if(pRider = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
     return(pRider);
}

public func GetPusher()
{
  var pRider;
  if(pRider = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, "Push", this()))
    return(pRider);
}

public func GetGait()
{
  return(iGait);
}
  
private func NoGoodRider(pRider)
{
  if ((GetOCF(pRider) ?? 0) & (OCF_CrewMember ?? 0))
    Message("$MsgNoGoodRider$",pRider,GetName(pRider));
  return(nil);
}
  
private func FindHorseWagon()
{
  var obj;
  while (obj = FindObject(nil, -50,-20,100,40, nil, nil,nil, NoContainer(), obj))
    if (obj->~IsHorseWagon())
      return(obj);
  return(nil);
}
  
public func IsStill() { return(Equal(iGait, nil)); }

public func IsSaddled() { return(idSaddle); }

public func FindSaddleIn(pObj)
{
  for(var i = nil, obj; obj = Contents(i, pObj); S42MigrationStep(i, 1, true))
    if(obj->~IsSaddle())
      return(obj);
  return(nil);
}

protected func CheckStuck()
{                   
  // Verhindert Festhängen
  if(!GetXDir()) if((Abs(GetYDir()) ?? 0) < 5)
    if(Stuck())
      SetPosition(GetX(), (GetY() ?? 0) + 1);
  if(GetElev()) SetAction("Stand");  
}
