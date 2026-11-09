/*-- Flaschenzugkorb --*/

#strict 3

// Local 8: Obergrenze
// Local 9: Untergrenze

local AutoMode;       // Dauerbetrieb
local AutoMoveDir;    // Dauerbetrieb Richtung
local RangeTop;       // Obergrenze
local RangeBottom;    // Untergrenze
local timer;          // Timer

local oElev;          // Das Fahrstuhl-Objekt

local pIgnoreClonkDown; // Von diesem Clonk sollte das nächste ControlDownSingle ignoriert werden (Notfall-Hack)

protected func Initialize()
{
  SetAction("Wait");
  UpdateRangeTop();
  UpdateRangeBottom();
}

protected func Tool_Init()
{
  SetAction("Wait");
}

public func Init(pFlaschenzug)
{
  oElev= pFlaschenzug;
}

protected func Timer() 
{
  if(And(Abs(GetYDir()), !KeyIsOccupied())) return(DoControlDig(this()));
  timer= (S42MigrationStep(timer, 1)) % 30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  if (!timer) Activity();
}
  
protected func Activity()
{
  UpdateTransferZone();
  // Legacy
  if (And(Local(8), !RangeTop)) ImportSettings();
  // Zu wartenden Clonks fahren
  if (And(And(And(Equal(AutoMode, nil), !Passenger()), !GetCommand()), KeyIsOccupied()))
    MoveToWaitingClonk();
  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
  if (GetComDir())
    if (!GetCommand())
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
     , !SEqual(GetAction(oClnk), "Swim"))) return(nil);

  // Zielposition bestimmen
  var iTargetY = GetY(oClnk);
  if(!!SEqual(GetAction(oClnk), "Swim"))
    S42MigrationAssign(iTargetY, "-=", 2);
  if((GetY() ?? 0) > (iTargetY ?? 0))
    S42MigrationAssign(iTargetY, "-=", 3);
  
  // Dorthin fahren
  SetMoveTo(iTargetY);
  return(1);              
}

private func FindWaitingClonk()
{
  var clnk, best;
  while(clnk = FindObject(nil, -40, (RangeTop ?? 0) - (GetY() ?? 0), 80, 6000, OCF_CrewMember, nil, nil, NoContainer(), clnk) )
    if(Or(Equal(GetComDir(clnk), COMD_Stop), (And(!!SEqual(GetAction(clnk), "Swim"), Inside(GetXDir(clnk), -5, 5)))) ) //(GetComDir(clnk) == COMD_Down() || GetComDir(clnk) == COMD_Up() )) )
      if(Or(Or(Or(Or(!!SEqual(GetAction(clnk), "Walk"), !!SEqual(GetAction(clnk), "Push")), !!SEqual(GetAction(clnk), "Scale")), !!SEqual(GetAction(clnk), "Hangle")), !!SEqual(GetAction(clnk), "Swim")))
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if((GetY(clnk) ?? 0) < (GetY() ?? 0) - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk)) ) continue;
          if((GetY(clnk) ?? 0) > (GetY() ?? 0) + 7) if(!PathFree(GetX(), (GetY() ?? 0) + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(Equal(GetCursor(GetController(clnk)), clnk))
            best = clnk;
          else if(!Equal(GetCursor(GetController(best)), best))
            if((ObjectDistance(clnk) ?? 0) < (ObjectDistance(best) ?? 0))
              best = clnk;
        }
  return(best);
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
  while (pObject = FindObject(nil, -8, -10, 16, 30, OCF_Grab, nil, nil, nil, pObject))
  {
    // Objekt passt in den Fahrstuhlkorb
    if (FitsInElevator(pObject)) 
    {
      if (!((GetCategory(pObject) ?? 0) & (C4D_Vehicle ?? 0))) continue;
      if (!Inside(GetXDir(pObject, 100), -1, +1)) continue;
      if (And(pObject->~IsTree(), (GetCon(pObject) ?? 0) >= 50)) continue;
      if (And(Equal(GetX(pObject), GetX()), Equal(GetY(pObject), (GetY() ?? 0) + 1))) continue;
      if (pObject->~IsElevator() ) continue;
      did_grab = true;
      
      GrabAdjustPosition(pObject);
    }
  }
  
  // Gegenstände
  var obj;
  var item_ocf = (OCF_Collectible ?? 0) | (OCF_NotContained ?? 0);
  while (obj = FindObject(nil, -10, 7, 20, 4, item_ocf, nil, nil, nil, obj))
  {
    if (!((GetCategory(obj) ?? 0) & (C4D_Object ?? 0))) continue;
    if (Or((GetYDir(obj) ?? 0) < -2, !Inside(GetXDir(obj), -2, +2))) continue;
    GrabAdjustPosition(obj);
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
  else if (Equal(GetID(obj), LORY))
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
    if (target->~IsElevator() ) continue;
    if (!Inside(GetX(target), (GetX() ?? 0) - 8, (GetX() ?? 0) + 8)) continue;
    if ((ObjectDistance(this(), target) ?? 0) > 15) continue;
    if (!Inside(GetXDir(target), -2, +2)) continue;
    if (!PathFree(GetX(this()), GetY(this()), GetX(target), GetY(target))) continue;
    if (!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if (!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -5, +5))
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this());
      // Hack: der Clonk hat wahrscheinlich soeben erst mit ControlDown angehalten.
      // Das darauf folgende ControlDownSingle sollte nicht als Kommando 'Runter'
      // für den Fahrstuhl ausgewertet werden!
      pIgnoreClonkDown = clonk;
    } 
    else 
    {
      SetActionTargets(this(), nil, clonk);
    }
  }
}

