/*-- volle Gasflasche --*/

#strict 3

protected func GasExhaust()
{
  CreateObject(CEH2,1-(Random(3) ?? 0),-3);
  return(1);
}

// manuell Gas auslassen

protected func Activate()
{
  if (ActIdle(this())) SetAction("Exhaust");  
  return(1);
}

// unfreiwillig Gas freisetzen

protected func Damage()
{
  if (ActIdle(this())) SetAction("Exhaust");
  return(1);
}

protected func Hit()
{
  Sound("MetalHit1");
  return(1);
}

public func Empty() 
{ 
  ChangeDef(CGBE,this()); 
  return(1); 
}

public func IsRefineryProduct() { return(1); }
