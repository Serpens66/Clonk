/*-- Der Clonk --*/

#strict 3

// Zauberei - benötigt, wenn der Clonk Zielzauber z.B. aus dem Zauberturm zaubert
// Auch benötigt für den König
local pAimer;			// Aktive Zielsteuerung; wird abgrbrochen, wenn der Zauberer gestört wird (Nur Fantasypack)
local pAimedSpell;		// Zauber, der gezielt wird  (Nur Fantasypack)
local pAimedSpellOrigin;        // Objekt, das einen Zielzauber initiiert hat. An dieses werden SpellFailed/SpellSucceeded-Nachrichten weitergeleitet
local isHidden;
local name;
local portraitt,portraittID;
local starterfahrung,stufe;
local bauarbeiter,ausbildungstimer,pinguin;

local zielSichtweite,zahler, SetSichtweite;


private func GetObject2Drop(pCollectObj) { return((Contents(nil))); }  // in enemyAI Core wird dies verwendet und _inherited() aufgerufen. das command ablegen funktioniert nicht mehr, wenn ich diese funktion hier nicht auch zufüge, die sagt, was automatisch abgelegt werden soll, wenn inventar voll.

// je nach Einstellung in den variablen Regeln gibt es unterschiedliches Sichtradienverhalten der Clonks
protected func FogWar() 
{
    if(AntiFussel) AntiFusselFree(nil,nil,nil,nil);
    
    if(FindObject(SWIO)) // Wenn regel des automatischen auftauchens aktiv, dann clonk auftauchen lassen, wenn unter wasser und nicht kontrolliert
    {
        if(And(And(InLiquid(), (GetBreath() ?? 0) <= 45), !Equal(GetComDir(), COMD_Up)))
        {
            var pCrew, i=nil, aktiv=nil;
            while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true)))
                if(Equal(pCrew, this())) // wenn clonk angewählt, alles okay, ist wohl gewollt, nichts machen
                    aktiv=1;
            if(!aktiv)
                SetComDir(COMD_Up);
        }    
    }
        
    
    if(GetLength(hashSpielRegeln))
    zahler=HashGet(hashSpielRegeln, "SichtradiusVerhalten");
    else {zahler=nil;}

    if(Equal(zahler, nil)) {
    // if (FrameCounter()<5000) {  // unabhängig von Framecounter
    if(!Random(100))  // zu anfang ab und zu wieder Kriegsnebel deaktivieren
    for(var i1=nil; (i1 ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i1, 1, true)) {
      SetFoW(nil,i1);} //}
    return(nil);}

    if(!Random(200))  // ab und zu wieder Kriegsnebel erstellen, falls er weggeht
    for(var i1=nil; (i1 ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i1, 1, true)) {
      SetFoW(1,i1);}
       
    if(Equal(zahler, 1))
    FogWar1();

    if(Equal(zahler, 2))
    FogWar2();

    if(Equal(zahler, 3))
    if(!Random(100))  // muss nicht sooft gecheckt werden
    FogWar3();
}

func AntiFusselFree(X,Y,Breite,Hohe) // wird durch TimerCall aufgerufen, wenn globale variable AntiFussel in clonk appendto über contextmenu wahr ist.
{
    if(!X) X = -2; // dies sollten die perfekten einstellungen sein
    if(!Y) Y = -4;
    if(!Breite) Breite = 4;
    if(!Hohe) Hohe = 10;
    FreeRect((GetX(this()) ?? 0)+(X ?? 0),(GetY(this()) ?? 0)+(Y ?? 0),Breite,Hohe,50); // die 50 ist die dichte allen nicht fluessigen materials
}

// Blendfunktion für Blendgranate
func Blend()
{
  // gibt die x und y Koordinaten von Clonk und Granate aus, zu testzwecken.
  //Log(Format("clonk:%v,%v. Granate:%v,%v.",GetX(this()),GetY(this()),Par(0),Par(1)));
 if(Contained()) return; // Wenn in einem Gebäude/Fahrzeug oderso, dann nicht blenden.
 if(Or(Or(PathFree((GetX(this()) ?? 0)-10,GetY(this()),(Par(nil) ?? 0)-3,(Par(1) ?? 0)-5), PathFree((GetX(this()) ?? 0)-10,GetY(this()),(Par(nil) ?? 0)+3,(Par(1) ?? 0)-5)), PathFree((GetX(this()) ?? 0)-10,GetY(this()),Par(nil),(Par(1) ?? 0)-5)))  // wenn direkte Verbindung zwischen Clonk und Explosion
  {
   if(Or(((Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)>1200), (And(Equal(Par(nil), GetX(this())), Equal(Par(1), GetY(this()))))) ) return;  //wenn Distanz größer als 1200, oder Granate dabei (selbe position) dann keine Blendwirkung
  
   // clonk hat reingeschaut und ist näher als 600 dran
   if(And(And((Par(nil) ?? 0)<=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Left)), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=600) )  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this())); ProtectedCall(Var(),"Initialize",nil,this());return;}
   if(And(And((Par(nil) ?? 0)>=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Right)), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=600) )  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this()));ProtectedCall(Var(),"Initialize",nil,this());return;}

   // clonk hat reingeschaut, aber zwischen 600 und 1200 weit weg, dann geht blendung schneller weg
   if(And(And(And((Par(nil) ?? 0)<=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Left)), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)>600), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=1200) )  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this())); ProtectedCall(Var(),"Initialize",1,this());return;}
   if(And(And(And((Par(nil) ?? 0)>=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Right)), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)>600), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=1200) )  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return;}

   // Clonk hat weggeschaut ist aber näher als 600, dann verschwindets schneller wieder.
   if((And((And((Par(nil) ?? 0)<=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Right))), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=600)))  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return;}
   if((And((And((Par(nil) ?? 0)>=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Left))), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=600)))  {Var()=CreateObject(FL_H,nil,nil,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return;}

   // Clonk hat weggeschaut und ist weiter als 600 weg, dann keine Auswirkung
   if((And(And((And((Par(nil) ?? 0)<=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Right))), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)>600), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=1200)))  return;
   if((And(And((And((Par(nil) ?? 0)>=(GetX(this()) ?? 0), Equal(GetDir(),DIR_Left))), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)>600), (Distance(GetX(this()),GetY(this()),Par(2),Par(1)) ?? 0)<=1200)))  return;
  }
 return(1);
}


func CheckzielSichtweite(verhalten) // die zahl ist das kriegsnebelverhalten. Zum bestimmen der Position des Clonks bzgl der sicht. Speichert Ergebnis in lokaler Variable zielSichtweite, sodass diese auch von außerhalb abgefragt werden kann, ohne die ganzen checks nochmal zu machen
{
    if(Equal(verhalten, 1))
    {
        if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
        if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

        var z2=nil;
        while(!GBackSky(AbsX(GetX(nil)),(AbsY(GetY(nil)) ?? 0)-(z2 ?? 0))) S42MigrationStep(z2, 1); // so sollte es jetzt ricihtig sein
        var himmely = (GetY(nil) ?? 0)-(z2 ?? 0); // y koordiante über Clonk, wo das erste mal himmer ist
        // Log(Format("himmely: %v , z:%v",himmely,z2));

          if(
        And((Equal(GetMaterial(), Material("Tunnel")))
         , (!PathFree(GetX(nil),GetY(nil),GetX(nil),himmely)))
        ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagTunnel");  else{zielSichtweite = 150;} } // Tunnel und keine direkte Verbindung zum Himmel? Dann eine Sichtweite von 25 anstreben

          if(
        And(And((Equal(GetMaterial(), Material("Tunnel"))), IsNight())
         , (!PathFree(GetX(nil),GetY(nil),GetX(nil),himmely)))
         ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtTunnel"); else{zielSichtweite = 100;} } // Tunnel+ fehlende Verbindung zum  Himmel? UND Nacht?! Dann eine Sichtweite von 15 anstreben
    }
    
    if(Equal(verhalten, 2))
    {
        if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
        if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben
    }
    
    if(Equal(verhalten, 3))
    {
        if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
    }
    return(zielSichtweite);
}

func GetzielSichtweite() // returned die aktuelle zielsichtweite, damit von außen abgefragt werden kann, ohne neue Checks
{
    return(zielSichtweite); // wird z.b vom Teamview objekt abgefragt
}


// Tag, Nacht und Tunnel Sichtradius
func FogWar1()
{
 // if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  // if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

// var z2=0;
// while(!GBackSky(AbsX(GetX(0)),AbsY(GetY(0))-z2)) ++z2; // so sollte es jetzt ricihtig sein
// var himmely = GetY(0)-z2; // y koordiante über Clonk, wo das erste mal himmer ist

  // if(
// (GetMaterial() == Material("Tunnel"))
 // && (!PathFree(GetX(0),GetY(0),GetX(0),himmely))  
// ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagTunnel");  else{zielSichtweite = 150;} } // Tunnel und keine direkte Verbindung zum Himmel? Dann eine Sichtweite von 25 anstreben

  // if(
// (GetMaterial() == Material("Tunnel")) && IsNight()
 // && (!PathFree(GetX(0),GetY(0),GetX(0),himmely))
 // ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtTunnel"); else{zielSichtweite = 100;} } // Tunnel+ fehlende Verbindung zum  Himmel? UND Nacht?! Dann eine Sichtweite von 15 anstreben
  zielSichtweite = CheckzielSichtweite(1);
  
  if(Equal(SetSichtweite, zielSichtweite)) return(1); // wenn schon gleich ist, dann nichts machen
  
  if((Abs((SetSichtweite ?? 0)-(zielSichtweite ?? 0)) ?? 0)<2) SetSichtweite=zielSichtweite; // wenn Differenz zu klein, dann gleichsetzen.
  else
  {
    if((SetSichtweite ?? 0) > (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "-=", 2);
    if((SetSichtweite ?? 0) < (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "+=", 2);
  }
// Message("%d %d",this(),SetSichtweite,zielSichtweite); 

  SetPlrViewRange(SetSichtweite,nil,1);
}


// nur Tag Nacht Sichtradius
func FogWar2()
{
 // if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  // if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben
  zielSichtweite = CheckzielSichtweite(2);
  if(Equal(SetSichtweite, zielSichtweite)) return(1); // wenn schon gleich ist, dann nichts machen
  
  if((Abs((SetSichtweite ?? 0)-(zielSichtweite ?? 0)) ?? 0)<2) SetSichtweite=zielSichtweite; // wenn Differenz zu klein, dann gleichsetzen.
  else
  {
    if((SetSichtweite ?? 0) > (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "-=", 2);
    if((SetSichtweite ?? 0) < (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "+=", 2);
  }
//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

  SetPlrViewRange(SetSichtweite,nil,1);
}


// Immer Sichtweite 
func FogWar3()
{
 // if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
 zielSichtweite = CheckzielSichtweite(3);
 if(Equal(SetSichtweite, zielSichtweite)) return(1);
 SetSichtweite = zielSichtweite;


//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

  SetPlrViewRange(SetSichtweite,nil,1);
}


func TimerAusbildung()
{
S42MigrationStep(ausbildungstimer, 1);
}

func StandTimerAusbildung()
{
return(ausbildungstimer);
}

func TimerAusbildungNull()
{
ausbildungstimer=nil;
}


//wenn im ausbildungscamp ausgebildet, erhält der Clonk eine neue Stufe. Diese ist wichtig dafür, wann neue fähigkeiten erlernt werden bzw. die Grafik sich ändert.
func Stufenanstieg(zahl)
{
if(Equal(zahl, 1)) {stufe=5; return(nil);} // für den Aufstieg von Clonk zu Hazardclonk die Stufen übernehmen
if(Equal(zahl, 10)) {stufe=10; return(nil);}
if(Equal(zahl, 15)) {stufe=15; return(nil);}
if(Equal(zahl, 20)) {stufe=20; return(nil);}
S42MigrationStep(stufe, 1);
}

func AktuelleStufe()
{
return(stufe);
}

func NunBauarbeiter(zahl)
{
if(Equal(zahl, 6)) {bauarbeiter=6;return;}
S42MigrationStep(bauarbeiter, 1);
if(Equal(zahl, 10)) bauarbeiter=nil;
}

func IstBauarbeiter()
{
return(bauarbeiter);
}



func Klimaschutz(klima) // im hazardskript wird noch auf spezielle kleidung geprueft
{
//Kälte
if(Equal(klima, 1))
{
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, kein Schaden.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_ID(LAGF),Find_ID(TORC),Find_ID(TOR3))))
  {
//nur wenn Feuer brennt
if(Equal(GetID(feuer), LAGF)) if(!(!!SEqual(GetAction(feuer), "Burn"))) return(nil);
 if(Or(Equal(GetID(feuer), TORC), Equal(GetID(feuer), TOR3))) if(!(!!SEqual(GetAction(feuer), "Burning"))) return(nil);
  return(1);
  }
if(feuer = FindObject2(Find_Distance(100),Find_ID(TONN))) {if(!(!!SEqual(GetAction(feuer), "Burn"))) return(nil); return(1);} // wenn feuertonne in nähe
if(FindObject2(Find_Distance(250),Find_ID(MVNT))) return(1); // wenn kleiner lüfter im bunker verbaut sind
if(FindObject2(Find_Distance(500),Find_ID(VENT))) return(1); // wenn großer lüfter im bunker verbaut sind

return(nil);
}

//Hitze
if(Equal(klima, 2))
{
// wenn in Flüssigkeit, kein schaden
if((GetOCF() ?? 0) & (OCF_InLiquid ?? 0)) return(1);
if(FindObject2(Find_Distance(80),Find_NoContainer(),Find_Or(Find_ID(ICE1),Find_ID(WBRL),Find_ID(WDWB),Find_ID(WTON)))) return(1); // wenn eisklumpen oder Wasserfass in der nähe und nicht in gebäude

if(FindObject2(Find_Distance(250),Find_ID(MVNT))) return(1); // wenn kleiner lüfter im bunker verbaut sind
if(FindObject2(Find_Distance(500),Find_ID(VENT))) return(1); // wenn großer lüfter im bunker verbaut sind

return(nil);
}

}

func RundenErfahrung()
{
// gibt die in dieser Runde bereits erhaltene Erfahrung zum Zeitpunkt der Abfrage aus
return((GetObjectInfoCoreVal("Experience", nil, nil) ?? 0)-(starterfahrung ?? 0));
}


public func ResetName()
{
  if(name) SetName(name);
  return(1);
}

/* Initialisierung */

protected func Initialize()
{
// Umweltverhältnisse erstellen, falls nicht vorhanden
Pollute();
//Spielregelchooser erstellen, falls nicht vorhanden:
if(!FindObject(CHO6)) CreateObject(CHO6,1,1,-1);
// einfach nur startsichtweite bei spielstart
SetSichtweite=400;
  // Clonks mit Magiephysikal aus fehlerhaften Szenarien korrigieren
  if (Equal(GetID (), CLNK))
    if (GetPhysical ("Magic", 1))
      SetPhysical ("Magic", nil, 1);
  SetAction("Walk");
  SetDir(Random(2));
  // Broadcast für Spielregeln
  GameCallEx("OnClonkCreation", this());

Physicsanpassen();

//bei Spieleintritt kurz den aktuellen Erfahrungswert speichern, um später rauszufinden, wieiviel Erfahrung in der Runde gesammelt wurde.
ScheduleCall(nil, "Erfahrungerfassen", 50);

  return(1);
}

//in extrafkt weil es beim kauf von clonks sonst zu fehlern kommen kann
func Erfahrungerfassen()
{
if(!starterfahrung) starterfahrung = GetObjectInfoCoreVal("Experience", nil, nil);
}



func Physicsanpassen()
{
// vorher noch kurz einen Effekt erstellen. Denn beim Tod von Clonks werden die Items entfernt, dies lässt sich nicht mit der Deathfunktion unterbinden, weil es schon davor passiert. Vor death werden die Effekte gekillt, das kann man nutzen um schon dann die Items in eine Leiche zu verschieben
 if(!GetEffect("LeichenDeath",this(),nil,nil,1))
    AddEffect("LeichenDeath", this(), 1, nil, this); // stopp zu finden über der death funktion

//wenn diese Regel aktiviert ist, hat jeder neue Clonk (nur mit ID cLNK), weniger Physics
if(Equal(GetID(), CLNK))
    if(FindObject2(Find_ID(PPP2)))
    {
    SetPhysical("Energy", 20000, PHYS_StackTemporary, this());
    SetPhysical("Breath", 20000, PHYS_StackTemporary, this());
    SetPhysical("Walk", 50000, PHYS_StackTemporary, this());
    SetPhysical("Jump", 30000, PHYS_StackTemporary, this());
    SetPhysical("Scale", 20000, PHYS_StackTemporary, this());
    SetPhysical("Hangle", 20000, PHYS_StackTemporary, this());
    SetPhysical("Dig", 20000, PHYS_StackTemporary, this());
    SetPhysical("Swim", 30000, PHYS_StackTemporary, this());
    SetPhysical("Throw", 40000, PHYS_StackTemporary, this());
    SetPhysical("Push", 20000, PHYS_StackTemporary, this());
    SetPhysical("Fight", 20000, PHYS_StackTemporary, this());
    }
}


/* Bei Hinzufügen zu der Crew eines Spielers */

protected func Recruitment(int iPlr) {
  // Alchemieregel: Jeder Clonk kriegt einen angelegten Beutel spendiert
  iPlr ??= 0;
  if(ObjectCount(ALCO)) CreateObject(ALC_,nil,nil,-1)->~BelongTo(this());
  // Broadcast für Crew
  GameCallEx("OnClonkRecruitment", this(), iPlr);
}

/* Kontext */

public func HasConstructMenu() { return And(HasKnowledge(), GetPhysical("CanConstruct")); }
public func HasKnowledge() { return(GetPlrKnowledge(GetOwner(),nil,nil,C4D_Structure)); }
public func HasBase()      { return(And(FindBase(GetOwner()), !Equal(GetBase(Contained()), GetOwner()))); }
public func ReleaseAllowed() { return(ObjectCount(REAC)); }
public func AtConstructionSite() { return(And(And(!Contained(), FindConstructionSite()), ObjectCount(CNMT))); }
public func AtEnergySite() { return(And(!Contained(), FindEnergySite())); }
public func AtTreeToChop() { return(And(And(!Contained(), FindTree()), GetPhysical("CanChop"))); }

public func FindConstructionSite()
{
  return FindObject2(Find_AtRect(-1,-16,2,32), Find_OCF(OCF_Construct), Find_Layer(GetObjectLayer()));
}

public func FindEnergySite()
{
  return FindObject2(Find_AtPoint(), Find_OCF(OCF_PowerConsumer), Find_NoContainer(), Find_Layer(GetObjectLayer()), Find_Func("NeedsEnergy"));
}

public func FindTree()
{
  return FindObject2(Find_AtPoint(), Find_OCF(OCF_Chop), Find_Layer(GetObjectLayer()));
}

/* Steuerung */

public func ControlLeft()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlLeft")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeft(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlLeftDouble()
  {
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlLeftDouble")) return(true);
  }

protected func ControlRightDouble()
  {
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRightDouble")) return(true);
  }

protected func ControlLeftReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeftReleased(this()));
  // Keine überladene Steuerung
  return(nil);
}

public func ControlRight()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRight")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlRight(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlRightReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlRightReleased(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlUp()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlUp")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlUp(this()));
  // Bei JnR Delfinsprung
  if(GetPlrCoreJumpAndRunControl(GetController()))
  {
    var pCrew,i=nil; // i bei 0 starten, weil der befehl auch fuer den hauptclonk ausgefuehrt werden soll
    if(Equal(this(), GetCursor(GetOwner()))) // wenn dies der Hauptclonk ausführt, dann den Befehl an alle angewählten nicht-hauptclonks weitergeben. (damits befehl nur einmal ausgeführt wird)
    while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
    {
        pCrew->~DolphinJump();
    }
  }
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlUpReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlUpReleased(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlUpDouble() 
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlUpDouble")) return(1);
  
  var pCrew,i=nil; // i bei 0 starten, weil der befehl auch fuer den hauptclonk ausgefuehrt werden soll
  if(Equal(this(), GetCursor(GetOwner()))) // wenn dies der Hauptclonk ausführt, dann den Befehl an alle angewählten nicht-hauptclonks weitergeben. (damits befehl nur einmal ausgeführt wird)
  while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
  {
    pCrew->~DolphinJump();
  }
  
}

