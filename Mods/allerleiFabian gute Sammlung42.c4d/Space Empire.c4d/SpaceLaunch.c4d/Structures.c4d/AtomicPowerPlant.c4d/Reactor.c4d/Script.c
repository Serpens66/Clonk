/* Reaktor-Objekt */

#strict 3

local iResttime, iSwitch, PowerPlant;

public func IsWaste(){ return(50);}


/* Initialisierung */

protected func Initialize(...) {
  iResttime = nil;
  iSwitch = 1;
  return(1);
}

public func Init(...) {
  // wird vom Atomkraftwerk aufgerufen
  PowerPlant = Par(nil);
  return(1);
}


/* Ein-/Ausschalten */

public func SwitchOnOff(...) {
  // Ein/Ausschalten vom AKW aus
  iSwitch = !iSwitch;
  if (iSwitch) return(SwitchOn());
  return(SwitchOff());
}

private func SwitchOn(...) {
  Message("Reaktor ein",PowerPlant);
  Sound("Energize");
  return(1);
}

private func SwitchOff(...) {
  Message("Reaktor aus", PowerPlant);
  Sound("DeEnergize");
  return(1);
}


/* Energieproduktion */

private func Production(...) {
  // Update für Anzeige
  ShowInfoBar(IBRE, PowerPlant, (CurrentBoxValue() ?? 0)+(FullBoxesCount() ?? 0)*100, 1000, nil,1);
  // Überhaupt eingeschaltet?
  if (!iSwitch) return(1);
  // EMP-Einwirkung?
  if (FindObject(SEMP, nil,nil,nil,nil, nil,"Active",PowerPlant)) return(1);
  // Alles verbraucht?
  if (!iResttime)
    return(ProductionEnd());
  // Energieerzeugung
  PowerPlant->EnergyProduction();
  S42MigrationStep(iResttime, -1);
  // Warnungen, wenn Brennstäbe fast verbraucht
  if (FindContents(SBST,PowerPlant)) return(1);
  if (Equal(iResttime, 1000)) return(Message("@Wenig Material",PowerPlant));
  if (Equal(iResttime, 320))  return(Message("@Sehr wenig Material",PowerPlant));
  return(1);
}

private func ProductionEnd(...) {
  SetAction("Idle");
  if (FindContents(SBST,PowerPlant)) return(1);
  Message("Brennstäbe verbraucht!",PowerPlant);
  Sound("DeEnergize");
  Enter(Contained(),CreateObject(PLUT,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(AMWS,nil,nil,GetOwner()));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());

  return(1);
}


/* Inhaltsüberprüfung */

private func ContentsCheck(...) {
  // noch aktiv?
  if (!ActIdle()) return(1);
  // Update für Anzeige
  ShowInfoBar(IBRE, PowerPlant, (CurrentBoxValue() ?? 0)+(FullBoxesCount() ?? 0)*100, 1000, nil,1);
  // auch eingeschaltet?
  if (!iSwitch) return(1);
  // Brennstäbe finden
  var obj = FindContents(SBST,PowerPlant);
  if (!obj) return(1);
  // verbrauchen
  RemoveObject(obj);
  iResttime = 4800;
  SetAction("Process");
  return(1);
}


/* Energieanzeige */

public func ShowEnergyInfo(...) {
  // aktuelle Kiste
  ShowInfoBar(IBRE, PowerPlant, (CurrentBoxValue() ?? 0)+(FullBoxesCount() ?? 0)*100, 1000, 5,1);
  // Meldung löschen
  if (And(!FindContents(SBST,PowerPlant), ((iResttime ?? 0) <= 1000)))
    return(1);
  Message("", PowerPlant);
  return(1);
}


/* Infos */

public func CurrentBoxValue(...) { return((iResttime ?? 0)/48);
}
public func FullBoxesCount(...) {  return(ContentsCount(SBST,PowerPlant));
}

public func GetRestTime(...) { return(iResttime);
}
public func GetSwitch(...) { return(iSwitch);
}

