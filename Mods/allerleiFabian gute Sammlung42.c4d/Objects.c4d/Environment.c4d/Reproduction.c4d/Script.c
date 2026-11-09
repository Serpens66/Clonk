/*-- Nachwuchs --*/

#strict 3

protected func Initialize()
{
  // Tierbestand verzögert speichern (für Tiere, die erst im Szenarienscript erzeugt werden)
  ScheduleCall(this(), "StoreAnimals", 5);
}

// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }

public func StoreAnimals()
{
  // Tierbestand speichern
  var pAnimal;
  while (pAnimal = FindObject(nil, nil, nil, nil, nil, OCF_Living, nil, nil, nil, pAnimal))
    if (~(GetOCF(pAnimal) ?? 0) | (OCF_CrewMember ?? 0))
      if (GetAlive(pAnimal))
        Add(GetID(pAnimal));  
  return(1);
}

protected func ReproductionControl()
{
  // Bestand kontrollieren
  var i = -1;
  var idAnimal;
  while (idAnimal = Get(nil, S42MigrationStep(i, 1)))
    if ((ObjectCount(idAnimal) ?? 0) < (Get(idAnimal) ?? 0) )
      if(!Random( 23 )) 
        Reproduce(idAnimal);
}

private func Reproduce(idAnimal)
{
  SetPosition(Random(LandscapeWidth()), Random(LandscapeHeight()));
  var pAnimal;
  while (pAnimal = FindObject(idAnimal, nil, nil, -1, -1, nil, nil, nil, nil, pAnimal))
    if (Or(ObjectCall(pAnimal, "Reproduction", 1), (ObjectCount(idAnimal) ?? 0) >= (Get(idAnimal) ?? 0)))
      return(1);
  return(nil);
}

/* ID-Listenverwaltung (Components) */       

public func Set(idc, iCount)
{
  return(SetComponent(idc, iCount));
}
  
public func Get(idc, iIndex)
{
  return(GetComponent(idc, iIndex));
}
  
public func Add(idc, iCount)
{
  return(Set(idc, (Get(idc) ?? 0) + (Max(iCount, 1) ?? 0)));
}
  
public func Sub(idc, iCount)
{
  return(Set(idc, (Get(idc) ?? 0) - (Max(iCount, 1) ?? 0)));
}
