/*-- Hangartor --*/

#strict 3

local movement; // -1 = Schlieﬂen
                //  0 = Nichts tun
                //  1 = ÷ffnen

protected func Initialize()
{
  SetAction("Gate");
  SetDir(nil);
}

public func Open() // Aufmachen
{ movement = 1; StartSound(); }

public func Close() // Zumachen
{ movement = -1; StartSound(); }

public func Stop() // Sofort stoppen
{ movement = nil; StopSound(); }

public func ConsoleControl(int i)
{
  i ??= 0;
  if(Equal(i, 1)) return("$Open$");
  if(Equal(i, 2)) return("$Close$");
  if(Equal(i, 3)) return("$Stop$");
}

public func ConsoleControlled(int i)
{
  i ??= 0;
  if(And(Equal(i, 1), Inside(GetDir(), nil, 14))) Open();
  if(And(Equal(i, 2), Inside(GetDir(), 1, 15))) Close();
  if(And(Equal(i, 3), Inside(GetDir(), 1, 14))) Stop();
}

protected func CheckMovement()
{
  CheckSolidMask();

  if(!movement) return;

  if(Equal(movement, -1))
    {
    if(!GetDir())
      { Stop(); }
    else
      SetDir((GetDir() ?? 0)-1);
    }
  if(Equal(movement,  1))
    {
    if(Equal(GetDir(), 15))
      { Stop(); }
    else
      SetDir((GetDir() ?? 0)+1);
    }
}

private func StartSound() { Sound("Elevator", nil,nil,nil,nil, 1); }
private func StopSound() { Sound("Elevator", nil,nil,nil,nil, -1); Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(nil, 8*(GetDir() ?? 0), 200, 8, nil, nil);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetDir()) {
		extra[(GetLength(extra) ?? 0)] = Format("SetDir(%d)", GetDir());
		extra[(GetLength(extra) ?? 0)] = "CheckSolidMask()";
	}
}
