/*-- Abwehrturm --*/

#strict
#include BOB2 // Gebäude-Hilfsobjekt


local weapon,basement,manager;

// außer Partikelbehälter und Munitionskisten nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=STAP && def!=PTAP && def!=CLNK && def!=HZCK) return(1); }


func Zumachen()
{
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
return(0);
}

protected func Construction()
{
  basement = CreateObject(BS07,0,8,GetOwner());
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
  if (weapon) RemoveObject(weapon);
  return(inherited());
}

protected func Initialize()
{
  manager=CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,1);
  CheckStatus();
  SetEntrance(1);
  return(1);
}

protected CheckStatus:
  if (GetEnergy() < 10) SetAction("NoEnergy"); 
                   else SetAction("Glowing"); 
return(1);

// Steuerung



protected func ContainedUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

func ControlDig(caller)
{
ContainedDig(caller);
}

// Waffenwechsel

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  if (HasEMP()) return(Sound("Error"));
  CreateMenu(GetID(), caller, 0,2, "Waffenwechsel",0,1);
  AddMenuItem("keine Waffe", "SelectWeapon", SYNW,caller, 0,0,0, 128,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Plasmakanone 1", "SelectWeapon", CPG1,caller, 0,0,0, 128,0, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Plasmakanone 2", "SelectWeapon", CPG2,caller, 0,0,0, 128,0, GetCost(CPG2));
      if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Maschinengewehr", "SelectWeapon", CMG1,caller, 0,0,0, 128,0, GetCost(CMG1));
  return(1);  
}

private func SelectWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (ID == SYNW) ID = 0;
  if (ID == GetID(weapon)) return(0);
  if (!ID && !weapon)      return(0);
  // genug Geld da?
  var cost = GetCost(ID);
  if (GetWealth(GetOwner()) < cost)
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    Sound("Error");
    return(0);
  }
  // bezahlen
  DoWealth(GetOwner(), -cost);
  if (cost > 0) Sound("UnCash");
           else Sound("Cash");
  // alte Waffe weg
  if (weapon)
    RemoveObject(weapon);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  weapon = CreateObject(ID, 0,0,GetOwner());
  weapon->Init(this(),1);
  return(1);
}

private func GetCost(ID)
{
  // Waffe bereits verwendet?
  if (!ID && !weapon)      return(0);
  if (ID == GetID(weapon)) return(0);
  // Kostenberechnung
  var cost = 0;
  if (ID)     cost += GetValue(0,ID);         // neue Waffe kostet was
  if (weapon) cost -= GetValue(weapon)*4/5;   // alte wird für 80% des Wertes verkauft
  return(cost);
}

public func EMP_Reaction()
{
  SetAction("NoEnergy");
  return(1);
}

// Eigenschaften

public func EnoughEnergy() { return(GetEnergy() >= 5); }
public func GetMaxDamage() { return(150); }
public func InfoBarSize() { return(1); }
public func InfoBarPos() { return(8); }
public func IsEnergyConsumer() { return(1); }