private func DolphinJump()
{
  // nur wenn an Meeresoberfläche
  if(!InLiquid()) return(nil);
  if(GBackSemiSolid(nil,-1)) return(nil);
  // Nicht wenn deaktiviert (z.B. Ohnmacht)
  if (GetActMapVal("ObjectDisabled", GetAction(), GetID())) return nil;
  // herausspringen
  SetPosition(GetX(),(GetY() ?? 0)-1);
  SetAction("Jump");
  SetSpeed(GetXDir(),-(BoundBy((GetPhysical("Swim") ?? 0)/2500,24,38) ?? 0));
  var iX=GetX(),iY=GetY(),iXDir=GetXDir(),iYDir=GetYDir();
  // Wenn Sprung im Wasser endet und das Wasser tief genug ist, Kopfsprung machen
  if(SimFlight(iX,iY,iXDir,iYDir,25,50))
    if(And(GBackLiquid((iX ?? 0)-(GetX() ?? 0),(iY ?? 0)-(GetY() ?? 0)), GBackLiquid((iX ?? 0)-(GetX() ?? 0),(iY ?? 0)+9-(GetY() ?? 0))))
      SetAction("Dive");
}

protected func ControlDown()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDown")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDown(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDownReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDownReleased(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDownSingle()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownSingle")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownSingle(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDownDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownDouble(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDig()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDig")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDig(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDigReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDigReleased(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDigSingle()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigSingle(this()));
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlDigDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDigDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigDouble(this()));
  // Keine überladene Steuerung
  return(nil);
}

public func ControlThrow() {

  UnHide();
  // Bei vorherigem Doppel-Stop nur Ablegen
  if (GetPlrDownDouble(GetOwner())) return(nil);
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlThrow")) return(1);
  // Reiten und Werfen
  if (IsRiding())
    if (Contents(nil))
    {
      SetAction("RideThrow");
      return(1);
    }
  // Keine überladene Steuerung
  return(nil);
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  // Steuerung an Pferd weiterleiten
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(IsRiding()) return(GetActionTarget()->~ControlUpdate(self, comdir, dig, throw));
  // Keine überladene Steuerung
  return(nil);
}


protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data)
{
  UnHide();
  // Kommando MoveTo an Pferd weiterleiten
  if (!!SEqual(szCommand, "MoveTo"))
    if (IsRiding())
      return(GetActionTarget()->~ControlCommand(szCommand, pTarget, iTx, iTy));
  // Anderes Kommando beim Reiten: absteigen (Ausnahme: Context)
  if (And(IsRiding(), !SEqual(szCommand, "Context")))
  {
    SetComDir(COMD_Stop,GetActionTarget());
    GetActionTarget()->~ControlDownDouble(this());
  }
  // RejectConstruction Callback beim Bauen durch Drag'n'Drop aus einem Gebaeude-Menu
  if(!!SEqual(szCommand, "Construct"))
  {
    // Data ist eigentlich keine ID, sondern ein C4Value* - Damit ein DirectCall
    // möglich ist, muss sie aber zu einer C4ID gecastet werden.
    if(CastC4ID(Data)->~RejectConstruction((iTx ?? 0) - (GetX() ?? 0), (iTy ?? 0) - (GetY() ?? 0), this()) )
    {
      return(1);
    }
  }
  // Kein überladenes Kommando
  return(nil);
}

