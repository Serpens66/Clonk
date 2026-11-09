/*-- Liftdock --*/

#strict 3
#include BOB2

local basement, manager;
local liftplate;


/* Fundament und Schwebeplatte erzeugen */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  // Liftplatte
  SetAction("OffStatus");
  liftplate = CreateObject(SLP1,nil,+10,GetOwner());
  liftplate->SetOwnerObject(this());
  return(1);
}

protected func Construction()
{
  basement = CreateObject(BSA2,nil,+8,GetOwner());
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
  if (manager)   RemoveObject(manager);
  if (liftplate) RemoveObject(liftplate);
  return(inherited());
}


/* Steuerung */

public func ControlUp(clonk)
{
  [Statusanzeige]
  if (!inherited(clonk)) return(1);   // wenn verfeindeter Spieler
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 5, 1);
  if (!liftplate)
    Message("Kein Lift!",this());
  return(1);
}

public func ControlThrow(clonk)
{
  [Lift bauen]
  if (liftplate) return(Message("Lift bereits vorhanden!",this()));
  var obj;
  if (And(!FindContents(METL), (obj = FindContents(METL,clonk))))  PushObject(obj);
  if (And(!FindContents(SURO), (obj = FindContents(SURO,clonk))))  PushObject(obj);
  return(BuildLift());
}

private func PushObject(obj)
{
  Sound("Grab");
  Enter(this(), obj);
  return(1);
}


/* Energieüberprüfung */

public func CheckEnergy()
{
  // Energieverbrauch
  if (liftplate)
    if (!Inside((GetY(liftplate) ?? 0)-(GetY() ?? 0), -3, nil))    // im Liftdock eingerastet?
    {
      var dir = GetComDir(liftplate);
      if (Equal(dir, COMD_Up))   DoEnergy(-5);
      if (Equal(dir, COMD_Stop)) DoEnergy(-3);
      if (Equal(dir, COMD_Down)) DoEnergy(-2);
    }
  // Update für Energieanzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, nil, 1);
  // genügende Energieversorgung / welcher Status gesetzt?
  var Energynum = GetEnergyNum();
  var Statusnum = GetStatusNum();
  // keine Statusänderung?
  if (Equal(Energynum, Statusnum)) return(1);
  // neuen Status setzen
  if (Equal(Energynum, nil))  SetAction("OffStatus");
  if (Equal(Energynum, 1))  SetAction("WarningStatus");
  if (Equal(Energynum, 2))  SetAction("NormalStatus");
  return(1);
}

private func GetStatusNum()
{
  if (!!SEqual(GetAction(), "NormalStatus"))  return(2);
  if (!!SEqual(GetAction(), "WarningStatus")) return(1);
  return(nil);   // OffStatus
}

private func GetEnergyNum()
{
  if ((GetEnergy() ?? 0) > 80) return(2);   // ab 80
  if (!GetEnergy())     return(nil);   // genau 0
  return(1);   // 1 bis 80
}

public func EnoughEnergy(...) { return(GetEnergy());
}


/* neuen Lift bauen */

public func BuildLift()
{
  // genug Energie?
  if ((GetEnergy() ?? 0) < 50)
    return(Message("Nicht genügend Energie!",this()));
  // Metall & Uranerz gebraucht
  if (And(!FindContents(METL), !FindContents(SURO)))
    return(Message("Benötigt|1x Metall|1x Uranerz",this()));
  if (!FindContents(METL))
    return(Message("Benötigt noch|1x Metall",this()));
  if (!FindContents(SURO))
    return(Message("Benötigt noch|1x Uranerz",this()));
  // beides verbrauchen
  RemoveObject(FindContents(METL));
  RemoveObject(FindContents(SURO));
  // Lift erstellen
  liftplate = CreateObject(SLP1,nil,+10,GetOwner());
  liftplate->Init(this());
  return(1);
}


/* Eigenschaften */

public func IsEnergyConsumer(...) { return(1);
}
public func InfoBarPos(...) {  return(5);
}
public func InfoBarSize(...) { return(3);
}
public func GetMaxDamage(...) { return(80);
}

