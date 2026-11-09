/*-- Nebelerzeuger alles x Frames erzeugen --*/

#strict 3

local windvorherr;


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
// public func IsVariabelChooseable2() { return(1); }      // nebel sorgt fuer zuviel lagg... 

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  return(1);
}


/* Eventuell Nebel erzeugen */
private func DoNebel()
{
if (!Random(3)) // mit einer chance von 1 zu 3 werden alle 20.000Frames zwischen 1 und 6 Nebelobjekte erstellt 
  {
  if((ObjectCount2(Find_ID(NBL4)) ?? 0)>=1) RemoveAll(NBL4);
  if((ObjectCount2(Find_ID(NBL2)) ?? 0)>=1) RemoveAll(NBL2);
if((ObjectCount2(Find_ID(NBL1)) ?? 0)>=1) RemoveAll(NBL1);
CreateObject(NBL1);
Log("Nebelerzeuger und frame-Entferner erzeugt");
CreateObject(NBL4);
}
return(1);
}



func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

