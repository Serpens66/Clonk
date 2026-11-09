/*--- Flammenwerfer ---*/

#strict 3

#include CTW3

protected func ConnectID() { return(ACT4); }

/* Blitzumwandlung */

public func LightningStrike()
{
  // Nur wenn fertig gebaut
  if (LessThan(GetCon(),100)) return(nil);
  // Energiegewinnung
  CreateContents(CRYS);
  // Sound
  Sound("Electric");
}

