/*-- Geothermalkraftwerk --*/

#strict 3
#include BOB1
#include XMTR

local basement, manager;
local lavatank;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-21,-11);
  lavatank = CreateObject(SLVT,nil,nil,GetOwner());
  lavatank->Init(this());
}

protected func Construction()
{
  basement = CreateObject(BS23,nil,+8,GetOwner());
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
    CastPXS("DuroLava", (lavatank->GetFill() ?? 0)/3, 50);
  inherited();
}


/* Steuerung von innen */

protected func ContainedUp(...) {
  [Füllstand|Image=SLVT]
  ShowInfoBar(IBRE, this(), lavatank->GetFill(), lavatank->GetMaxFill(), 5, 1);
  return(1);
}

protected func ContainedDig(...) {
  [ein/ausschalten]
  lavatank->SwitchOnOff();
  return(1);
}


/* Auffüllen per Leitung */

public func FillWithPipe(mat, value)
{
  if (Equal(mat, -1)) return(nil);
  if (And((!Equal(mat, Material("DuroLava")))
   , (!Equal(mat, Material("Lava"))))) return(nil);
  // versuchen, aufzufüllen
  return(lavatank->DoFill(value));
}


/* Energieerzeugung */

public func EnergyProduction()
{
  // Aufruf vom Tank
  Smoke(-26, -22, (Random(5) ?? 0)+3);
  Smoke(-26, -17, (Random(5) ?? 0)+3);
  Smoke(2, -22, (Random(5) ?? 0)+3);
  Smoke(2, -17, (Random(5) ?? 0)+3);
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
  if (Equal(barID, IBRE))
    return(Format("@Füllmenge:  %d%", (fill ?? 0)*100/(maxf ?? 0)));
  return(nil);
}


/* Eigenschaften */

public func InfoBarPos(...) {  return(5);
}
public func InfoBarSize(...) { return(4);
}
public func GetMaxDamage(...) { return(110);
}

