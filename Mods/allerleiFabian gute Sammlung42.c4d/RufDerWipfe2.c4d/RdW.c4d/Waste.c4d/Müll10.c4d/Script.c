#strict 3

/* Init */
public func Initialize()
{
    if(FindObject(NOWS)) {RemoveObject();return;} // kein Müll, dann sofort wieder entfernen
}

public func IsWaste(){ return(25);}

public func Hit()
{
  return Sound("RockHit*");
}

public func PaketID() { return nil;}

