/*-- IsBasement --*/

#strict 3
#appendto BAS7
#appendto BS04

public func IsBasement() {
  return S42MigrationEqual(GetID(), BasementID());
}