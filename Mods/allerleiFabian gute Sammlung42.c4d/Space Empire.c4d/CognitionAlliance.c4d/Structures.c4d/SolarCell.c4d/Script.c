/*-- Solarzelle --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, manager;

protected func Construction()
{
  basement = CreateObject(BS16,nil,8,GetOwner());
  return(1);
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction()
{
  if (manager)  RemoveObject(manager);

  return(inherited());
}

protected func Initialize()
{  
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
   manager->InitLight(1,1);
  SetAction("Inactive");
  return(1);
}

// Action-Calls

protected func CheckSun()
{
  // Bei Tag aktivieren, falls inaktiv
//  if (DayDarkness < 195)
//if (IsDay())
if ((dayhelligkeit ?? 0) > 20)
    if (!!SEqual(GetAction(), "Inactive")) return(SetAction("Open"));
  
  // Bei Nacht deaktivieren, falls aktiv
//  if (DayDarkness >= 195)
//if(IsNight())
if ((dayhelligkeit ?? 0) <= 20)
    if (!!SEqual(GetAction(), "Active")) SetAction("Close");
  return(1);
}

// Statusabfrage

protected func ControlUp()
{
  // Statusanzeige
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),((GetEnergyOutput() ?? 0)*(GetFree() ?? 0))/10, 100,5,1);
  return(1);
}

// überladene Transmitterfunktion

protected func TransmitEnergy()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  if (HasEMP()) return(1);
  DoEnergy(((GetEnergyOutput() ?? 0)*(GetFree() ?? 0)*9)/500);
  return(inherited());
}

// liegen die Solarzellen im Freien?

private func GetFree()
{
  var e;
  for (var i = nil; (i ?? 0) < 10; S42MigrationStep(i, 1, true)) if (GBackSky(-10+(i ?? 0)*2,-10)) S42MigrationStep(e, 1, true);
  return(e);
}

// Energieoutput 

protected func GetEnergyOutput()
{
  if (!FindObject(TIME)) return(100); // ohne Zeitobjekt ständig 100% Energie liefern
  if (!GetFree()) return(nil);
  var n = dayhelligkeit; // 60 = Heller Tag
  if ((n ?? 0) >= 20) return(((n ?? 0)*100)/60); // Nur am Tag Energie liefern
  return(nil);  // bei Nacht nix
}

// Eigenschaften

public func GetMaxDamage() { return(80); }
public func InfoBarPos()   { return(10); }
public func InfoBarSize()  { return(3); }

private func InfoBarText() {        
  if (Equal(Par(nil), IBRE))
    return(Format("@Energieoutput:  %d%", (Par(1) ?? 0)*100/(Par(2) ?? 0)));
  return(nil);
}     


