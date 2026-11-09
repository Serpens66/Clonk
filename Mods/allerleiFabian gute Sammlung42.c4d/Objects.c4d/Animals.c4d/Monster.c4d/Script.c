/*-- Monster --*/

#strict 3
#include ANIM

public func IsPossessible() { return(1); }
public func IsThreat() {return(true);}

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  // Nichts machen
  if (Or(Random(2), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return(1);

  // Ein Ei legen
  if (!Random(ReproductionRate())) 
   Reproduction();	

  // Springen
  if (!!SEqual(GetAction(), "Walk"))
   if (!Random(3)) return(DoJump());

  // Umsehen
  if (!!SEqual(GetAction(), "Walk"))
   if (!Random(8)) return(SetAction("LookUp"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  return(TurnRight());
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  return(TurnLeft());
}

/* Aktionen */

public func TurnRight()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

public func TurnLeft()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

private func HitCheck()
{
  var obj;
  if (obj = FindObject(nil, +1,nil,nil,nil, OCF_Prey, nil,nil, NoContainer()))
   Punch(obj, 10);
  return(1);
}

public func DoJump()
{
  if (!SEqual(GetAction(), "Walk")) return;
  if (Random(2)) Sound("Growl*");
  Jump();
}

/* Einwirkungen */

protected func Death()
{
  Sound("DeathGrowl");
  SetDir(DIR_Left);
  ChangeDef(DMNS);
  SetAction("Dead");
  GameCallEx("Zaehler1");
  return(1);
}

/* Vermehrung */

private func ReproductionRate() { return(2000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()   { return(4); }    // Maximale Tieranzahl im Umkreis

private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = (ReprodSize ?? 0)  / -2;
  return((ObjectCount(MONS, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive) ?? 0) + (ObjectCount(MEGG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , nil) ?? 0));
}

private func SpecialRepr()
{
  Exit(CreateContents(MEGG), nil, 10);
  return(1);
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (!!SEqual(szCommand, "MoveTo"))
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(nil);
}

protected func ContainedLeft(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnLeft();
 return(1);
}

protected func ContainedRight(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnRight();
 return(1);
}

protected func ContainedUp(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(Contained()) return SetCommand(this, "Exit");
 if (!!SEqual(GetAction(), "Swim"))
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Up);

  return(1);
 }

 DoJump();
 return(1);
}

protected func ContainedDown(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (!!SEqual(GetAction(), "Swim"))
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down);
  return(1);
 }

 if (!!SEqual(GetAction(), "Walk"))
  SetAction("LookUp");
  
 return(1);
}

/* JumpAndRun Steuerung */

private func ClearDir(bool fX)
{
  fX ??= false;
  if(And(fX, GetXDir()))
  {
    if((GetXDir() ?? 0) > 0) SetXDir(Max((GetXDir() ?? 0) - 2, nil));
    else SetXDir(Min((GetXDir() ?? 0) + 2, nil));
  }
  if(And(!fX, GetYDir()))
  {
    if((GetYDir() ?? 0) > 0) SetYDir(Max((GetYDir() ?? 0) - 2, nil));
    else SetYDir(Min((GetYDir() ?? 0) + 2, nil));
  }
}

public func ContainedUpdate(object self, int comdir)
{
  comdir ??= 0;
  if(!!SEqual(GetAction(), "Swim"))
  {
    SetComDir(comdir);
    ClearScheduleCall(this(), "ClearDir");
    if(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_Up))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetXDir()) ?? 0)+1)/2, true);
    if(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetYDir()) ?? 0)+1)/2, nil);
  }
  else
  {
    if(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_DownRight))) comdir = COMD_Right;
    if(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_Down))) comdir = COMD_Stop;
    if(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_DownLeft))) comdir = COMD_Left;
    SetComDir(comdir);
  }

  return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}




public func Incineration() {
  // Monster brennen ncht wirklich, deshalb in firelight als ausnahme deklarieren
  Extinguish();
  Schedule("DoDmg(1,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}

