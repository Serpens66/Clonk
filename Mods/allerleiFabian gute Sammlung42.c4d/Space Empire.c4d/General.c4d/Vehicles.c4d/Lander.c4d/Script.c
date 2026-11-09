/*-- Flugzeug-Lander --*/

#strict 3

local vehicle, scanY;


public func Init(par_vehicle, posY)
{
  vehicle = par_vehicle;
  scanY = posY;
}

protected func ResetPosition()
{
  // Position an Shuttle anpassen
  SetPosition((GetX(vehicle) ?? 0)+(GetXDir(vehicle) ?? 0), (GetY(vehicle) ?? 0)+(GetYDir(vehicle) ?? 0)+(scanY ?? 0));
  // Berührung mit dem Boden?
  if (Stuck())
    vehicle->~DoLand();
}

