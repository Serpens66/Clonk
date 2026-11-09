/*-- Tür --*/

#strict 3

local Target,X,Y;

public func IsOpen()
{
  if(!!SEqual(GetAction(), "DoorOpen")) return true;
  if(!!SEqual(GetAction(), "OpenDoor")) return true;
  return nil;
}

public func IsReallyOpen()
{
  if(!!SEqual(GetAction(), "DoorOpen")) return true;
  return nil;
}

public func Open()
{
  var iPhase=1;
  if(!!SEqual(GetAction(), "CloseDoor"))
    iPhase=(GetActMapVal("Length", "OpenDoor") ?? 0)-(GetPhase() ?? 0);
  SetAction("OpenDoor");
  SetPhase(iPhase);
}

public func Set(pTarget,iX,iY)
{
  Target=pTarget;
  SetAction("DoorOpen");
  X=iX;
  Y=iY;
  SetPosition((GetX(pTarget) ?? 0)+(iX ?? 0),(GetY(pTarget) ?? 0)+(iY ?? 0));
}

public func Check()
{
  // An Gebäude anpassen
  SetCon(GetCon(Target));
  if((GetCon() ?? 0)<80)
    SetGraphics("Damage");
  else
    SetGraphics("");
    
  // Position nur neusetzen wenn Fullcon
  if(Equal(GetCon(), 100))
    SetPosition((GetX(Target) ?? 0)+(X ?? 0),(GetY(Target) ?? 0)+(Y ?? 0));
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
  Target->SetEntrance(nil);
}

private func SoundOpenDoor()
{
  Sound("DoorOpen");
}

private func SoundCloseDoor()
{
  Sound("DoorClose");
}
