/*-- Brennstoffzelle --*/

#strict 3
#include BOB2 // Gebäude-Hilfsobjekt


local basement, fillstatus, manager;

protected func Construction()
{
  basement = CreateObject(CB10,nil,8,GetOwner());
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
  // Wasserstoff freisetzen
  CastObjects(CEH2,(fillstatus ?? 0)/100,30,5-(Random(11) ?? 0),5-(Random(11) ?? 0));
  return(1);
}

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-35,31);
  return(1);
}

// Steuerung

protected func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),fillstatus,MaxFill(),5,1);
  return(1);
}

protected func ControlDig(caller)
{
  [Tankoptionen]
  if (HasEMP()) return(Sound("Error"));
  
  CreateMenu(CFCP,caller,nil,nil,"Brennstoffelle",nil,1);
  AddMenuItem("Tankstatus anzeigen","TankInfo",INFO,caller);
  
  if ((fillstatus ?? 0) >= 900) {
    if (FindContents(CGBE))       AddMenuItem("Gasflasche füllen",       "FillGas",  CGBE, caller);
    if ((ContentsCount(CGBE) ?? 0) >= 2) AddMenuItem("alle Gasflaschen füllen", "FillGas",  CGBE, caller, nil, 1);
  }
  if ((fillstatus ?? 0) <= ((MaxFill() ?? 0)-900)) {
    if (FindContents(CGBF))       AddMenuItem("Gasflasche leeren",       "EmptyGas", CGBF, caller);
    if ((ContentsCount(CGBF) ?? 0) >= 2) AddMenuItem("alle Gasflaschen leeren", "EmptyGas", CGBF, caller, nil, 1);
  }
  return(1);
}

private func TankInfo()
{
  if (fillstatus) Message("Wasserstofftank: %d % voll | Reicht für: %d Füllungen", this(),((fillstatus ?? 0)*100)/(MaxFill() ?? 0),(fillstatus ?? 0)/900);
  else Message("Keine Wasserstoffreserven verfügbar",this());
  return(1);
}

// Gasflaschen füllen / leeren

private func FillGas()
{
  var bottle=FindContents(Par(nil));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) { 
    S42MigrationAssign(fillstatus, "-=", 900);
    bottle->Fill();
    return(1);
  } 
  // oder alles, was geht ?
  while (And(FindContents(Par(nil)), ((fillstatus ?? 0) >= 900))) {
    bottle = FindContents(Par(nil));
    S42MigrationAssign(fillstatus, "-=", 900);
    bottle->Fill(); 
  }  
  return(1);
}

private func EmptyGas() 
{
  var bottle=FindContents(Par(nil));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) { 
    if (((fillstatus ?? 0)+900) > (MaxFill() ?? 0)) return(Message("Wasserstofftank ist voll",this()));
    S42MigrationAssign(fillstatus, "+=", 900);
    bottle->Empty();
    return(1);
  }
  // oder alles, was geht
  while (And(FindContents(Par(nil)), (((fillstatus ?? 0)+900) <= (MaxFill() ?? 0)))) {
    bottle = FindContents(Par(nil));
    S42MigrationAssign(fillstatus, "+=", 900);
    bottle->Empty();  
  }
  return(1);
}

// Action-Calls

protected func CheckStatus()
{
  if (HasEMP()) return(1);
  // Actions prüfen und evtl neu einstellen
  if (fillstatus) {
    if (And(ActIdle(), ((GetEnergy() ?? 0) <= 75))) SetAction("BeginGlow");
  } 
  // gegebenenfalls nachladen    
  var new;
  if (And(((fillstatus ?? 0) <= 100), (new = FindContents(CGBF)))) {
    S42MigrationAssign(fillstatus, "+=", 900);
    new->Empty();
  }   
  return(1);
}

protected func CheckWork() // Action-Call bei der Arbeit
{
  if (!FindObject(ENRG)) return(DoEnergy(100));
  if (HasEMP()) return(1);
  if (Or((!CheckPWRL(this())), (!fillstatus))) return(SetAction("EndGlowing")); // kein Bedarf oder kein Sprit mehr?
  if ((GetEnergy() ?? 0) <= 75) {
    DoEnergy(11);
    Smoke(-20,-35,(Random(5) ?? 0)+4);
    Smoke(28,-35,(Random(5) ?? 0)+4);
    S42MigrationStep(fillstatus, -1, true);
  }
  return(1);  
}

protected func CheckWork2()
{
  if (HasEMP()) return(1);
  if (And(And(((GetEnergy() ?? 0) <= 75), CheckPWRL(this())), fillstatus)) {
    var phase = GetPhase();
    SetAction("BeginGlow");
    SetPhase(9-(phase ?? 0));
  }
  return(1);
}

public func EMP_Reaction()
{ 
  if (!ActIdle()) SetAction("EndGlowing");
  return(1);
}

private func Nothing() { return(nil); } // Macht absolut nichts

// Eigenschaften

public func MaxFill() { return(10000); }
public func GetMaxDamage() { return(70); }
public func InfoBarSize() { return(3); }
public func InfoBarPos() { return(4); }
private func InfoBarText()
{        
  if (Equal(Par(nil), IBRE))
    return(Format("@Füllstand:  %d%", (Par(1) ?? 0)*100/(Par(2) ?? 0)));
  return(nil);
}    

