/*-- H2-Speicher --*/

#strict
#include BOB1  // Gebäude-Hilfsobjekt
#include XMTR  // Transmitter


local basement, vents, fill, mode,caller,manager;


protected func Construction()
{
  basement = CreateObject(BS23,0,8,GetOwner()); 
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
  if (fill) CastObjects(CEH2,fill/10,30);
  if(manager) RemoveObject(manager);
  return(inherited());
}

protected func Initialize() 
{
  vents = CreateObject(VNNN,0,0,GetOwner());
  vents->Init(this());
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(0,-35);
  // Energiespeichermodus
  mode=1;
  return(1);
}

// Timer-Call

protected func Convert()
{
  if (mode) { // Batteriemodus
    // internen Energiewert auf das Maximum 100 bringen
    var nrg = Min(100-GetEnergy(), fill);
    DoEnergy(+nrg);
    fill -= nrg;
    TransmitEnergy();
    // Regelniveau: GetEnergy() = 50
    nrg = BoundBy(50-GetEnergy(), fill-MaxFill(),fill);                 
    DoEnergy(+nrg);
    fill -= nrg;
//  if ((nrg > 0) && !Random(5)) {  // Wenn Energie angegeben wird, die Schlote rauchen lassen
    if (nrg > 0) {  // Wenn Energie angegeben wird, die Schlote rauchen lassen
     Smoke(34,-26,10);
     Smoke(42,-26,10);
    }
    // Update für Anzeige
    ShowInfoBar(IBRE, this(), fill, MaxFill(), 0, 1);
    return(1);
  }
  // H2-Speichermodus
  if ((GetEnergy() > 5) && (fill < (MaxFill()-5))) {
    fill+=1;
    DoEnergy(-5);   
    // Update für Anzeige
    ShowInfoBar(IBRE, this(), GetFillStatus(), MaxFill(), 0, 1);
  }
  return(1);
}

// Statusabfrage

protected func ContainedUp()
{
  [Statusabfrage]
  ShowInfoBar(IBRS, this(), GetMaxDamage()-GetDamage(),GetMaxDamage(), 5);
  ShowInfoBar(IBRE, this(), GetFillStatus(),                MaxFill(), 5, 1);
  return(1);
}

// Aufträge

protected func ContainedDig()
{
  [Aufträge]
  caller = Par(0);
  CreateMenu(CH2R,caller,0,0,"Einstellungen");
  // Modus wechesln (mit unterschiedlichen Symbolen)
  AddMenuItem("Modus wechseln","SetMode",SYHR,caller,0,0,"Wechselt zwischen Batterie- und H2-Speichermodus",2,mode);
  // Energieoptionen
  if (FindContents(CNBE) && (fill > 100))
    AddMenuItem("Akkus auffüllen", "NRGMenu", CNBE, caller, 0,0,"Lädt leere Akkus wieder auf",0,caller);
  if (FindContents(CNBF) && ((fill+100) < MaxFill()))
    AddMenuItem("Akkus leeren", "NRGMenu", CNBF, caller, 0,1,"Entleert volle Akkus in den Energiespeicher",0,caller);
  // Wasserstoffoptionen
  if (FindContents(CGBE) && (fill > 100)) 
    AddMenuItem("Wasserstoff abfüllen", "H2Menu",CGBE,caller,0,0,"Fülle Wasserstoff aus dem Tank in Gasflaschen ab",0,caller);
  if (FindContents(CGBF) && ((fill+100) < MaxFill())) 
    AddMenuItem("Wasserstoff einfüllen", "H2Menu",CGBF,caller,0,1,"Fülle den Wasserstoff aus Gasflaschen in den Tank",0,caller);
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
  CreateMenu(Par(0),caller);
  if (Par(1)) { // Tank füllen
    AddMenuItem("Einen Akku", "NRGOption",CNBF,caller,0,0);
    AddMenuItem("Alle Akkus", "NRGOption",CNBF,caller,0,1);
  }
  if (!Par(1)) { // Tank leeren
    AddMenuItem("Einen Akku", "NRGOption",CNBE,caller,0,0);
    AddMenuItem("Alle Akkus", "NRGOption",CNBE,caller,0,1);
  }
  return(1);
}

protected func H2Menu()
{
  CreateMenu(Par(0),caller);
  if (Par(1)) { // Tank füllen
    AddMenuItem("Eine Gasflasche", "GasOption",CGBF,caller,0,0);
    AddMenuItem("Alle Gasflaschen", "GasOption",CGBF,caller,0,1);
  }
  if (!Par(1)) { // Tank leeren
    AddMenuItem("Eine Gasflasche", "GasOption",CGBE,caller,0,0);
    AddMenuItem("Alle Gasflaschen", "GasOption",CGBE,caller,0,1);
  }
  return(1);
}

protected func NRGOption()
{
  if (Par(0) == CNBE) {  // Tank leeren
    if (!Par(1)) {  // Einen Akku
      fill-=100;
      FindContents(CNBE)->Fill();
      return(1);
    }
    while ((fill >= 100) && FindContents(CNBE)) {  // oder alle
      fill-=100;
      FindContents(CNBE)->Fill();
    }
  }
  if (Par(0) == CNBF) {  // Tank füllen
    if (!Par(1)) {  // Einen Akku
      fill+=1000;
      FindContents(CNBF)->Empty();
      return(1);
    }
    while ((fill <= (MaxFill()-100)) && FindContents(CNBF)) {  // oder alle
      fill+=1000;
      FindContents(CNBF)->Empty();
    }
  }
  return(1);
}


protected func GasOption()
{
  if (Par(0) == CGBE) {  // Tank leeren
    if (!Par(1)) {  // Eine Flasche
      fill-=100;
      FindContents(CGBE)->Fill();
      return(1);
    }
    while ((fill >= 100) && FindContents(CGBE)) {  // oder alle
      fill-=100;
      FindContents(CGBE)->Fill();
    }
  }
  if (Par(0) == CGBF) {  // Tank füllen
    if (!Par(1)) {  // Eine Flasche
      fill+=1000;
      FindContents(CGBF)->Empty();
      return(1);
    }
    while ((fill <= (MaxFill()-100)) && FindContents(CGBF)) {  // oder alle
      fill+=1000;
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
  return(5000+ContentsCount(CENM)*enrg); 
}

public func GetFillStatus() {
  var enrg;
  for (var i = 1; i < ContentsCount(CENM); i++) enrg += FindContents(CENM)->~GetStatus();
  return(fill+enrg);
}

public func EnoughEnergy()     { return(GetEnergy() >= 10); }
public func InfoBarSize() { return(5); }
public func InfoBarPos() { return(4); }
public func IsEnergyConsumer() { return(1); }
