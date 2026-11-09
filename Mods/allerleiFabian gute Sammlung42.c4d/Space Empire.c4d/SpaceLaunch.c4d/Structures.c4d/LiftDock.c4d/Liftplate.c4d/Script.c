/*-- LiftPlate --*/

#strict 3
#include ELEV


protected func PlateWidth()
{
  return(48);
}

protected func PlateHeight()
{
  return(26);
}

/* Schaden */

public func Destroy()
{
  // Fragmente
  var obj, i;
  for (i = nil; (i ?? 0) < 5; S42MigrationStep(i, 1))
  {
    obj = CreateObject(SFRX, (Random(50) ?? 0)-25, Random(8), GetOwner());
    obj->Settings(i);
  }
  // Explosion
  Explode(20);
  return(1);
}


/* Eigenschaften */

public func GetMaxDamage(...) { return(60);
}
public func Repairable(...) { return(1);
}