/* Verwandlung */

private func RedefinePhysical(szPhys, idTo)
{
  // Physical-Werte ermitteln
  var physDefFrom = GetPhysical(szPhys, nil, nil, GetID()),
      physDefTo   = GetPhysical(szPhys, nil, nil, idTo),
      physCurr    = GetPhysical(szPhys);
  // Neuen Wert berechnen
  var physNew; if (physDefTo) physNew=BoundBy((physDefTo ?? 0)-(physDefFrom ?? 0)+(physCurr ?? 0), nil, 100000);
  // Neuen Wert für den Reset immer temporär setzen, selbst wenn keine Änderung besteht, damit der Reset richtig funktioniert
  SetPhysical(szPhys, physNew, PHYS_StackTemporary);
  // Fertig
  return(1);
}

protected func FxIntRedefineStart(object trg, int num, int tmp, id idTo)
  {
  // Ziel-ID in Effektvariable
  num ??= 0;
  tmp ??= 0;
  if (tmp)
    idTo = EffectVar(nil, trg, num);
  else
    {
    EffectVar(nil, trg, num) = idTo;
    EffectVar(1, trg, num) = GetID();
    }
  // Physicals anpassen
  RedefinePhysical("Energy", idTo);
  RedefinePhysical("Breath", idTo);
  RedefinePhysical("Walk", idTo);
  RedefinePhysical("Jump", idTo);
  RedefinePhysical("Scale", idTo);
  RedefinePhysical("Hangle", idTo);
  RedefinePhysical("Dig", idTo);
  RedefinePhysical("Swim", idTo);
  RedefinePhysical("Throw", idTo);
  RedefinePhysical("Push", idTo);
  RedefinePhysical("Fight", idTo);
  RedefinePhysical("Magic", idTo);
  RedefinePhysical("Float", idTo);
  /*if (GetRank()<4) RedefinePhysical("CanScale", idTo);
  if (GetRank()<6) RedefinePhysical("CanHangle", idTo);*/ // z.Z. können es alle
  RedefinePhysical("CanDig", idTo);
  RedefinePhysical("CanConstruct", idTo);
  RedefinePhysical("CanChop", idTo);
  RedefinePhysical("CanSwimDig", idTo);
  RedefinePhysical("CorrosionResist", idTo);
  RedefinePhysical("BreatheWater", idTo);
  // Damit Aufwertungen zu nicht-Magiern keine Zauberenergie übrig lassen
  if ((GetPhysical("Magic") ?? 0)/1000 < (GetMagicEnergy() ?? 0)) DoMagicEnergy((GetPhysical("Magic") ?? 0)/1000-(GetMagicEnergy() ?? 0));
  // Echtes Redefine nur bei echten Aufrufen (hat zu viele Nebenwirkungen)
  if (tmp) return(FX_OK);
  Redefine(idTo);
  // Fertig
  return(FX_OK);
  }
  
protected func FxIntRedefineStop(object trg, int num, int iReason, bool tmp)
  {
  // Physicals wiederherstellen
  num ??= 0;
  iReason ??= 0;
  tmp ??= false;
  ResetPhysical(nil, "BreatheWater");
  ResetPhysical(nil, "CorrosionResist");
  ResetPhysical(nil, "CanSwimDig");
  ResetPhysical(nil, "CanChop");
  ResetPhysical(nil, "CanConstruct");
  ResetPhysical(nil, "CanDig");
  ResetPhysical(nil, "Float");
  ResetPhysical(nil, "Magic");
  ResetPhysical(nil, "Fight");
  ResetPhysical(nil, "Push");
  ResetPhysical(nil, "Throw");
  ResetPhysical(nil, "Swim");
  ResetPhysical(nil, "Dig");
  ResetPhysical(nil, "Hangle");
  ResetPhysical(nil, "Scale");
  ResetPhysical(nil, "Jump");
  ResetPhysical(nil, "Walk");
  ResetPhysical(nil, "Breath");
  ResetPhysical(nil, "Energy");
  // Keine Rückänderung bei temporären Aufrufen oder beim Tod/Löschen
  if (Or(tmp, iReason)) return;
  // Damit Aufwertungen von nicht-Magiern keine Zauberenergie übrig lassen
  if ((GetPhysical("Magic") ?? 0)/1000 < (GetMagicEnergy() ?? 0)) DoMagicEnergy((GetPhysical("Magic") ?? 0)/1000-(GetMagicEnergy() ?? 0));
  // OK; alte Definition wiederherstellen
  Redefine(EffectVar(1, trg, num));
  }

public func Redefine2(idTo)
{
  if (Equal(GetID(), idTo)) return(true);
  RemoveEffect("IntRedefine", this());
  if (Equal(GetID(), idTo)) return(true);
  return(!!AddEffect("IntRedefine", this(), 10, nil, this(), nil, idTo));
}

public func Redefine(idTo)
{
  // Aktivitätsdaten sichern
  var phs=GetPhase(),act=GetAction();
  // Umwandeln
  ChangeDef(idTo);
  // Aktivität wiederherstellen
  var chg=SetAction(act);
  if (!chg) SetAction("Walk");
  if (chg) SetPhase(phs);
  // Fertig
  return(1);
}

/* Essen */  

public func Feed(iLevel)
{
  DoEnergy(iLevel);
  Sound("ClonkMunch");
    return(1);
}

/* Aktionen */
private func Riding()
{
  if(InLiquid(this()))
    {  
    if(!(pinguin))
      {
        SetAction("Swim");
        return(nil);
      }
    }
  // Richtung an die des Pferdes anpassen
  SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if (GetActionTarget()->~IsStill())
  {
    if (!SEqual(GetAction(), "RideStill"))
      SetAction("RideStill");
  }
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  else
    if (!SEqual(GetAction(), "Ride"))
      SetAction("Ride");
  return(1);
}



private func Throwing(pObj) 
{            
  
  // if(InLiquid(this()))
    // {  
    // if(!(pinguin))
      // {
        // SetAction("Swim");
      // }
    // }
    
  // Ggf. zu werfendes Objekt suchen 
  if (!pObj) 
  { 
    pObj = Contents(nil);
    if (!Contents()) return(nil);
  } 
  // Wurfparameter berechnen 
  var iX, iY, iR, iXDir, iYDir, iRDir; 
  if(And(!Equal(GetID(pObj), SPER), !pObj->~IsSpear()))
  { 
    // Standardwurf   
    iX = nil; if (!GetDir()) iX = -(iX ?? 0);
    iY = -10; 
    iR = Random(360); 
    iXDir = (GetPhysical("Throw") ?? 0) / 25000; if(!GetDir()) iXDir = -(iXDir ?? 0);
    iYDir = -(GetPhysical("Throw") ?? 0) / 25000;
    iRDir = (Random(40) ?? 0) - 20;
  } 
  else 
  { 
    // Speerwurf 
    iX = 10; if (!GetDir()) iX = -(iX ?? 0);
    iY = -6; 
    iR = 150; if(!GetDir()) iR = 180 - (iR ?? 0);
    iXDir = 5; if(!GetDir()) iXDir = -(iXDir ?? 0);
    iYDir = -2; 
    iRDir = 4; if(!GetDir()) iRDir = -(iRDir ?? 0);
  } 
  // Reitet? Eigengeschwindigkeit addieren 
  if (GetActionTarget()) if (IsRiding()) 
  {
    S42MigrationAssign(iXDir, "+=", (GetXDir(GetActionTarget()) ?? 0) / 10);
    // Steigungen beim Gehen werden leider nicht in die Y-Dir eingerechnet. Bei Steigungen
    // kann es daher vorkommen, dass der Wurf den Reiter vom Pferd haut. Was dagegen tun..?
    S42MigrationAssign(iYDir, "+=", (GetYDir(GetActionTarget()) ?? 0) / 10);
  } 
  // Werfen! 
  Exit(pObj, iX, iY, iR, nil, nil, iRDir);
  pObj->SetXDir((GetXDir() ?? 0)+(iXDir ?? 0)*10);
  pObj->SetYDir((GetYDir() ?? 0)+(iYDir ?? 0)*10);
  return(1);   
}





