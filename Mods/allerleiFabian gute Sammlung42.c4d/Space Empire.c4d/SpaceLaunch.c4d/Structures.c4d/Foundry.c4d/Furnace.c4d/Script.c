/*-- Schmelzofen-Objekt --*/

#strict

local foundry, mode, working;
local materialtime, burntime;
local product, prodtime;


/* Initialisierung */

protected func Initialize()
{
  mode = 1;   // Material-Modus
}

public func Init(fdy)
{
  // wird vom Hochofen aufgerufen
  foundry = fdy;
  SetAction("Nothing",foundry);
}


/* Steuerung durch Hochofen */

public func SwitchMode()
{
  // Moduswechsel vom Hochofen aus
  mode = !mode;
  Sound("Click");
  if (mode) Message("Material-Herstellung",foundry);
       else Message("Energie-Erzeugung",foundry);
}


/* Aufrufsfunktionen */

public func ShowBurnInfo()
{
  if (burntime)     ShowInfoBar(IBRE, foundry, Min(burntime,1000),        1000, 10,0,3);
  if (materialtime) ShowInfoBar(IBRP, foundry, prodtime-materialtime, prodtime, 10,1,1);
}

public func DoFill(diff)
{
  if (burntime >= 3000) return(0);
  burntime = Min(burntime + diff*15, 3000);
}

public GetMode: return(mode);


/* Produktion */

protected func Production()
{
  // EMP?
  if (HasEMP(foundry)) return(0);
  // Metall- oder Energiemodus
  if (working) --working;
  if (mode) MaterialMode();
       else EnergyMode();
  // Anzeigen updaten
  ShowInfoBar(IBRE, foundry, Min(burntime,3000), 3000, 0,0,3);
  ShowInfoBar(IBRP, foundry, 200-materialtime,    200, 0,1,1);
  return(1);
}


/* Materialmodus */

private func MaterialMode()
{
  // Produktion beginnen, wenn nötig
  if (!materialtime)  TryMaterial(ORE1, ANML, 200);
  if (!materialtime)  TryMaterial(CTIO, CTIT, 300);
  if (!materialtime)  TryMaterial(CALO, CALU, 300);
  if (!materialtime)  TryMaterial(MTWS, SHLK, 300);
  if (!materialtime) return(1);
  // neues Material in den Ofen, wenn nötig
  if (!burntime)  TryBurnFuel();
  // Brennstoff verbrauchen
  if (burntime)
  {
    materialtime = Max(materialtime-4, 0);   // viermal so schnell
    burntime = Max(burntime-4, 0);
  }
  else   // zur Not geht auch Energie
  {
    if (GetEnergy(foundry) < 10) return(1);   // zuwenig Energie
    DoEnergy(-10,foundry);
    --materialtime;
  }
  // Herstellung
  if (!materialtime)
  {
    // Produkt fertig
    foundry->Production(product);
    // Rohstoffe endgültig verbrauchen
    var obj;
    while (obj = Contents())
      RemoveObject(obj);
  }
  foundry->DoSmoke();
  working = 5;
  return(1);
}

private func TryMaterial(ore, prod, time)
{
  // Erz finden
  var obj = FindContents(ore,foundry);
  if (!obj) return(0);
if (GetID(obj)==MTWS && !(ContentsCount(MTWS,foundry)>4)) {return(0);};
  // Material verbrennen
  if (!burntime)
    if (!TryBurnFuel())             // kein Brennmaterial:
      if (GetEnergy(foundry) < 30)  // zur Not muss Energie her
        return(1);
  // Erz verbrauchen, indem es hier herein kommt
  Enter(this(),obj);
if(GetID(obj)==MTWS){Enter(this(),FindContents(MTWS,foundry));Enter(this(),FindContents(MTWS,foundry));Enter(this(),FindContents(MTWS,foundry));Enter(this(),FindContents(MTWS,foundry));};
  product = prod;
  materialtime = time;
  return(1);
}


/* Energiemodus */

private func EnergyMode()
{
  // Materialherstellung beenden
  if (materialtime)
  {
    materialtime = 0;
    // Anzeige löschen
    DisappearInfoBar(IBRP, foundry);
    // Erz wieder rausrücken
    var obj;
    while (obj = Contents())
      Enter(foundry, obj);
  }
  // neues Material in den Ofen, wenn nötig
  if (!burntime)
    if (!TryBurnFuel())
      return(1);
  // Brennstoff verbrauchen
  --burntime;
  // Energie erzeugen
  DoEnergy(+10,foundry);
  foundry->DoSmoke();
  working = 5;
  return(1);
}


/* Hilfsfunktionen */

private func GetFuelMaterial()
{
  var obj = FindContents(WOOD,foundry);
  if (!obj)  obj = FindContents(COAL,foundry);
  if (!obj)  obj = FindContents(OBRL,foundry);
  if (!obj)  obj = FindContents(GCAN,foundry);
  return(obj);
}

private func TryBurnFuel()
{
  // Material finden
  var fuel = GetFuelMaterial();
  if (!fuel) return(0);
  // Ölfass?
  if (GetID(fuel)==OBRL)
  {
    burntime += fuel->GetAmount()*15;  // max. 3000
    ChangeDef(BARL,fuel);
    return(burntime);
  }
  // Benzinkanister, Holz oder Kohle
  if (GetID(fuel)==GCAN)  burntime += 3000;
  if (GetID(fuel)==WOOD)  burntime += 300;
  if (GetID(fuel)==COAL)  burntime += 400;
  RemoveObject(fuel);
  return(1);
}


/* Action-Calls */

protected WorkingOff:
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(foundry)),foundry);
  // Ofen anmachen?
  if (working)
    SetAction("StartProcess");
  return(1);

protected WorkingOn:
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(foundry)),foundry);
  // Ofen ausmachen?
  if (!working)
    SetAction("EndProcess");
  return(1);


/* Aufruf durch die Anzeige */

public func GetSignInfo()
{
  // Materialproduktion
  if (mode)
  {
    if (!materialtime) return(1);   // Leere Anzeige
    if (burntime) return(2);        // Produktion mit Brennmaterial
    if (working) return(3);         // Produktion mit Energie
    return(1);                      // keine Energie!
  }
  // Energieproduktion
  if (burntime && working)
    return(4);                     // Produktion von Energie
  return(1);                       // Leere Anzeige
}


/* Energieinfo */

public func EnoughEnergy()
{
  if (!mode)         return(1);    // Energieproduktion
  if (!materialtime) return(1);    // nichts in Produktion
  if (burntime)      return(1);    // Brennmaterial vorhanden
  if (GetEnergy(foundry) >= 10) return(0); // oder Energie vorhanden?
  // NoEnergy-Symbol anzeigen
  return(0);
}

