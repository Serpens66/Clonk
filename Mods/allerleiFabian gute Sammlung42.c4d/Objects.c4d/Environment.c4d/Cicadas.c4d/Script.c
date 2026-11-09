/*-- Zikaden --*/

#strict 3

protected func Initialize()
{
  SetAction("Cicadas");
  SetPhase(Random(20));
  return(1);
}

private func Cicadas()
{
  // Nur bei Nacht aktiv
  if (!IsNight()) return(nil);
  if (!Random(20)) Sound("Cicada*",1);
}


// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }