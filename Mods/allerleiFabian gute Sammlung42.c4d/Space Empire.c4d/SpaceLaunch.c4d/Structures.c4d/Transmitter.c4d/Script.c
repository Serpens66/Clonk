/*-- Energietransmitter --*/

#strict 3
#include BOB2
#include XMTR

local basement, manager;
local iSwitch;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  // einschalten
  iSwitch = 1;
  SetAction("Transmitting");
}

protected func Construction()
{
  iSwitch = nil;
  basement = CreateObject(BS05,nil,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  inherited();
}


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (!inherited(clonk)) return(1);   // wenn verfeindeter Spieler
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 5, 1);
  return(1);
}

protected func ControlDig(clonk)
{
  [Ein/Ausschalten]
  if (Hostile(GetController(clonk),GetOwner())) return(1);
  iSwitch = !iSwitch;
  if (!iSwitch) return(Message("ausgeschaltet",this()));
  // eingeschaltet
  SetAction("Transmitting");
  Message("eingeschaltet",this());
  return(1);
}

protected func ControlThrow(clonk)
{
  [Reichweite]
  if (Hostile(GetController(clonk),GetOwner())) return(1);
  var i, marker;
  for (i = nil; (i ?? 0) < 72; S42MigrationStep(i, 1, true))
  {
    marker = CreateObject(SMK1, Cos((i ?? 0)*5,TransmitterRange()), Sin((i ?? 0)*5,TransmitterRange()), GetOwner(clonk));
    marker->SetTimer(15);
  }
  Message("Reichweite|wird angezeigt",this());
  return(1);
}


/* Action-Call: Energieübertragung */

protected func Transmit()
{
  // ausgeschaltet oder EMP-Einwirkung
  if (Or(!iSwitch, HasEMP()))
    return(SetAction("Idle"));
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, nil, 1);
  // keine Energie da?
  if (!GetEnergy())
    return(SetAction("Wait"));
  // Energie senden
  TransmitEnergy();
}

protected func CheckEnergy()
{
  if (Or(!iSwitch, HasEMP())) return(1);
  if (GetEnergy())
    SetAction("Transmitting");
  return(1);
}

public func GetSwitch(...) { return(iSwitch);
}
public func EnoughEnergy(...) { return(Or(GetEnergy(), !iSwitch));
}

public func IsEnergyConsumer(...) { return(1);
}


/* EMP-Reaktion */

public func EMP_Reaction(...) {
  DoEnergy(-100);
  return(1);
}


/* Eigenschaften */

public func TransmitterRange(...) { return(600);
}
public func InfoBarSize(...) { return(1);
}
public func GetMaxDamage(...) { return(80);
}

