/*-- Verbrannter Fahrstuhlkorb --*/

#strict 3

/* Status */

private func Elevator()
{
  return (FindObject(nil, nil, nil, nil, nil, nil, "LiftCase", this()));
}

/* Zerstörung: Reduzieren des Fahrstuhls für Wiederaufbau/Reparatur */

protected func Incineration(iCausedBy) { return(ObjectCall(Elevator(), "RemoveCase")); }
protected func IncinerationEx(iCausedBy) { return(ObjectCall(Elevator(), "RemoveCase")); }
