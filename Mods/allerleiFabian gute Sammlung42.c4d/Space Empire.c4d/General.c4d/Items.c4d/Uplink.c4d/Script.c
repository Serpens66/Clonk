/*-- Uplink --*/

#strict 3

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
  if (FindObject(FLAG,nil,nil,nil,nil,nil,"FlyBase",building)) return(1);
  if (FindObject(UPLB,nil,nil,nil,nil,nil,"Turn",building)) return(1);
  if (FindObject(UPLT,nil,nil,nil,nil,nil,"Turn",building)) return(1);
  // umwandeln & drauf damit
  SetOwner(GetOwner(), building);
  Sound("Trumpet");
  if (Equal(Local(nil), 1)) ChangeDef(UPLT);
              else ChangeDef(UPLB);
  // Initialisierungsaufruf
  this()->Init(building);
}