private func Fighting()
{  
  if(IsHidden()) SetAction("Punch");
  if (!Random(2)) SetAction("Punch");
  return(1);
}

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  if (!Random(5)) Sound("Punch*");
  if (!Random(2)) return(1);
  Punch(GetActionTarget());
  return(1);
}
  
public func Chopping()
{
  if (!GetActTime()) return; // Erster Schlag kein Sound. Clonk holt noch aus.
  Sound("Chop*");
  CastParticles("Dust",(Random(3) ?? 0)+1,6,-8+16*(GetDir() ?? 0),1,10,12);
  return(1);
}


private func Building()
{
    // var allesda =1;      // am liebsten würde ich nur das Bauen an sich beschleunigen/verlangsamen, aber ohne das Baumaterial dadurch zu beeinflussen. Mit DoCon werden aber leider die Baumaterialien geliefert und ich kenne keine moeglichkeit, an die fehlenden Baustoffe bis zur nächsten stufe zu kommen (bis nicht mehr weitergebaut werden kann und neue materialien benötigt werden). stattdessen werden nun bei halber augeschw. alle gebäude halbfertig gebaut.
    // var i,cid,num;
    // while (id = GetComponent(0, i++, 0, GetID()))
        // if ((num = GetComponent(id) - GetComponent(id, 0, 0, GetID())) > 0)
            // allesda = 0;  
        
   var target = GetActionTarget(nil,this());
  if((GetOCF(target) ?? 0) & (OCF_Construct ?? 0))  // nur wenn Ziel eine Baustelle ist
  {
      // immer wenn Bauarbeiter, schneller bauen
      if(!InArray(globArrayBaustellenBauarbeiter,target))   // nur einmal pro Gebäude den Boost geben
      {
          if(Equal(bauarbeiter, 1)) DoCon(5,target);
          if(Equal(bauarbeiter, 2)) DoCon(10,target);
          if(Equal(bauarbeiter, 3)) DoCon(15,target);
          if(Equal(bauarbeiter, 4)) DoCon(20,target);
          if(Equal(bauarbeiter, 5)) DoCon(25,target);
          if(Equal(bauarbeiter, 6)) DoCon(30,target);
          ArrayAdd(globArrayBaustellenBauarbeiter, target); // zum arrray zufuegen
      }
      
      if(!InArray(globArrayBaustellenRegel,target))   // nur einmal pro Gebäude den Boost geben
      {  
          if(GetLength(hashSpielRegeln)) DoCon(HashGet(hashSpielRegeln, "Baustellenboost"),target);
          ArrayAdd(globArrayBaustellenRegel, target); // zum arrray zufuegen
      }
      
      if (FindObject(SOB_)) // sofortbau, statische regel
      {
          var building=target;
          SetCon(100,building);
      }
  }

  if (!Random(2)) Sound("Build*");
  return(1);
}


private func Processing()
{
  Sound("Build1");
  return(1);
}

private func Digging()
{
  Sound("Dig*");
  return(1);
}

protected func Scaling()
{
  var szDesiredAction;
  if ((GetYDir() ?? 0)>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (!SEqual(GetAction(), szDesiredAction)) SetAction(szDesiredAction);
  return(1);   
}

/* Ereignisse */

protected func QueryCatchBlow(object projectile)
{
  // Objekt will den Clonk nicht treffen?
  if (And(projectile, projectile->~QueryClonkHit(this)))
    return true;;
  // Treffer OK
}
  
protected func CatchBlow()
{
  if (!!SEqual(GetAction(), "Dead")) return(nil);
  if (!Random(5)) Hurt();
  return(1);
}
  
protected func Hurt()
{
  Sound("Hurt*");
  return(1);
}
  
protected func Grab(object pTarget, bool fGrab)
{
  fGrab ??= false;
  Sound("Grab");
  return(1);
}

protected func Get()
{
  Sound("Grab");
  return(1);
}

protected func Put()
{
  Sound("Grab");
  return(1);
}

// wird beim Tod vor Death aufgerufen, sodass die Items noch von der Leiche gegrabt werden können
protected func FxLeichenDeathStop(object pTarget, int iNr, int iReason) {
  iNr ??= 0;
  iReason ??= 0;
  if(Equal(iReason, 4)) {
    // Leiche erstellen, welche der Umwelt schädigt.
   var pCorpse=CreateObject(CORP,nil,(GetDefBottom() ?? 0)-(GetY() ?? 0),GetOwner());
  pCorpse->SetLeiche(this());
  }
}


protected func Death(int iKilledBy)
{
  
  iKilledBy ??= 0;
  if(Equal(IsPinguin(), 2)) // wenn er als Taucher gestorben is
  {
      SetColorDw(GetPlrColorDw(GetOwner()));
      var suit= CreateObject(TAAZ,nil,nil,GetOwner());
      if(Contained()) Enter(Contained(),suit);
  }
  
  UnHide();
  // Info-Broadcasts für sterbende Clonks
  GameCallEx("OnClonkDeath", this(), iKilledBy);
  
  // Der Broadcast könnte seltsame Dinge gemacht haben: Clonk ist noch tot?
  if (GetAlive()) return;
  
  // den Beutel fallenlassen
  if(GetAlchemBag()) GetAlchemBag()->~Loose();

  Sound("Die");
  DeathAnnounce();

  // Letztes Mannschaftsmitglied tot: Script benachrichtigen
  if (!GetCrew(GetOwner()))
    GameCallEx("RelaunchPlayer",GetOwner(),iKilledBy);
    SetGraphics(nil, nil, nil, 1);
    SetGraphics(nil, nil, nil, 2);

  RemoveObject();
  return(1);

}

protected func Destruction()
{
  // Clonk war noch nicht tot: Jetzt ist er es
  if (GetAlive())
    GameCallEx("OnClonkDeath", this(), GetKiller());
  // Dies ist das letztes Mannschaftsmitglied: Script benachrichtigen
  if (Equal(GetCrew(GetOwner()), this()))
    if (Equal(GetCrewCount(GetOwner()), 1))
      //Nur wenn der Spieler noch lebt und nicht gerade eleminiert wird
      if (GetPlayerName(GetOwner()))
        {
        GameCallEx("RelaunchPlayer",GetOwner(),GetKiller());
        }
  return(1);
}

protected func DeepBreath()
{
  Sound("Breath");
  return(1); 
}
  
protected func CheckStuck()
{                   
  // Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if((Abs(GetYDir()) ?? 0) < 5)
    if(GBackSolid(nil, 3))
      SetPosition(GetX(), (GetY() ?? 0) + 1);
}

/* Status */

public func IsRiding()
{
  // Reitet der Clonk?
  return (WildcardMatch(GetAction(), "Ride*"));
}

public func IsClonk() { return(1); }

/* Kontext */

public func ContextRelease(pCaller) 
{
  [$CtxRelease$|Image=CXRL|Condition=ReleaseAllowed]
  FindObject(REAC)->Activate(GetOwner());
  return(1);
}

public func ContextEnergy(pCaller)
{
  [$TxtEnergysupply$|Image=CXEC|Condition=AtEnergySite]
  UnHide();
  var pSite; 
  if (pSite = FindEnergySite())
    SetCommand(this(), "Energy", pSite);
  return(1);
}

public func ContextFeighDeath(pCaller)
{
  [$CtxFeignDeath$|Image=CLNK|Condition=IsNotFeign]
  FeighDeathnow();  
}

func FeighDeathnow()
{
  UnHide();
  portraitt=GetPortrait(nil,nil,nil);
  portraittID=GetPortrait(nil,1,nil);
  SetPortrait("1",nil, CG8Z, nil, nil);
  Sound("Die");
  DeathAnnounce();
  SetAction("StartFeign");
  return(1);
}

public func ContextHide(pCaller)
{
  [$CtxHide$|Image=CLNK|Condition=IsHidable]
  //Message("Hiding",this);
  //SetAction("Hiding");
  Hide();
  return(1);
}

protected func StartHide() {
  if((GetActTime() ?? 0) > 100) {
    Hide();
    SetAction("Walk");
  }
}

protected func Hide() {
  SetComDir(COMD_None);
  isHidden = true;
  SetClrModulation(RGBa(205,205,205,170));
  //if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump" || GetAction() eq "WalkArmed" || GetAction() eq "SwimArmed" || GetAction() eq "JumpArmed") 
 if(Or(!!SEqual(GetAction(), "HoldWalk"), !!SEqual(GetAction(), "HoldJump")))
  {
    SetClrModulation(RGBa(205,205,205,170),this,1);
    SetClrModulation(RGBa(205,205,205,170),this,2);
  }
}

public func UnHide() {
  isHidden = nil;
  SetClrModulation(RGBa(255,255,255,nil));
    //if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump" || GetAction() eq "WalkArmed" || GetAction() eq "SwimArmed" || GetAction() eq "JumpArmed") 
   if(Or(!!SEqual(GetAction(), "HoldWalk"), !!SEqual(GetAction(), "HoldJump")))
  {
    SetClrModulation(RGBa(255,255,255,nil), this,1);
    SetClrModulation(RGBa(255,255,255,nil), this,2);
  }
}

public func IsHidable() {
  if (And(And((Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "HoldWalk"))), !IsHidden()), (Or(FindObject2(Find_AtPoint (nil,nil), Find_Category((C4D_Structure ?? 0) | (C4D_StaticBack ?? 0))), Equal(GetMaterial(), 18))))) return(true);
}

