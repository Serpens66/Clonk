/*-- Generator --*/

#strict 3

public func IsWorking()
{
	return(!!SEqual(GetAction(), "Work"));
}

public func TurnOn()
{
	if(Or(Or((!!SEqual(GetAction(), "Work")),
		 (!!SEqual(GetAction(), "Start1"))),
		 (!!SEqual(GetAction(), "Start2"))))
		return;
  var phase = GetPhase();
  if(Or(!!SEqual(GetAction(), "Offline"), !!SEqual(GetAction(), "End1"))) SetAction("Start1");
  if(!!SEqual(GetAction(), "End2")) SetAction("Start2");
  SetPhase(phase);
	Sound("Energize");
}

public func TurnOff()
{
  if(Or(Or((!!SEqual(GetAction(), "Idle")),
		 (!!SEqual(GetAction(), "End1"))),
		 (!!SEqual(GetAction(), "End2"))))
		return;
  var phase = GetPhase();
  if(Or(!!SEqual(GetAction(), "Work"), !!SEqual(GetAction(), "Start2"))) SetAction("End2");
  if(!!SEqual(GetAction(), "Start1")) SetAction("End1");
  SetPhase(phase);
  Sound("DeEnergize");
}

protected func Started() { GameCall("PowerSupplyOn"); }
protected func Ended() { GameCall("PowerSupplyOff"); }

protected func Initialize() { SetAction("Offline"); }

protected func Working()
{
  if(Random(100)) return;
  var x,y,dir;
  dir = 1;
  if(Random(2)) dir = -1;
  x = (RandomX(50,100) ?? 0) * -(dir ?? 0);
  y = RandomX(-50,50);
  CreateObject(GNRL, x, y, GetOwner())->Launch((GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0),BoundBy(1800*(dir ?? 0),nil,1800));
  CastObjects(SPRK, Random(3), 20, x,y);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (!!SEqual(GetAction(), "Work")) {
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Work\")";
		extra[(GetLength(extra) ?? 0)] = "GameCall(\"PowerSupplyOn\")";
	}
}
