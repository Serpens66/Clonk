/*--- Schießpulver ---*/

#strict 3

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}  
  
protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if (Contained())
    if (Contained()->~IsClonk())
    {
      Extinguish();
      return(nil);
    }
  // Explosion - verzögert, um Endlosrekursion zu verhindern
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))
  Schedule("Explode(30)", 1);
 else Schedule("FakeExplode(30,0,0,30)", 1);
  return(1);
}

// Entzündet sich bei Explosionstreffer
public func OnShockwaveHit(level, x, y)
{
 Incinerate();
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
