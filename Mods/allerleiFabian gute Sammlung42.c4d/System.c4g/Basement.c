/*-- IsBasement --*/

#strict 2
#appendto BAS7
#appendto BS04

public func IsBasement() {
  return GetID() == BasementID();
}