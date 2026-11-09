/*-- Solarzelle --*/

#strict
#include BOB2 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, manager;

protected func Construction()
{
  basement = CreateObject(BS16,0,8,GetOwner());
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
  manager = CreateObject(BMAN,0,0,GetOwner());
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
if (dayhelligkeit > 20)
    if (GetAction() eq "Inactive") return(SetAction("Open")); 
  
  // Bei Nacht deaktivieren, falls aktiv
//  if (DayDarkness >= 195)
//if(IsNight())
if (dayhelligkeit <= 20)
    if (GetAction() eq "Active") SetAction("Close");
  return(1);
}

// Statusabfrage

protected func ControlUp()
{
  // Statusanzeige
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),(GetEnergyOutput()*GetFree())/10, 100,5,1);
  return(1);
}

// überladene Transmitterfunktion

protected func TransmitEnergy()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  if (HasEMP()) return(1);
  DoEnergy((GetEnergyOutput()*GetFree()*9)/500);
  return(inherited());
}

// liegen die Solarzellen im Freien?

private func GetFree()
{
  var e;
  for (var i = 0; i < 10; i++) if (GBackSky(-10+i*2,-10)) e++;
  return(e);
}

// Energieoutput 

protected func GetEnergyOutput()
{
  if (!FindObject(TIME)) return(100); // ohne Zeitobjekt ständig 100% Energie liefern
  if (!GetFree()) return(0);
  var n = dayhelligkeit; // 60 = Heller Tag
  if (n >= 20) return((n*100)/60); // Nur am Tag Energie liefern
  return(0);  // bei Nacht nix 
}

// Eigenschaften

public func GetMaxDamage() { return(80); }
public func InfoBarPos()   { return(10); }
public func InfoBarSize()  { return(3); }

private func InfoBarText() {        
  if (Par(0) == IBRE)       
    return(Format("@Energieoutput:  %d%", Par(1)*100/Par(2)));                            
  return(0);
}     


