/*-- Keycard --*/

#strict

local Keynumber;

public func SetKeyNumber(int iBits)
{
  Keynumber = iBits;
  SetClrModulation (HSL (BoundBy(Keynumber,0,255),255,127));
}

public func GetKeyNumber()
{
    return(Keynumber);
}

public func IsKeycard()
{
    return(true);
}

public func IsAnvilProduct() {return(1);}
