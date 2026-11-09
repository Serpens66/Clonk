/*-- Verbrannter Geschützturm --*/

#strict 3

local cannon;

#include BAS2

public func ReleaseCannon()
{
  if(!cannon) return(nil);
  cannon->ComStopDouble();
  cannon = nil;
}

/* Destruction */

protected func Destruction()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

protected func Incineration()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}