public func IsHidden() {
  return(isHidden);
}


public func ContextRiseFeighDeath(pCaller)
{
  [$CtxRise$|Image=CLNK|Condition=IsFeign]
  RisefromFeighDeath();
  return(1);
}

func RisefromFeighDeath()
{
  SetPortrait(Format("%s",portraitt),nil , portraittID); // da lokaler aufruf reicht 0
  SetAction("KneelUp");
}

protected func IsNotFeign() {
  if(And(And(And(!SEqual(GetAction(), "Feign"), !SEqual(GetAction(), "Faint")), !SEqual(GetAction(), "StartFeign")), !SEqual(GetAction(), "StartFaint"))) return(true);
}

protected func IsFeign() {
  if(!!SEqual(GetAction(), "Feign")) return(true);
}

// die pinguin funktionen. wird fuer pinguinanzug hazard, aber auch fuer taucheranzug clonk verwendet
private func Building()
{

if(InLiquid(this()))
{  
if(!(pinguin)){
    SetAction("Swim");
    return;
  }}
  _inherited();
}

private func StartHide()
{

if(InLiquid(this()))
{  
if(!(pinguin)){
    SetAction("Swim");
    return;
  }}
  _inherited();
}

func Walking()
{
    if(InLiquid(this()))
    {  
    if(!(pinguin))
      {
        SetAction("Swim");
        return;
      }
    }
    return(_inherited());
}

private func Digging()
{

if(InLiquid(this()))
{  
if(!(pinguin)){
    SetAction("Swim");
    return;
  }}
  _inherited();
}

private func Chopping()
{

if(InLiquid(this()))
{  
if(!(pinguin)){
    SetAction("Swim");
    return;
  }}
  _inherited();
}

func IsPinguin()
{
    return(pinguin);
}

func Pinguinan(izahl)
{
    if(!izahl) izahl = 1;
pinguin=izahl;  // fuer den taucheranzug wird es gleich 2. wichtig fuer das anzug ausziehen contextmenue
}

func Pinguinaus()
{
pinguin=nil;
}

protected func ContactBottom()
{
  if(!!SEqual(GetAction(), "Swim")) if(Equal(IsPinguin(), 2)) SetAction("Walk");
  return(_inherited());
}

// Taucherclonk funktionen:
public func ContextAnzugAusziehen(pCaller)
{
  ["Taucheranzug ausziehen"|Image=TAAZ|Condition=IsItOK]
  SetComDir(COMD_Stop);
  var suit= CreateObject(TAAZ,nil,nil,GetOwner());
  if(Contained()) Enter(Contained(),suit);
  
  pCaller->Pinguinaus();
  
  SetColorDw(GetPlrColorDw(GetOwner()));
  Sound("PutItOn");
  PlayerMessage(GetOwner(),"Taucheranzug ausgezogen.",this());
  return(1);
}

public func IsItOK()
{
  if(InLiquid()) return(nil); // nicht im wasser ausziehen
   if(Equal(IsPinguin(), 2))// nur ausziehbar, wenn auch angezogen
    return(1);
  return(nil);
}

func ControlSpecial()
{
  if(!Equal(IsPinguin(), 2)) return(_inherited());
  if(!ShiftContents()) return(nil);
  if(!ObjectCall(Contents(),"Selection")) Sound("Grab");
}


public func ContextConstructionSite(pCaller)
{
  [$CtxConstructionMaterial$|Image=CXCM|Condition=AtConstructionSite]
  UnHide();
  var pSite; 
  if (pSite = FindConstructionSite())
    PlayerMessage(GetOwner(), pSite->GetNeededMatStr(), pSite);
  return(1);
}

public func ContextChop(pCaller)
{
  [$CtxChop$|Image=CXCP|Condition=AtTreeToChop]
  UnHide();
  var pTree; 
  if (pTree = FindTree())
    SetCommand(this(), "Chop", pTree);
  return(1);
}

public func ContextConstruction(pCaller)
{
  [$CtxConstructionDesc$|Image=CXCN|Condition=HasConstructMenu]
  UnHide();
  SetCommand(this(), "Construct");
  ExecuteCommand();
  return(1);
}

public func ContextHome(pCaller)
{
  [$CtxHomeDesc$|Image=CXHM|Condition=HasBase]
  UnHide();
  SetCommand(this(), "Home");
  return(1);
}

/* Hilfsfunktion */

public func ContainedCall(string strFunction, object pTarget)
{
  // Erst das betreffende Gebäude betreten, dann die Zielfunktion aufrufen 
  SetCommand(this(), "Call", pTarget, this(), nil, nil, strFunction);
  AddCommand(this(), "Enter", pTarget);
}

/* Steuerung */

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
  // In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if (Contained())
    if (Or(((Contained()->GetCategory() ?? 0) & (C4D_Structure ?? 0)), ((Contained()->GetCategory() ?? 0) & (C4D_Vehicle ?? 0))))
      return(S42MigrationFirst(SetCommand(this(),"Context",nil,nil,nil,Contained()), ExecuteCommand()));
  // Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if (!!SEqual(GetAction(), "Push"))
    return(S42MigrationFirst(SetCommand(this(),"Context",nil,nil,nil,GetActionTarget()), ExecuteCommand()));
  // Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if (Contents(nil))
    return(S42MigrationFirst(SetCommand(this(),"Context",nil,nil,nil,Contents(nil)), ExecuteCommand()));
  // Ansonsten das Kontextmenü des Clonks öffnen
  return(S42MigrationFirst(SetCommand(this(),"Context",nil,nil,nil,this()), ExecuteCommand()));
}

/* Callback beim Auswahl aus dem Construct-Kontextmenu */

public func ControlCommandConstruction(target, x, y, target2, def)
{
  // Keine Konstruktion erlaubt?
  if(def->~RejectConstruction((x ?? 0) - (GetX() ?? 0), (y ?? 0) - (GetY() ?? 0), this()) )
    // Construct-Kommando beenden
    return(FinishCommand(this(), nil, nil) );
}

/* Automatische Produktion */

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return ;
  //Gebäude suchen worin man's herstellen kann  
  if (obj = GetProducerOf (def)) {
    AddCommand (this (), "Call", this (), nil, nil, nil, nil, "AutoProduction", nil, 1);
    obj -> HowToProduce (this(), def);
    return (1);
  }
}

public func AutoProduction() { return (1); }

