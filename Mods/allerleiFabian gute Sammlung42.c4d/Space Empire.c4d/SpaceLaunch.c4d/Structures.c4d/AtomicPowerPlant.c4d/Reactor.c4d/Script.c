/* Reaktor-Objekt */

#strict

local iResttime, iSwitch, PowerPlant;

public func IsWaste(){ return(50);}


/* Initialisierung */

protected Initialize:
  iResttime = 0;
  iSwitch = 1;
  return(1);

public Init:
  // wird vom Atomkraftwerk aufgerufen
  PowerPlant = Par(0);
  return(1);


/* Ein-/Ausschalten */

public SwitchOnOff:
  // Ein/Ausschalten vom AKW aus
  iSwitch = !iSwitch;
  if (iSwitch) return(SwitchOn());
  return(SwitchOff());

private SwitchOn:
  Message("Reaktor ein",PowerPlant);
  Sound("Energize");
  return(1);

private SwitchOff:
  Message("Reaktor aus", PowerPlant);
  Sound("DeEnergize");
  return(1);


/* Energieproduktion */

private Production:
  // Update für Anzeige
  ShowInfoBar(IBRE, PowerPlant, CurrentBoxValue()+FullBoxesCount()*100, 1000, 0,1);
  // Überhaupt eingeschaltet?
  if (!iSwitch) return(1);
  // EMP-Einwirkung?
  if (FindObject(SEMP, 0,0,0,0, 0,"Active",PowerPlant)) return(1);
  // Alles verbraucht?
  if (!iResttime)
    return(ProductionEnd());
  // Energieerzeugung
  PowerPlant->EnergyProduction();
  --iResttime;
  // Warnungen, wenn Brennstäbe fast verbraucht
  if (FindContents(SBST,PowerPlant)) return(1);
  if (iResttime==1000) return(Message("@Wenig Material",PowerPlant));
  if (iResttime==320)  return(Message("@Sehr wenig Material",PowerPlant));
  return(1);

private ProductionEnd:
  SetAction("Idle");
  if (FindContents(SBST,PowerPlant)) return(1);
  Message("Brennstäbe verbraucht!",PowerPlant);
  Sound("DeEnergize");
  Enter(Contained(),CreateObject(PLUT,0,0,GetOwner()));
  Enter(Contained(),CreateObject(AMWS,0,0,GetOwner()));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());

  return(1);


/* Inhaltsüberprüfung */

private ContentsCheck:
  // noch aktiv?
  if (!ActIdle()) return(1);
  // Update für Anzeige
  ShowInfoBar(IBRE, PowerPlant, CurrentBoxValue()+FullBoxesCount()*100, 1000, 0,1);
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


/* Energieanzeige */

public ShowEnergyInfo:
  // aktuelle Kiste
  ShowInfoBar(IBRE, PowerPlant, CurrentBoxValue()+FullBoxesCount()*100, 1000, 5,1);
  // Meldung löschen
  if (!FindContents(SBST,PowerPlant) && (iResttime <= 1000))
    return(1);
  Message("", PowerPlant);
  return(1);


/* Infos */

public CurrentBoxValue: return(iResttime/48);
public FullBoxesCount:  return(ContentsCount(SBST,PowerPlant));

public GetRestTime: return(iResttime);
public GetSwitch: return(iSwitch);

