/*--- Dampfmaschine ---*/

#strict 3

local iWait; // Wartezeit (an einem Signal)

/* Seil beim Ziehen? */

private func DrawBeam() { return(nil); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetComDir(COMD_Stop);
}

/* Effekte */

protected func Puff()
{
  // Geräusch nur wenn wir aktiv sind
  if (!!SEqual(GetAction(), "Push"))
  {
    Sound("Chuff");
    Smoke(-5, -14, 5 + (Random(4) ?? 0));
  }
}

/* Signalsteuerung */

protected func CheckSignal()
{
  // Noch in Wartezeit
  if (iWait) return(nil);
  // Signal suchen
  var pSignal = FindObject(SGNL, nil, +1);
  if (!pSignal) return(nil);
  // Signal ist abgeschaltet
  if (!GetPhase(pSignal)) return(nil);
  // Signal ans Signal ;)
  pSignal->SGNL::TrainArrive(this());
  // Wartezeit
  iWait = 9; 
  // Anhalten
  SetComDir(COMD_Stop);
  return(1);
}

protected func SignalDelay()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  // Keine Wartezeit
  if (!iWait) return(nil);
  // Wartezeit aufzählen
  S42MigrationStep(iWait, -1);
  // Abfahrtszeit vorm Zählerende
  if (Equal(iWait, 2)) DepartFromSignal();
  return(1);
}

private func DepartFromSignal()
{
  // Signal suchen
  var pSignal = FindObject(SGNL, nil, +1);
  if (!pSignal) return(1);
  // Signal benachrichtigen
  pSignal->SGNL::TrainDepart(this());
  // Signal deaktiviert: Stehen bleiben
  var iPhase = GetPhase(pSignal);
  if (!iPhase) return(1);
  // Geschwindigkeit anpassen
  AdjustTrainSpeed();
  // Signal rechts
  if (!(S42MigrationStep(iPhase, -1))) return(SetComDir(COMD_Right));
  // Signal links
  if (!(S42MigrationStep(iPhase, -1))) return(SetComDir(COMD_Left));
  // Signal Weiterfahrt: Nach GetDir
  if (Equal(GetDir(), DIR_Left))
    return(SetComDir(COMD_Left));
  else
    return(SetComDir(COMD_Right));
}

/* Ereignisse */

protected func GrabLost(obj) 
{
  // Geräusch abspielen, wenn wir angekoppelt waren
  var do_sound;
  if (!obj) do_sound = 1;
  if (Equal(GetType(obj), C4V_C4Object))
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      do_sound = 1;
  if (do_sound) Sound("LineBreak");
  // Anhalten
  SetComDir(COMD_Stop);
  return(1);
}

private func GrabTarget() 
{
  // Nächstes greifbares Objekt suchen
  var obj = FindObject(nil, -1, -1, -1, -1, OCF_Grab, nil, nil, NoContainer());
  // Außer Reichweite
  if (Or(!obj, (ObjectDistance(this(), obj) ?? 0) > 50)) return(nil);
  // Keine fixierten, nicht verschieb oder ziebare Objekte
  if(Or(Or(GetDefHorizontalFix(GetID(obj)), Equal(GetDefGrab(GetID(obj)), 2)), obj->~NoPull())) return nil;
  // Ankuppeln
  SetAction("Push", obj);
  Sound("Connect");
  // Verbindungslinie zeigen
  if (DrawBeam()) CreateObject(STBM, nil, nil, GetOwner())->~Connect(obj, this());
  return(1);
}

private func ReleaseTarget()
{
  var pLine;
  if (DrawBeam()) if(pLine = FindObject(STBM, nil, nil, nil, nil, nil, "Connect", GetActionTarget()))
    RemoveObject(pLine);
  if (DrawBeam()) if(pLine = FindObject(STBM, nil, nil, nil, nil, nil, "Connect", this()))
    RemoveObject(pLine);
  SetAction("Walk");
  Sound("LineBreak");
}

protected func Entrance(object pNewContainer)
  {
    // Im Gebäude abkoppeln (sonst hat man vor der Tür einen Wagon, der immer noch verbunden ist)
    ReleaseTarget();
  }

private func AdjustTrainSpeed()
{
  // Zug angefasst: langsam fahren
  if (Or(FindObject(nil, nil,nil,nil,nil, OCF_Living, "Push", this()), (And(GetActionTarget(), FindObject(nil, nil,nil,nil,nil, OCF_Living, "Push", GetActionTarget())))))
    SetPhysical("Walk", 70000, 2);
  // Zug nicht angefasst: schnell fahren
  else
    SetPhysical("Walk", 100000, 2);
}
  
/* Signalsteuerung */

public func SignalCall(object pSignal, bool fLoad)
{
  // An Waggon weiterleiten
  fLoad ??= false;
  if (!!SEqual(GetAction(), "Push"))
    return(GetActionTarget()->~SignalCall(pSignal,fLoad));
}
  
public func SignalDepart(object pSignal)
{
  // An Waggon weiterleiten
  if (!!SEqual(GetAction(), "Push"))
    return(GetActionTarget()->~SignalDepart(pSignal));
}

/* Steuerung */

public func SetDirection(int comdir)
{
  // Geschwindigkeit anpassen
  comdir ??= 0;
  AdjustTrainSpeed();
  // Fahren
  Sound("Click");
  SetComDir(comdir);
}

public func ControlLeft(object pByObject)
{
  [$TxtLeft$|Image=STMS:1]
  // Zähler blockieren
  iWait = 2;
  // Nicht angekoppelt: nur schieben lassen
  if (!SEqual(GetAction(), "Push")) return(nil);

  if(!GetPlrCoreJumpAndRunControl(pByObject->GetController()))
    SetDirection(COMD_Left);

  return(1);
}

public func ControlRight(object pByObject)
{
  [$TxtRight$|Image=STMS:2]
  // Zähler blockieren
  iWait = 2;
  // Nicht angekoppelt: nur schieben lassen
  if (!SEqual(GetAction(), "Push")) return(nil);

  if(!GetPlrCoreJumpAndRunControl(pByObject->GetController()))
    SetDirection(COMD_Right);

  return(1);
}

public func ControlDownSingle(object pByObject)
{
  [$TxtStop$|Method=None]
  if(!GetPlrCoreJumpAndRunControl(pByObject->GetController()))
  {
    // Anhalten
    SetDirection(COMD_Stop);
    // Zähler resetten
    iWait = nil;
    // Vorm Signal abgestellt?
    CheckSignal();
  }

  return(1);
}

/* JumpAndRun-Steuerung */

public func ControlUpdate(object pByObject, int comdir)
{
  comdir ??= 0;
  if(Or(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_Left)), Equal(comdir, COMD_DownLeft)))
    comdir = COMD_Left;
  else if(Or(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_Right)), Equal(comdir, COMD_DownRight)))
    comdir = COMD_Right;
  else
    comdir = COMD_Stop;

  SetDirection(comdir);

  if(Equal(comdir, COMD_Stop))
  {
    iWait = nil;
    CheckSignal();
  }
}

// Schadet nicht, wenn die auch fuer klassische Steuerung zugänglich sind
public func ControlLeftDouble(object clonk)
{
  [$TxtLeftDouble$|Image=STMS:3]
  // Fahrt links
  SetDirection(COMD_Left);

  // Loslassen
  if(!!SEqual(clonk->GetAction(), "Push"))// && clonk->GetActionTarget() == this())
    AddCommand(clonk, "UnGrab");
}

public func ControlRightDouble(object clonk)
{
  [$TxtRightDouble$|Image=STMS:4]
  // Fahrt rechts
  SetDirection(COMD_Right);

  // Loslassen
  if(!!SEqual(clonk->GetAction(), "Push"))// && clonk->GetActionTarget() == this())
    AddCommand(clonk, "UnGrab");
}

/* Attach/Detach */

public func ControlDigDouble()
{
  [$TxtAttachOrDetach$|Image=STMS:0]
  if (!!SEqual(GetAction(), "Push")) return(ReleaseTarget());
  return(GrabTarget());
}