public func AutoProductionFailed() 
{
  var def = GetCommand (this (), 5, 1);
  if (!FindContents(def)) {
    var obj = GetAvailableObject (def, GetCommand (this (), 4, 1));
    if (obj) return (AddCommand (this(), "Get", obj,nil,nil,nil,40));
    AddCommand (this(), "Buy", nil, nil, nil, nil, 100, GetCommand(this(), 5, 1), nil, 3);
  }
  return (1);
}

public func GetProducerOf(def)
{
  // Nur Objekte mit typischen OCFs befragen damit's schneller geht.
  var obj, prod, ocf = (OCF_Container  ?? 0) | (OCF_Exclusive  ?? 0) | (OCF_Entrance  ?? 0) | (OCF_LineConstruct  ?? 0) | (OCF_PowerConsumer  ?? 0);
  while (obj = FindObject (nil, -500, -250, 1000, 500, ocf,nil,nil,nil, obj))
    if (obj -> ~IsProducerOf (this (), def))
      if (Or(!prod, (ObjectDistance (prod) ?? 0) > (ObjectDistance (obj) ?? 0)))
        prod = obj;
  return (prod);
}


/* Fahrstuhlsteuerung */

public func ContextPermanentModeTurnOn(pCaller) 
{
  [$TxtPermanentModeTurnOn$|Image=ECS1:5|Condition=GrabbingNonPermanentElevator]
  if (!!SEqual(GetAction(), "Push"))
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ECS1:5|Condition=GrabbingPermanentElevator]
  if (!!SEqual(GetAction(), "Push"))
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}

private func GrabbingNonPermanentElevator()
{
  if (!!SEqual(GetAction(), "Push"))
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsNotInPermanentMode())
        return(1);
  return(nil);
}

private func GrabbingPermanentElevator()
{
  if (!!SEqual(GetAction(), "Push"))
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsInPermanentMode())
        return(1);
  return(nil);
}


/* Trinken */

public func Drink(object pDrink)
{
  // Trinkaktion setzen, wenn vorhanden
  if (GetActMapVal("Name", "Drink"))
    SetAction("Drink");
  // Vorsicht: erstmal nichts mit pDrink machen,
  // die Potions löschen sich meist selber...
}

/* Einsammeln */

public func RejectCollect(id idObject, object pObject)
{
  if(!!SEqual(GetAction(), "Faint")) return(1);
  // Objekt kann gepackt werden
  // automatisches Packen aber nur wenn die Paktteile nicht extra gezählt werden
  if(!IsSpecialItem(pObject)) if(pObject->~JoinPack(this())) return(1);
    
  // Objektaufnahme mit Limit verhindern, wenn bereits genug getragen
  if(And(pObject->~CarryLimit(), (ContentsCount(idObject) ?? 0) >= (pObject->~CarryLimit() ?? 0)) ) return(1);
    
  // Spezialitem?
  var i, iCount;
  if(i = IsSpecialItem(pObject))
  {
    // Noch genug Platz für das ganze Packet?
    if((GetSpecialCount(GetMaxSpecialCount((i ?? 0)-1)) ?? 0)+(Max(pObject->~PackCount(),1) ?? 0)<=(GetMaxSpecialCount((i ?? 0)-1, 1) ?? 0)) return(nil);
    iCount = (GetMaxSpecialCount((i ?? 0)-1, 1) ?? 0)-(GetSpecialCount(GetMaxSpecialCount((i ?? 0)-1)) ?? 0);
    // Ansonten so viel wie geht rein
    if(pObject->~SplitPack((pObject->~PackCount() ?? 0)-(iCount ?? 0))) return(nil);
    else return(1);
  }
  
  return((GetNonSpecialCount() ?? 0)>=(MaxContentsCount() ?? 0));
}

/* Itemlimit */

public func MaxContentsCount() { 
if(Or(Or(Equal(bauarbeiter, 1), Equal(bauarbeiter, 2)), Equal(bauarbeiter, 3))) return(3);
if(Or(Equal(bauarbeiter, 4), Equal(bauarbeiter, 5))) return(4);
if(Equal(bauarbeiter, 6)) return(5);
if(And(pinguin, !bauarbeiter)) return(3); // wenn taucher oder pinguin dann kann 3 getragen werden, auch wenn kein bauarbeiter ist
return(1); }



public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Hier könnten Spezialbehandlungen von Itemgruppen definiert werden
  // wie z.B. zu dem Inventar noch 30 Pfeile aufnehmen (siehe auch Ritter)
  //  if(iIndex == 0) { if(fAmount) return(30); return("IsArrow"); }
}

/* Liefert die Gesamtzahl eines Objekt(paket)typs */ 
private func GetObjectCount(idObj) 
  { 
  var idUnpackedObj; 
  if (idUnpackedObj = idObj->~UnpackTo()) 
    // Auch verschachtelte Pakete mitzählen 
    return((GetObjectCount(idUnpackedObj) ?? 0) * (idObj->PackCount() ?? 0));
  // Ansonsten ist es nur ein Objekt 
  return(1); 
  }

/* Spezialgegenstände im Inventar zählen */ 
private func GetSpecialCount(szTest) 
  { 
  var iCnt, pObj; 
  // Einzelne Pfeile... 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if(ObjectCall(pObj, szTest)) 
      S42MigrationStep(iCnt, 1, true);
  // Pakete... 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if(pObj->~UnpackTo())
      if(DefinitionCall(pObj->~UnpackTo(), szTest))
        S42MigrationAssign(iCnt, "+=", (GetObjectCount(pObj) ?? 0));
  // Wert zurückgeben 
  return(iCnt); 
  }
  
/* Testen eines Objektes */
private func IsSpecialItem(pObj)
{
  // Spezialitem?
  var j=-1;
  while(GetMaxSpecialCount(S42MigrationStep(j, 1), 1))
    if(ObjectCall(pObj, GetMaxSpecialCount(j)))
      return((j ?? 0)+1);
  // Spezialitempacket?
  if(pObj->~UnpackTo())
  {
    j=-1;
    while(GetMaxSpecialCount(S42MigrationStep(j, 1), 1))
      if(DefinitionCall(pObj->~UnpackTo(), GetMaxSpecialCount(j)))
        return((j ?? 0)+1);
  }
}

/* Anzahl an normalen Objekten */
private func GetNonSpecialCount() 
  { 
  var iCnt, pObj; 
  // Inventar einzeln auf nicht-Spezial überprüfen 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    // Spezialitems nicht zählen
    if(!IsSpecialItem(pObj))
        S42MigrationStep(iCnt, 1, true);
   
  // Wert zurückgeben 
  return(iCnt); 
  } 

/* Reiten */

public func ContextDescend(pCaller) 
{
  [$TxtDescend$|Image=DSCN|Condition=IsRiding]
  DescendVehicle();
}

public func DescendVehicle()
{
  var pOldVehicle = GetActionTarget();
  SetAction("Walk");
  // Feststecken nach Absteigen? Dann besser direkt beim Gefährt absteigen.
  if (Stuck()) if (pOldVehicle)
  {
    var x=GetX(), y=GetY();
    SetPosition(GetX(pOldVehicle), GetY(pOldVehicle));
    if (Stuck())
    {
      // Das Gefährt steckt auch? Dann hilft es alles nichts. Zurück zum Ursprungsort.
      SetPosition(x,y);
    }
  }  
}


/* Effektsteuerung */

private func Control2Effect(string szControl)
  {
  // Von Effektzahl abwärts zählen
  var i = GetEffectCount(nil, this()), iEffect;
  var res;
  while (S42MigrationStep(i, -1, true))
    {
    // Effekte mit Control im Namen benachrichtigen	  
    iEffect = GetEffect("*Control*", this(), i);
    //  Message("%s", this(), GetEffect(0, this(), iEffect, 1));
    if ( GetEffect(nil, this(), iEffect, 1) )
      S42MigrationAssign(res, "+=", (EffectCall(this(), iEffect, szControl) ?? 0));
    }
  return(res);
  }


/* Pfeile */

// Pfeilpaket aufteilen 
public func SplitPack2Components(pPack) 
  { 
  // Aufteilen 
  if(!pPack->~Unpack(this()) ) Split2Components(pPack); 
  // Fertig, Erfolg 
  return(1); 
  }
 
