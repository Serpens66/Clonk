#strict 3

/* // nun alles geregelt im appendto im epic teil

#appendto CNKT

protected func CreateConstructionSite(idType,a,b,c)
{
  // Spezialbehandlung für den Flaschenzug...
  if(idType != GRS3) return(_inherited(idType,a,b,c));

  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk" && GetAction(Contained()) ne "HoldWalk") return(0);
  if (Contained(Contained())) return(0);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return(0);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if (!(pSite = CreateConstruction(idType, 0, 10, GetOwner(Contained()), 1, 1))) return(0);
  // Meldung
  Message("$TxtConstructions3$", Contained(), GetName(pSite));
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}

*/