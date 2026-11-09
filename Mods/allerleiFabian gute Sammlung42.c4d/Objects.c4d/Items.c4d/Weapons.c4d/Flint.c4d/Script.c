/*--- Flint ---*/

#strict 3

public func ExplodeSize() 
{ 
  return(18); 
}

protected func Hit()
{ 
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()) ))
  Explode(ExplodeSize());
else FakeExplode(ExplodeSize(),nil,nil,ExplodeSize());
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }