/*-- Uplink --*/

#strict

// Local(0): Funktionsmodus:  0 = Basis,  1 = Handelssystem
// Local(1): noch zu vergebende "Heilpunkte", bis wieder Geld fällig wird


/* Timer-Call */

protected func CheckBuilding()
{
  // in einem Basisgebäude?
  var building = Contained();
  if (!building) return(1);
  if (!GetDefBase(GetID(building)))
    return(1);
  // schon ein Uplink oder Fahne drauf?
  if (FindObject(FLAG,0,0,0,0,0,"FlyBase",building)) return(1);
  if (FindObject(UPLB,0,0,0,0,0,"Turn",building)) return(1);
  if (FindObject(UPLT,0,0,0,0,0,"Turn",building)) return(1);
  // umwandeln & drauf damit
  SetOwner(GetOwner(), building);
  Sound("Trumpet");
  if (Local(0)==1) ChangeDef(UPLT);
              else ChangeDef(UPLB);
  // Initialisierungsaufruf
  this()->Init(building);
}

