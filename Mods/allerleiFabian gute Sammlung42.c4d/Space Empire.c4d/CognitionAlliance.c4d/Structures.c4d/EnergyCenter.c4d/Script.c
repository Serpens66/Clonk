/*-- Energiezentrum --*/

#strict 3
#include BOB1 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, Antenna, glow, FillEnergy, switch, manager;

protected func Construction()
{
  basement=CreateObject(BS15,nil,8,GetOwner());
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

  if (Antenna) RemoveObject(Antenna);
  if (glow) RemoveObject(glow);  
  return(inherited());
}

public func SpecialExplosion()
{
  // EMP auslösen
  if (!FillEnergy) return(1);
  var obj = CreateObject(SWV2);
  obj->SetSize((FillEnergy ?? 0)/10,GetX(),GetY(), nil,(FillEnergy ?? 0)/7);
  return(1);
}

protected func Initialize()
{
  manager=CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,-1);
  
  Antenna = CreateObject(CAN1,nil,-5,GetOwner());
  Antenna->Init(this());
  glow = CreateObject(CLE1,-2,26,GetOwner());
  glow->Init(this());
  switch = 1;  
  return(1);
}

protected func CheckEnergy()
{
  // Energie versenden
  if (switch) TransmitEnergy(); // hat immer nur 50 als GetEnergy Wert, kann also nie mehr als 50 aufeinmal abgeben
  // internen Energiewert auf das Maximum 100 bringen, um es danach sicher auf 50 zu bringen
  var nrg = Min(100-(GetEnergy() ?? 0), FillEnergy);
  DoEnergy(+(nrg ?? 0));
  S42MigrationAssign(FillEnergy, "-=", (nrg ?? 0));
  // Regelniveau: GetEnergy() = 50
  nrg = BoundBy(50-(GetEnergy() ?? 0), (FillEnergy ?? 0)-(MaxIntern() ?? 0),   // nicht mehr abgeben, als da ist ... dieses verändern der energie sorgt dafuer, dass hinterher genau 50 zur verfügung stehen, welche zb. durch leitungen an andere Gebäude übertragen werden kann. Der Rest bleibt im pseudo Speicher, der bis zu 3500 fasst
                                FillEnergy);                  // und Batterie nicht überfüllen
  DoEnergy(+(nrg ?? 0));
  S42MigrationAssign(FillEnergy, "-=", (nrg ?? 0));  // auch wenn es so aussieht, als wuerde es hier weniger werden, kann nrg auch negativ sein, dadurch also mehr werden!
  // alles was über 50GetEnergy im Gebäude ist, landet im FillEnergy oder im Energiemodul. Wenn weniger als 50 da ist, wird von dort nachgepumpt
  
  // JETZT weitere Checks über evlt vorhandene Energiemodule, falls Energy des Gebäudes mehr oder weniger als 50 ist (weil interner speicher leer oder voll)
  if(!Equal(GetEnergy(), 50))
    for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
    {
        if(!Equal(GetEnergy(), 50))
        {
            var nrg = Min(100-(GetEnergy() ?? 0), batt->GetStatus()); // dieses auffüllen auf hundert ist nur dazu da, um im nächsten schritt sichergehen zu können, dass am ende genau 50 da sind
            DoEnergy(+(nrg ?? 0));
            batt->ReduceFill(nrg);
            nrg = BoundBy(50-(GetEnergy() ?? 0), (batt->GetStatus() ?? 0)-((batt->GetMaxFill()) ?? 0), batt->GetStatus()); // sorgt dafuer, dass immer genau 50 verfügbar ist, die weitergegeben werden kann
            DoEnergy(+(nrg ?? 0));
            batt->ReduceFill(nrg);
        }
    }
  
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), GetFillStatus(), MaxFillEnergy(), nil, 1);
  
  var enrg=nil;
  for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
      enrg=(enrg ?? 0)+((batt->~GetStatus()) ?? 0);
  if(FindObject(ENNN))
      Message(Format("Intern: %v, Speicher: %v,| Gesamt: %v",FillEnergy,enrg,GetFillStatus()),this()); // hat aber immer nur 50 als GetEnergy Wert, kann also nie mehr als 50 aufeinmal abgeben
  
  return(1);
}

// Steuerung 

protected func ContainedUp(caller)
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  // Menü generieren und füllen
  CreateMenu(CENC, caller, nil, nil, "Statusabfrage", nil, 1);
  // Status / Reichweite anzeigen
  AddMenuItem("Status anzeigen",     "ShowInfos", INFO, caller);
  AddMenuItem("Reichweite anzeigen", "ShowInfos", CAN1, caller);  
  return(1);
}

protected func ContainedDig(caller)
{
  [Energieoptionen]
  if (HasEMP()) return(Sound("Error"));
  // Menü generieren und füllen
  CreateMenu(CENC, caller, nil, nil, "Energiespeicher", nil, 1);
  
  // Akkus auffüllen
  if ((FillEnergy ?? 0) >= 100) {
    if (FindContents(CNBE))       AddMenuItem("Akku auffüllen",       "FillAccu", CNBE, caller);
    if ((ContentsCount(CNBE) ?? 0) >= 2) AddMenuItem("alle Akkus auffüllen", "FillAccu", CNBE, caller, ContentsCount(CNBE), 1);
  }
  // Akkus leeren
  if ((FillEnergy ?? 0) <= ((MaxFillEnergy() ?? 0)-100)) {
    if (FindContents(CNBF))       AddMenuItem("Akku leeren",       "EmptyAccu", CNBF, caller);
    if ((ContentsCount(CNBF) ?? 0) >= 2) AddMenuItem("alle Akkus leeren", "EmptyAccu", CNBF, caller, ContentsCount(CNBF), 1);
  }
  // Energieübermittlung (de-)aktivieren
  AddMenuItem("Übertragung an/aus", "SwitchMode", CAN1, caller);
}

