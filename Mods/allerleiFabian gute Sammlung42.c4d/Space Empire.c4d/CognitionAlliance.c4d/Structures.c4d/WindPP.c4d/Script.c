/*-- Windkraftwerk --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, wing, manager;

protected func Construction()
{
  basement = CreateObject(BS06,nil,8,GetOwner());
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
  if (wing) RemoveObject(wing);
  if (manager) RemoveObject(manager);
  return(1);
}

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-1,-8,50);

  wing = CreateObject(C_W1,-1,18,GetOwner());
  wing->Init(this());
  return(1);
}

// Steuerung

protected func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),Abs(GetWind()),100,5,1);
  return(1);
}

// Eigenschaften :

public func GetMaxDamage() { return(100); }
public func InfoBarSize() { return(2); }
public func InfoBarPos() { return(8); }
private func InfoBarText()
{        
  if (Equal(Par(nil), IBRE))
    return(Format("@Energieoutput:  %d%", ((Par(1) ?? 0)*100)/(Par(2) ?? 0)));
  return(nil);
}     

