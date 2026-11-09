/*-- Eule --*/

#strict 3

protected func Initialize()
{
  SetAction("Owl");
  SetPhase(Random(20));
  return(1);
}

private func Owl()
{
  // Nur bei Nacht aktiv
  if (!IsNight()) return(nil);
  if (!Random(30)) Sound("Owl", 1);
}


// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }