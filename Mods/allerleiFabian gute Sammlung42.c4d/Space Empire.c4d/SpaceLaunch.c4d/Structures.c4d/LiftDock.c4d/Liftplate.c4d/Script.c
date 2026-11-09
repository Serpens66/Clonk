/*-- LiftPlate --*/

#strict
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
  for (i = 0; i < 5; ++i)
  {
    obj = CreateObject(SFRX, Random(50)-25, Random(8), GetOwner());
    obj->Settings(i);
  }
  // Explosion
  Explode(20);
  return(1);
}


/* Eigenschaften */

public GetMaxDamage: return(60);
public Repairable: return(1);