private func SwitchMode()
{
  if (!switch) { 
    Antenna->Switch(1);
    Message("eingeschaltet",this()); switch = 1;
    return(Sound("Energize")); 
  }
  Antenna->Switch(nil);
  Message("ausgeschaltet",this()); switch = nil;
  Sound("DeEnergize"); 
  return(1);
}

private func ShowInfos()
{
  if (Equal(Par(nil), INFO)) {
    ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
    ShowInfoBar(IBRE,this(),GetFillStatus(),MaxFillEnergy(),5,1);
    return(1);
  } 
  // Reichweite anzeigen 
  var i, marker;
  for (i = nil; (i ?? 0) < 72; S42MigrationStep(i, 1, true))
  {
    marker = CreateObject(SMK1, Cos((i ?? 0)*5,GetRange()), Sin((i ?? 0)*5,GetRange()), GetOwner());
    marker->SetTimer(15);
  }
  Message("Reichweite wird angezeigt",this());
  return(1);      
} 

protected func FillAccu()
{
  var accu = FindContents(Par(nil));
  if (!accu) return(Sound("Error"));
  
  // ein Akku?
  if (!Par(1)) {
    if ((FillEnergy ?? 0) < 100) return(Sound("Error"));
    S42MigrationAssign(FillEnergy, "-=", 100);
    accu->Fill(); 
    return(1);
  }
  // oder alles, was geht?
  while (And(FindContents(Par(nil)), ((FillEnergy ?? 0) >= 100))) {
    accu = FindContents(Par(nil));
    S42MigrationAssign(FillEnergy, "-=", 100);
    accu->Fill();  
  } 
  return(1);
}

protected func EmptyAccu()
{
  var accu = FindContents(Par(nil));
  if (!accu) return(Sound("Error"));
  
  // ein Akku ?
  if (!Par(1)) { 
    if (((FillEnergy ?? 0)+100) > (MaxFillEnergy() ?? 0)) return(Sound("Error"));
    S42MigrationAssign(FillEnergy, "+=", 100);
    accu->Empty(); 
    return(1);
  }
  // oder alles, was geht?
  while (And(FindContents(Par(nil)), (((FillEnergy ?? 0)+100) <= (MaxFillEnergy() ?? 0)))) {
    accu = FindContents(Par(nil));
    S42MigrationAssign(FillEnergy, "+=", 100);
    accu->Empty();  
  } 
  return(1);
}

public func EMP_Reaction()
{
  Antenna->Switch(nil);
  switch = nil;
  Sound("DeEnergize"); 
  return(1);
}

/*-- 
folgende Problematik:

Am einfachsten wäre es, wenn das maximum, was fillenergy annehmen kann, durch das vorhandensein von Energiemodulen einfach ansteigen würde, ohne das irgendwie energie in die dinger geladen wird.
nur dann hätte man das problem, dass wenn man sie raus nimmt, die energie sofort wieder auf 3500 reduziert werden würde und die energie quasi verpufft ist, da auch nicht in den moduelen gespeichert.
Daher halt nun das speochern im energiemodul.

Dennoch solte FillEnergy weiterhin auf 3500 begrenzt sein und nicht weiter anwachsen können.
Daher wäre es sinnvoll, direkt nach dem 3500check, den check für die energiemodule zu machen. 
Anstatt dass diese checks nun unabhängig gemacht werden (was zu komplikationen führen kann), werden sie nun alle im energiezntrum skript gemacht.





 --*/



// Eigenschaften :

public func EnoughEnergy()     { return((GetEnergy() ?? 0) >= 5); }
public func GetRange()         { return(1000);              }

public func GetFillStatus()    { 
  var enrg=nil;
  // for (var i = 0; i < ContentsCount(CENM); i++) enrg += FindContents(CENM)->~GetStatus();
  for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
      enrg=(enrg ?? 0)+((batt->~GetStatus()) ?? 0);
  return((FillEnergy ?? 0)+(enrg ?? 0));
}
public func MaxFillEnergy()    { 
  if (!FindContents(CENM)) return(MaxIntern());
  var enrg = FindContents(CENM)->~GetMaxFill();
  return((MaxIntern() ?? 0)+(ContentsCount(CENM) ?? 0)*(enrg ?? 0));
}

func MaxIntern()
{
    return(3500);
}

public func GetMaxDamage()     { return(100);                }
public func IsEnergyConsumer() { return(1);                 }

public func InfoBarSize()  { return(3); }
public func InfoBarPos()   { return(4); }
private func InfoBarText()
{        
  if (Equal(Par(nil), IBRE))
    return(Format("@Füllstand:  %d%", (Par(1) ?? 0)*100/(Par(2) ?? 0)));
  return(nil);
}    

