/*-- Speer --*/

#strict 3

/* Status */
public func IsSpear() 
{ 
  return(1); 
}

public func CarryLimit()
{
  // Nur eines tragbar
  return(1);
}

public func Entrance()
{
  // Damit es die richtige Austrittsrotation annimmt
  SetRDir();
}

/* Werfen */
public func ControlThrow()
{
  // Träger ermitteln
  var pClonk = Contained();
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(nil);
  // Träger bereit zum Werfen?
  if (And(And(And(!SEqual(GetAction(pClonk), "Walk"), !SEqual(GetAction(pClonk), "Jump")), !SEqual(GetAction(pClonk), "Ride")), !SEqual(GetAction(pClonk), "RideStill"))) return;
  // Beim Träger Wurfaktion setzen (reitend)
  if (Contained()->~IsRiding())
    return(ObjectSetAction(Contained(), "RideThrowSpear"));
  // Beim Träger Wurfaktion setzen (normal)
  if (!ObjectSetAction(Contained(), "ThrowSpear")) return(nil);
  // Fertig
  return(1);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }

