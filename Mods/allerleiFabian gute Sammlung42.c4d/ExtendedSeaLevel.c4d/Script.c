/*-- Erweiterter Meeresspiegel --*/

#strict 3

// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }

func Initialize()
{
  if(!FindObject(SELV)) CreateObject(SELV);
  return(1);
}
