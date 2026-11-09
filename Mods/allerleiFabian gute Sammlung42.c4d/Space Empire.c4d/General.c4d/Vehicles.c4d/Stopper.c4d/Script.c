/*-- Stopper --*/

#strict

local vehicle, stopX, stopY;


/* Object-Call */

public func Init(par_veh)
{
  vehicle = par_veh;
  stopX = 1;
  stopY = 1;
}

public DoStopX: return(stopX);


/* Timer-Call */

protected func StopEngines()
{
  // neuer Befehl oder angehalten
  if (FlightX() || !GetXDir(vehicle)) stopX = 0;
  if (FlightY() || !GetYDir(vehicle)) stopY = 0;
  // anhalten
  if (stopX)  if (GetXDir(vehicle) < 0) SetXDir(GetXDir(vehicle)+1, vehicle);
                                   else SetXDir(GetXDir(vehicle)-1, vehicle);
  if (stopY)  if (GetYDir(vehicle) < 0) SetYDir(GetYDir(vehicle)+1, vehicle);
                                   else SetYDir(GetYDir(vehicle)-1, vehicle);
  // genug gebremst?
  if (!stopX && !stopY) RemoveObject();
}


/* Hilfsfunktionen */

private func FlightX()
{
  var d = GetComDir(vehicle);
  if ((d >= 6) && (d <= 8)) return(-1);
  if ((d >= 2) && (d <= 4)) return(+1);
  return(0);
}

private func FlightY()
{
  var d = GetComDir(vehicle);
  if ((d >= 1) && (d <= 2)) return(-1);
  if (d == 8)               return(-1);
  if ((d >= 4) && (d <= 6)) return(+1);
  return(0);
}

