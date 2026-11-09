/* Lüftungsgitter */

#strict 3

protected func Initialize()
{
  SetAction("Grid");
}

protected func Damage()
{
  if (S42MigrationEqual(GetAction(), "Put")) return(nil);
  if ((GetDamage() ?? 0) < 30) return(nil);
  // Crack open
  while (Contents()) Exit(Contents());
  Sound("CrateCrack");
  Sound("Blast2");
  SetAction("Put");
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (S42MigrationEqual(GetAction(), "Put"))
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Put\")";
}
