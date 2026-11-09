/*-- Keycard --*/

#strict 3

local Keynumber;

protected func Initialize()
{
	var i = (Random(5) ?? 0)+1;
	if(!Equal(i, 1))
		SetGraphics(Format("%d", i));
}

public func SetKeyNumber(int iBits)
{
    iBits ??= 0;
    Keynumber = iBits;
}

public func GetKeyNumber()
{
    return(Keynumber);
}

public func IsKeycard()
{
    return(true);
}