private func SetMoveTo(iPos)
{
  SetAction("Ride"); 
  var iDir = nil;
  if ((iPos ?? 0) > (GetY() ?? 0)) iDir = +3;
  if ((iPos ?? 0) < (GetY() ?? 0)) iDir = -3;
  SetCommand(this(),"MoveTo", nil, GetX(), (Max(iPos, RangeTop) ?? 0) + (iDir ?? 0));
  SetYDir(nil);
  return(1);
}

public func Halt()
{
  if (!Elevator()) { SetAction("Idle"); return; }
  ObjectSetAction(Schalter(),"Normal");
  SetAction("Wait");
  SetCommand(this(),"None");
  SetComDir(COMD_Stop);
  SetYDir(nil);
  if ((GetY() ?? 0) > (RangeBottom ?? 0)) UpdateRangeBottom();
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

protected func Command(strCommand, oTarget, iTx, iTy)
{
  if (!Elevator()) return(nil);
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
  if (Equal(pIgnoreClonkDown, pObj))
    { pIgnoreClonkDown = nil; return(nil); }
  pIgnoreClonkDown = nil;
  
  if (!Elevator()) return(nil);
  if (!KeyIsOccupied()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  SetMoveTo((LandscapeHeight() ?? 0) - 12);
  ObjectSetAction(Schalter(),"Work");  
}

private func DoControlUp(pObj)
{
  if (!Elevator()) return(nil);
  if (!KeyIsOccupied()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  SetMoveTo(RangeTop);
  ObjectSetAction(Schalter(),"Work");  
}

private func DoControlDig(pObj)
{
  if (!Elevator()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
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
    AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "DoControlAuto", pObj);
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
    if (!PathFree(GetX(caller), GetY(caller), GetX(vehicle), GetY(vehicle))) continue;
    
    // Kommando 'Werfen'
    if (!!SEqual(call, "ControlThrow"))
    {
      var getput = GetDefGrabPutGet(GetID(vehicle));
      PrivateCall(vehicle, call, caller);
      // Will etwas aus dem Fahrzeug rausnehmen?
      if (And(Equal(ContentsCount(nil, caller), nil), (getput ?? 0) & 2))
      {
        SetCommand(caller, "Activate", nil, nil, nil, vehicle);
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
      return(1);
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
  return(FindObject(nil, -8, -10, 16, 30, OCF_Grab, nil, nil, nil, vehicle));
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

private func UpdateRangeTop()
{  
  // set Local(8) for backwards compatibility
  Local(8) = RangeTop = (GetY() ?? 0)-2;
  UpdateTransferZone();
}

private func UpdateRangeBottom()
{
  var x = GetX(), y = (GetY() ?? 0) + 13;
  while(And(PathFree(x, y, x, (y ?? 0) + 5), (y ?? 0) < (LandscapeHeight() ?? 0))) S42MigrationAssign(y, "+=", 5);
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

private func Elevator() { return(oElev); }

private func Schalter() { return(LocalN("Drehrad",oElev)); }

public func KeyIsOccupied() { return(ObjectCall(Schalter(),"IsOccupied")); }

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

protected func Destruction()
{
  if(Var()= FindObject(GHLP,nil,nil,nil,nil,nil,"Attach",this())) RemoveObject(Var());
}

protected func Incinerate()
{
  if(Var()= FindObject(GHLP,nil,nil,nil,nil,nil,"Attach",this())) RemoveObject(Var());
}

/* Bei Laden von alten Spielständen: Locals zurückkopieren */

func ImportSettings()
{
  AutoMode = Local(nil);
  AutoMoveDir = Local(7);
  RangeTop = Local(8);
  RangeBottom = Local(9);
}

/* Steuerung */

public func ControlDownSingle(pObj) { [Abwärts]  DoControlDown(pObj); return(1); }
public func ControlUp(pObj)         { [Aufwärts] DoControlUp(pObj);   return(1); }

public func ControlLeft(pObj) 
{
  [Anhalten]
  if (!SEqual(GetAction(), "Wait"))
  {
    DoControlDig(pObj);
    SetComDir(COMD_Stop, pObj);
  } 
  else
    RouteToVehicle("ControlLeft", pObj);
  return(1);
}

public func ControlRight(pObj) 
{
  [Anhalten]
  if (!SEqual(GetAction(), "Wait"))
  {
    DoControlDig(pObj);
    SetComDir(COMD_Stop, pObj);
  } 
  else
    RouteToVehicle("ControlRight", pObj);
  return(1);
}

public func ControlThrow(pObj) 
{
  [Ablegen/Nehmen]
  return(RouteToVehicle("ControlThrow", pObj));
}

public func ContextPermanentModeTurnOn(pObj) 
{
  [Dauerbetrieb ein|Image=ELEC|Condition=IsNotInPermanentMode]
  DoControlAuto(pObj);
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [Dauerbetrieb aus|Image=ELEC|Condition=IsInPermanentMode]
  DoControlAuto(pObj);
}
 
/* Nachrichten */

private func GetAutoModeString(iAutoMode)
{
  if (iAutoMode) 
    return("$TxtPermanentModeTurnOn$");
  else
    return("$TxtPermanentModeTurnOff$");
}

public func IsInPermanentMode()
{
  return(AutoMode);
}
  
public func IsNotInPermanentMode()
{
  return(!AutoMode);
}

public func GetResearchBase() { return(GRS3); }

/*-- Tooltime :) --*/

// X-Koordinate für CreateObject
public func GetXPos(TurnedLeft)
{
  if(TurnedLeft) return(-31);
  return(+31);
}
// X-Koordinate für CreateObject
public func GetYPos() { return(+27); }

// Tool-Erkennung.
public func IsTooool() { return(1); }