/*-- Fenster --*/

#strict

func Initialize()
{
	SetAction("Window1");
}

public func Set(int iDir)
{
	SetDir(iDir);
	SetSolidMask(70, 91 * iDir, 70, 91, 0,0);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d)", GetDir());
}
