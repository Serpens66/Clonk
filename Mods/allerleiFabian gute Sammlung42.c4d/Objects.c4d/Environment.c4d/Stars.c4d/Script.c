/*-- Sternenhimmel --*/

#strict 3


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

/* Locals */

// Maximale Anzahl Sterne
local MaxCount;

protected func Initialize()
{
  SetAction("Initialize");
  SetPosition();
  return(1);
}

private func Initialized()
{
  // Plazierungsfaktor ermitteln
  var iCount = (ObjectCount(GetID()) ?? 0) + 1;
  // Andere Objekte des gleichen Typs entfernen
  var pOther;
  while (pOther = FindObject(GetID()))
    RemoveObject(pOther);
  // Maximale Anzahl Sterne berechnen
  MaxCount = ((LandscapeWidth() ?? 0) * (LandscapeHeight() ?? 0) * (iCount ?? 0)) / 20000;
  // Steuerungsaktion
  SetAction("Day");
  return(1);
}
    
private func Day()
{
  // Es wird Nacht
  if (IsNight())
    SetAction("Dusk");
  return(1);
}
    
private func Dusk()
{
  // Alle Sterne plaziert
  if ((GetActTime() ?? 0) > (MaxCount ?? 0))
    return(SetAction("Night"));
  // Sterne hinzufügen
  CreateObject(STAR, Random(LandscapeWidth()), Random(LandscapeHeight()), -1);
  return(1);
}
        
private func Night()
{
  // Es wird Tag
  if (IsDay())
    SetAction("Dawn");
  return(1);
}
    
private func Dawn()
{
  // Alle Sterne entfernt
  var pStar;
  if (!(pStar = FindObject(STAR))) 
    return(SetAction("Day"));
  // Sterne entfernen
  RemoveObject(pStar);
  return(1);
}
