/*-- Fenster --*/

#strict 3

func Initialize()
{
	SetAction("Window1");
}

public func Set(int iDir)
{
	iDir ??= 0;
	SetDir(iDir);
	SetSolidMask(70, 91 * (iDir ?? 0), 70, 91, nil,nil);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d)", GetDir());
}
