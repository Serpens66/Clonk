/*-- Abwehrturm --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt


local weapon,basement,manager;

// außer Partikelbehälter und Munitionskisten nichts aufnehmen
protected func RejectCollect(def,pObj) { if(And(And(And(!Equal(def, STAP), !Equal(def, PTAP)), !Equal(def, CLNK)), !Equal(def, HZCK))) return(1); }


func Zumachen()
{
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);
return(nil);
}

protected func Construction()
{
  basement = CreateObject(BS07,nil,8,GetOwner());
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
  manager=CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,1);
  CheckStatus();
  SetEntrance(1);
  return(1);
}

protected func CheckStatus(...) {
  if ((GetEnergy() ?? 0) < 10) SetAction("NoEnergy");
                   else SetAction("Glowing"); 
return(1);
}

// Steuerung



protected func ContainedUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
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
  CreateMenu(GetID(), caller, nil,2, "Waffenwechsel",nil,1);
  AddMenuItem("keine Waffe", "SelectWeapon", SYNW,caller, nil,nil,nil, 128,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Plasmakanone 1", "SelectWeapon", CPG1,caller, nil,nil,nil, 128,nil, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Plasmakanone 2", "SelectWeapon", CPG2,caller, nil,nil,nil, 128,nil, GetCost(CPG2));
      if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Maschinengewehr", "SelectWeapon", CMG1,caller, nil,nil,nil, 128,nil, GetCost(CMG1));
  return(1);  
}

private func SelectWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (Equal(ID, SYNW)) ID = nil;
  if (Equal(ID, GetID(weapon))) return(nil);
  if (And(!ID, !weapon))      return(nil);
  // genug Geld da?
  var cost = GetCost(ID);
  if ((GetWealth(GetOwner()) ?? 0) < (cost ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    Sound("Error");
    return(nil);
  }
  // bezahlen
  DoWealth(GetOwner(), -(cost ?? 0));
  if ((cost ?? 0) > 0) Sound("UnCash");
           else Sound("Cash");
  // alte Waffe weg
  if (weapon)
    RemoveObject(weapon);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  weapon = CreateObject(ID, nil,nil,GetOwner());
  weapon->Init(this(),1);
  return(1);
}

private func GetCost(ID)
{
  // Waffe bereits verwendet?
  if (And(!ID, !weapon))      return(nil);
  if (Equal(ID, GetID(weapon))) return(nil);
  // Kostenberechnung
  var cost = nil;
  if (ID)     S42MigrationAssign(cost, "+=", (GetValue(nil,ID) ?? 0));         // neue Waffe kostet was
  if (weapon) S42MigrationAssign(cost, "-=", (GetValue(weapon) ?? 0)*4/5);   // alte wird für 80% des Wertes verkauft
  return(cost);
}

public func EMP_Reaction()
{
  SetAction("NoEnergy");
  return(1);
}

// Eigenschaften

public func EnoughEnergy() { return((GetEnergy() ?? 0) >= 5); }
public func GetMaxDamage() { return(150); }
public func InfoBarSize() { return(1); }
public func InfoBarPos() { return(8); }
public func IsEnergyConsumer() { return(1); }
