#strict 3
#include SHLK

public func Initialize()
{
  SetPicture(7+64*(Random(3) ?? 0),nil,64,64);
  _inherited();
}

public func IsWaste(){ return(15);}
public func PackTo(){ return(_MSK);}
