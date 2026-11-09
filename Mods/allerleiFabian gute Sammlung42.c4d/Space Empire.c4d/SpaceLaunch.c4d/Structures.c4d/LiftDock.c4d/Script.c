/*-- Liftdock --*/

#strict
#include BOB2

local basement, manager;
local liftplate;


/* Fundament und Schwebeplatte erzeugen */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  // Liftplatte
  SetAction("OffStatus");
  liftplate = CreateObject(SLP1,0,+10,GetOwner());
  liftplate->SetOwnerObject(this());
  return(1);
}

protected func Construction()
{
  basement = CreateObject(BSA2,0,+8,GetOwner());
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
  if (!FindContents(METL) && (obj = FindContents(METL,clonk)))  PushObject(obj);
  if (!FindContents(SURO) && (obj = FindContents(SURO,clonk)))  PushObject(obj);
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
    if (!Inside(GetY(liftplate)-GetY(), -3, 0))    // im Liftdock eingerastet?
    {
      var dir = GetComDir(liftplate);
      if (dir==COMD_Up())   DoEnergy(-5);
      if (dir==COMD_Stop()) DoEnergy(-3);
      if (dir==COMD_Down()) DoEnergy(-2);
    }
  // Update für Energieanzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 0, 1);
  // genügende Energieversorgung / welcher Status gesetzt?
  var Energynum = GetEnergyNum();
  var Statusnum = GetStatusNum();
  // keine Statusänderung?
  if (Energynum == Statusnum) return(1);
  // neuen Status setzen
  if (Energynum == 0)  SetAction("OffStatus");
  if (Energynum == 1)  SetAction("WarningStatus");
  if (Energynum == 2)  SetAction("NormalStatus");
  return(1);
}

private func GetStatusNum()
{
  if (GetAction() eq "NormalStatus")  return(2);
  if (GetAction() eq "WarningStatus") return(1);
  return(0);   // OffStatus
}

private func GetEnergyNum()
{
  if (GetEnergy() > 80) return(2);   // ab 80
  if (!GetEnergy())     return(0);   // genau 0
  return(1);   // 1 bis 80
}

public EnoughEnergy: return(GetEnergy());


/* neuen Lift bauen */

public func BuildLift()
{
  // genug Energie?
  if (GetEnergy() < 50)
    return(Message("Nicht genügend Energie!",this()));
  // Metall & Uranerz gebraucht
  if (!FindContents(METL) && !FindContents(SURO))
    return(Message("Benötigt|1x Metall|1x Uranerz",this()));
  if (!FindContents(METL))
    return(Message("Benötigt noch|1x Metall",this()));
  if (!FindContents(SURO))
    return(Message("Benötigt noch|1x Uranerz",this()));
  // beides verbrauchen
  RemoveObject(FindContents(METL));
  RemoveObject(FindContents(SURO));
  // Lift erstellen
  liftplate = CreateObject(SLP1,0,+10,GetOwner());
  liftplate->Init(this());
  return(1);
}


/* Eigenschaften */

public IsEnergyConsumer: return(1);
public InfoBarPos:  return(5);
public InfoBarSize: return(3);
public GetMaxDamage: return(80);

