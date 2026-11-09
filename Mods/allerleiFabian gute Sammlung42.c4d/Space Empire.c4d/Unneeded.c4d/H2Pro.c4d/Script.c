/*-- H2 Produktion --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt


local basement, manager, fill, fill2;

protected func Construction()
{
  basement = CreateObject(CB11,nil,8,GetOwner());
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
  return(inherited());
}

public func SpecialExplosion()
{
  CastPXS("Water",(fill2 ?? 0)/5,30);
  CastObjects(CEH2,(fill ?? 0)/50,20,5-(Random(11) ?? 0),5-(Random(11) ?? 0));
  return(1);
}

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(nil,-35);
  return(1);
}


// interne Funktionen

public func FillWithPipe()
{
  if ((fill2 ?? 0) < (GetMaxFill() ?? 0)) { S42MigrationStep(fill2, 1, true);  return(1); }
  return(nil);
}

protected func CheckStatus()
{
  if (!FindObject(ENRG)) DoEnergy(100);
  if (!CheckPWL()) return(1);
  
  if (Or(((GetEnergy() ?? 0) < 10), HasEMP())) return(1);
  
  if (And(fill2, ((fill ?? 0) < (GetH2MaxFill() ?? 0)))) {
    if (ActIdle()) SetAction("StartWorking");
    DoEnergy(-7);
    S42MigrationStep(fill2, -1, true);
    S42MigrationStep(fill, 1, true);
  }
  return(1);
}

private func CheckPWL()
{
  var line;
  if (!FindObject(ENRG)) return(1);
  while (line = FindObject(PWRL,nil,nil,nil,nil, nil,"Connect",this(),nil, line))
    if (Equal(GetActionTarget(1,line), this()))
      return(1);
  return(nil);
}

protected func CheckStatus2()
{
  if (And(FindObject(ENRG), ((GetEnergy() ?? 0) < 10))) return(1);
  if (!CheckPWL()) return(1); 
  
  if (HasEMP()) return(1);
  
  if (And(fill2, ((fill ?? 0) < (GetH2MaxFill() ?? 0)))) SetAction("StartWorking");
  return(1);
}

// Steuerung

protected func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func ControlDig(caller)
{
  [Tankoptionen]
  if (HasEMP()) return(Sound("Error"));
  CreateMenu(CH2P,Par(nil),nil,nil,"H2 Produktion",nil,1);
  AddMenuItem("Füllstand anzeigen","Infos",INFO,caller);
  
  // Gasflasche füllen
  if ((fill ?? 0) >= 1400) {
    if (FindContents(CGBE))       AddMenuItem("Gasflasche füllen",       "FillGas",  CGBE, caller);
    if ((ContentsCount(CGBE) ?? 0) >= 2) AddMenuItem("alle Gasflaschen füllen", "FillGas",  CGBE, caller, nil, 1);
  }
  // Gasflasche leeren
  if (((fill ?? 0)+1400) <= (GetH2MaxFill() ?? 0)) {
    if (FindContents(CGBF))       AddMenuItem("Gasflasche leeren",       "EmptyGas", CGBF, caller);
    if ((ContentsCount(CGBF) ?? 0) >= 2) AddMenuItem("alle Gasflaschen leeren", "EmptyGas", CGBF, caller, nil, 1);
  }
  return(1);
}

protected func ControlThrow()
{
  var fuel = FindContents(WBRL);
  if (!fuel) return(nil);
  
  if (((fill2 ?? 0)+(LocalN("iFillLevel",fuel) ?? 0)) > (GetMaxFill() ?? 0)) return(Message("Wassertank ist voll"));
 
  S42MigrationAssign(fill2, "+=", (LocalN("iFillLevel",fuel) ?? 0));
  ChangeDef(BARL,fuel);  
  return(1);
}

private func Infos()
{
  Message("Wassertank: %d % voll | H2-Tank: %d % voll | mögliche H2-Füllungen: %d", this(), (100*(fill2 ?? 0))/(GetMaxFill() ?? 0), (100*(fill ?? 0))/(GetH2MaxFill() ?? 0), (fill ?? 0)/1400);
  return(1);
}

// Gasflaschen füllen / leeren

private func FillGas()
{
  var bottle = FindContents(Par(nil));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) {
    if ((fill ?? 0) < 1400) return(Sound("Error"));
    S42MigrationAssign(fill, "-=", 1400);
    bottle->Fill();
    return(1);
  }
  // oder alles, was geht?
  while (And(FindContents(Par(nil)), ((fill ?? 0) >= 1400))) {
    bottle = FindContents(Par(nil));
    S42MigrationAssign(fill, "-=", 1400);
    bottle->Fill();
  }   
  return(1);
}

private func EmptyGas()
{
  var bottle = FindContents(Par(nil));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) {
    if (((fill ?? 0)+1400) > (GetH2MaxFill() ?? 0)) return(Message("Wasserstofftank ist voll",this()));
    S42MigrationAssign(fill, "+=", 1400);
    bottle->Empty();
    return(1);
  }
  // oder alles, was geht?
  while (And(FindContents(Par(nil)), (((fill ?? 0)+1400) <= (GetH2MaxFill() ?? 0)))) {
    bottle = FindContents(Par(nil));
    S42MigrationAssign(fill, "+=", 1400);
    bottle->Empty();
  }   
  return(1); 
}

public func EnoughEnergy() { return((GetEnergy() ?? 0) >= 10); }

public func EMP_Reaction() { SetAction("EndWorking"); return(1); }

// Eigenschaften

public func GetMaxDamage() { return(100); }
public func GetFillStatus() { return(fill); }
protected func GetMaxFill() { return(7500); }
protected func GetH2MaxFill() { return(28000); }
public func InfoBarSize() { return(4); }
public func InfoBarPos() { return(4); }
