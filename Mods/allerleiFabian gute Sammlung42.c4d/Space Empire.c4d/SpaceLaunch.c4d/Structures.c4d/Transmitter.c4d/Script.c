/*-- Energietransmitter --*/

#strict
#include BOB2
#include XMTR

local basement, manager;
local iSwitch;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  // einschalten
  iSwitch = 1;
  SetAction("Transmitting");
}

protected func Construction()
{
  iSwitch = 0;
  basement = CreateObject(BS05,0,+8,GetOwner());
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
  for (i = 0; i < 72; i++)
  {
    marker = CreateObject(SMK1, Cos(i*5,TransmitterRange()), Sin(i*5,TransmitterRange()), GetOwner(clonk));
    marker->SetTimer(15);
  }
  Message("Reichweite|wird angezeigt",this());
  return(1);
}


/* Action-Call: Energieübertragung */

protected func Transmit()
{
  // ausgeschaltet oder EMP-Einwirkung
  if (!iSwitch || HasEMP())
    return(SetAction("Idle"));
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 0, 1);
  // keine Energie da?
  if (!GetEnergy())
    return(SetAction("Wait"));
  // Energie senden
  TransmitEnergy();
}

protected func CheckEnergy()
{
  if (!iSwitch || HasEMP()) return(1);
  if (GetEnergy())
    SetAction("Transmitting");
  return(1);
}

public GetSwitch: return(iSwitch);
public EnoughEnergy: return(GetEnergy() || !iSwitch);

public IsEnergyConsumer: return(1);


/* EMP-Reaktion */

public EMP_Reaction:
  DoEnergy(-100);
  return(1);


/* Eigenschaften */

public TransmitterRange: return(600);
public InfoBarSize: return(1);
public GetMaxDamage: return(80);

