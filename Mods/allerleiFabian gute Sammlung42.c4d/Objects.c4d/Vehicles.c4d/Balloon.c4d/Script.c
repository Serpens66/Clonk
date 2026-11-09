/*-- Ballon --*/

local target_y;

#strict 3

func Initialize()
{
  target_y = -1;
}

/* Steuerung */

public func ControlUp(object controller)
{
  [$TxtRise$]
  ClearCommand();
  if(!!SEqual(GetAction(), "WaterLand")) SetAction("WaterLaunch");

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    var comdir = GetComDir();
    if(GBackSolid(nil, (GetObjHeight() ?? 0)/2+2)) comdir = COMD_Stop;

    if(Equal(comdir, COMD_Down))
      SetDirection(COMD_Stop);
    else
      SetDirection(COMD_Up);
  }
}

public func ControlDownSingle(object controller)
{
  [$TxtLower$|Method=Classic]
  ClearCommand();

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    if(Equal(GetComDir(), COMD_Up))
      SetDirection(COMD_Stop);
    else
      SetDirection(COMD_Down);
  }
}

public func ControlDown(object controller)
{
  [$TxtLower$|Method=JumpAndRun]
}

private func ClearDir()
{
  if(GetYDir())
  {
    if((GetYDir() ?? 0) > 0) SetYDir((GetYDir() ?? 0) - 1);
    else SetYDir((GetYDir() ?? 0) + 1);
  }
}

public func ControlUpdate(object controller, int comdir)
{
  comdir ??= 0;
  SetDirection(comdir);
}

public func SetDirection(int comdir)
{
  comdir ??= 0;
  if(Or(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_UpLeft)), Equal(comdir, COMD_UpRight)))
    SetComDir(COMD_Up);
  else if(Or(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_DownLeft)), Equal(comdir, COMD_DownRight)))
    SetComDir(COMD_Down);
  else
  {
    ScheduleCall(this(), "ClearDir", 1, Abs(GetYDir()));
    SetComDir(COMD_Stop);
  }

  CheckWindY();
}

protected func ControlCommand(string szCommand, object pTarget, int iX, int iY)
{
  // Pilot läßt los
  iX ??= 0;
  iY ??= 0;
  if(!!SEqual(szCommand, "UnGrab"))
    return(S42MigrationFirst(nil,ClearCommand()));
  // Bewegungskommando vom Piloten
  if(!!SEqual(szCommand, "MoveTo"))
  {
    target_y = iY;
    return(true);
  }
}

private func ClearCommand() { target_y = -1; }

/* Initialisierung */

protected func Initialize()
{
  target_y = -1;
  SetAction("Float");
  SetComDir(COMD_Down);
}

/* Aktivität */

protected func Wind2Flag() { SetDir(BoundBy((GetWind() ?? 0)/10+4,nil,8)); }

private func Wind2Float()
{
  if(!GetContact(nil, -1, 8)) SetXDir( BoundBy((GetWind() ?? 0)/4,-10,+10));
  if (!FindObject(nil,nil,nil,nil,nil,nil,"Push",this()))
    SetComDir(COMD_Down);
  else if(!Equal(target_y, -1))
  {
    SetYDir(BoundBy(((target_y ?? 0) - (GetY() ?? 0))/4, -10, 10) );
    SetComDir(COMD_None);

    if(Inside(GetY(), (target_y ?? 0) - 5, (target_y ?? 0) + 5))
      target_y = -1;
  }

  CheckWindY();
  Wind2Flag();
}

private func CheckWindY()
{
  // Ohne Kommanda ein bisschen rauf- und runter geweht werden
  if(And(And(Equal(GetComDir(), COMD_Stop), Equal(target_y, -1)), !GBackSolid(nil, (GetObjHeight() ?? 0)/2 + 2)))
  {
    if(!GetEffect("IntWindYDir", this()))
      AddEffect("IntWindYDir", this(), 1, 10, this());
  }
  else
  {
    if(GetEffect("IntWindYDir", this()))
      RemoveEffect("IntWindYDir", this());

    // Wenn der Ballon am Boden ist ComDir auf COMD_Down setzen,
    // sonst hebt der Ballon zu schnell wieder ab
    if(And(Equal(GetComDir(), COMD_Stop), Equal(target_y, -1)))
      SetComDir(COMD_Down);
  }
}

protected func FxIntWindYDirTimer(object target, int number, int time)
{
  number ??= 0;
  time ??= 0;
  SetYDir(BoundBy((GetYDir() ?? 0) + (RandomX(-(GetYDir() ?? 0)) ?? 0) - 1 + (Random(3) ?? 0), Max((GetYDir() ?? 0) - 1, -3), Min((GetYDir() ?? 0) + 1, 3)));
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject, object pObj)
{
  // Pfeile vertragen sich nicht mit einem Ballon
  if (DefinitionCall(idObject,"IsArrow"))
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

protected func Collection(object pObj)
  {
  var idObj = GetID(pObj);
  // bei brennendem Pfeil anzünden
  if(OnFire(pObj)) Incinerate();
  // ansonsten abstürzen
  else SetAction("DropOff");
  // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
  ProtectedCall(pObj, "Hit");
  return(1);
  }

/* Externe Einwirkungen */

public func Poke()
{
  Sound("Rip");
  SetAction("DropOff");
}
