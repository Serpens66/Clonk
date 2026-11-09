/*-- Treibholz --*/

#strict 3

// Maximale Treibholzmenge
local MaxAmount;


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

protected func Initialize()
{
  SetAction("Initialize");
  SetPosition(50,60);
  return(1);
}
  
/* Verzögerte Initialisierung */  
  
private func Initialized()
{
  // Maximale Treibholzmenge richtet sich nach der
  // Anzahl der platzierten Treibholzobjekte x 10 + 10
  MaxAmount = (ObjectCount(GetID(this())) ?? 0) * 10 + 10;
  // Alle übrigen Steuerungsobjekte dieser Art entfernen
  var pOther;
  while (pOther = FindObject(GetID(this())))
    RemoveObject(pOther);
  // Steuerungsaktion
  SetAction("Driftwood");
}
  
/* Treibholz plazieren */  
  
private func PlaceDriftwood()
{
  // Zufallssteuerung
  if (Random(30)) return(nil);
  // Nicht zu viel Treibholz gleichzeitig in der Landschaft (im Freien)
  if ((ObjectCount(WOOD) ?? 0) > (MaxAmount ?? 0)) return(nil);
  // Position ermitteln
  var x,y;
  // X-Position
  x = Random(LandscapeWidth());
  // Überprüfen, ob an dieser Position Flüssigkeit ist
  while (And(((y ?? 0) < (LandscapeHeight() ?? 0)), !GBackSemiSolid(x, y)))
    S42MigrationAssign(y, "+=", 20);
  // Keine Flüssigkeit
  if (!GBackLiquid(x, y)) return(nil);
  // Treibholz erzeugen
  SetR(Random(360), CreateObject(WOOD, x, y, -1));
}
