/*-- Fahrstuhlkorb --*/

#strict 3

// Local 8: Obergrenze
// Local 9: Untergrenze

local AutoMode;       // Dauerbetrieb
local AutoMoveDir;    // Dauerbetrieb Richtung
local RangeTop;       // Obergrenze
local RangeBottom;    // Untergrenze
local timer;          // Timer

local oElev;          // Das Fahrstuhl-Objekt

/* TimerCall */

protected func Timer() 
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(Elevator())),this());
  timer = (S42MigrationStep(timer, 1)) % 30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Feststecken -> Schachtbohrung
  if (And(Stuck(), !Equal(GetComDir(), COMD_Stop)))
    if (!NoEnergy())
      DigFreeRect((GetX() ?? 0) - 13, (GetY() ?? 0) - 14, 26, 28);
  if (!timer) Activity();
}


protected func Riding() 
{
  var elev = Elevator();
  if (!elev) return Halt();
  // Energieverbrauch
  if (ObjectCount(ENRG))
    if (EnergyCheck(25000, elev))
      DoEnergy(-25000, elev, true);
    else
      Halt();
}
  
protected func Activity()
{
  UpdateTransferZone();
  // Legacy
  if (And(Local(8), !RangeTop)) ImportSettings();
  // Fahrstuhl zerstört
  // if (!Elevator() || OnFire(Elevator())) { SetAction("Idle"); Incinerate(); return(); }
  // Zu wartenden Clonks fahren
  if (And(And(Equal(AutoMode, nil), !Passenger()), !GetCommand()))
    MoveToWaitingClonk();
  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
  if (GetComDir())
    if (And(!GetCommand(), !SEqual(GetAction(), "Drill")))
      Halt();
  // Fertig
  return(1);
}

private func MoveToWaitingClonk()
{
  // Wartender Clonk da? 
  var oClnk;
  if(!(oClnk = FindWaitingClonk()) ) return(nil);

  // Bewegung nötig?
  if(And((Abs((GetY(oClnk) ?? 0) - (GetY() ?? 0)) ?? 0) < 4
     , !SEqual(GetProcedure(oClnk), "SWIM"))) return nil;

  // Zielposition bestimmen
  var iTargetY = GetY(oClnk);
  if(!!SEqual(GetProcedure(oClnk), "SWIM"))
    S42MigrationAssign(iTargetY, "-=", 2);
  if((GetY() ?? 0) > (iTargetY ?? 0))
    S42MigrationAssign(iTargetY, "-=", 3);
  
  // Dorthin fahren
  SetMoveTo(iTargetY);
  return(1);              
}

private func FindWaitingClonk()
{
  var clnk, best, proc;
  while(clnk = FindObject(nil, -30, (RangeTop ?? 0) - (GetY() ?? 0), 60, 6000, OCF_CrewMember, nil, nil, NoContainer(), clnk) )
  {
    proc = GetProcedure(clnk);
    if(Or(Equal(GetComDir(clnk), COMD_Stop), (And(!!SEqual(proc, "SWIM"), Inside(GetXDir(clnk), -5, 5)))) )
    {
      if(Or(Or(Or(Or(Or(Or(!!SEqual(proc, "WALK"), !!SEqual(proc, "PUSH")), !!SEqual(proc, "SCALE")), !!SEqual(proc, "HANGLE")), !!SEqual(proc, "SWIM")), !(!!SEqual(GetAction(clnk), "ArmedWalk"))), !(!!SEqual(GetAction(clnk), "HoldWalk"))))
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if((GetY(clnk) ?? 0) < (GetY() ?? 0) - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk),this()) ) continue;
          if((GetY(clnk) ?? 0) > (GetY() ?? 0) + 7) if(!PathFree(GetX(), (GetY() ?? 0) + 16, GetX(), GetY(clnk),this()) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(Equal(GetCursor(GetController(clnk)), clnk))
            best = clnk;
          else if(!Equal(GetCursor(GetController(best)), best))
            if((ObjectDistance(clnk) ?? 0) < (ObjectDistance(best) ?? 0))
              best = clnk;
        }
    }
  }
  return best;
}

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  for (var i = nil; (i ?? 0) < (vertexCount ?? 0); S42MigrationStep(i, 1, true))
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (Or(!Inside(vertexX, -12, 11), !Inside(vertexY, -20, 15))) return(nil);
  }
  // Passt
  return(1);  
}

private func GrabObjects()
{
  var pObject, did_grab;
  
  // Fahrzeuge
  while (pObject = FindObject(nil, -8, -10, 16, 30, OCF_Grab, nil, nil, NoContainer(), pObject))
  {
    // Objekt passt in den Fahrstuhlkorb
    if (FitsInElevator(pObject)) 
    {
      if (!((GetCategory(pObject) ?? 0) & (C4D_Vehicle ?? 0))) continue;
      if (!Inside(GetXDir(pObject, 100), -1, +1)) continue;
      if (And(pObject->~IsTree(), (GetCon(pObject) ?? 0) >= 50)) continue;
      if (And(Equal(GetX(pObject), GetX()), Equal(GetY(pObject), (GetY() ?? 0) + 1))) continue;
      if (pObject->~IsElevator() ) continue;
      if (!!SEqual(GetProcedure(pObject), "FLOAT")) continue;
      did_grab = true;
      
      GrabAdjustPosition(pObject);
    }
  }
  
  return(did_grab);
}

private func GrabAdjustPosition(obj) 
{
  var offset = ((GetObjHeight(this()) ?? 0) - (GetObjHeight(obj) ?? 0)) / 2 - 1;
  var x = GetX();

  if ((GetCategory(obj) ?? 0) & (C4D_Object ?? 0))
  {
    if((GetOCF(obj) ?? 0) & (OCF_HitSpeed1 ?? 0) ) return(nil);
    offset = 8 + (GetYDir() ?? 0) / 10;
    x = GetX(obj);
  }
  else if (obj->~IsLorry())
    offset = -1 + (GetYDir() ?? 0) / 10;
  
  SetSpeed(nil, (GetYDir() ?? 0) / 20, obj); SetRDir(nil, obj);
  SetR(nil, obj);
  SetPosition(x, (GetY() ?? 0) + (offset ?? 0), obj);
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = (OCF_Living ?? 0) | (OCF_NotContained ?? 0);
  // Schiebende Clonks suchen
  while (clonk = FindObject(nil, -15, -10, 30, 20, clonk_ocf, "Push", nil, nil, clonk))
  {
    if (And(!Equal(GetComDir(clonk), COMD_Stop), !Equal(GetComDir(clonk), COMD_Up))) continue;
    if (!Inside(GetXDir(clonk), -2, +2)) continue;
    if (And(GetCommand(clonk), !SEqual(GetCommand(clonk), "None"))) continue;
    if (GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(nil, clonk);
    if (!target) continue;
    if (target->~IsElevator() )
    {
      if(Equal(target, this()))
      {
        // In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -8, +8))
        {
          SetCommand(clonk, "Grab", this());
          AddCommand(clonk, "MoveTo", nil, BoundBy(clonk->GetX(), (GetX() ?? 0)-5, (GetX() ?? 0)+5), GetY());
          AddCommand(clonk, "UnGrab");
        }
      }

      continue;
    }

    if (!Inside(GetX(target), (GetX() ?? 0) - 8, (GetX() ?? 0) + 8)) continue;
    if ((ObjectDistance(this(), target) ?? 0) > 15) continue;
    if (!Inside(GetXDir(target), -2, +2)) continue;
    if (!PathFree(GetX(this()), GetY(this()), GetX(target), GetY(target),this())) continue;
    if (!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if (!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -5, +5))
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this());

      // Clonk soll in die Mitte laufen, wenn das Target gerade erst
      // reingeschoben wurde, sonst steht der Clonk so blöd am Rand des
      // Fahrstuhls und lässt beim Runterfahren den Korb los.
      if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -8, +8))
      {
        AddCommand(clonk, "MoveTo", nil, BoundBy(clonk->GetX(), (GetX() ?? 0)-5, (GetX() ?? 0)+5), GetY());
        AddCommand(clonk, "UnGrab");
      }
    } 
    else 
    {
      SetActionTargets(this(), nil, clonk);
    }
  }
}

private func SetMoveTo(iPos)
{
  if (NoEnergy()) return(1);
  SetAction("Ride"); 
  var iDir = nil;
  if ((iPos ?? 0) > (GetY() ?? 0)) iDir = +3;
  if ((iPos ?? 0) < (GetY() ?? 0)) iDir = -3;
  SetCommand(this(),"MoveTo", nil, GetX(), (Max(iPos, RangeTop) ?? 0) + (iDir ?? 0));
  SetYDir(nil);
  return(1);
}

private func Halt()
{
  if (!Elevator()) { SetAction("Idle"); return; }
  SetAction("Wait");
  SetCommand(this(),"None");
  SetComDir(COMD_Stop);
  SetYDir(nil);
  if ((GetY() ?? 0) > (RangeBottom ?? 0)) UpdateRangeBottom();
  return(1);
}

