/*--- Saw ---*/

#strict 3

protected func Hit()
{
  Sound("MetalHit*");
	return(1);
}

public func Activate(caller) 
{
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Sound("Fuse");
  var helper = CreateObject(E28Z,nil,nil,GetOwner());
  SetYDir(-130,helper); 
  RemoveObject();
  return(1);
}

public func IsAnvilProduct() { return(1); }
