/*-- H2-Speicher --*/

#strict 3
#include BOB1  // Gebäude-Hilfsobjekt
#include XMTR  // Transmitter


local basement, vents, fill, mode,caller,manager;


protected func Construction()
{
  basement = CreateObject(BS23,nil,8,GetOwner());
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
  if (vents) RemoveObject(vents);
  if (fill) CastObjects(CEH2,(fill ?? 0)/10,30);
  if(manager) RemoveObject(manager);
  return(inherited());
}

protected func Initialize() 
{
  vents = CreateObject(VNNN,nil,nil,GetOwner());
  vents->Init(this());
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(nil,-35);
  // Energiespeichermodus
  mode=1;
  return(1);
}

// Timer-Call

protected func Convert()
{
  if (mode) { // Batteriemodus
    // internen Energiewert auf das Maximum 100 bringen
    var nrg = Min(100-(GetEnergy() ?? 0), fill);
    DoEnergy(+(nrg ?? 0));
    S42MigrationAssign(fill, "-=", (nrg ?? 0));
    TransmitEnergy();
    // Regelniveau: GetEnergy() = 50
    nrg = BoundBy(50-(GetEnergy() ?? 0), (fill ?? 0)-(MaxFill() ?? 0),fill);
    DoEnergy(+(nrg ?? 0));
    S42MigrationAssign(fill, "-=", (nrg ?? 0));
//  if ((nrg > 0) && !Random(5)) {  // Wenn Energie angegeben wird, die Schlote rauchen lassen
    if ((nrg ?? 0) > 0) {  // Wenn Energie angegeben wird, die Schlote rauchen lassen
     Smoke(34,-26,10);
     Smoke(42,-26,10);
    }
    // Update für Anzeige
    ShowInfoBar(IBRE, this(), fill, MaxFill(), nil, 1);
    return(1);
  }
  // H2-Speichermodus
  if (And(((GetEnergy() ?? 0) > 5), ((fill ?? 0) < ((MaxFill() ?? 0)-5)))) {
    S42MigrationAssign(fill, "+=", 1);
    DoEnergy(-5);   
    // Update für Anzeige
    ShowInfoBar(IBRE, this(), GetFillStatus(), MaxFill(), nil, 1);
  }
  return(1);
}

// Statusabfrage

