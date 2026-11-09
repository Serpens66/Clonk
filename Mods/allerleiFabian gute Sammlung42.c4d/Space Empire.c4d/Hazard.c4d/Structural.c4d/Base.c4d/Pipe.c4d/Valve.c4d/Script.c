/*-- Ventil --*/

#strict 3

local open;

protected func Initialize() {
  open = nil;
  SetAction("Exist");
}

protected func ControlDig() {
  open = !open;
  UpdatePipesystem();
}

public func Status() { return(open); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (open)
		extra[(GetLength(extra) ?? 0)] = "ControlDig()";
}
