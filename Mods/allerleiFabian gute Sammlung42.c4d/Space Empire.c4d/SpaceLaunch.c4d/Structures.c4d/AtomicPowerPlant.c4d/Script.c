/*-- Atomkraftwerk --*/

#strict
#include BOB1
#include XMTR

local basement, manager;
local reactor;

public func IsWaste(){ return(20);}

/* Reaktor und Fundament erzeugen bzw. löschen */

protected Initialize:
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(-5,-11);
  reactor = CreateObject(SREA,0,0,GetOwner());
  reactor->Init(this());
  return(1);

protected Construction:
  basement = CreateObject(BS25,0,+8,GetOwner());
  return(1);
  
protected Destruction:
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);

public PreDestruction:
  if (manager) RemoveObject(manager);
  if (reactor) RemoveObject(reactor);
  return(inherited());


/* Steuerung von innen */

protected ContainedUp:
  [Energiewert]
  reactor->ShowEnergyInfo();
  return(1);

protected ContainedDigDouble:
  [Reaktor ein/abschalten|Image=SREA]
  reactor->SwitchOnOff();
  return(1);


/* Anzeige */

private SoundOpenDoor: 
  inherited();
  reactor->ShowEnergyInfo();
  return(1);


/* Zerstörung */

public Destroy:
  // kein Reaktor (z.B. weil AKW noch im Bau)
  if (!reactor)
    return(inherited());
  // Reaktor inaktiv?
  if (!reactor->GetRestTime() || !reactor->GetSwitch())
    return(inherited());
  // Effekte & Strahlung/EMP
  var wave = CreateObject(SWAV);
  wave->SetSize(300,GetX(),GetY(), 300+Random(100), 300+Random(100));
  CreateParticle("Blast", 0,0,0,0, 1500,RGB(0,255,0));
  return(inherited());


/* Energieerzeugung */

public EnergyProduction:
  // Aufruf vom Reaktor
  Smoke(16,-25, Random(10)+12);
  DoEnergy(+100);
  return(1);


/* Anzeigentext */

private func InfoBarText(barID, fill, max)
{
  if (barID == IBRE)
  {
    if (!fill)       return("@keine Brennstäbe");
    if (fill==100)   return("@Brennstäbe:  1 Kiste");
    if (!(fill%100)) return(Format("@Brennstäbe:  %d Kisten", fill/100));
    if (fill < 100)  return(Format("@Brennstäbe:  %d %", fill));
    if (fill < max)  return(Format("@Brennstäbe:  %d % + %d", fill%100, fill/100));
    return("@noch mehr als 10 Kisten");
  }  
  return(0);
}


/* Eigenschaften */

public InfoBarSize: return(5);
public GetMaxDamage: return(125);

