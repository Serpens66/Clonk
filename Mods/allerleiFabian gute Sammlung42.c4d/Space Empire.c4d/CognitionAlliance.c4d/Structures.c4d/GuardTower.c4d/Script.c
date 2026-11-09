/*-- Wachturm  --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt


local basement, weaponbase, weapon1, weapon2, manager;


// außer Partikelbehälter, Munitionskisten, Raketen, Energiezellen und Benzin nichts aufnehmen
protected func RejectCollect(def,pObj) { if(And(And(And(And(!Equal(def, STAP), !Equal(def, PTAP)), !Equal(def, MIAP)), !Equal(def, ENAP)), !Equal(def, GSAP))) return(1); }

func Zumachen() 
{
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);
return(nil);
}


protected func Construction()
{
  if(!basement) basement=CreateObject(BS15,nil,8);
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
  
  manager=CreateObject(BMAN,nil,nil,GetOwner());
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
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  if (HasEMP()) return(Sound("Error"));
  // Menü für Aktionsvorgabe
  CreateMenu(GetID(), caller, nil,1, "Waffenwechsel",nil,1);
  // linke Waffe
  AddMenuItem("Links: keine Waffe",       "ChangeWeapon", SYNW,caller, nil,nil,"keine Bewaffnung",            2,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Links: Maschinengewehr", "ChangeWeapon", STW1,caller, nil,nil,nil, 				   2,nil, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Links: Vulcan-Waffe",    "ChangeWeapon", STW4,caller, nil,nil,nil,				   2,nil, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Links: Elektrowaffe",    "ChangeWeapon", STW3,caller, nil,nil,nil,				   2,nil, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Links: Raketenwerfer", "ChangeWeapon", STW2,caller, nil,nil,nil, 2,nil, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Links: Laser",          "ChangeWeapon", SLSW,caller, nil,nil,nil, 2,nil, GetCost(SLSW));
  
  // rechte Waffe  
  AddMenuItem("Rechts: keine Waffe",       "ChangeWeapon", SYNW,caller, nil,1,"keine Bewaffnung",            2,nil, GetCost());
 if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Rechts: Maschinengewehr", "ChangeWeapon", STW1,caller, nil,1,nil, 				   2,nil, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Rechts: Vulcan-Waffe",    "ChangeWeapon", STW4,caller, nil,1,nil,				   2,nil, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Rechts: Elektrowaffe",    "ChangeWeapon", STW3,caller, nil,1,nil,				   2,nil, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Rechts: Raketenwerfer", "ChangeWeapon", STW2,caller, nil,1,nil, 2,nil, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Rechts: Laser",          "ChangeWeapon", SLSW,caller, nil,1,nil, 2,nil, GetCost(SLSW));
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
  if (Equal(ID, SYNW)) ID = nil;
  if (dir) {
    if (Equal(ID, GetID(weapon1))) return(nil);
    if (And(!ID, !weapon1))      return(nil);
  }
  else {
    if (Equal(ID, GetID(weapon2))) return(nil);
    if (And(!ID, !weapon2))      return(nil);
  }
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
    weapon1 = CreateObject(ID, nil,-100,GetOwner());
    weapon1->Init(this(),1,-52,3,1);  //links
  }    
  else {
    weapon2 = CreateObject(ID, nil,-100,GetOwner());
    weapon2->Init(this(),1,-19,3,-1);  // rechts
  }
  return(1);
}

private func GetCost(ID, weapon)
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

public func EnoughEnergy() { return(GetEnergy()); }

// Eigenschaften
public func GetMaxDamage() { return(150); }
public func InfoBarSize() { return(2); }
public func IsEnergyConsumer() { return(1); }
