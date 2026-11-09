/*-- Super-FeuerFlint --*/

#strict

local pProduced;

/* Fertigstellung */
protected func Initialize()
{
  // Wurden wir produziert? Dann gibts das leere Ölfass zurück
  if(pProduced && Contained() == pProduced)
    CreateContents(BARL, pProduced);
}

/* Aufschlag */

protected func Hit()
{
  CastObjects(DFLM, 20, 30);
  Sound("Inflame");
  Sound("Fuse");
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(30);
  else FakeExplode(30,0,0,30);
  RemoveObject();
  return(1);
}

/* Produktion */
public func OnStartProduction(pChemicalPlant)
{
  pProduced = pChemicalPlant;
}

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Forschung */

public func GetResearchBase() { return(SFLN); }
