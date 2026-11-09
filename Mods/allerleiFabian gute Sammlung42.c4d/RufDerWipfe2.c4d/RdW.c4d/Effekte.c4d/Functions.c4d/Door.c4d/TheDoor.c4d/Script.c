/*-- Tür --*/

#strict

local Target,X,Y;

public func IsOpen()
{
  if(GetAction() eq "DoorOpen") return true;
  if(GetAction() eq "OpenDoor") return true;
  return false;
}

public func IsReallyOpen()
{
  if(GetAction() eq "DoorOpen") return true;
  return false;
}

public func Open()
{
  var iPhase=1;
  if(GetAction() eq "CloseDoor")
    iPhase=GetActMapVal("Length", "OpenDoor")-GetPhase();
  SetAction("OpenDoor");
  SetPhase(iPhase);
}

public func Set(pTarget,iX,iY)
{
  Target=pTarget;
  SetAction("DoorOpen");
  X=iX;
  Y=iY;
  SetPosition(GetX(pTarget)+iX,GetY(pTarget)+iY);
}

public func Check()
{
  // An Gebäude anpassen
  SetCon(GetCon(Target));
  if(GetCon()<80)
    SetGraphics("Damage");
  else
    SetGraphics("");
    
  // Position nur neusetzen wenn Fullcon
  if(GetCon()==100)
    SetPosition(GetX(Target)+X,GetY(Target)+Y);
}

public func SetState(iState)
{
  if(iState){SetAction("DoorOpen");OpenEntrance();return true;}
  SetAction("DoorClose");CloseEntrance();return true;
}

private func OpenEntrance()
{
  Target->SetEntrance(1);
}

private func CloseEntrance()
{
  Target->SetEntrance(0);
}

private func SoundOpenDoor()
{
  Sound("DoorOpen");
}

private func SoundCloseDoor()
{
  Sound("DoorClose");
}