private func Drilling()
{
  // Steuermann verloren? Suche schiebenden Clonk selbst wenn er ein Fahrzeug im Fahrstuhlkorb schiebt (->Lore)
  var elev = Elevator();
  if (!elev) return Halt();
  if (!FindObject2(Find_InRect(-12,-12,24,24), Find_Action("Push")))
    return Halt();
  // Energieverbrauch
  if (ObjectCount(ENRG))
    if (EnergyCheck(25000, elev))
      DoEnergy(-25000, elev, true);
    else
      Halt();
  return(1);
}

private func Waiting()
{
  // Dauerbetrieb: selbständig losfahren
  if (Equal(AutoMode, 1))
    SelfMovement();
  return(1);
}

private func SelfMovement()
{
  // Selbständig rauf- bzw. runterfahren
  if (AutoMoveDir) SetMoveTo(RangeTop);
  if (!AutoMoveDir) SetMoveTo((LandscapeHeight() ?? 0) - 12);
  AutoMoveDir = !AutoMoveDir;
  return(1);
}


/* Steuerung */

protected func ControlCommand(strCommand, oTarget, iTx, iTy)
{
  if (!Elevator()) return(nil);
  if (Hostile(GetController(oTarget), GetOwner())) return(nil);
  // Bewegungskommando vertikal
  if (!!SEqual(strCommand, "MoveTo"))
    if (Inside(iTx, (GetX() ?? 0)-15, (GetX() ?? 0)+15))
      return(SetMoveTo(iTy));
  // Andere Kommandos nicht auswerten
  return(nil);
}

protected func ControlCommandFinished()
{
  Halt();
  return(1);
}

