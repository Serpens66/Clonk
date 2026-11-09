/*-- Stopper --*/

#strict 3

local vehicle, stopX, stopY;


/* Object-Call */

public func Init(par_veh)
{
  vehicle = par_veh;
  stopX = 1;
  stopY = 1;
}

public func DoStopX(...) { return(stopX);
}


/* Timer-Call */

protected func StopEngines()
{
  // neuer Befehl oder angehalten
  if (Or(FlightX(), !GetXDir(vehicle))) stopX = nil;
  if (Or(FlightY(), !GetYDir(vehicle))) stopY = nil;
  // anhalten
  if (stopX)  if ((GetXDir(vehicle) ?? 0) < 0) SetXDir((GetXDir(vehicle) ?? 0)+1, vehicle);
                                   else SetXDir((GetXDir(vehicle) ?? 0)-1, vehicle);
  if (stopY)  if ((GetYDir(vehicle) ?? 0) < 0) SetYDir((GetYDir(vehicle) ?? 0)+1, vehicle);
                                   else SetYDir((GetYDir(vehicle) ?? 0)-1, vehicle);
  // genug gebremst?
  if (And(!stopX, !stopY)) RemoveObject();
}


/* Hilfsfunktionen */

private func FlightX()
{
  var d = GetComDir(vehicle);
  if (And(((d ?? 0) >= 6), ((d ?? 0) <= 8))) return(-1);
  if (And(((d ?? 0) >= 2), ((d ?? 0) <= 4))) return(+1);
  return(nil);
}

private func FlightY()
{
  var d = GetComDir(vehicle);
  if (And(((d ?? 0) >= 1), ((d ?? 0) <= 2))) return(-1);
  if (Equal(d, 8))               return(-1);
  if (And(((d ?? 0) >= 4), ((d ?? 0) <= 6))) return(+1);
  return(nil);
}

