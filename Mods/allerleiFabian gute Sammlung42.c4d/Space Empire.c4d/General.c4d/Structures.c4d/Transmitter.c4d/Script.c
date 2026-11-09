/*-- Transmitterscript --*/

#strict


/* TimerCall (möglichst alle 5 Frames) */

protected func TransmitEnergy()
{
  // Abbruch, wenn Gebäude noch nicht fertig
  if (!(GetOCF() & OCF_Fullcon())) return(0);
  // Verbraucher in Reichweite ermitteln
  var count = 0;
  var need = 0;
  var range = TransmitterRange();
  var obj;
  while (obj = FindObject(0,-range,-range,range*2,range*2, OCF_PowerConsumer(),0,0,0, obj))
    if (obj->~IsEnergyConsumer())
      if (Distance(GetX(),GetY(),GetX(obj),GetY(obj)) <= range) // kreisförmiger Sendebereich
      {
        Var(count) = obj;            // Speicherung in nummerierten Variablen
        need += 100-GetEnergy(obj);  // gesamter Energiebedarf
        ++count;
      }
  // wird etwa nix gebraucht?
  if (!need) return(1);
  // Prozent des Bedarfs, der gedeckt werden kann
  var perc = Min(GetEnergy()*100/need, 100);
  // Energie nach Bedarf verteilen
  var nrg;
  var i = 0;
  for (i; i < count; ++i)
  {
    obj = Var(i);
    nrg = (100-GetEnergy(obj)) * perc/100;
    // Energie senden
    DoEnergy(-nrg);
    DoEnergy(+nrg, obj);
  }
}


/* Standard-Eigenschaften */

public func TransmitterRange() { return(250); }

