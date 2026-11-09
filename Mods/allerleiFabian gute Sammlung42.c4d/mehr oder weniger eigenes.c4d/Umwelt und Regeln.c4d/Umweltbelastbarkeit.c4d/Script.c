/*-- Umweltstärke --*/

#strict

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition(1,1);
  return(1);
}

//Rest wird im Effektobjekt "Umweltschutz" von Ruf der Wipfe Remake gemacht
// Dort wird es nicht mehr über die Anzahl der Regel, sondern über 
// HashPut(hashSpielRegeln, "Umweltbelastbarkeit",1500);  gemacht, der wert wird im Regelchooser gesetzt

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}
// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }