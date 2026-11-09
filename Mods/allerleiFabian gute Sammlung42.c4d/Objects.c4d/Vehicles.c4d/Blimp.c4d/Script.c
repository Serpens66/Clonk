/*-- Luftschiff --*/

#strict 3

local turn_end_dir, mlight;

/* Drehung */

private func TurnStart()
{
  // Alle Clonks die das Luftschiff anfassen registrieren
  var controllers = FindObjects(Find_Action("Push"), Find_ActionTarget(this()));
  AddEffect("IntTurn", this(), 1, 1, this(), nil, controllers);
}

protected func FxIntTurnStart(object target, int number, int temp, array controllers)
{
  number ??= 0;
  temp ??= 0;
  if(temp) return(nil);

  // Alle luftschiffanfassenden Clonks
  EffectVar(nil, target, number) = controllers;
  // Deren initiale Position auf dem Luftschiff...
  EffectVar(1, target, number) = CreateArray(GetLength(controllers));
  // ... die hier ermittelt wird
  for(var i = nil; (i ?? 0) < (GetLength(controllers) ?? 0); S42MigrationStep( i, 1))
    EffectVar(1, target, number)[(i ?? 0)] = (controllers[(i ?? 0)]->GetX() ?? 0) - (GetX() ?? 0);
}

protected func FxIntTurnTimer(object target, int number, int time)
{
  // Laenge der Turn-Action bestimmen
  number ??= 0;
  time ??= 0;
  var delay = GetActMapVal("Delay", "Turn");
  var phases = GetActMapVal("Length", "Turn");
  // Schon vorbei
  if(Equal(time, (delay ?? 0) * (phases ?? 0))) return(-1);
  // Eine Sinus-Kurve von -90 bis +90 Grad hernehmen um die neue Position
  // der Clonks auf dem Luftschiff zu bestimmen.
  var sin_phase = ((time ?? 0) * 180 / ((phases ?? 0) * (delay ?? 0))) - 90;
  var pos_phase = (Sin(sin_phase, 50) ?? 0) + 50;
  // Clonkpositionen anpassen
  var controller_count = GetLength(EffectVar(nil, target, number));
  for(var i = nil; (i ?? 0) < (controller_count ?? 0); S42MigrationStep( i, 1))
  {
    var controller = EffectVar(nil, target, number)[(i ?? 0)];
    var pos = EffectVar(1, target, number)[(i ?? 0)];

    // Koennte sein, dass ein Controller verloren geht
    if(!controller) continue;
    // Koennte auch sein, dass er kein Controller mehr ist
    if(Or(!SEqual(controller->GetAction(), "Push"), !Equal(controller->GetActionTarget(), this())))
    {
      // Dann nichts mehr mit ihm anstellen, selbst wenn er das Luftschiff
      // wieder anfaesst - er koennte es an einer anderen Position tun und
      // dann beamen wir ihn komisch auf dem Luftschiff rum. Lieber nicht.
      EffectVar(nil, target, number)[(i ?? 0)] = nil;
      continue;
    }

    // Dir anpassen wenn Haelfte der Turn-Action vorbei
    if(Equal(time, (delay ?? 0) * (phases ?? 0) / 2))
      controller->SetDir(GetDir());

    // Neue Position ermitteln (Sinusfoermig um den Luftschiff-Mittelpunkt)
    var pos_t = -(pos ?? 0) * 2;
    controller->SetPosition((GetX() ?? 0) + (pos ?? 0) - ((pos ?? 0)*2 * (pos_phase ?? 0) / 100), controller->GetY());
  }
}

protected func FxIntTurnStop(object target, int number, bool temp)
{
  number ??= 0;
  temp ??= false;
  if(temp) return(nil);
  var length = GetLength(EffectVar(nil, target, number));

  // Nochmal alle Controller durchgehen
  for(var i = nil; (i ?? 0) < (length ?? 0); S42MigrationStep( i, 1))
  {
    var object = EffectVar(nil, target, number)[(i ?? 0)];
    var pos = EffectVar(1, target, number)[(i ?? 0)];

    // Und an fertige Position tun, zur Sicherheit, wenn es noch Controller sind
    if(!object) continue;
    if(And(!!SEqual(object->GetAction(), "Push"), Equal(object->GetActionTarget(), this())))
      object->SetPosition((GetX() ?? 0) - (pos ?? 0), object->GetY());
  }
}

private func TurnEnd()
{
  if(!Equal(turn_end_dir, -1))
  {
    SetDirection(turn_end_dir);
    turn_end_dir = -1;
  }
}

/* Steuerung */

