/*-- Keycard --*/

#strict 3

local Keynumber;

public func SetKeyNumber(int iBits)
{
  iBits ??= 0;
  Keynumber = iBits;
  SetClrModulation (HSL (BoundBy(Keynumber,nil,255),255,127));
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
