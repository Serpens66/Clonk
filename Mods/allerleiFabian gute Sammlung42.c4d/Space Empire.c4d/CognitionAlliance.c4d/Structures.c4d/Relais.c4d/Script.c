/*-- Energieempfänger --*/

#strict
#include BOB2 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, active, manager;

protected func Construction()
{
  basement = CreateObject(BS06,0,8,GetOwner());  
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
  if (manager) RemoveObject(manager);

  return(inherited());
}

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(0,-10,40);
  
  SetAction("wait");
  active=1;
  return(1);
}

public func EnoughEnergy() { return(GetEnergy()); }

// Steuerung

protected func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func ControlDigDouble()
{
  [an-/ausschalten]
  if (HasEMP()) return(Sound("Error"));
  if (active) Message("ausgeschaltet",this());
         else Message("eingeschaltet",this());
  active = !active;
  return(1);
}

protected func ControlThrow(clonk)
{
  [Reichweite]
  if (Hostile(GetController(clonk),GetOwner())) return(1);
  var i, marker;
  for (i = 0; i < 72; i++)
  {
    marker = CreateObject(SMK1, Cos(i*5,TransmitterRange()), Sin(i*5,TransmitterRange()), GetOwner(clonk));
    marker->SetTimer(15);
  }
  Message("Reichweite|wird angezeigt",this());
  return(1);
}

protected func Transmit()
{
  // ausgeschaltet oder EMP-Einwirkung
  if (!active || HasEMP())
    return(SetAction("wait"));
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 0, 1);
  // keine Energie da?
  if (!EnoughEnergy())
    return(SetAction("wait"));
  else SetAction("working"); 
  // Energie senden
  TransmitEnergy();
  return(9);
}

// überladene Transmitterfunktion
/*
public func TransmitEnergy()
{
  return(1);
}
*/
// Empfangsbereitschaft

public func IsActive()
{
  if (HasEMP() || (!active)) return(0);
  return(1);
}

// Eigenschaften
 
public func GetMaxDamage() { return(80); }
public func TransmitterRange() { return(300); }
public func InfoBarSize() { return(2); }
public func InfoBarPos() { return(4); }
public func IsEnergyConsumer() { return(active); }