private func DoDirection(int comdir)
{
  comdir ??= 0;
  var cur_comdir = GetComDir();
  if(!!SEqual(GetAction(), "FloatIdle")) cur_comdir = COMD_Stop;

  cur_comdir = ComDirTransform(cur_comdir, comdir);
  //while(cur_comdir == COMD_Stop)
  //  cur_comdir = ComDirTransform(cur_comdir, comdir);

  SetDirection(cur_comdir);
}

private func ClearDir(bool fX)
{
  fX ??= false;
  if(And(fX, GetXDir()))
  {
    if((GetXDir() ?? 0) > 0) SetXDir((GetXDir() ?? 0) - 1);
    else SetXDir((GetXDir() ?? 0) + 1);
  }
  if(And(!fX, GetYDir()))
  {
    if((GetYDir() ?? 0) > 0) SetYDir((GetYDir() ?? 0) - 1);
    else SetYDir((GetYDir() ?? 0) + 1);
  }
}

private func SetDirection(int comdir)
{
  // Richtungsaenderung nach oben/unten geht auch mit "Turn", aber eine
  // ComDir-Aenderung, die wieder eine Turn-Action erfordern wuerde muss
  // warten, bis die jetzige Turn-Action fertig ist.
  comdir ??= 0;
  if(!!SEqual(GetAction(), "Turn"))
  {
    turn_end_dir = comdir;
    if(Or(Or(Equal(comdir, COMD_Stop), (And(ComDirLike(comdir, COMD_Right), Equal(GetDir(), DIR_Left)))), (And(ComDirLike(comdir, COMD_Left), Equal(GetDir(), DIR_Right)))))
    {
      return(nil);
    }
  }

  // ComDir uebernehmen
  SetComDir(comdir);

  // Angebrachte Action setzen
  if(And(!SEqual(GetAction(), "Turn"), !SEqual(GetAction(), "WaterLaunch")))
  {
    if(And(!Equal(comdir, COMD_Stop), !SEqual(GetAction(), "FloatPropel")))
      SetAction("FloatPropel");
    if(And(Equal(comdir, COMD_Stop), !SEqual(GetAction(), "FloatIdle")))
      SetAction("FloatIdle");
  }

  // Vorherige ClearDirs wegtun
  ClearScheduleCall(this(), "ClearDir");
  // Wenn Bewegung in nur eine Richtung, dann Bewegung in die andere
  // Richtung aufgeben (macht FLOAT-Prozedur nicht automagisch)
  if(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_Up)))
    ScheduleCall(this(), "ClearDir", 1, Abs(GetXDir()), true);
  if(Or(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right)), Equal(comdir, COMD_Stop)))
  {
    ScheduleCall(this(), "ClearDir", 1, Abs(GetYDir()), nil);

    // Bei Bewegung nur in X-Richtung ein wenig schaukeln
    if(!GetEffect("IntWindYDir", this()))
      AddEffect("IntWindYDir", this(), 1, 10, this());
  }
  else
  {
    if(GetEffect("IntWindYDir", this()))
      RemoveEffect("IntWindYDir", this());
  }

  if(And(ComDirLike(comdir, COMD_Right), Equal(GetDir(), DIR_Left)))
  {
    SetDir(DIR_Right);
    SetAction("Turn");
  }

  if(And(ComDirLike(comdir, COMD_Left), Equal(GetDir(), DIR_Right)))
  {
    SetDir(DIR_Left);
    SetAction("Turn");
  }

  // Beim Landen auf COMD_Down lassen, sonst hopst das Luftschiff schnell
  // wieder in die Luft
  if(And(Equal(comdir, COMD_Stop), GBackSolid(nil, (GetObjHeight() ?? 0)/2+2)))
    SetComDir(COMD_Down);
}

protected func ControlUpdate(object controller, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  SetDirection(comdir);
}

protected func ControlUp(object controller)      // hoch
{
  [$TxtAscend$]
  ClearCommand();
  if (!!SEqual(GetAction(), "DropOff")) return(nil);
  if (!!SEqual(GetAction(), "WaterLand")) SetAction("WaterLaunch");

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
    DoDirection(COMD_Up);
}

protected func ControlDownSingle(object controller)  // runter
{
  [$TxtDescend$|Method=Classic]
  if (!!SEqual(GetAction(), "DropOff")) return(nil);
  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    ClearCommand();
    DoDirection(COMD_Down);
  }
}

protected func ControlDown(object controller)
{
  [$TxtDescend$|Method=JumpAndRun]
  // Lediglich fuer das Kommandosymbol bei JnR-Steuerung
}

protected func ControlRight(object controller)       // rechts
{
  [$TxtTurnright$]
  ClearCommand();

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    if (!!SEqual(GetAction(), "DropOff")) return(nil);
    if (!!SEqual(GetAction(), "Turn")) return(nil);
    SetAction("FloatPropel");
    DoDirection(COMD_Right);
  }
}

