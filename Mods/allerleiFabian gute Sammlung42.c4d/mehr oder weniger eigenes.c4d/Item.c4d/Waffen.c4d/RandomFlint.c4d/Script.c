/*-- Random-Flint --*/

#strict

#include FLNT

protected func Hit()
{
var boom;
boom = Random(20);
if (boom >= 15) {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(Random(80)+10); else FakeExplode(Random(80)+10,0,0,Random(80)+10);}
if (boom <= 3) {FakeExplode(45);}
}


public func IsChemicalProduct() { return(1); }

func IsAlchemContainer() { return(true); }

public func GetResearchBase() { return(SFLN); }
