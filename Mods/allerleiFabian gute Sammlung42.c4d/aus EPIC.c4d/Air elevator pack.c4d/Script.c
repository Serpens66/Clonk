/*--- Saw ---*/

#strict

protected func Hit()
{
  Sound("MetalHit*");
	return(1);
}

public func Activate(caller) 
{
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Sound("Fuse");
  var helper = CreateObject(E28Z,0,0,GetOwner());
  SetYDir(-130,helper); 
  RemoveObject();
  return(1);
}

public func IsAnvilProduct() { return(1); }
