/*-- Rittersattel --*/

#strict 3

private func ID_Horse() { return(HORS); }
private func ID_HorseSaddled() { return(HRSK); }

public func Activate(object pClonk)
{
  [$TxtPuton$]
  // Kein Pferd
  var pHorse;
  if (!(pHorse = FindObject(ID_Horse(), nil, 1))) return(nil);
  // Pferd satteln
  if (!pHorse->Saddle(ID_HorseSaddled(), GetID())) return(nil);
  pHorse->SetOwner(GetOwner(pClonk));
  // Sattel entfernen
  RemoveObject();
  // Fertig
  return(1);
}

/* Eigenschaften */

public func IsAnvilProduct() { return(1); }

public func IsSaddle() { return(1); }