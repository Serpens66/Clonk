/*-- Geothermalkraftwerk --*/

#strict
#include BOB1
#include XMTR

local basement, manager;
local lavatank;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(-21,-11);
  lavatank = CreateObject(SLVT,0,0,GetOwner());
  lavatank->Init(this());
}

protected func Construction()
{
  basement = CreateObject(BS23,0,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager)  RemoveObject(manager);
  if (lavatank) RemoveObject(lavatank);
  inherited();
}


/* Zerstörung */

public func Destroy()
{
  // Inhalt rausschleudern
  if (lavatank)
    CastPXS("DuroLava", lavatank->GetFill()/3, 50);
  inherited();
}


/* Steuerung von innen */

protected ContainedUp:
  [Füllstand|Image=SLVT]
  ShowInfoBar(IBRE, this(), lavatank->GetFill(), lavatank->GetMaxFill(), 5, 1);
  return(1);

protected ContainedDig:
  [ein/ausschalten]
  lavatank->SwitchOnOff();
  return(1);


/* Auffüllen per Leitung */

public func FillWithPipe(mat, value)
{
  if (mat == -1) return(0);
  if ((mat != Material("DuroLava"))
   && (mat != Material("Lava"))) return(0);
  // versuchen, aufzufüllen
  return(lavatank->DoFill(value));
}


/* Energieerzeugung */

public func EnergyProduction()
{
  // Aufruf vom Tank
  Smoke(-26, -22, Random(5)+3);
  Smoke(-26, -17, Random(5)+3);
  Smoke(2, -22, Random(5)+3);
  Smoke(2, -17, Random(5)+3);
  DoEnergy(+20);
}


/* EMP-Reaktion */

public func EMP_Reaction()
{
  DoEnergy(-100);
  return(1);
}


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (barID == IBRE)
    return(Format("@Füllmenge:  %d%", fill*100/maxf));
  return(0);
}


/* Eigenschaften */

public InfoBarPos:  return(5);
public InfoBarSize: return(4);
public GetMaxDamage: return(110);