private func DoControlDown(pObj)
{  
  if (!Elevator()) return(nil);
  if (NoEnergy()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  SetMoveTo((LandscapeHeight() ?? 0) - 12);
}

private func DoControlDownReleased(object pObj)
{
  Halt();
}

private func DoControlUp(pObj)
{
  if (!Elevator()) return(nil);
  if (NoEnergy()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  SetMoveTo(RangeTop);
}

private func DoControlUpReleased(object pObj)
{
  Halt();
}

private func DoControlStop(pObj)
{
  if (!Elevator()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  Halt();
}

private func DoControlDig(pObj)
{
  if (!Elevator()) return(nil);
  if (NoEnergy()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  SetCommand(this(),"None");
  SetAction("Drill");
  SetComDir(COMD_Down);
}

private func DoControlDigReleased(pObj)
{
  Halt();
}

public func DoControlAuto(pObj)
{
  // Kein Fahrstuhl
  if (!Elevator()) return(nil);
  // Der Clonk soll uns erst anfassen
  if (Or((!SEqual(GetAction(pObj), "Push")), (!Equal(GetActionTarget(nil, pObj), this()))))
  {
    SetCommand(pObj, "None");
    AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "DoControlAuto");
    AddCommand(pObj, "Grab", this());
    return(nil);
  }
  // Ein Geräusch machen
  Sound("Click");
  // Feindliche Clonks können den Modus nicht ändern
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  // Modus ändern
  AutoMode = !AutoMode;
  // Meldung ausgeben
  Message(GetAutoModeString(AutoMode), this());
}        

private func RouteToVehicle(call, caller, silent)
{
  var vehicle;
  while (vehicle = FindVehicle(vehicle)) 
  {
    if (!((GetCategory(vehicle) ?? 0) & (C4D_Vehicle ?? 0))) continue;
    if (!PathFree(GetX(caller), GetY(caller), GetX(vehicle), GetY(vehicle),this())) continue;
    
    // Kommando 'Werfen'
    if (!!SEqual(call, "ControlThrow"))
    {
      var getput = GetDefGrabPutGet(GetID(vehicle));
      // Kommando per Script überladen?
      if (PrivateCall(vehicle, call, caller))
        return true;
      // Will etwas aus dem Fahrzeug rausnehmen?
      if (And(Equal(ContentsCount(nil, caller), nil), (getput ?? 0) & 2))
      {
        SetCommand(caller, "Get", vehicle, nil,nil, nil, 1);
      } 
      // Will etwas ins Fahrzeug reinlegen
      else
      {
        if ((getput ?? 0) & 1)
        {
          SetCommand(caller, "Put", vehicle);
          AppendCommand(caller, "Grab", this());
        }      
      }
      // Kommando überladen
      return true;
    } 
    // Anderes Kommando
    else 
    {
      if (!PrivateCall(vehicle, call, caller)) 
      {
        var comd;
        if (!!SEqual(call, "ControlLeft")) comd = COMD_Left;
        else if (!!SEqual(call, "ControlRight")) comd = COMD_Right;
        if (comd) SetComDir(comd, caller);
      }
    }
    // Aktionsziele des Fahrzeugs setzen
    SetActionTargets(vehicle, nil, caller);
    // Kommando überladen    
    return(1);
  }
  // Geräusch
  if (!silent) 
    Sound("Click");
  // Kommando nicht überladen
  return(nil);
}

private func FindVehicle(vehicle) 
{
  return(FindObject(nil, -8, -10, 16, 30, OCF_Grab, nil, nil, NoContainer(), vehicle));
}

/* Kontakt */

protected func ContactBottom()
{
  Halt();
  UpdateRangeBottom();
  return(1);
}

protected func ContactTop()
{
  Halt();
  return(1);             
}

/* Initialisierung */

protected func Initialize()
{
  SetAction("Wait");
  UpdateRangeTop();
  UpdateRangeBottom();
}

private func UpdateRangeTop()
{  
  // set Local(8) for backwards compatibility
  Local(8) = RangeTop = GetY();
  UpdateTransferZone();
}

private func UpdateRangeBottom()
{
  var x = GetX(), y = (GetY() ?? 0) + 13;
  while(And(PathFree(x, y, x, (y ?? 0) + 5,this()), (y ?? 0) < (LandscapeHeight() ?? 0))) S42MigrationAssign(y, "+=", 5);
  // set Local(9) for backwards compatibility
  RangeBottom = Local(9) = Max(Local(9), (y ?? 0)+5);
  UpdateTransferZone();
}

/* Status */

private func Passenger()
{
  // Clonk finden
  var clnk = FindObject(nil, -12,-13,24,20, OCF_CrewMember,nil,nil,NoContainer());
  if(!clnk) return (nil);
  // steht wirklich auf dem Fahrstuhl?
  if(!SEqual(GetProcedure(clnk), "WALK")) return (nil);
  return (clnk);
}

private func Elevator()
{
  if(Or(!oElev, !Equal(GetActionTarget(nil, oElev), this())))
    oElev = FindObject(nil,nil,nil,nil,nil,nil,"LiftCase",this());
  return(oElev);
}

private func NoEnergy()
{
  if (EnergyCheck(25000,Elevator())) return(nil);
  Halt();
  //Sound("ElevatorNoEnergy");
  return(1);
}

public func IsElevator() { return(1); }

/* Transfer */

protected func UpdateTransferZone()
{         
  if (!Elevator()) return(SetTransferZone());
  SetTransferZone(-12, (GetY(oElev) ?? 0) - (GetY() ?? 0), 24, (RangeBottom ?? 0) - (GetY(oElev) ?? 0) + 12);
  return(1);
}

protected func ControlTransfer(pObj, iTx, iTy)  
{
  // Fahrstuhl gehört dem Gegner? Dann nicht.
  if (Hostile(GetController(pObj), GetOwner())) return(nil);

  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if (Inside((GetY(pObj) ?? 0) - (RangeBottom ?? 0), -10, +10))
    if (Inside((iTy ?? 0) - (RangeBottom ?? 0), -10, +10))
      return(nil);

  // Maximale Obergrenze Zielposition durch Fahrstuhl
  if (!Elevator()) return(nil);
  iTy = Max(iTy, (GetY(oElev) ?? 0) + 15);

  // Fahrstuhl an Zielposition: erledigt
  if (Inside((iTy ?? 0) - (GetY() ?? 0), -5, +5))
    return(nil);

  // Fahrstuhl fährt: warten
  if (!SEqual(GetAction(), "Wait"))
    return(1);

  // Fahrstuhl noch nicht beim Clonk: warten
  if (!Inside((GetY() ?? 0) - (GetY(pObj) ?? 0), -15, +15))
    return(1);

  // Fahrstuhl nur in der Mitte anfassen
  if (!Inside((GetX() ?? 0) - (GetX(pObj) ?? 0), -5, +5))
    return(AddCommand(pObj,"MoveTo",this()));

  // Fahrstuhl anfassen
  if (Or(And(!InLiquid(pObj), !SEqual(GetAction(pObj), "Push")), !Equal(GetActionTarget(nil, pObj), this())))
    return(AddCommand(pObj,"Grab",this()));
  
  // Fahrstuhl zur Zielposition steuern
  return(SetMoveTo(iTy));
}


/* Zerstörung: Reduzieren des Fahrstuhls für Wiederaufbau/Reparatur */

func Destruction() { if(Elevator()) ObjectCall(Elevator(),"RemoveCase"); }

/* Bei Laden von alten Spielständen: Locals zurückkopieren */

func ImportSettings()
{
  AutoMode = Local(nil);
  AutoMoveDir = Local(7);
  RangeTop = Local(8);
  RangeBottom = Local(9);
}

/* Steuerung */

public func ControlDownSingle(pObj) 
{ 
  [$TxtDown$|Image=ECS1:0|Method=Classic]       
  if(!GetPlrCoreJumpAndRunControl(GetController(pObj)))
    DoControlDown(pObj); 

  return(1); 
}

public func ControlDown(pObj)
{
  [$TxtDown$|Image=ECS1:0|Method=JumpAndRun]
  if(GetPlrCoreJumpAndRunControl(GetController(pObj)))
    DoControlDown(pObj);

  return(1);
}

public func ControlDownReleased(pObj)
{
  DoControlDownReleased(pObj);
}

public func ControlUp(pObj)         
{ 
  [$TxtUp$|Image=ECS1:1]      
  DoControlUp(pObj);   
  return(1); 
}

public func ControlUpReleased(pObj)
{
  DoControlUpReleased(pObj);
}

public func ControlDig(pObj)
{
  [$TxtDrillshaft$|Image=ECS1:2|Method=JumpAndRun]
  if(GetPlrCoreJumpAndRunControl(GetController(pObj)))
    DoControlDig(pObj);
  return(1);
}

public func ControlDigDouble(pObj)  
{
  [$TxtDrillshaft$|Image=ECS1:2|Method=Classic|Condition=NoContext]
  if(!GetPlrCoreJumpAndRunControl(GetController(pObj)))
    DoControlDig(pObj);

  return(1); 
}

public func ControlDigReleased(pObj)
{
  DoControlDigReleased(pObj);
}

public func ControlLeft(pObj) 
{
  [$TxtStop$|Image=ECS1:3|Method=Classic]
  if (And(!SEqual(GetAction(), "Wait"), !GetPlrCoreJumpAndRunControl(GetController(pObj))))
  {
    DoControlStop(pObj);
    SetComDir(COMD_Stop, pObj);
  } 
  else
  {
    if(!RouteToVehicle("ControlLeft", pObj))
      if(GetPlrCoreJumpAndRunControl(GetController(pObj)))
      {
        AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "ContMoveLeft");
        AddCommand(pObj, "UnGrab");
      }
  }

  return(1);
}

public func ControlRight(pObj) 
{
  [$TxtStop$|Image=ECS1:3|Method=Classic]
  if (And(!SEqual(GetAction(), "Wait"), !GetPlrCoreJumpAndRunControl(GetController(pObj))) )
  {
    DoControlStop(pObj);
    SetComDir(COMD_Stop, pObj);
  } 
  else
  {
    if(!RouteToVehicle("ControlRight", pObj))
      if(GetPlrCoreJumpAndRunControl(GetController(pObj)))
      {
        AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "ContMoveRight");
        AddCommand(pObj, "UnGrab");
      }
  }
  return(1);
}

public func ContMoveLeft(object caller) { caller->SetComDir(COMD_Left); }
public func ContMoveRight(object caller) { caller->SetComDir(COMD_Right); }

public func ControlThrow(pObj) 
{
  [$TxtPutGet$|Image=ECS1:4]
  return(RouteToVehicle("ControlThrow", pObj));
}

public func ContextPermanentModeTurnOn(pObj) 
{
  [$TxtPermanentModeTurnOn$|Image=ECS1:5|Condition=IsNotInPermanentMode|Desc=$TxtPermanentModeDesc$]
  DoControlAuto(pObj);
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ECS1:5|Condition=IsInPermanentMode|Desc=$TxtPermanentModeDesc$]
  DoControlAuto(pObj);
}
 
protected func NoContext()
{
  return(nil);
}

private func GetAutoModeString(iAutoMode)
{
  if (iAutoMode) 
    return("$TxtPermanentmodeon$");
  else
    return("$TxtPermanentmodeoff$");
}

public func IsInPermanentMode()
{
  return(AutoMode);
}
  
public func IsNotInPermanentMode()
{
  return(!AutoMode);
}
