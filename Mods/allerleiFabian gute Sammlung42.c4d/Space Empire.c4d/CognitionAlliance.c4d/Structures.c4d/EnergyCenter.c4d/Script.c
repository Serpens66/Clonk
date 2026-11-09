/*-- Energiezentrum --*/

#strict
#include BOB1 // Gebäude-Hilfsobjekt
#include XMTR // Transmitter

local basement, Antenna, glow, FillEnergy, switch, manager;

protected func Construction()
{
  basement=CreateObject(BS15,0,8,GetOwner());
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
  obj->SetSize(FillEnergy/10,GetX(),GetY(), 0,FillEnergy/7);
  return(1);
}

protected func Initialize()
{
  manager=CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,-1);
  
  Antenna = CreateObject(CAN1,0,-5,GetOwner());
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
  var nrg = Min(100-GetEnergy(), FillEnergy);
  DoEnergy(+nrg);
  FillEnergy -= nrg;
  // Regelniveau: GetEnergy() = 50
  nrg = BoundBy(50-GetEnergy(), FillEnergy-MaxIntern(),   // nicht mehr abgeben, als da ist ... dieses verändern der energie sorgt dafuer, dass hinterher genau 50 zur verfügung stehen, welche zb. durch leitungen an andere Gebäude übertragen werden kann. Der Rest bleibt im pseudo Speicher, der bis zu 3500 fasst
                                FillEnergy);                  // und Batterie nicht überfüllen
  DoEnergy(+nrg);   
  FillEnergy -= nrg;  // auch wenn es so aussieht, als wuerde es hier weniger werden, kann nrg auch negativ sein, dadurch also mehr werden!
  // alles was über 50GetEnergy im Gebäude ist, landet im FillEnergy oder im Energiemodul. Wenn weniger als 50 da ist, wird von dort nachgepumpt
  
  // JETZT weitere Checks über evlt vorhandene Energiemodule, falls Energy des Gebäudes mehr oder weniger als 50 ist (weil interner speicher leer oder voll)
  if(GetEnergy()!=50)
    for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
    {
        if(GetEnergy()!=50)
        {
            var nrg = Min(100-GetEnergy(), batt->GetStatus()); // dieses auffüllen auf hundert ist nur dazu da, um im nächsten schritt sichergehen zu können, dass am ende genau 50 da sind
            DoEnergy(+nrg);
            batt->ReduceFill(nrg);
            nrg = BoundBy(50-GetEnergy(), batt->GetStatus()-(batt->GetMaxFill()), batt->GetStatus()); // sorgt dafuer, dass immer genau 50 verfügbar ist, die weitergegeben werden kann
            DoEnergy(+nrg);
            batt->ReduceFill(nrg);
        }
    }
  
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), GetFillStatus(), MaxFillEnergy(), 0, 1);  
  
  var enrg=0;
  for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
      enrg=enrg+(batt->~GetStatus());
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
  CreateMenu(CENC, caller, 0, 0, "Statusabfrage", 0, 1);
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
  CreateMenu(CENC, caller, 0, 0, "Energiespeicher", 0, 1);
  
  // Akkus auffüllen
  if (FillEnergy >= 100) {
    if (FindContents(CNBE))       AddMenuItem("Akku auffüllen",       "FillAccu", CNBE, caller);
    if (ContentsCount(CNBE) >= 2) AddMenuItem("alle Akkus auffüllen", "FillAccu", CNBE, caller, ContentsCount(CNBE), 1);
  }
  // Akkus leeren
  if (FillEnergy <= (MaxFillEnergy()-100)) {  
    if (FindContents(CNBF))       AddMenuItem("Akku leeren",       "EmptyAccu", CNBF, caller);
    if (ContentsCount(CNBF) >= 2) AddMenuItem("alle Akkus leeren", "EmptyAccu", CNBF, caller, ContentsCount(CNBF), 1);
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
  Antenna->Switch(0);
  Message("ausgeschaltet",this()); switch = 0;
  Sound("DeEnergize"); 
  return(1);
}

private func ShowInfos()
{
  if (Par(0) == INFO) { 
    ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
    ShowInfoBar(IBRE,this(),GetFillStatus(),MaxFillEnergy(),5,1);
    return(1);
  } 
  // Reichweite anzeigen 
  var i, marker;
  for (i = 0; i < 72; i++)
  {
    marker = CreateObject(SMK1, Cos(i*5,GetRange()), Sin(i*5,GetRange()), GetOwner());
    marker->SetTimer(15);
  }
  Message("Reichweite wird angezeigt",this());
  return(1);      
} 

protected func FillAccu()
{
  var accu = FindContents(Par(0));
  if (!accu) return(Sound("Error"));
  
  // ein Akku?
  if (!Par(1)) {
    if (FillEnergy < 100) return(Sound("Error")); 
    FillEnergy -= 100; 
    accu->Fill(); 
    return(1);
  }
  // oder alles, was geht?
  while (FindContents(Par(0)) && (FillEnergy >= 100)) {
    accu = FindContents(Par(0));
    FillEnergy -= 100; 
    accu->Fill();  
  } 
  return(1);
}

protected func EmptyAccu()
{
  var accu = FindContents(Par(0));
  if (!accu) return(Sound("Error"));
  
  // ein Akku ?
  if (!Par(1)) { 
    if ((FillEnergy+100) > MaxFillEnergy()) return(Sound("Error"));
    FillEnergy += 100; 
    accu->Empty(); 
    return(1);
  }
  // oder alles, was geht?
  while (FindContents(Par(0)) && ((FillEnergy+100) <= MaxFillEnergy())) {
    accu = FindContents(Par(0));
    FillEnergy += 100; 
    accu->Empty();  
  } 
  return(1);
}

public func EMP_Reaction()
{
  Antenna->Switch(0);
  switch = 0;
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

public func EnoughEnergy()     { return(GetEnergy() >= 5); }
public func GetRange()         { return(1000);              }

public func GetFillStatus()    { 
  var enrg=0;
  // for (var i = 0; i < ContentsCount(CENM); i++) enrg += FindContents(CENM)->~GetStatus();
  for(var batt in FindObjects(Find_ID(CENM),Find_Container(this())))
      enrg=enrg+(batt->~GetStatus());
  return(FillEnergy+enrg); 
}
public func MaxFillEnergy()    { 
  if (!FindContents(CENM)) return(MaxIntern());
  var enrg = FindContents(CENM)->~GetMaxFill();
  return(MaxIntern()+ContentsCount(CENM)*enrg); 
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
  if (Par(0) == IBRE)       
    return(Format("@Füllstand:  %d%", Par(1)*100/Par(2)));
  return(0); 
}    

