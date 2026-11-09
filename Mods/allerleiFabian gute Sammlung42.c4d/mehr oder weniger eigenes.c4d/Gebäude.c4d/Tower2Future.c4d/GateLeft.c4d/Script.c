#strict 3

private func CheckRange()
{
  var y = GetY(GetActionTarget() );
  if((GetY() ?? 0) < (y ?? 0) - 2)
    SetComDir(COMD_Down );
  if((GetY() ?? 0) > (y ?? 0) + 27)
    SetComDir(COMD_Up );
  return(1);
}

private func DestructCheck()
{
  if(!Equal(GetID(GetActionTarget()), TWRF)) RemoveObject();
}

public func Activate()
{
  Sound("Click");
  Sound("SteelGate1");
  if(Equal(GetComDir(), COMD_Up) ) return(Close() );
  return(Open() );
}

private func Close()
{
  if((GetY() ?? 0) > (GetY(GetActionTarget()) ?? 0) + 20) return(nil);
  SetComDir(COMD_Down );
}

private func Open()
{
  if((GetY() ?? 0) < (GetY(GetActionTarget()) ?? 0) - 2) return(nil);
  SetComDir(COMD_Up );
}

protected func Completion()
{
  SetComDir( COMD_Up );
}

