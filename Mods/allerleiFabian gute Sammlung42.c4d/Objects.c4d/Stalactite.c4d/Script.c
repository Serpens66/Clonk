/*-- Stalaktit --*/

#strict 3

local phase;

func Initialize() {
  SetAction("Hanging");
  SetPhase(phase=Random(4));
}

protected func Drop()
{
	if(And(Random(2), FindObject(_CSN)))
	{
		CastPXS("Water", 1, nil, RandomX(-7,7), RandomX(-19,nil));
		ScheduleCall(this, "DropSound", 20, 1);
	}
}

func DropSound()
{
	Sound("CaveDrip*");
}