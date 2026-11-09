/*-- Steinschlag --*/

#strict 3

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  return(1);
}

/* Eventuell Steinschlag erstellen */
private func DoRocks()
{
  if (Random(150)) return(1);
  var pRock = CreateObject(RCK2, Random(LandscapeWidth()), nil, -1);
  SetXDir(25 - (Random(51) ?? 0), pRock);
  SetYDir(20 - (Random(16) ?? 0), pRock);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }