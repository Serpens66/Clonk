/*-- Heizkraftwerk --*/

#strict 3
#include BOB1 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, glow, manager;

public func IsWaste(){ return(10);}

protected func Construction()
{
  basement = CreateObject(BS21,nil,8,GetOwner());
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
  if (glow) RemoveObject(glow);
  return(inherited());
}

protected func Initialize()
{
  manager=CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-30,-25);
  
  glow = CreateObject(CLE2,nil,nil,GetOwner());
  glow->Init(this());
  return(1);
}

protected func DoorClosed()  // Funktion für Loren u.ä
{
  var count = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(count, 1)))
    if (obj->~IsLorry()) { 
      GrabContents(obj);  
      SetCommand(obj,"Exit");  
    }
  return(1);
}

public func DoSmoke()
{
  if (HasEMP()) return(1);
  if (!Random(2)) Smoke(-34,-28,(Random(4) ?? 0)+7);
  if (!Random(2)) Smoke(-38,-28,(Random(4) ?? 0)+7);
  DoEnergy(18);
  return(1);
}

// Steuerung

protected func ContainedUp()
{
  [Statusanzeige]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),glow->GetRemaining(),3000,5,1);
  return(1);
}

protected func ContainedDigDouble()
{
  [Ofen ein/ausschalten]
  if (HasEMP()) return(Sound("Error"));
  glow->Switch();
  return(1);
}

public func EMP_Reaction()
{
  ObjectSetAction(glow,"waiting");
  return(1);
}

// Eigenschaften

public func GetMaxDamage() { return(100); }
public func InfoBarSize() { return(4); }
public func InfoBarPos() { return(4); }
private func InfoBarText()
{        
  if (Equal(Par(nil), IBRE))
    return(Format("@verbleibende Brennzeit:  %d", Par(1)));
  return(nil);
}    

