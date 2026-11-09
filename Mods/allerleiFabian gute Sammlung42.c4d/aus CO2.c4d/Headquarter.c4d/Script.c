/*-- Headquarter--*/

#strict 3

#include BP5C // Burgteilfunktion
#include DOOR // Tuersteuerung

/* Initialisierung */

protected func Initialize()
{
  // Verzögerte Initialisierung
  SetAction("Initialization");
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  return(_inherited() );
}

private func Initialize2()
{
}


/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSCC); }


/* Forschung */

public func GetResearchBase() { return(CPW2); }
