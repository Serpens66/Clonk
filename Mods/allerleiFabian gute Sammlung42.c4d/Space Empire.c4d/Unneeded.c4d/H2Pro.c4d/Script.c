/*-- H2 Produktion --*/

#strict
#include BOB2 // Gebäude-Hilfsobjekt


local basement, manager, fill, fill2;

protected func Construction()
{
  basement = CreateObject(CB11,0,8,GetOwner());
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
  CastPXS("Water",fill2/5,30);
  CastObjects(CEH2,fill/50,20,5-Random(11),5-Random(11));
  return(1);
}

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(0,-35);  
  return(1);
}


// interne Funktionen

public func FillWithPipe()
{
  if (fill2 < GetMaxFill()) { fill2++;  return(1); }
  return(0);
}

protected func CheckStatus()
{
  if (!FindObject(ENRG)) DoEnergy(100);
  if (!CheckPWL()) return(1);
  
  if ((GetEnergy() < 10) || HasEMP()) return(1);
  
  if (fill2 && (fill < GetH2MaxFill())) {
    if (ActIdle()) SetAction("StartWorking");
    DoEnergy(-7);
    fill2--;
    fill++;
  }
  return(1);
}

private func CheckPWL()
{
  var line;
  if (!FindObject(ENRG)) return(1);
  while (line = FindObject(PWRL,0,0,0,0, 0,"Connect",this(),0, line))
    if (GetActionTarget(1,line) == this())
      return(1);
  return(0);  
}

protected func CheckStatus2()
{
  if (FindObject(ENRG) && (GetEnergy() < 10)) return(1);
  if (!CheckPWL()) return(1); 
  
  if (HasEMP()) return(1);
  
  if (fill2 && (fill < GetH2MaxFill())) SetAction("StartWorking");  
  return(1);
}

// Steuerung

protected func ControlUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func ControlDig(caller)
{
  [Tankoptionen]
  if (HasEMP()) return(Sound("Error"));
  CreateMenu(CH2P,Par(0),0,0,"H2 Produktion",0,1);
  AddMenuItem("Füllstand anzeigen","Infos",INFO,caller);
  
  // Gasflasche füllen
  if (fill >= 1400) {
    if (FindContents(CGBE))       AddMenuItem("Gasflasche füllen",       "FillGas",  CGBE, caller);
    if (ContentsCount(CGBE) >= 2) AddMenuItem("alle Gasflaschen füllen", "FillGas",  CGBE, caller, 0, 1); 
  }
  // Gasflasche leeren
  if ((fill+1400) <= GetH2MaxFill()) {
    if (FindContents(CGBF))       AddMenuItem("Gasflasche leeren",       "EmptyGas", CGBF, caller);
    if (ContentsCount(CGBF) >= 2) AddMenuItem("alle Gasflaschen leeren", "EmptyGas", CGBF, caller, 0, 1);
  }
  return(1);
}

protected func ControlThrow()
{
  var fuel = FindContents(WBRL);
  if (!fuel) return(0);
  
  if ((fill2+LocalN("iFillLevel",fuel)) > GetMaxFill()) return(Message("Wassertank ist voll"));
 
  fill2 += LocalN("iFillLevel",fuel);  
  ChangeDef(BARL,fuel);  
  return(1);
}

private func Infos()
{
  Message("Wassertank: %d % voll | H2-Tank: %d % voll | mögliche H2-Füllungen: %d", this(), (100*fill2)/GetMaxFill(), (100*fill)/GetH2MaxFill(), fill/1400);
  return(1);
}

// Gasflaschen füllen / leeren

private func FillGas()
{
  var bottle = FindContents(Par(0));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) {
    if (fill < 1400) return(Sound("Error"));
    fill-=1400;
    bottle->Fill();
    return(1);
  }
  // oder alles, was geht?
  while (FindContents(Par(0)) && (fill >= 1400)) { 
    bottle = FindContents(Par(0));
    fill-=1400; 
    bottle->Fill();
  }   
  return(1);
}

private func EmptyGas()
{
  var bottle = FindContents(Par(0));
  if (!bottle) return(Sound("Error"));
  
  // eine Gasflasche?
  if (!Par(1)) {
    if ((fill+1400) > GetH2MaxFill()) return(Message("Wasserstofftank ist voll",this()));
    fill+=1400;
    bottle->Empty();
    return(1);
  }
  // oder alles, was geht?
  while (FindContents(Par(0)) && ((fill+1400) <= GetH2MaxFill())) { 
    bottle = FindContents(Par(0));
    fill+=1400; 
    bottle->Empty();
  }   
  return(1); 
}

public func EnoughEnergy() { return(GetEnergy() >= 10); }

public func EMP_Reaction() { SetAction("EndWorking"); return(1); }

// Eigenschaften

public func GetMaxDamage() { return(100); }
public func GetFillStatus() { return(fill); }
protected func GetMaxFill() { return(7500); }
protected func GetH2MaxFill() { return(28000); }
public func InfoBarSize() { return(4); }
public func InfoBarPos() { return(4); }
