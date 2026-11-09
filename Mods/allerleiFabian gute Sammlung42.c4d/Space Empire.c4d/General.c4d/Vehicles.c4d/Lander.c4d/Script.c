/*-- Flugzeug-Lander --*/

#strict

local vehicle, scanY;


public func Init(par_vehicle, posY)
{
  vehicle = par_vehicle;
  scanY = posY;
}

protected func ResetPosition()
{
  // Position an Shuttle anpassen
  SetPosition(GetX(vehicle)+GetXDir(vehicle), GetY(vehicle)+GetYDir(vehicle)+scanY);
  // Berührung mit dem Boden?
  if (Stuck())
    vehicle->~DoLand();
}

