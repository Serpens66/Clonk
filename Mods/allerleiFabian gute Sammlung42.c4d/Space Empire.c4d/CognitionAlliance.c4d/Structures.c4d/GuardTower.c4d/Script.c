/*-- Wachturm  --*/

#strict
#include BOB2 // Gebäude-Hilfsobjekt


local basement, weaponbase, weapon1, weapon2, manager;


// außer Partikelbehälter, Munitionskisten, Raketen, Energiezellen und Benzin nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=STAP && def!=PTAP && def!=MIAP && def!=ENAP && def!=GSAP) return(1); }

func Zumachen() 
{
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
return(0);
}


protected func Construction()
{
  if(!basement) basement=CreateObject(BS15,0,8);  
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
//  if (weaponbase) { SetGraphics(""); RemoveObject(weaponbase); }
  if (weapon1) RemoveObject(weapon1);
  if (weapon2) RemoveObject(weapon2);
  return(inherited());
}

protected func Initialize()
{
  // neue Grafik setzen
//  weaponbase=CreateObject(CGTW,0,-10, GetOwner());
//  weaponbase->Init(this());
//  SetGraphics("Active");
  
  manager=CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,1);
  SetEntrance(1);
  return(1);
}

// Steuerung


protected func ContainedUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  if (HasEMP()) return(Sound("Error"));
  // Menü für Aktionsvorgabe
  CreateMenu(GetID(), caller, 0,1, "Waffenwechsel",0,1);
  // linke Waffe
  AddMenuItem("Links: keine Waffe",       "ChangeWeapon", SYNW,caller, 0,0,"keine Bewaffnung",            2,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Links: Maschinengewehr", "ChangeWeapon", STW1,caller, 0,0,0, 				   2,0, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Links: Vulcan-Waffe",    "ChangeWeapon", STW4,caller, 0,0,0,				   2,0, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Links: Elektrowaffe",    "ChangeWeapon", STW3,caller, 0,0,0,				   2,0, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Links: Raketenwerfer", "ChangeWeapon", STW2,caller, 0,0,0, 2,0, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Links: Laser",          "ChangeWeapon", SLSW,caller, 0,0,0, 2,0, GetCost(SLSW));
  
  // rechte Waffe  
  AddMenuItem("Rechts: keine Waffe",       "ChangeWeapon", SYNW,caller, 0,1,"keine Bewaffnung",            2,0, GetCost());
 if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Rechts: Maschinengewehr", "ChangeWeapon", STW1,caller, 0,1,0, 				   2,0, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Rechts: Vulcan-Waffe",    "ChangeWeapon", STW4,caller, 0,1,0,				   2,0, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Rechts: Elektrowaffe",    "ChangeWeapon", STW3,caller, 0,1,0,				   2,0, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Rechts: Raketenwerfer", "ChangeWeapon", STW2,caller, 0,1,0, 2,0, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Rechts: Laser",          "ChangeWeapon", SLSW,caller, 0,1,0, 2,0, GetCost(SLSW));
  return(1);
}


// kleine Funktion, die nix macht
private func noop() { return(1); } 

// Plattformen kombinieren / trennen (ObjectCall)
protected func SetMode()
{
  weaponbase->~ChangeMode();
  return(1);
}

// Waffenwechsel
protected func ChangeWeapon(ID, dir)
{
  // nochmal das gleiche gewählt?
  if (ID == SYNW) ID = 0;
  if (dir) {
    if (ID == GetID(weapon1)) return(0);
    if (!ID && !weapon1)      return(0);
  }
  else {
    if (ID == GetID(weapon2)) return(0);
    if (!ID && !weapon2)      return(0);
  }
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
  if (dir)
    if (weapon1)
      RemoveObject(weapon1);
  if (!dir)
    if (weapon2)
      RemoveObject(weapon2);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  if (dir) {
    weapon1 = CreateObject(ID, 0,-100,GetOwner());
    weapon1->Init(this(),1,-52,3,1);  //links
  }    
  else {
    weapon2 = CreateObject(ID, 0,-100,GetOwner());
    weapon2->Init(this(),1,-19,3,-1);  // rechts
  }
  return(1);
}

private func GetCost(ID, weapon)
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

public func EnoughEnergy() { return(GetEnergy()); }

// Eigenschaften
public func GetMaxDamage() { return(150); }
public func InfoBarSize() { return(2); }
public func IsEnergyConsumer() { return(1); }
