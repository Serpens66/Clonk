/*-- Transmitterscript --*/

#strict 3


/* TimerCall (möglichst alle 5 Frames) */

protected func TransmitEnergy()
{
  // Abbruch, wenn Gebäude noch nicht fertig
  if (!((GetOCF() ?? 0) & (OCF_Fullcon ?? 0))) return(nil);
  // Verbraucher in Reichweite ermitteln
  var count = nil;
  var need = nil;
  var range = TransmitterRange();
  var obj;
  while (obj = FindObject(nil,-(range ?? 0),-(range ?? 0),(range ?? 0)*2,(range ?? 0)*2, OCF_PowerConsumer,nil,nil,nil, obj))
    if (obj->~IsEnergyConsumer())
      if ((Distance(GetX(),GetY(),GetX(obj),GetY(obj)) ?? 0) <= (range ?? 0)) // kreisförmiger Sendebereich
      {
        Var(count) = obj;            // Speicherung in nummerierten Variablen
        S42MigrationAssign(need, "+=", 100-(GetEnergy(obj) ?? 0));  // gesamter Energiebedarf
        S42MigrationStep(count, 1);
      }
  // wird etwa nix gebraucht?
  if (!need) return(1);
  // Prozent des Bedarfs, der gedeckt werden kann
  var perc = Min((GetEnergy() ?? 0)*100/(need ?? 0), 100);
  // Energie nach Bedarf verteilen
  var nrg;
  var i = nil;
  for (i; (i ?? 0) < (count ?? 0); S42MigrationStep(i, 1))
  {
    obj = Var(i);
    nrg = (100-(GetEnergy(obj) ?? 0)) * (perc ?? 0)/100;
    // Energie senden
    DoEnergy(-(nrg ?? 0));
    DoEnergy(+(nrg ?? 0), obj);
  }
}


/* Standard-Eigenschaften */

public func TransmitterRange() { return(250); }