/* Pfeil aus dem Inventar nehmen */ 
public func GetArrow() 
  { 
  // Einzelne Pfeile suchen 
  var pObj, pArrow; 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Bei Bedarf Pakete aufteilen 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if(pObj->~IsArrowPack())
    {
      // Pfeil aus Paket verwenden
      if(pArrow = pObj->~GetItem()) return(pArrow);
      // oder bei alten Pfeilen Paket aufteilen
      if (SplitPack2Components(pObj))
        return(FindSingleArrow());
    }
  // Keine Pfeile gefunden 
  return(nil);
  } 

public func FindSingleArrow() 
  { 
  // Einzelne Pfeile suchen 
  var pObj; 
  for(var i = nil; pObj = Contents(i); S42MigrationStep(i, 1, true))
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Keiner gefunden 
  return(nil);
  } 
  
public func GetComboArrow() 
  { 
  // Pfeile als Komboobjekt: Nur wenn das erste Inventarobjekt ein Pfeil ist
  var pObj = Contents(nil), pArrow;
  if (!pObj) return;
  if(pObj->~IsArrow()) return(pObj); 
  // Bei Bedarf Pakete aufteilen 
  if(pObj->~IsArrowPack())
    {
    // Pfeil aus Paket verwenden
    if(pArrow = pObj->~GetItem()) return(pArrow);
    // oder bei alten Pfeilen Paket aufteilen
    if (SplitPack2Components(pObj))
      return(FindSingleArrow());
    }
  // Keine Pfeile gefunden 
  return(nil);
  } 

/* Pfeile im Inventar zählen */ 
private func GetArrowCount() 
  {
   return(GetSpecialCount("IsArrow"));
  }


/* Zauberei - benötigt, wenn der Clonk Zielzauber z.B. aus dem Zauberturm zaubert */

public func SpellFailed(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  var pSpellOrigin = pAimedSpellOrigin;
  pAimedSpellOrigin = nil;
  if (And(pSpellOrigin, !Equal(pSpellOrigin, this())))
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall die Zutaten/Zauberenergie für den
    // Zauber bereit gestellt, und diese sollten nicht an den Clonk zurück gegeben werden
    return (pSpellOrigin->~SpellFailed(idSpell, this()));
  // Magieenergie zurückgeben
  DoMagicEnergy(Value(idSpell), nil, true);
  // Alchemische Zutaten zurückgeben
  if(ObjectCount(ALCO)) IncreaseAlchem(idSpell);
}

public func SpellSucceeded(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  // LocalN als Workaround um ChangeDefs im Zauber
  var pSpellOrigin = LocalN("pAimedSpellOrigin");
  LocalN("pAimedSpellOrigin") = nil;
  if (And(pSpellOrigin, !Equal(pSpellOrigin, this())))
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall das Magietraining schon erledigt
    return (pSpellOrigin->~SpellSucceeded(idSpell, this()));
  // Globaler Aufruf für Zauber
  OnClonkSucceededSpell(idSpell);
}

// Der Clonk kann von sich aus nicht zaubern und hat keine Aktivitäten dafür
private func SetMagicAction(id idForSpell) {}
private func SetCastAction() {}
private func EndMagicAction() {}


/* Zielsteuerung - nur aktiv, wenn das Fantasypack die globalen Funktionen CreateAimer und CreateSelector überladen hat */

public func DoSpellAim(object pSpell, object pSpellOrigin)
  {
  pAimedSpell = pSpell;
  pAimedSpellOrigin = pSpellOrigin;
  pAimer = CreateAimer(this(), this(), (GetDir() ?? 0)*180-90);

  if (!pAimer) return(nil);
  
  // Callback an die Zauberquelle, dass noch gezielt wird
  if (pSpellOrigin) pSpellOrigin->~SpellAiming(pSpell, this());

  // Zielvorgang für Zauber
  SetComDir(COMD_Stop);
  SetCastAction();
  return(pAimer);
  }

public func OnAimerEnter(int iAngle)
  {
  // Zauber weg?
  iAngle ??= 0;
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  var idSpell = GetID(pAimedSpell);
  // Aktivität
  SetMagicAction();
  // Zauber benachrichtigen
  if (!pAimedSpell->~ActivateAngle(this(), iAngle))
    return(S42MigrationFirst(nil, SpellFailed(idSpell)));
  // OK; Zauber erfolgreich
  return(SpellSucceeded(idSpell));
  }

public func AimingAngle(int iAngle)
  {
  // Zauber weg?
  iAngle ??= 0;
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  // Zielaktion setzen, wenn im Laufen. Das ist etwas ungeschickt, weil damit
  // die Magic-Aktion abgebrochen wird, die der Magier bereits bis zur Haelfte
  // durchgefuehrt hat. Dummerweise laesst sich frueher nicht feststellen, ob
  // der auszufuehrende Zauber einen Aimer brauchen wird...
  if(GetActMapVal("Name", "AimMagic") )
    if(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Magic")))
      SetAction("AimMagic");

  // Phase anpassen
  if(!!SEqual(GetAction(), "AimMagic"))
    {
    // Auch richtigen Winkel verwenden wenn nach links gedreht
    var iHalfAngle = iAngle;
    if((iHalfAngle ?? 0) < 0) iHalfAngle = -(iHalfAngle ?? 0);
    SetPhase(BoundBy(((iHalfAngle ?? 0) + 9) / 18, nil, 9) );
    }
  // Weitergabe an den Zauber
  return(pAimedSpell->~AimingAngle(this(), iAngle));
  }

public func OnAimerAbort(int iAngle)
  {
  // Aktivität zurücksetzen
  iAngle ??= 0;
  EndMagicAction();
  // Benachrichtigung
  if (!pAimedSpell) return(1);
  var idSpell = GetID(pAimedSpell);
  if (!pAimedSpell->~AbortAiming(this()))
    // Standardaktion: Zauber löschen
    RemoveObject(pAimedSpell);
  pAimedSpell = nil;
  // OK; Zauber nicht erfolgreich
  return(SpellFailed(idSpell));
  }

public func DoSpellSelect(object pSpell, int iRadius, object pSpellOrigin)
  {
  // Zauber sichern
  iRadius ??= 0;
  pAimedSpell = pSpell;
  pAimedSpellOrigin = pSpellOrigin;
  pAimer = CreateSelector(pSpell, this(), iRadius);
  if (!pAimer) return;
  // Callback an die Zauberquelle, dass noch gezielt wird
  if (pSpellOrigin) pSpellOrigin->~SpellAiming(pSpell, this());
  // Zielvorgang für Zauber
  SetComDir(COMD_Stop);
  SetCastAction();
  return(pAimer);
  }

public func OnSelectorEnter(object pTarget)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort());
  var idSpell = GetID(pAimedSpell);
  // Aktivität
  SetMagicAction();
  // Zauber benachrichtigen
  if (!pAimedSpell->~ActivateTarget(this(), pTarget))
    return(S42MigrationFirst(nil, SpellFailed(idSpell)));
  // OK; Zauber erfolgreich
  return(SpellSucceeded(idSpell));
  }

public func OnSelectorAbort()
  {
  // Aktivität zurücksetzen
  EndMagicAction();
  // Benachrichtigung
  if (!pAimedSpell) return(1);
  var idSpell = GetID(pAimedSpell);
  if (!pAimedSpell->~AbortSelecting(this()))
    // Standardaktion: Zauber löschen
    RemoveObject(pAimedSpell);
  pAimedSpell = nil;
  // OK; Zauber nicht erfolgreich
  return(SpellFailed(idSpell));
  }

public func SelectorTarget(object pTarget) { if(pAimedSpell) return(pAimedSpell->~SelectorTarget(this(),pTarget)); }

// Momentanen Zauber abbrechen
protected func AbortCasting()
  {
  if (pAimer) pAimer->Abort();
  return(1);
  }
  
public func Abort() {} // dummy call to instantiate function name


