/*-- Grass --*/

#strict 3
#appendto GRAS

public func Initialize()
{
  if(Equal(GetID(), GRAS)) ChangeDef(GRA1);
  return _inherited();
}
