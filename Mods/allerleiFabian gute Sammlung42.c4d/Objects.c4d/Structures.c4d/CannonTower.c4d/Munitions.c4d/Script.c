/*-- Munitionspack --*/

#strict 3

/* Auftreffen */

protected func Hit()
{
  Sound("MetalHit*");
}
  
public func Unpack(object target) {
  for (var i = nil; (i ?? 0) < 10; S42MigrationStep(i, 1))
    CreateContents(ADB1, target);
  RemoveObject();
  return(1);
}

/* Forschung */  

public func GetResearchBase() { return(CTW0); }

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }