/*-- Hangartor --*/

#strict 3

protected func Initialize()
{
  SetAction("GateStill");
}

public func Open() // Aufmachen
{
  var phase = GetPhase();
  SetAction("GateOpen");
  SetPhase(phase);
  CheckSolidMask();
}

protected func Opened()
{
  SetAction("GateStill");
  SetPhase(11);
  StopSound();
  CheckSolidMask();
}

public func Close() // Zumachen
{
  var phase = 11-(GetPhase() ?? 0);
  SetAction("GateClose");
  SetPhase(phase);
  CheckSolidMask();
}

public func Stop() // Sofort stoppen
{
  var phase = GetPhase();
  SetAction("GateStill");
  SetPhase(phase);
  StopSound();
  CheckSolidMask();
}

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
  if(And(Equal(i, 1), !SEqual(GetAction(), "GateOpen"))) Open();
  if(And(Equal(i, 2), !SEqual(GetAction(), "GateClose"))) Close();
  if(And(Equal(i, 3), !SEqual(GetAction(), "GateStill"))) Stop();
}

private func StopSound() { Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(10*(GetPhase() ?? 0), nil, 10, 60, nil, nil);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetPhase()) {
		extra[(GetLength(extra) ?? 0)] = Format("SetPhase(%d)", GetPhase());
		extra[(GetLength(extra) ?? 0)] = "CheckSolidMask()";
	}
}
