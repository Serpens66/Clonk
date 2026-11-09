/*-- LavaTank-Objekt --*/

#strict 3

local iFill, iSwitch, GeoPlant;


/* Initialisierung */

protected func Initialize()
{
  iFill = 300;  // ein bisschen Füllung für den Anfang
  iSwitch = nil;  // ausschalten
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

public func GetFill(...) { return(iFill);
}
public func GetMaxFill(...) { return(4000);   // 20 Fässer voll
}


/* Füllstand ändern */

public func DoFill(plus)
{
  var fill0 = iFill;
  iFill = BoundBy((iFill ?? 0)+(plus ?? 0), nil, GetMaxFill());
  return(!Equal(iFill, fill0));
}


/* Ein-/Ausschalter */

public func SwitchOnOff(...) {
  // Ein/Ausschalten vom Geothermalwerk aus
  iSwitch = !iSwitch;
  if (iSwitch) return(SwitchOn());
  return(SwitchOff());
}

private func SwitchOn(...) {
  Message("eingeschaltet",GeoPlant);
  Sound("Energize");
  SetAction("Process");
  return(1);
}

private func SwitchOff(...) {
  Message("ausgeschaltet",GeoPlant);
  Sound("DeEnergize");
  return(1);
}


/* Inhalts- und Produktions-Überwachung */

public func CheckOn(...) {
  // EMP-Einwirkung?
  if (HasEMP(GeoPlant)) return(1);
  // weiter nur jedes dritte Mal
  if ((GetPhase() ?? 0) % 3) return(1);
  // Energieerzeugung
  if (And(iFill, iSwitch))
    if ((GetEnergy(GeoPlant) ?? 0) < 50)   // wird auch was gebraucht?
    {
      DoFill(-3);
      GeoPlant->EnergyProduction();
    }
  // ausgeschaltet
  if (!iSwitch)
    SetAction("Off");
  return(1);
}

private func GetFromBarrel(barrelID)
{
  var obj, barrelfill;
  var maxfill = GetMaxFill();
  while (And((obj = FindObject(barrelID,nil,nil,nil,nil, nil,nil,nil,GeoPlant,obj)), ((iFill ?? 0) < (maxfill ?? 0))))
  {
    // Fassinhalt ermitteln, nicht mehr als Restplatz im Tank
    barrelfill = BoundBy(obj->GetAmount(), nil, (maxfill ?? 0)-(iFill ?? 0));
    // aus dem Fass nehmen und in den Tank füllen
    obj->BarrelDoFill(-(barrelfill ?? 0));
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
  var newDir = ((iFill ?? 0)+200)*9 / (GetMaxFill() ?? 0);
  if (!Equal(GetDir(), newDir))
    SetDir(newDir);
  return(1);
}