protected func ContainedUp()
{
  [Statusabfrage]
  ShowInfoBar(IBRS, this(), (GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(), 5);
  ShowInfoBar(IBRE, this(), GetFillStatus(),                MaxFill(), 5, 1);
  return(1);
}

// Aufträge

protected func ContainedDig()
{
  [Aufträge]
  caller = Par(nil);
  CreateMenu(CH2R,caller,nil,nil,"Einstellungen");
  // Modus wechesln (mit unterschiedlichen Symbolen)
  AddMenuItem("Modus wechseln","SetMode",SYHR,caller,nil,nil,"Wechselt zwischen Batterie- und H2-Speichermodus",2,mode);
  // Energieoptionen
  if (And(FindContents(CNBE), ((fill ?? 0) > 100)))
    AddMenuItem("Akkus auffüllen", "NRGMenu", CNBE, caller, nil,nil,"Lädt leere Akkus wieder auf",nil,caller);
  if (And(FindContents(CNBF), (((fill ?? 0)+100) < (MaxFill() ?? 0))))
    AddMenuItem("Akkus leeren", "NRGMenu", CNBF, caller, nil,1,"Entleert volle Akkus in den Energiespeicher",nil,caller);
  // Wasserstoffoptionen
  if (And(FindContents(CGBE), ((fill ?? 0) > 100)))
    AddMenuItem("Wasserstoff abfüllen", "H2Menu",CGBE,caller,nil,nil,"Fülle Wasserstoff aus dem Tank in Gasflaschen ab",nil,caller);
  if (And(FindContents(CGBF), (((fill ?? 0)+100) < (MaxFill() ?? 0))))
    AddMenuItem("Wasserstoff einfüllen", "H2Menu",CGBF,caller,nil,1,"Fülle den Wasserstoff aus Gasflaschen in den Tank",nil,caller);
  return(1);
}

private func SetMode()
{
  mode = !mode;
  if (mode) return(Message("Batteriemodus",this()));
  return(Message("H2-Speichermodus",this()));
}

protected func NRGMenu()
{
  CreateMenu(Par(nil),caller);
  if (Par(1)) { // Tank füllen
    AddMenuItem("Einen Akku", "NRGOption",CNBF,caller,nil,nil);
    AddMenuItem("Alle Akkus", "NRGOption",CNBF,caller,nil,1);
  }
  if (!Par(1)) { // Tank leeren
    AddMenuItem("Einen Akku", "NRGOption",CNBE,caller,nil,nil);
    AddMenuItem("Alle Akkus", "NRGOption",CNBE,caller,nil,1);
  }
  return(1);
}

protected func H2Menu()
{
  CreateMenu(Par(nil),caller);
  if (Par(1)) { // Tank füllen
    AddMenuItem("Eine Gasflasche", "GasOption",CGBF,caller,nil,nil);
    AddMenuItem("Alle Gasflaschen", "GasOption",CGBF,caller,nil,1);
  }
  if (!Par(1)) { // Tank leeren
    AddMenuItem("Eine Gasflasche", "GasOption",CGBE,caller,nil,nil);
    AddMenuItem("Alle Gasflaschen", "GasOption",CGBE,caller,nil,1);
  }
  return(1);
}

protected func NRGOption()
{
  if (Equal(Par(nil), CNBE)) {  // Tank leeren
    if (!Par(1)) {  // Einen Akku
      S42MigrationAssign(fill, "-=", 100);
      FindContents(CNBE)->Fill();
      return(1);
    }
    while (And(((fill ?? 0) >= 100), FindContents(CNBE))) {  // oder alle
      S42MigrationAssign(fill, "-=", 100);
      FindContents(CNBE)->Fill();
    }
  }
  if (Equal(Par(nil), CNBF)) {  // Tank füllen
    if (!Par(1)) {  // Einen Akku
      S42MigrationAssign(fill, "+=", 1000);
      FindContents(CNBF)->Empty();
      return(1);
    }
    while (And(((fill ?? 0) <= ((MaxFill() ?? 0)-100)), FindContents(CNBF))) {  // oder alle
      S42MigrationAssign(fill, "+=", 1000);
      FindContents(CNBF)->Empty();
    }
  }
  return(1);
}


protected func GasOption()
{
  if (Equal(Par(nil), CGBE)) {  // Tank leeren
    if (!Par(1)) {  // Eine Flasche
      S42MigrationAssign(fill, "-=", 100);
      FindContents(CGBE)->Fill();
      return(1);
    }
    while (And(((fill ?? 0) >= 100), FindContents(CGBE))) {  // oder alle
      S42MigrationAssign(fill, "-=", 100);
      FindContents(CGBE)->Fill();
    }
  }
  if (Equal(Par(nil), CGBF)) {  // Tank füllen
    if (!Par(1)) {  // Eine Flasche
      S42MigrationAssign(fill, "+=", 1000);
      FindContents(CGBF)->Empty();
      return(1);
    }
    while (And(((fill ?? 0) <= ((MaxFill() ?? 0)-100)), FindContents(CGBF))) {  // oder alle
      S42MigrationAssign(fill, "+=", 1000);
      FindContents(CGBF)->Empty();
    }
  }
  return(1);
}

// Eigenschaften

public func GetMaxDamage() { return(100); }

public func MaxFill() { 
  if (!FindContents(CENM)) return(5000);
  var enrg = FindContents(CENM)->~GetMaxFill();
  return(5000+(ContentsCount(CENM) ?? 0)*(enrg ?? 0));
}

public func GetFillStatus() {
  var enrg;
  for (var i = 1; (i ?? 0) < (ContentsCount(CENM) ?? 0); S42MigrationStep(i, 1, true)) S42MigrationAssign(enrg, "+=", (FindContents(CENM)->~GetStatus() ?? 0));
  return((fill ?? 0)+(enrg ?? 0));
}

public func EnoughEnergy()     { return((GetEnergy() ?? 0) >= 10); }
public func InfoBarSize() { return(5); }
public func InfoBarPos() { return(4); }
public func IsEnergyConsumer() { return(1); }
