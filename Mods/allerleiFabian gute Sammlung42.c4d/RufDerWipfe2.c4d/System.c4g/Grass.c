/*-- Grass --*/

#strict
#appendto GRAS

public func Initialize()
{
  if(GetID()==GRAS) ChangeDef(GRA1);
  return _inherited();
}
