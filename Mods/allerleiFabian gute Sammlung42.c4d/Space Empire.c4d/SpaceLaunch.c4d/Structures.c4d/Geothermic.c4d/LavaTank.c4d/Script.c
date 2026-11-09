/*-- LavaTank-Objekt --*/

#strict

local iFill, iSwitch, GeoPlant;


/* Initialisierung */

protected func Initialize()
{
  iFill = 300;  // ein bisschen Füllung für den Anfang
  iSwitch = 0;  // ausschalten
  return(1);
}

public func Init(plant)
{
  // Wird vom Geothermalwerk aufgerufen
  GeoPlant = plant;
  SetAction("Process",plant);
  return(1);
}


/* Füllstand */

public GetFill: return(iFill);
public GetMaxFill: return(4000);   // 20 Fässer voll


/* Füllstand ändern */

public func DoFill(plus)
{
  var fill0 = iFill;
  iFill = BoundBy(iFill+plus, 0, GetMaxFill());
  return(iFill != fill0);
}


/* Ein-/Ausschalter */

public SwitchOnOff:
  // Ein/Ausschalten vom Geothermalwerk aus
  iSwitch = !iSwitch;
  if (iSwitch) return(SwitchOn());
  return(SwitchOff());

private SwitchOn:
  Message("eingeschaltet",GeoPlant);
  Sound("Energize");
  SetAction("Process");
  return(1);

private SwitchOff:
  Message("ausgeschaltet",GeoPlant);
  Sound("DeEnergize");
  return(1);


/* Inhalts- und Produktions-Überwachung */

public CheckOn:
  // EMP-Einwirkung?
  if (HasEMP(GeoPlant)) return(1);
  // weiter nur jedes dritte Mal
  if (GetPhase() % 3) return(1);
  // Energieerzeugung
  if (iFill && iSwitch)
    if (GetEnergy(GeoPlant) < 50)   // wird auch was gebraucht?
    {
      DoFill(-3);
      GeoPlant->EnergyProduction();
    }
  // ausgeschaltet
  if (!iSwitch)
    SetAction("Off");
  return(1);

private func GetFromBarrel(barrelID)
{
  var obj, barrelfill;
  var maxfill = GetMaxFill();
  while ((obj = FindObject(barrelID,0,0,0,0, 0,0,0,GeoPlant,obj)) && (iFill < maxfill))
  {
    // Fassinhalt ermitteln, nicht mehr als Restplatz im Tank
    barrelfill = BoundBy(obj->GetAmount(), 0, maxfill-iFill);
    // aus dem Fass nehmen und in den Tank füllen
    obj->BarrelDoFill(-barrelfill);
    DoFill(barrelfill);
  }
  return(1);
}

protected func CheckFill()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(GeoPlant)),GeoPlant);
  // aus DuroLava- und Lava-Fässern holen
  GetFromBarrel(DBRL);
  GetFromBarrel(LBRL);
  // Füllstand anzeigen
  var newDir = (iFill+200)*9 / GetMaxFill();
  if (GetDir() != newDir)
    SetDir(newDir);
  return(1);
}

