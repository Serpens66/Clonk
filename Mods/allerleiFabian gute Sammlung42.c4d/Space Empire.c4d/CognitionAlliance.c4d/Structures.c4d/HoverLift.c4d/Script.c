/*-- Liftdock --*/

#strict
#include BOB2 //Gebäude-Hilfsobjekt

local liftplate, basement, manager;


/* Fundament und Schwebeplatte erzeugen */

protected func Construction()
{
  basement = CreateObject(BSA3,0,+8,GetOwner());
  return(1);
}

// außer Energiezelle nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=ENAP) return(1); }

protected func Destruction()
{
  PreDestruction();
  
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  if (basement) RemoveObject(basement);
  if (liftplate) RemoveObject(liftplate);
  return(inherited());
}


protected func Initialize()
{
  manager=CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(0,-5,50);
  
  liftplate = CreateObject(CHLP,0,+25,GetOwner());
  liftplate->SetOwnerObject(this());
  SetAction("CheckStatus");
  return(1);
}


/* Steuerung */

public func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

public func ControlThrow()
{
  [Lift bauen]
  if (liftplate) return(Message("Lift bereits vorhanden!",this()));
  if (HasEMP()) return(Sound("Error"));
  var obj;
  if (!FindContents(METL) && (obj = FindContents(METL,Par(0))))  PushObject(obj);
  if (!FindContents(CALU) && (obj = FindContents(CALU,Par(0))))  PushObject(obj);
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
  if (!FindObject(ENRG)) DoEnergy(100);
  if (HasEMP()) return(Sound("Error"));
  if (liftplate)
  {
    var dir = GetComDir(liftplate);
    if (dir==COMD_Up())   DoEnergy(-5);
    if (dir==COMD_Stop()) DoEnergy(-3);
    if (dir==COMD_Down()) DoEnergy(-2);
  }
  var ActPhase = GetEnergy()/10;
  SetPhase(ActPhase);
  return(1);
}

/* neuen Lift bauen */

public func BuildLift()
{
  // genug Energie?
  if (GetEnergy() < 50)
    return(Message("Nicht genügend Energie!",this()));
  // Metall & Uranerz gebraucht
  if (!FindContents(METL) && !FindContents(CALU))
    return(Message("Benötigt|1x Metall|1x Aluminium",this()));
  if (!FindContents(METL))
    return(Message("Benötigt noch|1x Metall",this()));
  if (!FindContents(CALU))
    return(Message("Benötigt noch|1x Aluminium",this()));
  // beides verbrauchen
  RemoveObject(FindContents(METL));
  RemoveObject(FindContents(CALU));
  // Lift erstellen
  liftplate = CreateObject(CHLP,0,+25,GetOwner());
  ObjectCall(liftplate,"SetOwnerObject",this());
  return(1);
}

public func EnoughEnergy() { return(GetEnergy() >= 10); }
public func EMP_Reaction() { liftplate->Halt(); return(1); }

/* Eigenschaften */

public func GetMaxDamage() { return(80); }
public func InfoBarSize() { return(3); }
public func IsEnergyConsumer() { return(1); }