protected func ControlLeft(object controller)        // links
{
  [$TxtTurnleft$]
  ClearCommand();

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    if (!!SEqual(GetAction(), "DropOff")) return(nil);
    if (!!SEqual(GetAction(), "Turn")) return(nil);
    SetAction("FloatPropel");
    DoDirection(COMD_Left);
  }
}

protected func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Pilot läßt los
  iX ??= 0;
  iY ??= 0;
  if (!!SEqual(szCommand, "UnGrab"))
    return(S42MigrationFirst(nil,ClearCommand(),NoPilotCheck()));
  // Bewegungskommando vom Piloten
  if (!!SEqual(szCommand, "MoveTo"))
    return(SetCommand(this(),szCommand,pTarget,iX,iY));
  return(nil);
}

private func ClearCommand() { SetCommand(this(),"None"); }

/* Initialisierung */

protected func Initialize()
{
  turn_end_dir = -1;
  FloatIdleDown();
}

/* Aktivität */

protected func FxIntWindXDirTimer()
{
  if(And(!SEqual(GetAction(), "FloatIdle"), !SEqual(GetAction(), "WaterLand"))) return(-1);
  if(GetContact(nil, -1, 8)) return;
  SetXDir(BoundBy((GetWind() ?? 0)/4, Max((GetXDir() ?? 0) - 2, -10), Min((GetXDir() ?? 0) + 2, 10)));
}

protected func FxIntWindYDirTimer()
{
  if(GBackSolid(nil, (GetObjHeight() ?? 0)/2+2)) return(-1);
  SetYDir(BoundBy((GetYDir() ?? 0) + (RandomX(-(GetYDir() ?? 0)) ?? 0) - 1 + (Random(3) ?? 0), Max((GetYDir() ?? 0) - 1, -3), Min((GetYDir() ?? 0) + 1, 3)));
}

private func FloatProcess()
{
  if(!GetEffect("IntWindXDir", this()))
    AddEffect("IntWindXDir", this(), 1, 5, this());

  if(And(!GetEffect("IntWindYDir", this()), !GBackSolid(nil, (GetObjHeight() ?? 0)/2+2)))
  {
    AddEffect("IntWindYDir", this(), 1, 10, this());
  }

//  if(GetComDir() == COMD_Stop && GBackSolid(0, GetObjHeight()/2+2))
//    SetComDir(COMD_Down);

  NoPilotCheck();
  CheckCommandAction();
  return(1);
}

private func FlightProcess()
{
  NoPilotCheck();
  CheckCommandAction();
  return(1);
}

private func CheckCommandAction()
{
  // Propeller an für Kommando
  if (GetCommand()) 
    if (!!SEqual(GetAction(), "FloatIdle"))
      SetAction("FloatPropel");
  // Wenden nach rechts
  if (!!SEqual(GetAction(), "FloatPropel"))
    if (Inside(GetComDir(),COMD_UpRight,COMD_DownRight)) 
      if (Equal(GetDir(), DIR_Left))
        { SetAction("Turn"); SetDir(DIR_Right); return(true); }
  // Wenden nach links
  if (!!SEqual(GetAction(), "FloatPropel"))
    if (Inside(GetComDir(),COMD_DownLeft,COMD_UpLeft)) 
      if (Equal(GetDir(), DIR_Right))
        { SetAction("Turn"); SetDir(DIR_Left); return(true); }
  // Anhalten wenn Ziel erreicht
  if(And(!GetCommand(), !GetComDir()))
    SetDirection(COMD_Stop);
  return(nil);
}   

private func NoPilotCheck()
{
  if (!FindObject(nil,nil,nil,nil,nil,nil,"Push",this()))
    return(FloatIdleDown());
  return(nil);
}

private func FloatIdleDown()
{
  if (!SEqual(GetAction(), "FloatIdle"))
    SetAction("FloatIdle");
  SetComDir(COMD_Down);
  ClearCommand();
  return(1);
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject, object pObj)
{
  // Pfeile vertragen sich nicht mit einem Luftschiff
  if (DefinitionCall(idObject,"IsArrow") )
   // Nur, wenn der Pfeil sich auch bewegt
   if(Or(GetXDir(pObj), GetYDir(pObj)))
  {
    // bei brennendem Pfeil anzünden
    if(OnFire(pObj)) Incinerate();
    // ansonsten abstürzen
    else SetAction("DropOff");
    // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
    ProtectedCall(pObj, "Hit");
    return(1);
  }
  return(1);
}

/* Angriff durch Vögel */

public func Poke()
{
  Sound("Rip");
  SetAction("DropOff");
  return(1);
}
  
/* Forschung */

public func GetResearchBase() { return(BALN); }



public func Incineration() {
  //  brennen leuchtet
TurnLightOn();
return _inherited();
}




private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,50);
  mlight = AddLight(1000, iColor, this());
  return(1);
}

private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}


