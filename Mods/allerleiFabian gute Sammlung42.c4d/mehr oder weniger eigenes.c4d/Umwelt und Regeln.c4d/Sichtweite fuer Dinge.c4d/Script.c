/*-- Sichtweite fuer Dinge --*/

#strict


func Initialize()
{
SetPosition(1,1);
}

// beeinflusst den Rückgabe Wert von FogViewRange(), welches grßtenteils in Systemskripten den Objekten ihren sichtradius noch vollendung zuteilt. Die fkt wird auch in Lichtobjekten verwendet, aber die geben immer sichtradius, auch ohne diese Regel 
func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }