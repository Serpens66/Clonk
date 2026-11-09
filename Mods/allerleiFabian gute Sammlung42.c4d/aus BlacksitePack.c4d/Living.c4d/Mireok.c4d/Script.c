/*---- MireokClonk ----*/

#strict
// Zauberei - benötigt, wenn der Clonk Zielzauber z.B. aus dem Zauberturm zaubert
// Auch benötigt für den König
local pAimer;			// Aktive Zielsteuerung; wird abgrbrochen, wenn der Zauberer gestört wird (Nur Fantasypack)
local pAimedSpell;		// Zauber, der gezielt wird  (Nur Fantasypack)
local pAimedSpellOrigin;        // Objekt, das einen Zielzauber initiiert hat. An dieses werden SpellFailed/SpellSucceeded-Nachrichten weitergeleitet
local isHidden;
local name;
local portraitt,portraittID;
local starterfahrung,stufe;
local bauarbeiter,ausbildungstimer;

local zielSichtweite,zahler, SetSichtweite;

// je nach Einstellung in den variablen Regeln gibt es unterschiedliches Sichtradienverhalten der Clonks
protected func FogWar() 
{
if(GetLength(hashSpielRegeln))
zahler=HashGet(hashSpielRegeln, "SichtradiusVerhalten");
else {zahler=0;}

if(zahler==0) {
if (FrameCounter()<5000) {
if(!Random(100))  // zu anfang ab und zu wieder Kriegsnebel deaktivieren, falls er weggeht
for(var i1=0; i1<GetPlayerCount(); i1++) {
  SetFoW(0,i1);} }
return(0);}

if(!Random(200))  // ab und zu wieder Kriegsnebel erstellen, falls er weggeht
for(var i1=0; i1<GetPlayerCount(); i1++) {
  SetFoW(1,i1);}
   
if(zahler==1)
FogWar1();

if(zahler==2)
FogWar2();

if(zahler==3)
if(!Random(100))  // muss nicht sooft gecheckt werden
FogWar3();
}


// Blendfunktion für Blendgranate
func Blend()
{
  // gibt die x und y Koordinaten von Clonk und Granate aus, zu testzwecken.
  //Log(Format("clonk:%v,%v. Granate:%v,%v.",GetX(this()),GetY(this()),Par(0),Par(1)));
 if(Contained()) return(); // Wenn in einem Gebäude/Fahrzeug oderso, dann nicht blenden.
 if(PathFree(GetX(this())-10,GetY(this()),Par(0)-3,Par(1)-5)||PathFree(GetX(this())-10,GetY(this()),Par(0)+3,Par(1)-5)||PathFree(GetX(this())-10,GetY(this()),Par(0),Par(1)-5))  // wenn direkte Verbindung zwischen Clonk und Explosion
  {
   if((Distance(GetX(this()),GetY(this()),Par(2),Par(1))>1200) || (Par(0)==GetX(this()) && Par(1)==GetY(this())) ) return();  //wenn Distanz größer als 1200, oder Granate dabei (selbe position) dann keine Blendwirkung
  
   // clonk hat reingeschaut und ist näher als 600 dran
   if(Par(0)<=GetX(this()) && Equal(GetDir(),DIR_Left()) && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=600 )  {Var()=CreateObject(FL_H,0,0,GetOwner(this())); ProtectedCall(Var(),"Initialize",0,this());return();} 
   if(Par(0)>=GetX(this()) && Equal(GetDir(),DIR_Right())  && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=600 )  {Var()=CreateObject(FL_H,0,0,GetOwner(this()));ProtectedCall(Var(),"Initialize",0,this());return();}

   // clonk hat reingeschaut, aber zwischen 600 und 1200 weit weg, dann geht blendung schneller weg
   if(Par(0)<=GetX(this()) && Equal(GetDir(),DIR_Left()) && Distance(GetX(this()),GetY(this()),Par(2),Par(1))>600 && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=1200 )  {Var()=CreateObject(FL_H,0,0,GetOwner(this())); ProtectedCall(Var(),"Initialize",1,this());return();} 
   if(Par(0)>=GetX(this()) && Equal(GetDir(),DIR_Right()) && Distance(GetX(this()),GetY(this()),Par(2),Par(1))>600 && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=1200 )  {Var()=CreateObject(FL_H,0,0,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return();}

   // Clonk hat weggeschaut ist aber näher als 600, dann verschwindets schneller wieder.
   if(((Par(0)<=GetX(this()) && Equal(GetDir(),DIR_Right()))  && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=600))  {Var()=CreateObject(FL_H,0,0,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return();}  
   if(((Par(0)>=GetX(this()) && Equal(GetDir(),DIR_Left()))  && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=600))  {Var()=CreateObject(FL_H,0,0,GetOwner(this()));ProtectedCall(Var(),"Initialize",1,this());return();}

   // Clonk hat weggeschaut und ist weiter als 600 weg, dann keine Auswirkung
   if(((Par(0)<=GetX(this()) && Equal(GetDir(),DIR_Right())) && Distance(GetX(this()),GetY(this()),Par(2),Par(1))>600 && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=1200))  return();
   if(((Par(0)>=GetX(this()) && Equal(GetDir(),DIR_Left())) && Distance(GetX(this()),GetY(this()),Par(2),Par(1))>600 && Distance(GetX(this()),GetY(this()),Par(2),Par(1))<=1200))  return();
  }
 return(1);
}



// Tag, Nacht und Tunnel Sichtradius
func FogWar1()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

var z=0;
while(!GBackSky(GetX(0),GetY(0)-z)) ++z; 
var himmely = AbsY(GetY(0))-z; // y koordiante über Clonk, wo das erste mal himmer ist

  if(
(GetMaterial() == Material("Tunnel"))
 && (!PathFree(GetX(0),GetY(0),GetX(0),himmely))  
) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagTunnel");  else{zielSichtweite = 120;} } // Tunnel und keine direkte Verbindung zum Himmel? Dann eine Sichtweite von 25 anstreben

  if(
(GetMaterial() == Material("Tunnel")) && IsNight()
 && (!PathFree(GetX(0),GetY(0),GetX(0),himmely))
 ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtTunnel"); else{zielSichtweite = 80;} } // Tunnel+ fehlende Verbindung zum  Himmel? UND Nacht?! Dann eine Sichtweite von 15 anstreben


  if(SetSichtweite > zielSichtweite) SetSichtweite -= 1;
  if(SetSichtweite < zielSichtweite) SetSichtweite += 1;

//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

SetPlrViewRange(SetSichtweite,0,1); 
}


// nur Tag Nacht Sichtradius
func FogWar2()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

  if(SetSichtweite > zielSichtweite) SetSichtweite -= 1;
  if(SetSichtweite < zielSichtweite) SetSichtweite += 1;

//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

SetPlrViewRange(SetSichtweite,0,1); 
}


// Immer Sichtweite 
func FogWar3()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100

SetSichtweite = zielSichtweite;

//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

SetPlrViewRange(SetSichtweite,0,1); 
}


func TimerAusbildung()
{
++ausbildungstimer;
}

func StandTimerAusbildung()
{
return(ausbildungstimer);
}

func TimerAusbildungNull()
{
ausbildungstimer=0;
}


//wenn im ausbildungscamp ausgebildet, erhält der Clonk eine neue Stufe. Diese ist wichtig dafür, wann neue fähigkeiten erlernt werden bzw. die Grafik sich ändert.
func Stufenanstieg(zahl)
{
if(zahl==1) {stufe=5; return(0);} // für den Aufstieg von Clonk zu Hazardclonk die Stufen übernehmen
if(zahl==10) {stufe=10; return(0);} 
if(zahl==15) {stufe=15; return(0);} 
if(zahl==20) {stufe=20; return(0);} 
++stufe;
}

func AktuelleStufe()
{
return(stufe);
}

func NunBauarbeiter(zahl)
{
if(zahl==6) {bauarbeiter=6;return();}
++bauarbeiter;
if(zahl==10) bauarbeiter=0;
}

func IstBauarbeiter()
{
return(bauarbeiter);
}



func Klimaschutz(klima)
{
//Kälte
if(klima==1)
{
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, kein Schaden.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_ID(LAGF),Find_ID(TORC))))
{
//nur wenn Feuer brennt
if(GetID(feuer)==LAGF) if(!(GetAction(feuer) eq "Burn")) return(0);
 if(GetID(feuer)==TORC) if(!(GetAction(feuer) eq "Burning")) return(0);
  return(1);}
return(0);
}

//Hitze
if(klima==2)
{
// wenn in Flüssigkeit, kein schaden
if(GetOCF() & OCF_InLiquid()) return(1);
return(0);
}
}

func RundenErfahrung()
{
// gibt die in dieser Runde bereits erhaltene Erfahrung zum Zeitpunkt der Abfrage aus
return(GetObjectInfoCoreVal("Experience", 0, 0)-starterfahrung);
}


public func ResetName()
{
  if(name) SetName(name);
  return(1);
}

/* Initialisierung */

protected func Initialize()
{
// Umweltverhältnisse erstellen:
Pollute();
SetSichtweite=400;
  // Clonks mit Magiephysikal aus fehlerhaften Szenarien korrigieren
  if (GetID () == CLNK)
    if (GetPhysical ("Magic", 1))
      SetPhysical ("Magic", 0, 1);
  SetAction("Walk");
  SetDir(Random(2));
  // Broadcast für Spielregeln
  GameCallEx("OnClonkCreation", this());

Physicsanpassen();


//bei Spieleintritt kurz den aktuellen Erfahrungswert speichern, um später rauszufinden, wieiviel Erfahrung in der Runde gesammelt wurde.
ScheduleCall(0, "Erfahrungerfassen", 50);


  return(1);
}

//in extrafkt weil es beim kauf von clonks sonst zu fehlern kommen kann
func Erfahrungerfassen()
{
starterfahrung = GetObjectInfoCoreVal("Experience", 0, 0);
}



func Physicsanpassen()
{
// vorher noch kurz einen Effekt erstellen. Denn beim Tod von Clonks werden die Items entfernt, dies lässt sich nicht mit der Deathfunktion unterbinden, weil es schon davor passiert. Vor death werden die Effekte gekillt, das kann man nutzen um schon dann die Items in eine Leiche zu verschieben
 //AddEffect("LeichenDeath", this, 1, 0, this); // stopp zu finden über der death funktion

//wenn diese Regel aktiviert ist, hat jeder neue Clonk (nur mit ID cLNK), weniger Physics
if(GetID()==CLNK)
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
  if(ObjectCount(ALCO)) CreateObject(ALC_,0,0,-1)->~BelongTo(this());
  // Broadcast für Crew
  GameCallEx("OnClonkRecruitment", this(), iPlr);
}

/* Kontext */

public func HasConstructMenu() { return HasKnowledge() && GetPhysical("CanConstruct"); }
public func HasKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Structure())); }
public func HasBase()      { return(FindBase(GetOwner()) && GetBase(Contained()) != GetOwner()); }
public func ReleaseAllowed() { return(ObjectCount(REAC)); }
public func AtConstructionSite() { return(!Contained() && FindConstructionSite() && ObjectCount(CNMT)); }
public func AtEnergySite() { return(!Contained() && FindEnergySite()); }
public func AtTreeToChop() { return(!Contained() && FindTree() && GetPhysical("CanChop")); }

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
  return(0);
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
  return(0);
}

public func ControlRight()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRight")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlRight(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlRightReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlRightReleased(this()));
  // Keine überladene Steuerung
  return(0);
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
    DolphinJump();
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlUpReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpDouble() 
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlUpDouble")) return(1);
  DolphinJump();
}

private func DolphinJump()
{
  // nur wenn an Meeresoberfläche
  if(!InLiquid()) return(0);
  if(GBackSemiSolid(0,-1)) return(0);
  // Nicht wenn deaktiviert (z.B. Ohnmacht)
  if (GetActMapVal("ObjectDisabled", GetAction(), GetID())) return false;
  // herausspringen
  SetPosition(GetX(),GetY()-1);
  SetAction("Jump");
  SetSpeed(GetXDir(),-BoundBy(GetPhysical("Swim")/2500,24,38));
  var iX=GetX(),iY=GetY(),iXDir=GetXDir(),iYDir=GetYDir();
  // Wenn Sprung im Wasser endet und das Wasser tief genug ist, Kopfsprung machen
  if(SimFlight(iX,iY,iXDir,iYDir,25,50))
    if(GBackLiquid(iX-GetX(),iY-GetY()) && GBackLiquid(iX-GetX(),iY+9-GetY()))
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
  return(0);
}

protected func ControlDownReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDownReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDownSingle()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownSingle")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownSingle(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDownDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownDouble(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDig()
{
  UnHide();
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDig")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDig(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDigReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigSingle()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigSingle(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDigDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigDouble(this()));
  // Keine überladene Steuerung
  return(0);
}

public func ControlThrow() {

  UnHide();
  // Bei vorherigem Doppel-Stop nur Ablegen
  if (GetPlrDownDouble(GetOwner())) return(0);
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlThrow")) return(1);
  // Reiten und Werfen
  if (IsRiding())
    if (Contents(0))
    {
      SetAction("RideThrow");
      return(1);
    }
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  // Steuerung an Pferd weiterleiten
  if(IsRiding()) return(GetActionTarget()->~ControlUpdate(self, comdir, dig, throw));
  // Keine überladene Steuerung
  return(0);
}


protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data)
{
  UnHide();
  // Kommando MoveTo an Pferd weiterleiten
  if (szCommand eq "MoveTo")
    if (IsRiding())
      return(GetActionTarget()->~ControlCommand(szCommand, pTarget, iTx, iTy));
  // Anderes Kommando beim Reiten: absteigen (Ausnahme: Context)
  if (IsRiding() && szCommand ne "Context")
  {
    SetComDir(COMD_Stop(),GetActionTarget());
    GetActionTarget()->~ControlDownDouble(this());
  }
  // RejectConstruction Callback beim Bauen durch Drag'n'Drop aus einem Gebaeude-Menu
  if(szCommand S= "Construct")
  {
    // Data ist eigentlich keine ID, sondern ein C4Value* - Damit ein DirectCall
    // möglich ist, muss sie aber zu einer C4ID gecastet werden.
    if(CastC4ID(Data)->~RejectConstruction(iTx - GetX(), iTy - GetY(), this()) )
    {
      return(1);
    }
  }
  // Kein überladenes Kommando
  return(0);
}

/* Verwandlung */

private func RedefinePhysical(szPhys, idTo)
{
  // Physical-Werte ermitteln
  var physDefFrom = GetPhysical(szPhys, 0, 0, GetID()),
      physDefTo   = GetPhysical(szPhys, 0, 0, idTo),
      physCurr    = GetPhysical(szPhys);
  // Neuen Wert berechnen
  var physNew; if (physDefTo) physNew=BoundBy(physDefTo-physDefFrom+physCurr, 0, 100000);
  // Neuen Wert für den Reset immer temporär setzen, selbst wenn keine Änderung besteht, damit der Reset richtig funktioniert
  SetPhysical(szPhys, physNew, PHYS_StackTemporary);
  // Fertig
  return(1);
}

protected func FxIntRedefineStart(object trg, int num, int tmp, id idTo)
  {
  // Ziel-ID in Effektvariable
  if (tmp)
    idTo = EffectVar(0, trg, num);
  else
    {
    EffectVar(0, trg, num) = idTo;
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
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // Echtes Redefine nur bei echten Aufrufen (hat zu viele Nebenwirkungen)
  if (tmp) return(FX_OK);
  Redefine(idTo);
  // Fertig
  return(FX_OK);
  }
  
protected func FxIntRedefineStop(object trg, int num, int iReason, bool tmp)
  {
  // Physicals wiederherstellen
  ResetPhysical(0, "BreatheWater");
  ResetPhysical(0, "CorrosionResist");
  ResetPhysical(0, "CanSwimDig");
  ResetPhysical(0, "CanChop");
  ResetPhysical(0, "CanConstruct");
  ResetPhysical(0, "CanDig");
  ResetPhysical(0, "Float");
  ResetPhysical(0, "Magic");
  ResetPhysical(0, "Fight");
  ResetPhysical(0, "Push");
  ResetPhysical(0, "Throw");
  ResetPhysical(0, "Swim");
  ResetPhysical(0, "Dig");
  ResetPhysical(0, "Hangle");
  ResetPhysical(0, "Scale");
  ResetPhysical(0, "Jump");
  ResetPhysical(0, "Walk");
  ResetPhysical(0, "Breath");
  ResetPhysical(0, "Energy");
  // Keine Rückänderung bei temporären Aufrufen oder beim Tod/Löschen
  if (tmp || iReason) return();
  // Damit Aufwertungen von nicht-Magiern keine Zauberenergie übrig lassen
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // OK; alte Definition wiederherstellen
  Redefine(EffectVar(1, trg, num));
  }

public func Redefine2(idTo)
{
  if (GetID() == idTo) return(true);
  RemoveEffect("IntRedefine", this());
  if (GetID() == idTo) return(true);
  return(!!AddEffect("IntRedefine", this(), 10, 0, this(), 0, idTo));
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
  // Richtung an die des Pferdes anpassen
  SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if (GetActionTarget()->~IsStill())
  {
    if (GetAction() ne "RideStill")
      SetAction("RideStill");
  }
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  else
    if (GetAction() ne "Ride")
      SetAction("Ride");
  return(1);
}



private func Throwing(pObj) 
{            
  // Ggf. zu werfendes Objekt suchen 
  if (!pObj) 
  { 
    pObj = Contents(0); 
    if (!Contents()) return(0); 
  } 
  // Wurfparameter berechnen 
  var iX, iY, iR, iXDir, iYDir, iRDir; 
  if(GetID(pObj) != SPER && !pObj->~IsSpear()) 
  { 
    // Standardwurf   
    iX = 0; if (!GetDir()) iX = -iX; 
    iY = -10; 
    iR = Random(360); 
    iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir; 
    iYDir = -GetPhysical("Throw") / 25000; 
    iRDir = Random(40) - 20; 
  } 
  else 
  { 
    // Speerwurf 
    iX = 10; if (!GetDir()) iX = -iX; 
    iY = -6; 
    iR = 150; if(!GetDir()) iR = 180 - iR; 
    iXDir = 5; if(!GetDir()) iXDir = -iXDir; 
    iYDir = -2; 
    iRDir = 4; if(!GetDir()) iRDir = -iRDir; 
  } 
  // Reitet? Eigengeschwindigkeit addieren 
  if (GetActionTarget()) if (IsRiding()) 
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    // Steigungen beim Gehen werden leider nicht in die Y-Dir eingerechnet. Bei Steigungen
    // kann es daher vorkommen, dass der Wurf den Reiter vom Pferd haut. Was dagegen tun..?
    iYDir += GetYDir(GetActionTarget()) / 10; 
  } 
  // Werfen! 
  Exit(pObj, iX, iY, iR, 0, 0, iRDir); 
  pObj->SetXDir(GetXDir()+iXDir*10);
  pObj->SetYDir(GetYDir()+iYDir*10);
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
  if (!GetActTime()) return(); // Erster Schlag kein Sound. Clonk holt noch aus.
  Sound("Chop*");
  CastParticles("Dust",Random(3)+1,6,-8+16*GetDir(),1,10,12);
  return(1);
}




//Baustoff des Gebäudes wird ab und zu gespawnt beim Bauen, wenn Bauarbeiter
private func Building()
{

  if(FindObject(CNMT))  // nur wenn Baumaterial eingeschaltet ist, dann Material spawnen
  {
  var randomzahl = Random(4);  // damit nicht nur die erste Komponente erstellt wird, sondern bis zu den ersten 4
  if(bauarbeiter==1) {  var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(100) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 5

  if(bauarbeiter==2) {var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(95) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 10

  if(bauarbeiter==3) {var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(90) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 15

  if(bauarbeiter==4) {var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(80) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 20

  if(bauarbeiter==5) {var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(70) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 25

  if(bauarbeiter==6) {var building=GetActionTarget(0,this());
  var baustoff= GetComponent(0,randomzahl,building); if(!Random(60) && baustoff) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 30
  }
  // immer wenn Bauarbeiter schneller bauen
  if(bauarbeiter==1) {  var building=GetActionTarget(0,this());
  if(!Random(4)) DoCon(1,building);}
  if(bauarbeiter==2) {var building=GetActionTarget(0,this());
  if(!Random(2))DoCon(1,building);}
  if(bauarbeiter==3) {var building=GetActionTarget(0,this());
  if(!Random(4))DoCon(3,building);}
  if(bauarbeiter==4) {var building=GetActionTarget(0,this());
  if(!Random(2))DoCon(2,building);}
  if(bauarbeiter==5) {var building=GetActionTarget(0,this());
  if(!Random(4))DoCon(5,building);}
  if(bauarbeiter==6) {var building=GetActionTarget(0,this());
  if(!Random(2))DoCon(3,building);}
if (FindObject(SOB_))
{
var building=GetActionTarget(0,this());
  DoCon(100,building);}

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
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() ne szDesiredAction) SetAction(szDesiredAction);
  return(1);   
}

/* Ereignisse */

protected func QueryCatchBlow(object projectile)
{
  // Objekt will den Clonk nicht treffen?
  if (projectile && projectile->~QueryClonkHit(this))
    return true;;
  // Treffer OK
}
  
protected func CatchBlow()
{
  if (GetAction() eq "Dead") return(0);
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
  if(iReason == 4) {
    // Leiche erstellen, welche der Umwelt schädigt.
   var pCorpse=CreateObject(CORP,0,GetDefBottom()-GetY(),GetOwner());
  pCorpse->SetLeiche(this());
  }
}




protected func Destruction()
{
  // Clonk war noch nicht tot: Jetzt ist er es
  if (GetAlive())
    GameCallEx("OnClonkDeath", this(), GetKiller());
  // Dies ist das letztes Mannschaftsmitglied: Script benachrichtigen
  if (GetCrew(GetOwner()) == this())
    if (GetCrewCount(GetOwner()) == 1)
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
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(GBackSolid(0, 3))
      SetPosition(GetX(), GetY() + 1);
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
  portraitt=GetPortrait(0,0,0);
  portraittID=GetPortrait(0,1,0);
  SetPortrait("1",0, CG8Z, 0, 0);
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
  if(GetActTime() > 100) {
    Hide();
    SetAction("Walk");
  }
}

protected func Hide() {
  SetComDir(COMD_None);
  isHidden = true;
  SetClrModulation(RGBa(205,205,205,170));
  //if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump" || GetAction() eq "WalkArmed" || GetAction() eq "SwimArmed" || GetAction() eq "JumpArmed") 
 if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump")
  {
    SetClrModulation(RGBa(205,205,205,170),this,1);
    SetClrModulation(RGBa(205,205,205,170),this,2);
  }
}

public func UnHide() {
  isHidden = false;
  SetClrModulation(RGBa(255,255,255,0));
    //if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump" || GetAction() eq "WalkArmed" || GetAction() eq "SwimArmed" || GetAction() eq "JumpArmed") 
   if(GetAction() eq "HoldWalk" || GetAction() eq "HoldJump")
  {
    SetClrModulation(RGBa(255,255,255,0), this,1);
    SetClrModulation(RGBa(255,255,255,0), this,2);
  }
}

public func IsHidable() {
  if ((GetAction() eq "Walk" || GetAction() eq "HoldWalk") && !IsHidden() && (FindObject2(Find_AtPoint (0,0), Find_Category(C4D_Structure | C4D_StaticBack)) || GetMaterial() == 18)) return(true);
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
  SetPortrait(Format("%s",portraitt),0 , portraittID); // da lokaler aufruf reicht 0
  SetAction("KneelUp");
}

protected func IsNotFeign() {
  if(GetAction() ne "Feign" && GetAction() ne "Faint" && GetAction() ne "StartFeign" && GetAction() ne "StartFaint") return(true);
}

protected func IsFeign() {
  if(GetAction() eq "Feign") return(true);
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
  SetCommand(this(), "Call", pTarget, this(), 0, 0, strFunction);
  AddCommand(this(), "Enter", pTarget);
}

/* Steuerung */

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
  // In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if (Contained())
    if ((Contained()->GetCategory() & C4D_Structure) || (Contained()->GetCategory() & C4D_Vehicle))
      return(SetCommand(this(),"Context",0,0,0,Contained()), ExecuteCommand());
  // Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if (GetAction() eq "Push")
    return(SetCommand(this(),"Context",0,0,0,GetActionTarget()), ExecuteCommand());
  // Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if (Contents(0))
    return(SetCommand(this(),"Context",0,0,0,Contents(0)), ExecuteCommand());
  // Ansonsten das Kontextmenü des Clonks öffnen
  return(SetCommand(this(),"Context",0,0,0,this()), ExecuteCommand());
}

/* Callback beim Auswahl aus dem Construct-Kontextmenu */

public func ControlCommandConstruction(target, x, y, target2, def)
{
  // Keine Konstruktion erlaubt?
  if(def->~RejectConstruction(x - GetX(), y - GetY(), this()) )
    // Construct-Kommando beenden
    return(FinishCommand(this(), false, 0) );
}

/* Automatische Produktion */

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return ();
  //Gebäude suchen worin man's herstellen kann  
  if (obj = GetProducerOf (def)) {
    AddCommand (this (), "Call", this (), 0, 0, 0, 0, "AutoProduction", 0, 1);
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
    if (obj) return (AddCommand (this(), "Get", obj,0,0,0,40));
    AddCommand (this(), "Buy", 0, 0, 0, 0, 100, GetCommand(this(), 5, 1), 0, 3);
  }
  return (1);
}

public func GetProducerOf(def)
{
  // Nur Objekte mit typischen OCFs befragen damit's schneller geht.
  var obj, prod, ocf = OCF_Container () | OCF_Exclusive () | OCF_Entrance () | OCF_LineConstruct () | OCF_PowerConsumer ();
  while (obj = FindObject (0, -500, -250, 1000, 500, ocf,0,0,0, obj))
    if (obj -> ~IsProducerOf (this (), def))
      if (!prod || ObjectDistance (prod) > ObjectDistance (obj))
        prod = obj;
  return (prod);
}


/* Fahrstuhlsteuerung */

public func ContextPermanentModeTurnOn(pCaller) 
{
  [$TxtPermanentModeTurnOn$|Image=ECS1:5|Condition=GrabbingNonPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ECS1:5|Condition=GrabbingPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}

private func GrabbingNonPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsNotInPermanentMode())
        return(1);
  return(0);        
}

private func GrabbingPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsInPermanentMode())
        return(1);
  return(0);        
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
 return(1);
  if(GetAction() eq "Faint") return(1);
  // Objekt kann gepackt werden
  // automatisches Packen aber nur wenn die Paktteile nicht extra gezählt werden
  if(!IsSpecialItem(pObject)) if(pObject->~JoinPack(this())) return(1);
    
  // Objektaufnahme mit Limit verhindern, wenn bereits genug getragen
  if(pObject->~CarryLimit() && ContentsCount(idObject) >= pObject->~CarryLimit() ) return(1);
    
  // Spezialitem?
  var i, iCount;
  if(i = IsSpecialItem(pObject))
  {
    // Noch genug Platz für das ganze Packet?
    if(GetSpecialCount(GetMaxSpecialCount(i-1))+Max(pObject->~PackCount(),1)<=GetMaxSpecialCount(i-1, 1)) return(0);
    iCount = GetMaxSpecialCount(i-1, 1)-GetSpecialCount(GetMaxSpecialCount(i-1));
    // Ansonten so viel wie geht rein
    if(pObject->~SplitPack(pObject->~PackCount()-iCount)) return(0);
    else return(1);
  }
  
  return(GetNonSpecialCount()>=MaxContentsCount());
}

/* Itemlimit */

public func MaxContentsCount() { 
if(bauarbeiter==1 || bauarbeiter==2 || bauarbeiter==3) return(3);
if(bauarbeiter==4 || bauarbeiter==5) return(4);
if(bauarbeiter==6) return(5);
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
    return(GetObjectCount(idUnpackedObj) * idObj->PackCount()); 
  // Ansonsten ist es nur ein Objekt 
  return(1); 
  }

/* Spezialgegenstände im Inventar zählen */ 
private func GetSpecialCount(szTest) 
  { 
  var iCnt, pObj; 
  // Einzelne Pfeile... 
  for(var i = 0; pObj = Contents(i); i++) 
    if(ObjectCall(pObj, szTest)) 
      iCnt++; 
  // Pakete... 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~UnpackTo())
      if(DefinitionCall(pObj->~UnpackTo(), szTest))
        iCnt += GetObjectCount(pObj); 
  // Wert zurückgeben 
  return(iCnt); 
  }
  
/* Testen eines Objektes */
private func IsSpecialItem(pObj)
{
  // Spezialitem?
  var j=-1;
  while(GetMaxSpecialCount(++j, 1))
    if(ObjectCall(pObj, GetMaxSpecialCount(j)))
      return(j+1);
  // Spezialitempacket?
  if(pObj->~UnpackTo())
  {
    j=-1;
    while(GetMaxSpecialCount(++j, 1))
      if(DefinitionCall(pObj->~UnpackTo(), GetMaxSpecialCount(j)))
        return(j+1);
  }
}

/* Anzahl an normalen Objekten */
private func GetNonSpecialCount() 
  { 
  var iCnt, pObj; 
  // Inventar einzeln auf nicht-Spezial überprüfen 
  for(var i = 0; pObj = Contents(i); i++)
    // Spezialitems nicht zählen
    if(!IsSpecialItem(pObj))
        iCnt++;
   
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
  var i = GetEffectCount(0, this()), iEffect;
  var res;
  while (i--)
    {
    // Effekte mit Control im Namen benachrichtigen	  
    iEffect = GetEffect("*Control*", this(), i);
    //  Message("%s", this(), GetEffect(0, this(), iEffect, 1));
    if ( GetEffect(0, this(), iEffect, 1) )
      res += EffectCall(this(), iEffect, szControl);
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
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Bei Bedarf Pakete aufteilen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrowPack())
    {
      // Pfeil aus Paket verwenden
      if(pArrow = pObj->~GetItem()) return(pArrow);
      // oder bei alten Pfeilen Paket aufteilen
      if (SplitPack2Components(pObj))
        return(FindSingleArrow());
    }
  // Keine Pfeile gefunden 
  return(0); 
  } 

public func FindSingleArrow() 
  { 
  // Einzelne Pfeile suchen 
  var pObj; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Keiner gefunden 
  return(0); 
  } 
  
public func GetComboArrow() 
  { 
  // Pfeile als Komboobjekt: Nur wenn das erste Inventarobjekt ein Pfeil ist
  var pObj = Contents(0), pArrow;
  if (!pObj) return();
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
  return(0);
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
  pAimedSpellOrigin = 0;
  if (pSpellOrigin && pSpellOrigin != this())
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall die Zutaten/Zauberenergie für den
    // Zauber bereit gestellt, und diese sollten nicht an den Clonk zurück gegeben werden
    return (pSpellOrigin->~SpellFailed(idSpell, this()));
  // Magieenergie zurückgeben
  DoMagicEnergy(Value(idSpell), 0, true);
  // Alchemische Zutaten zurückgeben
  if(ObjectCount(ALCO)) IncreaseAlchem(idSpell);
}

public func SpellSucceeded(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  // LocalN als Workaround um ChangeDefs im Zauber
  var pSpellOrigin = LocalN("pAimedSpellOrigin");
  LocalN("pAimedSpellOrigin") = 0;
  if (pSpellOrigin && pSpellOrigin != this())
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
  pAimer = CreateAimer(this(), this(), GetDir()*180-90);

  if (!pAimer) return(0);
  
  // Callback an die Zauberquelle, dass noch gezielt wird
  if (pSpellOrigin) pSpellOrigin->~SpellAiming(pSpell, this());

  // Zielvorgang für Zauber
  SetComDir(COMD_Stop());
  SetCastAction();
  return(pAimer);
  }

public func OnAimerEnter(int iAngle)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  var idSpell = GetID(pAimedSpell);
  // Aktivität
  SetMagicAction();
  // Zauber benachrichtigen
  if (!pAimedSpell->~ActivateAngle(this(), iAngle))
    return(0, SpellFailed(idSpell));
  // OK; Zauber erfolgreich
  return(SpellSucceeded(idSpell));
  }

public func AimingAngle(int iAngle)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  // Zielaktion setzen, wenn im Laufen. Das ist etwas ungeschickt, weil damit
  // die Magic-Aktion abgebrochen wird, die der Magier bereits bis zur Haelfte
  // durchgefuehrt hat. Dummerweise laesst sich frueher nicht feststellen, ob
  // der auszufuehrende Zauber einen Aimer brauchen wird...
  if(GetActMapVal("Name", "AimMagic") )
    if(GetAction() S= "Walk" || GetAction() S= "Magic")
      SetAction("AimMagic");

  // Phase anpassen
  if(GetAction() S= "AimMagic")
    {
    // Auch richtigen Winkel verwenden wenn nach links gedreht
    var iHalfAngle = iAngle;
    if(iHalfAngle < 0) iHalfAngle = -iHalfAngle;
    SetPhase(BoundBy((iHalfAngle + 9) / 18, 0, 9) );
    }
  // Weitergabe an den Zauber
  return(pAimedSpell->~AimingAngle(this(), iAngle));
  }

public func OnAimerAbort(int iAngle)
  {
  // Aktivität zurücksetzen
  EndMagicAction();
  // Benachrichtigung
  if (!pAimedSpell) return(1);
  var idSpell = GetID(pAimedSpell);
  if (!pAimedSpell->~AbortAiming(this()))
    // Standardaktion: Zauber löschen
    RemoveObject(pAimedSpell);
  pAimedSpell = 0;
  // OK; Zauber nicht erfolgreich
  return(SpellFailed(idSpell));
  }

public func DoSpellSelect(object pSpell, int iRadius, object pSpellOrigin)
  {
  // Zauber sichern
  pAimedSpell = pSpell;
  pAimedSpellOrigin = pSpellOrigin;
  pAimer = CreateSelector(pSpell, this(), iRadius);
  if (!pAimer) return();
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
    return(0, SpellFailed(idSpell));
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
  pAimedSpell = 0;
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


/*---- HazardClonk ----*/

#strict
#include CLNK

#include L_DR	// can have drone (with context menu)
#include L_GE	// can use gear (with context menu)
#include L_LA	// can climb on ladders	
#include L_AG	// is agil
#include L_JP	// can use jetpack
#include L_CA	// can aim

static const WeaponDrawLayer = 1; //Layer in dem die Waffen gezeichnet werden

local ammobag;
local wpneffect;
local hatschutzanzug,brennt;
local pinguin;
local hatanorak;
local AutoCollectIstAnlokal;
local mehrmunition1, mehrmunition2,mehrmunition3, mehrmunition4,mehrmunition5, mehrmunition6,mehrmunition7,mehrmunition8,mehrmunition9,mehrmunition10,mehrmunition11,mehrmunition12,mehrmunition13,mehrmunition14,mehrmunition15,mehrmunition16,mehrmunition17,mehrmunition18,mehrmunition19,mehrmunition20;
local LastDmgType;
local geblendet;



func IsHazardCl() {return(1);}

/* Initialisierung */

protected func Construction()
{
  aMacroCommandList = CreateArray();
  _inherited();
}

func Autoistan()
{
AutoCollectIstAnlokal=1;
}

func Autoistaus()
{
AutoCollectIstAnlokal=0;
}


public func Chopping()
{
  if (!GetActTime()) return(); // Erster Schlag kein Sound. Clonk holt noch aus.
  if(bauarbeiter==1 ||bauarbeiter==2  ) DoDamage(15, GetActionTarget());
  if(bauarbeiter==3 ||bauarbeiter==4  ) DoDamage(30, GetActionTarget());
  if(bauarbeiter==5 ||bauarbeiter==6  ) DoDamage(40, GetActionTarget());  // mehr Schaden -> Baum fällt schneller um
  Sound("Chop*");
  CastParticles("Dust",Random(3)+1,6,-8+16*GetDir(),1,10,12);
  return(1);
}


protected func Initialize()
{
  //dieser Effekt ist für die Waffen in den Händen zuständig
  wpneffect = AddEffect("ShowWeapon",this(),1,1,this(),HZCK);
  // Magieanzeige ist hier die Munitionsanzeige - nicht mehr
//  SetPhysical("Magic",100000,2);
  //Energie fixen (kA wo das gechanged wird...) UH H4X
  DoEnergy( -GetPhysical("Energy") / 1000 + 1); //Noch 1 Energie
  SetPhysical("Energy", 150000, 2); //Physical ändern
  DoEnergy( GetPhysical("Energy") / 100 - 1); // Neu dazugeben
  // Munitionsgürtel
  AmmoStoring();
  GetHUD(this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);

  return(inherited());
}

public func IsThreat() { return(true); }

/* Rekrutierung */

// Anzahl Standardportraits
public func StdPortraitCount() { return(3); }

protected func Recruitment(int iPlr)
{
  // Zur Netzwerk-Sicherheit
  var iRandomPortrait = Random(StdPortraitCount());
  // Mal schauen, was fuer ein Portrait wir haben
  var portrait = GetPortrait(this(), 0, 1);
  var portrait_id = GetPortrait(this(), 1, 1);
  // Ist es ein eigenes Portrait? Dann lassen wir es.
  if(portrait S= "custom") return(_inherited(iPlr) );
  // Ist eine ID angegeben?
  if(portrait_id && portrait_id != NONE)
  {
    // Kein Hazardportrait?
    if(portrait_id != GetID() )
    {
      SetPortrait(Format("%d", 1 + iRandomPortrait), this(), GetID(), true, false);
      return(_inherited(iPlr) );
    }
  }
  // Es handelt sich um ein Hazardportrait. Aber ein richtiges?
  if(!WildcardMatch(portrait, "?") )
  {
    // Nein, das war ein Ruestungsportrait oder sowas. Wir wollen aber lieber ein normales.
    SetPortrait(Format("%d", 1 + iRandomPortrait), this(), GetID(), true, false);
  }
  return(_inherited(iPlr) );
}

/* Steuerung Richtung (an Inhalt weitergeben, sonst internen Befehl ausführen) */

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(Control2Grab("ControlUpdate", comdir, dig, throw)) return(0);
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlAim("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);

  return(_inherited(clonk, comdir, dig, throw));
}

protected func ControlUp() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlUp");

  if (ControlAim("ControlUp")) return(1);
  if (Control2Grab("ControlUp")) return(0);
  if (Control2Contents("ControlUp") ) return(1);
  if (ControlLadder("ControlUp") ) return(1);
  if (ControlAgility("ControlUp") ) return(1);
  return(_inherited());
}

protected func ControlUpDouble() {

  if (ControlAim("ControlUp")) return(1);
  if (Control2Grab("ControlUpDouble")) return(0);
  if (Control2Contents("ControlUpDouble") ) return(1);
  if (ControlJetpack("ControlUpDouble") ) return(1);
  if (ControlAgility("ControlUpDouble") ) return(1);
  return(_inherited());
}

protected func ControlDown() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDown");

  if (ControlAim("ControlDown")) return(1);
  if (Control2Grab("ControlDown")) return(0);
  if (Control2Contents("ControlDown") ) return(1);
  if (ControlLadder("ControlDown") ) return(1);
  if(Contents(0))
    if (GetPlrDownDouble(GetOwner()))
		if(ReadyToSquatAim())
    {
         StartSquatAiming(); 
         return(1);
    }
  return(_inherited());
}

private func ReadyToSquatAim() {
	return(	!GetEffect("SquatAimTimeout") 
		&&	Contents(0)->~CanAim()
		&& !IsAiming()
		&& (GetAction() eq "WalkArmed" || GetAction() eq "Walk"));
}

protected func ControlLeft() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlLeft");

  if (ControlAim("ControlLeft")) return(1);
  if (Control2Grab("ControlLeft")) return(0);
  if (Control2Contents("ControlLeft")) return(1);
  if (ControlLadder("ControlLeft") ) return(1);
  if (ControlJetpack("ControlLeft") ) return(1);
  if (ControlAgility("ControlLeft") ) return(1);

  return(_inherited());
}

protected func ControlRight() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlRight");

  if (ControlAim("ControlRight")) return(1);
  if (Control2Grab("ControlRight")) return(0);
  if (Control2Contents("ControlRight")) return(1);
  if (ControlLadder("ControlRight") ) return(1);
  if (ControlJetpack("ControlRight") ) return(1);
  if (ControlAgility("ControlRight") ) return(1);

  return(_inherited());
}

protected func ControlLeftDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlAim("ControlLeftDouble")) return(1);
  if (Control2Grab("ControlLeftDouble")) return(0);
  if (Control2Contents("ControlLeftDouble")) return(1);
  if (ControlJetpack("ControlLeftDouble") ) return(1);
  if (ControlAgility("ControlLeftDouble") ) return(1);
  return(_inherited());
}

protected func ControlRightDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlAim("ControlRightDouble")) return(1);
  if (Control2Grab("ControlRightDouble")) return(0);
  if (Control2Contents("ControlRightDouble")) return(1);
  if (ControlJetpack("ControlRightDouble") ) return(1);
  if (ControlAgility("ControlRightDouble") ) return(1);
  return(_inherited());
}

protected func ControlDigDouble() {
  if (GetAction() S= "Push" || IsRiding()) {
    GetActionTarget()->~ControlDigDouble(this());
    return(1);
  }
  if (Control2Grab("ControlDigDouble")) return(0);
  if (Control2Contents("ControlDigDouble")) return(1);
  return(_inherited());
}

protected func ControlDigSingle() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDig");
  if (Control2Grab("ControlDig")) return(0);
  if (Control2Contents("ControlDig")) return(1);
  if (ControlLadder("ControlDig") ) return(1);
  return(_inherited());
}



protected func ContactBottom() {
if(pinguin) {  if (SEqual(GetAction(), "Swim"))
    SetAction("Walk");}
if(pinguin) {  if (SEqual(GetAction(),"SwimArmed"))
    SetAction("WalkArmed");}

if(!(pinguin)) {  if (GetAction() S= "Swim")
    SetAction("Walk");}
if(!(pinguin))  { if (GetAction() S= "SwimArmed")
    SetAction("WalkArmed");}
  return(1);
}

protected func ContactTop()
{
if (Inside(GetYDir(), -50, -26))
SetAction("Hangle");
}

/* Steuerung Werfen */

protected func ControlThrow() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlThrow");

  // Bei vorherigem Doppel-Stop nur Ablegen  
  if ( GetPlrDownDouble(GetOwner()) )
  {
    AddEffect("SquatAimTimeout", this(), 1, 15, this());
    return(inherited());
  }
  // Steuerung an gerittenes Objekt weitergeben
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this()))
      return(1); 

  if (Control2Grab("ControlThrow")) return(0);
  if (Control2Contents("ControlThrow") ) return(1);
  if (ControlLadder("ControlThrow") ) return(1);

  return(inherited());
}

/* Steuerung Inhalt */

protected func ControlSpecial()
  {
  [$CtrlInventoryDesc$|Image=INVT]
  if(!Contents()) return();
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents()->GetID() == GBRB)
    return();
  // wenn wir zielen, wollen wir nur Waffen haben
  if(IsAiming() && Contents(0)->~IsWeapon())
  {
  	// nächste Waffe suchen
  	for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~IsWeapon())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
  }
  else
	  // Inventory verschieben
  	ShiftContents(0,0,0,1);
  UpdateCharge();
  }

protected func Ejection(object pObj)      // Wegwerfen
{
  // Nach Waffe suchen
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection();
}

protected func Collection2(object pObj)           // Einsammeln
{
//autocollect aktiv?
if (AutoCollectIstAnlokal == 1) return(_inherited(pObj));
  // das neue Item nach hinten verschieben (außer es ist Ammo)
  if(!(pObj->~IsAmmoPacket()) || NoAmmo())
    if (1 == ContentsCount(GetID(pObj)))
      if(!(pObj->GetOCF() & OCF_Living)) {
      ShiftContents(0,0,0,0);
      pObj ->~ OnDeselect();
    }
  UpdateCharge();
_inherited(pObj);
}

public func AmmoTransfered(object pAP) {
//  AddEffect("AmmoCollection",this(),100,10,this(),0,pAP->AmmoID(),pAP->AmmoCount());
//auf neues AmmoRest system umgestellt:
  AddEffect("AmmoCollection",this(),100,10,this(),0,pAP->AmmoID(),pAP->AmmoCountRest());
}


protected func RejectCollect(id idObj, object pObj)
{
  // Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this())
      return(1);
  // Spawnpunkt-Hack
  if(idObj == SPNP) return();
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return(0);
  //Waffe?
  if(pObj ->~ IsWeapon()) {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this()))) {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= 3)
        return(1);  //Ja, nicht aufnehmen
      else
        return(0);
    }
    else
      return(0);
  }
  // Einsammellimit für Ausrüstung
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return(1);

//Einsammellimit für Munitionskisten bei Bauarbeitern, maximal 2.
//if(bauarbeiter!=0){
//if(pObj ->~ IsAmmoPacket() && ContentsCount(idObj) > 1)
//    return(1);}

  //Wieviel haben wir denn schon im inventar? Gilt für alle Objekte, die keine Waffen/ausrüstung sind
  if(ContentsCount() - CustomContentsCount("IsWeapon") >= ObjectCollectionLimit())
    return(1);
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return(1);
  
  // Ok
  return(0);
}

protected func Hangling() {
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
}

protected func Scaling()
{
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
  return(_inherited());
}

func FxScaleReloadingTimer()
{
  if(!Contents())
    return();
  if(!WildcardMatch(GetAction(),"*Scale*") && !WildcardMatch(GetAction(), "*Hangle*"))
  {
    ResumeReloading();
    return(-1);
  }
  if(Contents()->~IsReloading())
  	PauseReloading();
}

public func PauseReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->PauseReload();
}

public func ResumeReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->ResumeReload();
}

public func CustomContentsCount(string funcname)
{
  // Alle Inventarobjekte durchlaufen und Waffen zählen
  for(var i=0,j,obj ; obj = Contents(i) ; i++)
    if(ObjectCall(obj, funcname))
      j++;
  // Waffen gezählt
  return(j);
}



// max. Waffen im Inventar. 
public func WeaponCollectionLimit() { return(3); } 

// max. Objekte im Inventar. Ein Bauarbeiter kann mehr tragen.
public func ObjectCollectionLimit() { 
if(bauarbeiter==2 || bauarbeiter==3) return(3);
if(bauarbeiter==4 || bauarbeiter==5) return(4);
if(bauarbeiter==6) return(5);
return(2);
 } 

protected func Departure(pContainer)      // Gebäude verlassen
{
  // nach Waffe suchen
  CheckArmed();
}

protected func ContentsDestruction()      // Wenn Inhaltsobjekte verschwinden
{
  // nach Waffe suchen
  ScheduleCall(this(), "CheckArmed", 1);
  UpdateCharge();
  // nächstes Objekt benachrichtigen
  if(Contents(1)) Schedule("Contents(0)->~Selection(this());", 1);
}

protected func ControlContents(idTarget)
{
  // Zielobjekt will gar nicht, dass wir was machen
  if(idTarget->~NoControlContents()) return();
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents())
    if(Contents()->GetID() == GBRB)
      return(1);

  // controlel Aim -> aufhören zu zielen außer die nächste Waffe kann es
  // Siehe ChangeWeapon
  if(ControlAim("ControlContents",idTarget)) return(1);

  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(0)) Contents(0)->~Deselection(this());
  // Rotieren
  if (!ShiftContents(0, 0, idTarget)) return(1);
  // Waffe ziehen/wegstecken
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection(this());
}

private func ChangeWeapon(object pTarget) {
    // Zielaktion anpassen
    if(pTarget->~IsWeapon())  {
      var phase = GetPhase();
      if(pTarget->~GetFMData(FM_Aim) == 2 || pTarget->~GetFMData(FM_Aim) == -1)
        SetAction("AimSquatLow");
      else
        SetAction("AimSquat");
        
      SetPhase(phase);
    }
    else {
      StopAiming();
    }
}

/* Leitet die Steuerung an das erste Inhaltsobjekt weiter. Rückgabewert 1 wenn erfolgreich */

private func Control2Contents (string command)
{
  // Haben wir was angefasst?
  if(GetAction() S= "Push")
    return(0);
  // Pause Reload: nicht wieder anfangen ey!!!
//  if(command S= "ControlThrow") {
//	if(WildcardMatch(GetAction(),"Scale*") || GetAction() S= "Hangle")
//	  return(1);
//  }
	
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return(1);
  return(0);
}

/* Und mit angefassten Objekten auch noch */

private func Control2Grab ()
{
  // Haben wir was angefasst?
  if(GetAction() eq "Push")
    return(1);

}

/* Spezielles Leiterverhalten */

public func ReleaseLadderUp() {
	//nach oben abspringen
	ReleaseLadder(-15*(GetDir()*2-1),-25);
}

/* Maussteuerung */

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlCommand");

  // Context -> Keine Sonderbehandlung 
  if(szCommand S= "Context") { return(0); }

  if (szCommand S= "MoveTo") { 

    if (Control2Grab("ControlCommand")) return(0);
  
	if (ControlAim("ControlCommand",Par(1),Par(2),Par(3))) return(1);
    // Klettert an einer Leiter
    if (ControlLadder("ControlCommand")) return(1);
  } 
  // Sonst nicht abfangen 
  return(_inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data) ); 
} 

/* Kontextmenü */

protected func ContextHelpMessagesOn(object pCaller) {
  [$CtxHelpMessagesOn$|Image=CXIN|Condition=HelpMessagesOff]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

protected func ContextHelpMessagesOff(object pCaller) {
  [$CtxHelpMessagesOff$|Image=CXIN|Condition=HelpMessagesOn]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

public func HelpMessagesOn() { return(!GetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg")); }
public func HelpMessagesOff() { return(!HelpMessagesOn()); }

protected func ContextGuard(object pCaller) {
  [$CtxGuard$|Image=FLA2|Condition=Outside]
  CreateMenu(FLA2, pCaller, this(),0,0,0,1);
//AddMenuItem("Nur schießen", "Attttack", FLA2, pCaller, 0, pCaller,"Schießt auf alle Feinde, ohne das vorherige Kommando abzubrechen");
//AddMenuItem("Schießen und ein Stück verfolgen", "Fooollow", FLA2, pCaller, 0, pCaller,"Schißet auf alle Feinde und verfolgt sie ein Stück");
//AddMenuItem("Position bewachen", "Beewachenradius", FLA2, pCaller, 0, pCaller,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 100", "Beewachen", FLA2, pCaller, 0, 100,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 200", "Beewachen", FLA2, pCaller, 0, 200,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 300", "Beewachen", FLA2, pCaller, 0, 300,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 400", "Beewachen", FLA2, pCaller, 0, 400,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 500", "Beewachen", FLA2, pCaller, 0, 500,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 600", "Beewachen", FLA2, pCaller, 0, 600,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 700", "Beewachen", FLA2, pCaller, 0, 700,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 800", "Beewachen", FLA2, pCaller, 0, 800,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 900", "Beewachen", FLA2, pCaller, 0, 900,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 1000", "Beewachen", FLA2, pCaller, 0, 1000,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
}

/*
//funktioniert nicht bei Spielerclonks...
func Attttack(idImage, pCaller)
{
  // nur schießen
  SetAggroLevel(Aggro_Shoot);
  Sound("Confirm*");
}

//funktioniert nicht bei Spielerclonks...
func Fooollow(idImage, pCaller)
{
  // schießen und stück verfolgen
  SetAggroLevel(Aggro_Follow);
  Sound("Confirm*");
}

func Beewachenradius(idImage, pCaller)
{
  CreateMenu(FLA2, pCaller, this(),0,0,0,1);
AddMenuItem("Bewachungsradius 100", "Beewachen", FLA2, pCaller, 0, 100,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 200", "Beewachen", FLA2, pCaller, 0, 200,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 300", "Beewachen", FLA2, pCaller, 0, 300,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 400", "Beewachen", FLA2, pCaller, 0, 400,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 500", "Beewachen", FLA2, pCaller, 0, 500,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
}
*/

func Beewachen(idImage, idistanz)
{
  // Bewachen
  SetAggroLevel(Aggro_Guard,idistanz);
  Sound("Confirm*");
}



public func Outside() { return(!Contained()); }

/*
protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=STAP|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Menü erzeugen
  CreateMenu(STAP,this(),0,0,GetName(0,ABAG),0,1,1);
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ammobag))
  {
    // nur Ammo
    if(!(ammo ->~IsAmmo())) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo), "Sound(\"Click\");", GetID(ammo), this(), GetAmmo(GetID(ammo)),0,GetDesc(ammo));
    //Zählervariable erhöhen
    x++;
  }
  return(1);
}

private func AmmoBagContextCheck() {
  return(!FindObject(NOAM));
}
*/


public func NoDeathAnnounce() // Hält 1 Frame lang, also Clonk schnell töten, dann kommt keine Nachricht
{
	AddEffect("NoAnnounce", this, 1, 1, this);
}

public func FxNoAnnounceStop(dummy, dummy2, int iReason)
{
	if(iReason == 4) return -1;
}

private func DeathAnnounce(int plr, object clonk, int killplr) {
  if(GetEffect("NoAnnounce", this)) return;

  if(killplr == -1)
    return(inherited(plr, clonk, killplr));
  
  if(!clonk)
  	return(inherited(plr, clonk, killplr));
  
  //Selfkill?
  if(plr == killplr)
    HHKS->SKMsg(plr);
  else
    HHKS->KTMsg(killplr, plr, clonk->~LastDamageType());
  
  //Killstatistik.
  //Zwar ungenau wenn mehrere Clonks eines Spielers im Spiel sind,
  //aber wann ist das schon der Fall?
  HHKS->KillStat(GetCursor(killplr),plr);
  
  inherited(plr, clonk, killplr);
}

protected func Destruction() {
  // Vorher töten, damit Relaunch ausgeführt wird.
  if(GetAlive(this())) Kill();
  if(ammobag)
    RemoveObject(ammobag);
  return();
}

/* Waffe/Feuern */

public func ReadyToFire() {     // nur aus bestimmten Aktionen feuern

   var a = GetAction();
   var waff;

   // In Gebäude
   if(Contained()) return(Contained()->~ReadyToFire());
   // Reitet
   if(IsRiding()) return(GetActionTarget()->~ReadyToFire());
   // Schiebt
   if(a S= "Push")
   	if(GetActionTarget()->~IsWeapon())
   		return(true);

   // Nur beim Laufen, Schwimmen oder Springen
   if(a eq "WalkArmed"
   || a eq "SwimArmed"
   || a eq "JumpArmed")
		return(true);

   // wenn Waffe die nur im Zielen abgefeuert werden kann, ist die Aktion ohne "armed" und daher bisher unberücksichtigt
   if(Contents()->~IsWeapon())
     if(Contents()->GetFMData(FM_Aim)>0)
       if(a eq "Walk" || a eq "None")    // nicht wenn waffe zum zielen aber schwimmend oder springend
       return(true);
   
   // JetpackFlight fehlt noch, sowie Aim*... das steht in den Funktionalitäten
   return(_inherited());
}

private func CheckArmed() {     // Bewaffnung prÃ¼fen

  if (GetAction() eq "Walk") if (IsArmed2()) return(SetAction("WalkArmed"));
  if (GetAction() eq "Jump") if (IsArmed2()) return(SetAction("JumpArmed"));
  if (GetAction() eq "Swim") if (IsArmed2()) return(SetAction("SwimArmed"));
  if (GetAction() eq "WalkArmed") if (!IsArmed2()) return(SetAction("Walk"));
  if (GetAction() eq "JumpArmed") if (!IsArmed2()) return(SetAction("Jump"));
  if (GetAction() eq "SwimArmed") if (!IsArmed2()) return(SetAction("Swim"));
}

// Der Clonk ist bewaffnet (auch mit einer Waffe die nicht angezeigt wird)
// also Waffen mit denen man Zielen muss
public func IsArmed() {
  if(!Contents()) return(); 
  if(Contents()->~IsWeapon())
    return(true);
  return(false);
}
// Der Clonk hat einen Gegenstand in der Hand, der gezeichnet wird
public func IsArmed2() {
  if(!Contents()) return();
  if(Contents()->~IsDrawable() ||
    (IsArmed() && (Contents()->~GetFMData(FM_Aim) <= 0)))
    return(true);
  return(false);

}

public func JumpStart(bool bBackflip)
{
  // bewaffnet?
  CheckArmed();

  _inherited(bBackflip);
}

protected func ComNone()
  {
  SetComDir(COMD_None);
  }

private func IsJumping() {
  if(GetAction() S= "Dive")
    return(true);
  return(WildcardMatch(GetAction(),"Jump*"));
}

public func StartSquatAiming() {     // Anfangen in der Hocke zu zielen
  // Abbrechen
  if(Contained()) return(1);
  if(GetAction() ne "WalkArmed" && GetAction() ne "Walk")
      return(1);

  SetXDir(0);
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop());

  InitCrosshair();
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func IsSquatAiming () { return(WildcardMatch(GetAction(), "AimSquat*")); }

public func AimOverride () {
	if(Contained()) return(Contained()->~AimOverride());
	
	return(_inherited());
}

public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
  // Parameter zurechtstutzen
  while(iAngle<0) iAngle+=360;
  while(iAngle>360) iAngle-=360;
  if(!iMaxAngle) iMaxAngle = 0;
  if(!iRange) iRange = 350;

  // Landschaft durchsuchen
  // es wird nur auf Lebewesen gezielt
  var targets = FindTargets(this, iRange, iMaxAngle, iAngle, false);
  var target;
  
  // nächstes Ziel finden...
	for(var t in targets) {
		if(!target) {
			target = t;
			continue;
		}
		if(ObjectDistance(t) < ObjectDistance(target)) {
			target = t;
			break;
		}
	}
  
  if(target) DebugLog("Target is %s","hazard",GetName(target));
  return(target);
}

/* Magieanzeige updaten */

public func UpdateCharge()
{

if(!Random(2))  //Sichtradius für Kriegsnebel, definiert im Clonkskript
FogWar();

  // Nur wenn ich Cursor bin
  if(GetCursor(GetOwner()) != this()) return();

  if(GetOwner() < -1) return();

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this()))
      return(1);

  // reitet
  if(IsRiding())
    if(GetActionTarget()->~UpdateCharge(this()))
      return(1);

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() S= "Push")
  	if(GetActionTarget()->~IsWeapon())
  		Content = GetActionTarget();

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, AmmoStoring(),this());
  // prüfen ob Waffe in Hand
  CheckArmed();
  return(1);
}


// Effekte für die neuen Rüstungen aaaaaaaa
private func FxRadiatefffffffzTimer(pTarget, iEffectNumber, iTime)
{
var player;
player= GetOwner(pTarget);
  var maximum_distance = 120;
  // suche nach Zielen und mache denen dann Schaden je nachdem wie weit sie weg sind
  for (var obj in FindObjects(Find_Distance(maximum_distance),Find_Hostile(player), Find_OCF(OCF_Alive)))
  {
    var damage = 100;
    DoEnergy(-damage, obj, true);
    AddEffect("Sparkleschadenz", obj, 1, 1,0,GetID());
    if (!Random(20) && obj->~IsClonk()) Sound("Hurt*");
  }
}


private func FxSparklezTimer(pTarget, iEffectNumber, iTime)
{

  pTarget->CastParticles("MaterialSpark", 1, 50, 0, 0, 10, 30, RGB(200, 200, 200), RGB(100, 255, 100));
}


private func FxSparkleschadenzTimer(pTarget, iEffectNumber, iTime)
{
 // if (iTime > 1 * 1) return -1;
  pTarget->CastParticles("PxSpark", 1, 50, 0, 0, 10, 30, RGB(0, 0, 100), RGB(100, 100, 255)); 
 RemoveEffect("Sparkleschadenz", pTarget);
}


public func AmmoStoring()
{
  // keiner da: neuen erstellen
  if(!ammobag) {
    ammobag = CreateObject(ABAG);
    //ammobag->Init(this());
  }
  return(ammobag);
}


/* Heilung */

public func StopHealing() {      // Heilen beenden
  SetComDir(COMD_Stop());
  SetAction("Walk");
  CheckArmed();
  Sound("Grab");
  while(GetEffect("*Heal*", this()))
    RemoveEffect("*Heal*", this());
}

// Heilt sich mit einem Medikit o.ä.
public func IsHealing () { return(WildcardMatch(GetAction(), "Heal*")); }

/* Bonis Testbereich */
//Mit dem betreten dieses Bereiches begeben sie sich in die Gefahrenzone.
//Weiterlesen nur mit Schutzhelm und funktionierendem Phaser erlaubt.

public func WeaponAt(&x, &y, &r) {

  if(Contained()) return(0);

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // hält Waffe in der Hand
  if(a eq "WalkArmed") {
    x=0;
    y=1000;
    return(1);
  }
  
  else if(a eq "JumpArmed") {
    x=500-125*p;
    y=1000;
    return(1);
  }
  else if(a eq "SwimArmed") {
    x=4000;
    return(1);
  }
  //Jetpack
  else if(a eq "JetpackFlight") {
  
    x=4000;
    r=35;
    return(1);
  }


  // zielt mit der Waffe
  else if(IsAiming()) {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = (Abs(crosshair->GetAngle())-90);
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
    if(a eq "AimLow")
      y += 4000;
    if(a eq "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    if(a eq "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return(1);
  }
/*
  // hat Waffe auf dem Rücken
  else {
    r=90;
    x=-3000;
    y=-2000;

    if(a eq "Walk") {
      y-=2000;
    }
    else if(a eq "Jump") {
      y-=2000;
    }
    else if(a eq "Scale" || a eq "ScaleDown") {
      x-=1000-Cos(p*45,250);
    }
    else if(a eq "Hangle") {
      x-=Cos(p*32,750);
    }
    else if(a eq "Push") {
      r=100;
      x+=1000-Cos(p*90,350);
      y-=2000;
    }
    else if(a eq "Swim") {
      r=135;
      x=-1000+Sin(p*22,500);
      y=-4000-Cos(p*22,250);
    }
    else if(a eq "Dive") {
      r=135+p*10;
      x=-1000;
      y=-4000;
    }
    else */return(0);

/*    return(1);
  }*/
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  return(EffectVar(1, this(), number));
}

/* Munitionsaufsammelanzeige */

func FxAmmoCollectionStart(object pTarget, int iEffectNumber, int iTemp, id idAmmo, int iAmmoCount) {
  if(iTemp)
    return(1);
  EffectVar(0,pTarget,iEffectNumber) = 100;
  EffectVar(1,pTarget,iEffectNumber) = idAmmo;
  EffectVar(2,pTarget,iEffectNumber) = iAmmoCount;
}

func FxAmmoCollectionEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, id idAmmo, int iAmmoCount) {
  if(szNewEffectName S= "AmmoCollection")
    //return(-2);
    // wird nich addiert: alten löschen
    RemoveEffect(0,pTarget,iEffectNumber);
  return(0);
}

func FxAmmoCollectionAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id idAmmo, int iAmmoCount) {
  var x=1;
  while(EffectVar(x,pTarget,iEffectNumber)) {
    if(EffectVar(x,pTarget,iEffectNumber) == idAmmo)
      break;
    else
      x++;
  }
  EffectVar(x++,pTarget,iEffectNumber) = idAmmo;
  EffectVar(x,pTarget,iEffectNumber) += iAmmoCount;

  EffectVar(0,pTarget,iEffectNumber) += 100;
  return(1);
}

func FxAmmoCollectionTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var x=1, string = " ";
  while(EffectVar(x,pTarget,iEffectNumber))
    string = Format("%s|{{%i}} %4d",string,EffectVar(x++,pTarget,iEffectNumber),EffectVar(x++,pTarget,iEffectNumber));
  
  HelpMessage(GetOwner(pTarget),string,pTarget);
  if(iEffectTime >= EffectVar(0,pTarget,iEffectNumber))
    return(-1);
}

public func Incineration() {
  // Hazardclonks brennen ncht wirklich ;)
  Extinguish();
if(!brennt){ // damit bei Brandbomben nicht sofort tot
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);brennt=1;ScheduleCall(0,"Feueristaus",20,0,0);}
}

func Feueristaus()
{brennt=0;}


public func OnHit(int iDmg, int iType, object pFrom)
{
  _inherited(iDmg, iType, pFrom);
  
  if(!GetAlive()) return();
  
  // Besondere Effekte
  if(iType == DMG_Bio)
    AddFireEffect(this(),10,RGB(20,255,30));
	
  // hurt
  if(iDmg > 12)
	  Hurt();
	
  // Aggro-Check
  if(pFrom)
    if(LocalN("shooter", pFrom))
      if(GetAggroLevel() > Aggro_Nothing)
        if(!GetAggroTarget())
          if(CheckTarget(LocalN("shooter", pFrom)))
            SetAggroTarget(LocalN("shooter", pFrom));
}

public func LastDamageType(int type) {
  if(type)
    LastDmgType = type;
  
  return(LastDmgType);
}

/* KI-Zeugs */

// Array mit Effektnummern
// Effektvariablen:
// 0: Kommandoname (string)
// 1: Zielobjekt (objekt)
// 2: Ziel-X (int)
// 3: Ziel-Y (int)
// 4: Verzögerung (int)
// 5: Callbackobjekt (object)
// 6: Aggrolevel
local aMacroCommandList;
// Wegpunktearray
local aPath;

// iAggro = -1 bedeutet keine Änderung am Aggrolevel
// Einen Makrobefehl setzen
public func SetMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
  // kein ordentlicher Befehl -> nichts tun
  if(!szCommand) return();
  if(szCommand ne "MoveTo" && szCommand ne "Follow" && szCommand ne "Wait" && szCommand ne "None")
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return();
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(szCommand eq "MoveTo")
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(szCommand eq "Follow")
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return();
    }
  // Aktuelle Beschäftigung beenden
  var currCom = GetMacroCommand(0,0);
  if(currCom)
  {
    ClearScheduleCall(this(), Format("MacroCom%s", currCom));
    SetCommand(this(), "None");
    SetComDir(COMD_Stop);
  }
  // Liste löschen
  //Log("%s #%d: ClearMacroCom by SetMacroCom", GetName(), ObjectNumber());
  ClearMacroCommands();
  // Befehl eintragen
  var iEffect = AddEffect("MacroCommand", this(), 300, 0, this());
  aMacroCommandList[0] = iEffect;
  EffectVar(0, this(), iEffect) = szCommand;
  EffectVar(1, this(), iEffect) = pTarget;
  EffectVar(2, this(), iEffect) = iX;
  EffectVar(3, this(), iEffect) = iY;
  EffectVar(4, this(), iEffect) = iDelay;
  EffectVar(5, this(), iEffect) = pCallback;
  EffectVar(6, this(), iEffect) = iAggro;
  // Ausführung beantragen
  ClearScheduleCall(this(), "MacroCommandMoveTo");
  ClearScheduleCall(this(), "MacroCommandFollow");
  ClearScheduleCall(this(), "MacroCommandWait");
  //Log("%s #%d: SetMacroCom: '%s' %d/%d Target: %s #%d", GetName(), ObjectNumber(), szCommand, iX, iY, GetName(pTarget), ObjectNumber(pTarget));
  if(szCommand != "None")
    ScheduleCall(this(), Format("MacroCom%s", szCommand), 1);
  else
  {
    ClearMacroCommands();
    if(iAggro != -1) SetAggroLevel(iAggro,0,0,0,"SetMacroCommand");
  }
  return(1);
}

// Befehl an den Anfang setzen
public func AddMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{//Log("%s #%d: AddMacroCom", GetName(), ObjectNumber());
  // Befehlsliste zwischenspeichern
  var aComs = aMacroCommandList;
  // Und leeren
  var currCom = GetMacroCommand(0,0);
  if(currCom)
  {
    ClearScheduleCall(this(), Format("MacroCom%s", currCom));
    SetCommand(this(), "None");
    SetComDir(COMD_Stop);
  }
  aMacroCommandList = CreateArray();
  // Kommando sollen nicht gelöscht werden
  AddEffect("MacroCommandLocked", this(), 301);
  // Befehl einsetzen
  var ret = SetMacroCommand(pCallback, szCommand, pTarget, iX, iY, iDelay, iAggro);
  // Andere Befehle wieder eintragen
  if(ret)
  {
    for(var iCom in aComs)
      if(iCom)
        aMacroCommandList[GetLength(aMacroCommandList)] = iCom;
  }
  else
    aMacroCommandList = aComs;
  // Kommandos freigeben
  RemoveEffect("MacroCommandLocked", this());
  // Rückgabe
  return(ret);
}

// Befehl ans Ende setzen
public func AppendMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
  // kein ordentlicher Befehl -> nichts tun
  if(!szCommand) return();
  if(szCommand ne "MoveTo" && szCommand ne "Follow" && szCommand ne "Wait" && szCommand ne "None")
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return();
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(szCommand eq "MoveTo")
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(szCommand eq "Follow")
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return();
    }
  // Befehl eintragen
  var iEffect = AddEffect("MacroCommand", this(), 300, 0, this());
  aMacroCommandList[GetLength(aMacroCommandList)] = iEffect;
  EffectVar(0, this(), iEffect) = szCommand;
  EffectVar(1, this(), iEffect) = pTarget;
  EffectVar(2, this(), iEffect) = iX;
  EffectVar(3, this(), iEffect) = iY;
  EffectVar(4, this(), iEffect) = iDelay;
  EffectVar(5, this(), iEffect) = pCallback;
  EffectVar(6, this(), iEffect) = iAggro;
  // Starten?
  if(GetLength(aMacroCommandList) == 1)
  {
    if(szCommand != "None")
      Call(Format("MacroCom%s", szCommand));
    else
      ClearMacroCommands();
  }
  return(1);
}

// Abfrage
public func GetMacroCommand(int iCommandNum, int iElement)
{
  // An der Stelle gibt es nichts?
  if(iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return();
  // Element zurückgeben
  return(EffectVar(iElement, this(), aMacroCommandList[iCommandNum]));
}

// Beenden (wenn, dann nur extern gebraucht)
public func FinishMacroCommand(bool fNoCallback, bool fNoSuccess, int iCommandNum)
{
  // An der Stelle gibt es nichts?
  if(iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return();
  // mit Callback -> Callback machen
  if(!fNoCallback)
  {
    if(!fNoSuccess)
      return(MacroComSuccess(iCommandNum));
    else
      return(MacroComSuccessFailed(iCommandNum));
  }
  // Ohne Callback -> kein Callback ;)
  RemoveMacroCommand(iCommandNum);
  // Nächstes Kommando starten
  if(GetMacroCommand() && !iCommandNum)
    if(GetMacroCommand() != "None")
      Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

// Löscht alle Makrobefehle (ohne Callbacks)
public func ClearMacroCommands()
{
  while(GetMacroCommand())
    RemoveMacroCommand();
  aMacroCommandList = CreateArray();
  ClearScheduleCall(this(), "MacroComMoveTo");
  ClearScheduleCall(this(), "MacroComWait");
  ClearScheduleCall(this(), "MacroComFollow");
  ClearScheduleCall(this(), "MacroComSuccessFailed");
  ClearScheduleCall(this(), "JumppadCheck");
}

/* Makros */

// MoveTo
protected func MacroComMoveTo()
{
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Kein Ziel? (bewirkt, dass der Clonk nicht nach 0,0 laufen kann!)
  if(!GetMacroCommand(0,2) && !GetMacroCommand(0,3))
    if(!GetMacroCommand(0,4))
      return(MacroComSuccessFailed(0, Macro_MoveToNoTarget));
  // Pfad suchen
  aPath = CreateArray();
  var start, end;
  start = FindWaypoint(GetX(), GetY());
  end = FindWaypoint(GetMacroCommand(0,2), GetMacroCommand(0,3));
  if(!start || !end) return(MacroComSuccessFailed(0, Macro_NoPath));
  aPath = FindPath(start, end, true);
  // Kein Pfad konnte gefunden werden?
  if(!aPath) return(MacroComSuccessFailed(0, Macro_NoPath));

  // ist der Endpunkt ein Transportpunkt? (ein Wegpunkt mit nur einem Ausgang
  // und dieser Ausgang zeigt nicht zum Eingang)
  if(end->GetPathCount() == 1) {
    var before = 0;
    var leng = GetLength(aPath);
    if(start != end)
      before = aPath[leng-2];
    if(end->GetPathTarget(0) != before) {
      // Ziel ändern (Regel: Pfad darf nicht auf Transportpunkt enden)
      aPath[leng] = end->GetPathTarget(0);
      // Ziel ändern auf folgenden Wegpunkt, Position entsprechend.
      EffectVar(1, this(), aMacroCommandList[0]) = end->GetPathTarget(0);
      EffectVar(2, this(), aMacroCommandList[0]) = GetX(end->GetPathTarget(0));
      EffectVar(3, this(), aMacroCommandList[0]) = GetY(end->GetPathTarget(0));
    }
  }
  // In Bewegung setzen (der erste Wegpunkt sollte durch MoveTo erreichbar sein, wenn nicht -> doof
  // Spezialhack: reitet!
  if(IsRiding()) SetAction("Walk");
  SetCommand(this(), "MoveTo", aPath[0]);
  AppendCommand(this(), "Call", this(), 1,0,0,0, "MacroComMoveToStep");
  // Automatischer Abbruch
  ScheduleCall(this(), "MacroComSuccessFailed", 500, 0, 0, Macro_PathImpossible);
  // Falls Startwegpunkt = Jumppadpunkt
  if(GetLength(aPath) > 1 && aPath[0]->GetPathCount() == 1)
    ScheduleCall(this(), "JumppadCheck", 5, 0, aPath[1], 2);
  // Aggrolevel
  if(GetMacroCommand(0, 6) != -1)
    SetAggroLevel(GetMacroCommand(0, 6),0,0,0,"MacroComMoveTo");
  //Log("%s #%d: MacroComMoveTo", GetName(), ObjectNumber());
}

protected func MacroComMoveToStep(object dummy, int iStep) // MoveTo-Schritt
{
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Wird haben das Ende des Pfades erreicht?
  if(GetLength(aPath) == iStep)
  {//Log("%s #%d: MacroComMoveTo finished", GetName(), ObjectNumber());
    // Zum Ziel laufen und beenden
    var x = GetMacroCommand(0,2), y = GetMacroCommand(0,3);
    SetCommand(this(), "MoveTo", 0, x,y);
    AppendCommand(this(), "Call", this(), 0,0,0,0, "MacroComSuccess");
    return(1);
  }
  // Aus irgendeinem Grund ist kein Wegpunkt da?
  if(!aPath[iStep]) return(MacroComSuccessFailed(0, Macro_PathBroken));
  if(!aPath[iStep-1]) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Nächsten Wegpunkt begehen
  MoveAlongPath(aPath[iStep-1], aPath[iStep], iStep+1);
  AppendCommand(this(), "Call", this(), iStep+1,0,0,0, "MacroComMoveToStep");
  // Automatischer Abbruch
  ClearScheduleCall(this(), "MacroComSuccessFailed");
  // 200 Frames pro 100 Pixel Abstand sind angesetzt
  var breaktime = Max(ObjectDistance(aPath[iStep-1], aPath[iStep]) / 100 * 200, 100);
  ScheduleCall(this(), "MacroComSuccessFailed", breaktime, 0, 0, Macro_PathImpossible);
  //Log("%s #%d: MacroComMoveToStep", GetName(), ObjectNumber());
  return(1);
}

protected func MacroComMoveToStepFailed() // MoveTo-Schritt fehlgeschlagen
{//Log("%s #%d: MacroComMoveToStepFailed", GetName(), ObjectNumber());
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Fehlschlag auf ganzer Linie
  MacroComSuccessFailed(0, Macro_PathImpossible);
  return(1);
}

// Follow
protected func MacroComFollow(bool fStarted)
{
  // Sonderbehandlung: wurde schon gestartet
  if(fStarted)
  {
    DebugLog(Format("%s #%d: MacroComFollow timer", GetName(), ObjectNumber()), "MacroComFollow");
    // Prüfen, ob an der Stelle iPos in der Kette "Follow" steht
    if(GetMacroCommand(1) ne "Follow" && GetMacroCommand(0) ne "Follow")
      // Steht nicht da, ergo muss sich ein anderes Kommando reingeschaltet haben -> nichts tun
      return();
    // Wir haben kein MoveTo mehr? Dann sind wir angekommen (wir starten einfach neu)
    if(GetMacroCommand() ne "MoveTo") return(MacroComFollow());
    // Unser Ziel ist irgendwie weg?
    if(!GetMacroCommand(1,1))
    {
      DebugLog(Format("%s #%d: MacroComFollow target lost", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      RemoveMacroCommand();
      // Fehlschlag ausgeben
      return(MacroComSuccessFailed(0, Macro_FollowTargetLost));
    }
    // Schauen, ob unser Bewegungsziel noch ok ist
    var x,y,target;
    x = GetMacroCommand(0,2);
    y = GetMacroCommand(0,3);
    target = GetMacroCommand(0,1);
    // Distanz > 30 Pixel?
    if(Distance(AbsX(x), AbsY(y), AbsX(target->GetX()), AbsY(target->GetY())) > 30)
    {
      DebugLog(Format("%s #%d: MacroComFollow new MoveTo", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      if(GetMacroCommand() eq "MoveTo") RemoveMacroCommand();
      // Neues Kommando setzen
      AddMacroCommand(0, "MoveTo", GetMacroCommand(0,1), 0, 0, 0, GetMacroCommand(0,6));
    }
    // Follow Weiterlaufen lassen
    return(ScheduleCall(this(), "MacroComFollow", 30, 0, true));
  }
  DebugLog(Format("%s #%d: MacroComFollow started", GetName(), ObjectNumber()), "MacroComFollow");
  // Erstes Kommando ist gar nicht Follow?
  if(GetMacroCommand(0,0) ne "Follow") return();
  // Kein Ziel? -> Fehlschlag
  if(!GetMacroCommand(0,1))
    return(MacroComSuccessFailed());
  // Wir sind dem Ziel noch nicht nahe genug?
  if(ObjectDistance(GetMacroCommand(0,1)) > 30)
    // Follow bedeutet viele MoveTos -> loslaufen bitte
    AddMacroCommand(0, "MoveTo", GetMacroCommand(0,1), 0, 0, 0, GetMacroCommand(0, 6));
  // Aggrolevel
  //if(GetMacroCommand(0, 6) != -1)
    //SetAggroLevel(GetMacroCommand(0, 6));
  // Alle 30 Frames aktualisieren wir
  return(ScheduleCall(this(), "MacroComFollow", 30, 0, true));
}

// Wait
protected func MacroComWait(bool fEnd)
{
  // Erstes Kommando ist gar nicht Wait?
  if(GetMacroCommand(0,0) ne "Wait") return();
  // Beenden?
  if(fEnd)
    return(MacroComSuccess(0));
  // Aggrolevel
  if(GetMacroCommand(0, 6) != -1)
    SetAggroLevel(GetMacroCommand(0, 6),0,0,0,"MacroComWait");
  // Dann warten wir jetzt eine Runde
  return(ScheduleCall(this(), "MacroComWait", GetMacroCommand(0,4), 0, true));
}

/* Makro-Funktionen */

// Makrobefehl an der Stelle iCommand ist erfolgreich
protected func MacroComSuccess(iCommand, int iCmd2)
{//Log("%s #%d: MacroComSuccess", GetName(), ObjectNumber());
  // Für Aufrufe aus SetCommand(... "Call");
  if(GetType(iCommand) == C4V_C4Object) iCommand = iCmd2;
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Hat super geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
  var callback = GetMacroCommand(iCommand, 5);
  if(callback) callback->~OnMacroCommandSuccess(this());
  // Kommando aus der Liste entfernen
  RemoveMacroCommand(iCommand);
  // Evtl. Weg löschen
  aPath = CreateArray();
  // Nächstes Kommando starten
  if(GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

protected func MacroComTumble()
{//Log("%s #%d: MacroComTumble", GetName(), ObjectNumber());
  MacroComSuccessFailed(0, Macro_PathImpossible);
  CheckStuck();
}

// Fehlercodes:
static const Macro_NoPath = 0; // Keinen Weg gefunden
static const Macro_PathImpossible = 1; // Weg konnte abgelaufen werden
static const Macro_PathBroken = 2; // Pfad wurde beim Ablaufen unterbrochen (Wegpunkt gelöscht o.ä.)
static const Macro_FollowTargetLost = 3; // Target für Follow verloren
static const Macro_MoveToNoTarget = 4; // MoveTo hat kein Ziel bekommen

// Makrobefehl an der Stelle iCommand ist fehlgeschlagen
protected func MacroComSuccessFailed(int iCommand, int iReason)
{//Log("%s #%d: MacroComSuccessFailed: %d", GetName(), ObjectNumber(), iCommand);
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Hat gar nicht geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
  var callback = GetMacroCommand(iCommand, 5);
  if(callback) callback->~OnMacroCommandFailed(this(), iReason);
  // Kommando aus der Liste entfernen
  RemoveMacroCommand(iCommand);
  if(!iCommand) SetCommand(this(), "None");
  // Evtl. Weg löschen
  aPath = CreateArray();
  // Nächstes Kommando starten
  if(GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

// Makrobefehle aus der Liste entfernen
private func RemoveMacroCommand(int iCommand)
{//Log("%s #%d: RemoveMacroCommand: %d", GetName(), ObjectNumber(), iCommand);
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Entfernen
  for(var i = 0, iEff, checked ; i < GetEffectCount("MacroCommand", this()) ; i++)
  {
    iEff = GetEffect("MacroCommand", this(), i);
    if(iEff == aMacroCommandList[iCommand])
    {
      RemoveEffect("MacroCommand", this(), i);
      continue;
    }
    if(GetEffect("MacroCommandLocked", this())) continue;
    for(var mac in aMacroCommandList)
      if(mac == iEff)
        checked = true;
    if(!checked) RemoveEffect("MacroCommand", this(), i, true);
    checked = false;
  }
/*  if(RemoveEffect(0, this(), aMacroCommandList[iCommand]))
    Log("Effect removed");*/
  var aNewMacroComList = CreateArray();
  i = 0;
  for(var iCom in aMacroCommandList)
  {
    if(i == iCommand) continue;
    aNewMacroComList[GetLength(aNewMacroComList)] = iCom;
    i++;
  }
  // Erfolgreich entfernt
  aMacroCommandList = aNewMacroComList;
  return(1);
}

public func FxMacroCommandStop(object pTarget)
{
  // Wenn wir gesichert sind, werden wir nicht entfernt
  if(GetEffect("MacroCommandLocked", pTarget)) return(-1);
}

/* Wegfindung */

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
public func FindPath(object pStart, object pEnd, bool fJetpack)
{
  // naive Wegfindung. Wir gehen davon aus, dass wenn der WP links von einem ist,
  // nach links gegangen werden muss und gehen einfach Schritt fuer Schritt in diese
  // Richtung.

  // maximale Iterationen fuer Wegfindung (abhaengig von Anzahl Wegpunkte)
  var iterationLimit = ObjectCount(WAYP);
  
  var ex = GetX(pEnd);
  var ey = GetY(pEnd);

  var pCurrent = pStart;

  var aWaypoints = CreateArray();
  
  var jetp = 0;
  var ammoload = DefinitionCall(JTPK,"GetFMData",FM_AmmoLoad);

  for(var j=0; j< iterationLimit; ++j) {

    aWaypoints[j] = pCurrent;

    // fertig!
    if(pCurrent == pEnd)
      return(aWaypoints);

    // ansonsten...
    var cx = GetX(pCurrent);
    var cy = GetY(pCurrent);
    var cangle = Angle(cx,cy,ex,ey);

    var pNext = 0;
    var pathcount = (pCurrent->WAYP::GetPathCount());
    var aBest = CreateArray();
    var good = 0;
    var neutral = 0;
    var bad = 0;

    // alle Nachbarknoten...
    for(var i=0; i<pathcount; ++i) {
      pNext = (pCurrent->WAYP::GetPathTarget(i));

      if(!Check4Jetpack(pCurrent,i,fJetpack,jetp,ammoload))
        continue;

      // Fuer die richtige Richtung zaehlt nicht der Winkel des naechsten Knotens, sondern
      // wenn pNext nur einen Nachfolger hat, zaehlt die Richtung in die der Nachfolger geht
      // (was rekursiv fortsetzbar ist)
      var jetpgedacht = 0;
      while((pNext->WAYP::GetPathCount()) == 1) {
        // Endknoten? Wir brauchen nicht weiteriterieren
        if(pNext == pEnd) break;
        // für den gedachten Pfad muss noch gecheckt werden, ob da nicht Jetpack benötigt wird
        var check = Check4Jetpack(pNext,0,fJetpack,jetp+jetpgedacht,ammoload);
        if(check)
          pNext = (pNext->WAYP::GetPathTarget(0));
        if(check == 2)
          jetpgedacht += ammoload;
      }

      // Endknoten? Super, fertig!
      if(pNext == pEnd) {
        aBest[0] = i;
        good = 1;
        break;
      }

      // ansonsten
      var nx = GetX(pNext);
      var ny = GetY(pNext);
      var nangle = Angle(cx,cy,nx,ny);
 
      // diffangle: Nimmt Werte von 0 (genau richtig) bis 180 (genau falsch) an
      var diffangle = Abs(Normalize(nangle-cangle,-180));
      // Schummeln: der WP von dem man grad gekommen ist, wird als "bad" eingestuft
      if(j>0)
        if(pNext == aWaypoints[j-1])
          diffangle = 180;
      // möglichst ein Array aus Pfaden zusammenstellen, die etwa in die gewünschte Richtung
      // führen... (Differenz <= 55°)
      if(diffangle <= 55) {
        aBest[good] = i;
        ++good;
      }
      // dann vielleicht nicht ganz so zielführende...? (nur wenn kein Pfad in richtige Richtung führt)
      if(diffangle <= 110 && diffangle > 55 && !good) {
        aBest[neutral] = i;
        ++neutral;
      }
      // ansonsten dann ein Array aus nicht-zielführenden...
      if(diffangle > 110 && !neutral && !good) {
        aBest[bad] = i;
        ++bad;
      }
    }
    // Debug
    if(Debug("waypoints")) {
      var dbg = 0;
      var str = " ";
      var wayp = 0;
      if(good) { str = "good"; dbg = good; }
      else if(neutral) { str = "neutral"; dbg = neutral; }
      else { str = "bad"; dbg = bad; }
      if(dbg>1) {
        DebugLog("  Choosing path from %s...","waypoints",str);
        for(var d = 0; d<dbg; ++d) {
          wayp = (pCurrent->WAYP::GetPathTarget(aBest[d]));
          DebugLog("  waypoint (%d), angle %d","waypoints",ObjectNumber(wayp),Abs(Normalize(Angle(GetX(pCurrent),GetY(pCurrent),GetX(wayp),GetY(wayp))-cangle,-180)));
        }
      }
    }
    
    // zufällig aus möglichen Pfaden auswählen
    var chosenone = -1;
    if(good)			{ chosenone = aBest[Random(good)]; }
    else if(neutral)	{ chosenone = aBest[Random(neutral)]; }
    else if(bad)		{ chosenone = aBest[Random(bad)]; }
    else {
      DebugLog("Failure: Selected waypoint leads into a dead end.","waypoints");
      return(false);
    }
    // ansonsten ok
    pNext = (pCurrent->WAYP::GetPathTarget(chosenone));

    // geschätzter Benzinverbrauch bei Jetpack hochzählen
    if(pCurrent->WAYP::GetPathJetpack(chosenone))
      jetp += ammoload;

    DebugLog("Next %s waypoint (%d), angle %d","waypoints",str,ObjectNumber(pNext),Abs(Normalize(Angle(GetX(pCurrent),GetY(pCurrent),GetX(pNext),GetY(pNext))-cangle,-180)));

    // Fertig
    if(pNext == pEnd) {
      DebugLog("Success: Complete path consists of %d waypoints.","waypoints",GetLength(aWaypoints));
    }    

    // kommt bei naechster Iteration ins Array
    pCurrent = pNext;
  }
  DebugLog("Failure: No path found after %d iterations.","waypoints",iterationLimit);
  return(false);
}

// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
private func FindWaypoint(int iX, int iY)
{
  var wp;
  while(wp = FindObject(WAYP, AbsX(iX), AbsY(iY), -1, -1, 0,0,0,0, wp)) {
    if(PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX(), wp->GetY()))
      return(wp);
  }
  // Nix gefunden
  return();
}

// Vorbedingung für Jetpack prüfen
private func Check4Jetpack(object pCurrent, int path, bool fJetpack, int jetp, int ammoload)
{
  // kein Jetpackpath... dann ist ja gut
  if(!(pCurrent->WAYP::GetPathJetpack(path)))
    return(1);
  // ansonsten Vorbedingungen checken
  if(fJetpack)
    if(HasJetpack())
      //if(ammoload <= GetAmmo(GSAM)-jetp)
        return(2);
  return(0);
}

// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
  // Rausfinden, welchen Weg wir gehen müssen
  for(var i = 0, path = -1 ; i < pCurrentWp->GetPathCount() ; i++)
    if(pCurrentWp->GetPathTarget(i) == pNextWp)
    {
      path = i;
      break;
    }
  // Kein Weg? -> Fehler
  if(path < 0) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Sonderbehandlung für Zielwegpunkte, die nur einen Pfad von sich wegführen haben (da könnten Jumppads o.ä. sein)
  ClearScheduleCall(this(), "JumppadCheck"); // Zur Sicherheit
  if(GetLength(aPath) > iNextStep && pNextWp->GetPathCount() == 1)
    ScheduleCall(this(), "JumppadCheck", 5, 0, pNextWp, iNextStep);
  // Jetpackfliegen
  if(pCurrentWp->GetPathJetpack(path))
    ScheduleCall(this(), "StartJetpack", pCurrentWp->GetPathJetpack(path), 0, pCurrentWp->GetPathJetpackFlag(path));
  // Flag rausfinden und entsprechend agieren
  var flag = pCurrentWp->GetPathFlag(path);
  if(flag == Path_MoveTo)
  {
    // Hinlaufen
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Jump)
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(dir);
    Jump();
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Backflip)
  {
    // Backflippen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(!dir);
    var iEff = AddEffect("ControlStack", this, 110, 5, this);
		if(GetDir() == DIR_Left)
		  EffectVar(0, this, iEff) = COMD_Right;
		else
		  EffectVar(0, this, iEff) = COMD_Left;
    Jump();
    ScheduleCall(0, "JumpStart", 1, 1, true);

    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Lift)
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(0, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
    return(AddSpecialCommands(pCurrentWp, path));
  }
  // Unbekanntes flag, MoveTo versuchen
  SetCommand(this(), "MoveTo", pNextWp);
  return(AddSpecialCommands(pCurrentWp, path));
}

/* Sonderfunktionen */

protected func JumppadCheck(object pTargetWp, int iNextStep)
{
  // Pfad hat sich irgendwie geändert
  if(GetLength(aPath) <= iNextStep) return();
  var pNextWp = aPath[iNextStep];
  // Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
  if(aPath[iNextStep-1] != pTargetWp)
    return();
  // Schauen, ob wir näher an pNextWp dran sind
  if(ObjectDistance(pTargetWp) > ObjectDistance(pNextWp))
    // Wegpunkt wird übersprungen
    return(MacroComMoveToStep(0, iNextStep));
  // Weiterlaufen
  ScheduleCall(this(), "JumppadCheck", 5, 0, pTargetWp, iNextStep);
}

protected func ClimbLadder()
{
  if(!GetMacroCommand() && !GetCommand()) return();
  // Testen, ob wir noch klettern
  if(GetAction() ne "ScaleLadder") return();
  // Feststellen, ob nach oben oder unten
  var targetx = GetCommand(this(), 2);
  var targety = GetCommand(this(), 3);
  
  /*
  if(GetActTime() > 5) {
    if(targetx < GetX() && GetDir() != DIR_Right)
      ControlLadder("ControlLeft");
    if(targetx > GetX() && GetDir() != DIR_Left)
      ControlLadder("ControlRight");
  }
  */
  
  if(targety < GetY()) SetComDir(COMD_Up);
  if(targety > GetY()) SetComDir(COMD_Down);
  
  // 2do: Wir müssen irgendwie feststellen, ob die Leiter aus ist. :(
  
  
  // Wegpunkt oben? früher abspringen.
  if(Inside(GetY()-targety,5,15) && Abs(GetX()-targetx) < 30)
  {
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
    
  // Sind wir ungefähr da?
  if(Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
  {
    // Wir versuchen mal, abzuspringen (das klappt spätestens beim 2. Mal(theoretisch))
    
    
    //wenn Ziel weiter oben: immer nach oben abspringen
    if(GetY() > targety)
      SetComDir(COMD_Up);
    //Ziel weiter unten? Nach unten abspringen.
    else if(Abs(GetY()-targety) > 50)
      SetComDir(COMD_Down);
    //Ziel weiter weg? Backflip! :D
    else if(Abs(GetX()-targetx) > 80)
      SetComDir(COMD_Stop);
    
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
  // Weiter checken
  ScheduleCall(this(), "ClimbLadder", 10);
}

protected func StartJetpack(int iDir)
{
  // Kein Jetpack mehr? oO
  var jetpack = HasJetpack();
  if(!jetpack) return();
  // Richtung zünden
  if(iDir == Jetpack_Left) // Links
  {
    SetDir(DIR_Left);
    jetpack->ControlLeftDouble(this());
  }
  if(iDir == Jetpack_UpLeft) // Oben links
  {
    SetDir(DIR_Left);
    jetpack->ControlUpDouble(this());
  }
  if(iDir == Jetpack_UpRight) // Oben rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlUpDouble(this());
  }
  if(iDir == Jetpack_Right) // Rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlRightDouble(this());
  }
}

protected func LiftControl(object dummy, int pCurrentWp, int pNextWp)
{
  // Fässt den Lift schon an?
  if(GetAction() eq "Push")
  {
    var lift = GetActionTarget();
    if(!lift) return();
    // Nah genug am Ziel? Absteigen
    if(ObjectDistance(Object(pNextWp)) <= 50)
      return(AddCommand(this(), "MoveTo", Object(pNextWp)));
    // Liftplatten befehligen
    lift->~ControlCommand("MoveTo",0, Object(pNextWp)->GetX());
    // Warten
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this(), "Wait", 0,0,0,0,0, 15);
    return(1);
  }
  // Liftplatte suchen
  var x1 = Object(pCurrentWp)->GetX();
  var x2 = Object(pNextWp)->GetX();
  if(x1 > x2) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
  var lift = FindObject2(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, x2-x1, 50));
  // Kein Lift? -> Fehlschlag
  if(!lift) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Lift nah genug? -> Einsteigen
  if(ObjectDistance(lift) <= 50)
  {
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this(), "Grab", lift);
    return(1);
  }
  // Warten
  AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
  
  AddCommand(this(), "Wait", 0,0,0,0,0, 15);
  return(1);
}

// Alles was der Wegpunkt als ArriveCommand hat
protected func AddSpecialCommands(object pCurrentWp, int path)
{
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= pCurrentWp->GetArriveCommandCount(path) - 1,command ; i > -1 ; i--)
    AddCommand(this(), pCurrentWp->GetArriveCommand(path, i), 
                       pCurrentWp->GetArriveCommand(path, i, 1),
                       pCurrentWp->GetArriveCommand(path, i, 2),
                       pCurrentWp->GetArriveCommand(path, i, 3),
                       pCurrentWp->GetArriveCommand(path, i, 4), 0,
                       pCurrentWp->GetArriveCommand(path, i, 5));
}

/* KI-Kampf */

// Setzt einen bestimmten Aggro-Wert für den Clonk
// 0 - keinerlei eigenmächtiges Handeln
// 1 - schießen auf Feinde, ohne dabei vom aktuellen Kommando abzuweichen
// 2 - auf Feinde wird geschossen und sie werden über kurze Strecken verfolgt
// 3 - Position (iX, iY) bewachen und dabei nur iDist weit weglaufen (wenn Feind verfolgt werden)
// Konstanten:
static const Aggro_Nothing = 0;
static const Aggro_Shoot   = 1;
static const Aggro_Follow  = 2;
static const Aggro_Guard   = 3;

public func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
//  if(GetOwner() < -1) Message("@SetAggroLevel: %d: %s", this(), iLevel, text);
  // > 3
  if(iLevel > 3) return();
  // Wir kommen von > 2
  var target;
  if(GetAggroLevel() >= 2 && iLevel < 2)
    if(target = EffectVar(1, this(), GetEffect("Aggro", this())))
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
  // 0
  if(!iLevel) return(RemoveEffect("Aggro", this()));
  // 1, 2 und 3
  var effect = GetEffect("Aggro", this());
  if(!effect) effect = AddEffect("Aggro", this(), 50, 10, this());
  EffectVar(0, this(), effect) = iLevel;
  // Parameter
  if(!iDist)
    {iDist = 500;}
  //Log(Format("distanz=%v",iDist));
  EffectVar(2, this(), effect) = iDist;
  // iX und iY, Default = GetX() & GetY()
  if(!iX && !iY && iLevel == 3)
  {
    iX = GetX();
    iY = GetY();
  }
  EffectVar(3, this(), effect) = iX;
  EffectVar(4, this(), effect) = iY;
  return(1);
}

public func GetAggroLevel()
{
  var effect = GetEffect("Aggro", this());
  if(!effect) return();
  return(EffectVar(0, this(), effect));
}

// Setzt sofort das Angriffsziel
public func SetAggroTarget(object pTarget)
{
  if(GetAggroLevel() == Aggro_Nothing) return();
  EffectVar(1, this(), GetEffect("Aggro", this())) = pTarget;
  return(1);
}

public func GetAggroTarget()
{
  return(EffectVar(1, this(), GetEffect("Aggro", this())));
}

public func FxAggroStart()
{
    // -> Waffen durchchecken, weil wenn nie zuvor geladene Waffe, dann funzt bewachen nicht.
    CheckIdleWeapon();
    return(1);
}

public func FxAggroTimer(object pTarget, int no)
{
 //Wenn geblendet, dann nichts machen
 if(geblendet) return;
  // keine Waffe geladen? Dann waffen durchchecken  
// Waffenstatus durchgehen, vorher zählen
  var waffenanzahl = CustomContentsCount("IsWeapon");
   var o = 0;
   for(var u = 0; u < ContentsCount(); u++)
       if(Contents(u)->~IsWeapon())
  		{
       if(!Contents(u)->~GetCharge()) ++o;
       if(Contents(u)->~GetCharge()) break;
  		}
  // wenn o nun gleich waffenanzahl, dann heißt das, dass alle Waffen leer sind, dann nachladen.
   if(o == waffenanzahl)
   return(CheckIdleWeapon());

  // Wir haben ein Ziel?
  if(EffectVar(1, this(), no)) { EffectCall(this(), no, "Fire"); return(1); }
  // Zielen beenden
  if(IsAiming()) StopAiming();
//  Message("@No target", this());
  // Ziel suchen
  var dist = EffectVar(2, this(), no);
  var dir = GetDir()*2-1;
  // Vorne
  var target = GetTarget(90*dir, 90,dist);
  // Hinten
  if(!target)
    if((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this(), no) != 1)
      target = GetTarget(-90*dir, 90,dist);
  // Gefunden?
  if(!target)
  {
    //Log("kein Ziel gefunden");
    // Nichts gefunden :(
    // -> Bescheid geben!
    if(EffectVar(99, this(), no))
    {
      if(Contained())
        Contained()->~HandleAggroFinished(this());
      else if(IsRiding())
        GetActionTarget()->~HandleAggroFinished(this());
      
      EffectVar(99, this(), no);
    }
    // -> Waffen durchchecken
    CheckIdleWeapon();
    return();
  }
  // Super
  EffectVar(1, this(), no) = target;
  EffectVar(99,this(), no) = true; // wir haben ein Ziel \o/
}

public func FxAggroFire(object pTarget, int no)
{

 //Wenn geblendet, dann nichts machen
 if(geblendet) return;

  // Zusatzhack: BR-Bombe!
  if(GetID(Contents()) == GBRB)
    // Nichts tun :C
    return();
  var y = EffectVar(4, this(), no);
  var x = EffectVar(3, this(), no);
  var dist = EffectVar(2, this(), no);
  var target = EffectVar(1, this(), no);
  var level = EffectVar(0, this(), no);
  var pathfree = true;
  
  // Nichts tun, wenn gerade verhindert
  if(ReadyToFire())
 {

  // Fahrzeugsteuerung
  if(Contained())
  {
    if(Contained()->~HandleAggro(this(), level, target, dist, x, y))
      return(1);
    else
      return(AddCommand(this(), "Exit", 0,0,0,0,0,0,0, C4CMD_SilentSub));
  }
  if(IsRiding())
  {
    if(GetActionTarget()->~HandleAggro(this(), level, target, dist, x, y))
      return(1);
    else
      return(SetAction("Walk"));
  }
 }
  
  // Zu weit von der Wachposition entfernt?
  if(level == 3) {
    if(Distance(GetX(), GetY(), x, y) > dist)
    {
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
       // Log("zu weit von Wachposition entfernt");
      AddMacroCommand(0, "MoveTo", 0, x,y, 0, level);
      EffectVar(1, this(), no) = 0;
	  //      Message("@Returning to guarded position", this());
      return();
    }
  }
  
  var maxdist = dist;
  if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(level == 1) maxdist = 0;
    if(level >= 2) maxdist = dist/2;
    pathfree = false;
  }
  //Log("kurz vor ziel weg");
  // Ziel irgendwie weg?
  // (Pathfree wurde schon gecheckt)...  reicht nicht, da zielen und schießen nicht aufhört, wenn was zwischen gegner, aber noch in Radius, und daher wird auch nicht nachgeladen -.-
  if(!CheckTarget(target,this,maxdist,0,0,true) || !PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
    {
      EffectVar(1, this(), no) = 0;
      if(EffectVar(0, this(), no) == 2)
        ClearMacroCommands();
      if(!IsAiming()) if(Contents(0)) Contents(0)->~AimStop(); // damit auch das Autofeuer aufhört
      if(IsAiming())
        StopAiming();     
        //Log("Ziel iwie weg");
      return();
    }
  // Ich hab nix? °-°
  if(!Contents()) return(); // Lauf, Forest, lauf!
  //Log("nach ich hab nix");
  // Waffe in die Hand nehmen
  if(!SelectWeapon(level, target, false))
    // Evtl. Feuermodus wechseln (dann muss erst nachgeladen werden, aber besser als nichts)
    if(!SelectWeapon(level, target, true))
    {
      // Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
      if(GetAggroLevel() == Aggro_Follow)
      {
//      Message("@Searching for weapons / ammo", this());
        // Waffen auffrischen?
        if(CustomContentsCount("IsWeapon") <= 1)
          return(SearchWeapon(Aggro_Shoot));
        // Munition auffrischen
        return(SearchAmmo(Aggro_Shoot));
      }
      // ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
        //Log("Balrog?");
        return();
    }
  // Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

  if(!ReadyToFire()) return();

  // Schaue ich in die richtige Richtung?
  if(GetX() < target->GetX())
  {
    if(GetDir() != DIR_Right)
      SetDir(DIR_Right);
  }
  else
  {
    if(GetDir() != DIR_Left)
      SetDir(DIR_Left);
  }
     //Log("kurz vor zielen muss auch mal sein");
  // Zielen, muss auch mal sein
 if((!GetCommand() && !GetMacroCommand()) || level != 1 || IsAiming())
 {
  if(pathfree && Contents()->GetBotData(BOT_Range) > 30) // Weg frei und keine Nahkampfwaffe?
  {
    var angle = Angle(GetX(), GetY(), target->GetX(), target->GetY());
    if(((!Inside(angle, 70, 120) && !Inside(angle, 250, 290)) || Contents()->GetFMData(FM_Aim)>0) 
       //  && maxdist != 300 && ObjectDistance(target) < 300
)
    {
      if(!IsAiming()) StartSquatAiming();
      if(IsAiming())
      {
	    //Log("zielen > 0 wird ausgeführt");
	    var tx = target->GetX();
		var ty = target->GetY();
		
        if(Contents()->GetBotData(BOT_Ballistic))
          ty -= 15;
		
		DoMouseAiming(tx, ty);
      }
    }
    else
      if(IsAiming())
        StopAiming();
  }
	if(IsAiming() && !CheckAmmo(Contents()->GetFMData(FM_AmmoID), Contents()->GetFMData(FM_AmmoLoad), Contents(), this()))
	 	StopAiming();
 }
 
   // Gut. Feuern wir bereits?
  if(Contents()->IsRecharging() || Contents()->IsShooting()) return();
 
  // Feuer!
  if(maxdist != 300 && pathfree) Control2Contents("ControlThrow");
//  Message("@My target: %s @%d/%d with level %d", this(), target->GetName(), target->GetX(), target->GetY(), level);
  // Stufe 2 - verfolgen!
  if(EffectVar(0, this(), no) >= 2)
    if(GetMacroCommand(1) ne "Follow" || GetMacroCommand(1, 1) != target)
      if(GetMacroCommand(0) ne "Follow" || GetMacroCommand(0,1) != target)
      {
        DebugLog("FxAggroFire - Adding Follow command","aggro");
        AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
        AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
      }
  //Log("sollte gefeuert haben?");
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  //Log("Selectweapon wird geprüft");
  // Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return();
  // Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  // Alle durchgehen und passende prüfen
  for(var i=0,obj,fav,mode,favmode ; obj = Contents(i) ; mode++)
  {
    // Nix Waffe
    if(!(obj->~IsWeapon())) { i++; mode = -1; continue; }
    // Feuermodus
    if(mode && !fFireModes) { i++; mode = -1; continue; }
    if(!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
    if(mode == obj->GetFireMode() && mode) continue;
    // Nix gut... Hier wird jetzt auch die Aim Action von Bazooka und Co erkannt, weil damit sonst nicht geschossen wird -.-
    if(obj->GetFMData(FM_Aim, mode)>0)
      if(iLevel == 1 || (!WildcardMatch(GetAction(), "*Walk*") && !WildcardMatch(GetAction(), "Aim*")))
        continue;
    // Keine Munition dafür?
    if(!(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
      continue;
    // EMP nur gegen Maschinen
    if(obj->GetBotData(BOT_EMP, mode))
      if(!(pTarget->~IsMachine()))
        continue;
    // Kein Favorit bisher?
    if(!fav)
    {
      fav = obj;
      type = fav->GetBotData(BOT_DmgType, mode);
      favmode = mode;
    }
    else
    {
      // Favorit hat nicht genug Reichweite
      if(fav->GetBotData(BOT_Range, favmode) < dist)
      {
        // Neue Waffe hat mehr
        if(obj->GetBotData(BOT_Range, mode) > dist)
        {
          fav = obj;
          type = obj->GetBotData(BOT_DmgType, mode);
          favmode = mode;
        }
      }
      else
      {
        // Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
        if(pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) &&
          // Allerdings darf die Waffe nicht zu schwach sein
          fav->GetBotData(BOT_Power, favmode)-1 <= obj->GetBotData(BOT_Power, mode))
        {
          // Neuer Favorit
          fav = obj;
          type = fav->GetBotData(BOT_DmgType);
          favmode = mode;
        }
        else
        {
          // Stärke der neuen Waffe ist größer oder Favorit ist ein Langlader
          if(fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
             (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
          {
            // Waffe hat keine extralange Nachladezeit
            if(obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
            {
              // Neuer Favorit
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
            // Waffe sollte nicht nachladen und nicht leer sein
            else if(obj->GetCharge() != 0 && !(obj->IsReloading()))
              {
                // Neuer Favorit
                fav = obj;
                type = fav->GetBotData(BOT_DmgType);
                favmode = mode;
              }
          }
        } 
      }
      // Reichweite passt
      if(fav->GetBotData(BOT_Range, favmode) >= dist)
        // Schadenstyp auch
        if(preftype == type)
          // Stärke auch
            if(fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
              break;
    }
  }
  // Auswählen
  if(!fav) return();
  // Feuermodus wechseln?
  if(fFireModes)
    if(favmode && favmode != fav->GetFireMode())
      fav->SetFireMode(favmode);
  if(ContentsCount() == 1) return(1);
  return(ShiftContents(0,0,fav->GetID()));
}

public func GetPrefDmgType(object pTarget)
{
  var min = pTarget->~OnDmg(DMG_Projectile);
  var type = DMG_Projectile;
  if(pTarget->~OnDmg(DMG_Melee) < min)
  {
    type = DMG_Melee;
    min = pTarget->~OnDmg(type);
  }
  if(pTarget->~OnDmg(DMG_Fire) < min)
  {
    type = DMG_Fire;
    min = pTarget->~OnDmg(type);
  }
  if(pTarget->~OnDmg(DMG_Explosion) < min)
  {
    type = DMG_Explosion;
    min = pTarget->~OnDmg(type);
  }
  if(pTarget->~OnDmg(DMG_Energy) < min)
  {
    type = DMG_Energy;
    min = pTarget->~OnDmg(type);
  }
  if(pTarget->~OnDmg(DMG_Bio) < min)
    type = DMG_Bio;
    
  return(type);
}

// Sucht nach Waffen und läuft dorthin
public func SearchWeapon(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(),this()))
        	// Hinlaufen
        	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Sucht nach Munition und läuft dorthin
public func SearchAmmo(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(),this()))
      	// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
      	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

/* Waffenbehandlung wenn nicht im Kampf */

public func CheckIdleWeapon()
{
  if(Contents())
    // Hack - mit BR-Bombe tut er gar nichts
    if(Contents()->GetID() == GBRB) return;
  // Keine Waffen im Inventar
  if(!CustomContentsCount("IsWeapon")) return();
  // nachladende Waffe in der Hand
  if(Contents()->~IsWeapon())
    if(Contents()->IsReloading())
      return();
     //Log("Inventar nach waffe zum laden durchsuchen");
  // Inventar nach Waffe durchsuchen, die man Nachladen könnte
  for(var i=0, mode=1, obj, favmode ; obj = Contents(i) ; mode++)
  {
    // Keine Waffe
    if(!(obj->~IsWeapon()))
    {
     //Log("keine Waffe");
      i++;
      mode = 0;
      continue;
    }
    // Waffe hat gar nicht so viele Modi
    if(!(obj->GetFMData(FM_Name, mode)))
    {
      //Log("Waffe hat nicht genug modi");
      i++;
      mode = 0;
      continue;
    }
    // Waffe ist voll geladen, hier geändert dass nicht /2, sondern auf andrer Seite der gleichung *2, weil 1/2==0 wäre.
    if(obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode))*2 >= obj->GetFMData(FM_AmmoLoad, mode))
    {
      //Log("waffe ist geladen");
      i++;
      mode = 0;
      continue;
    }
    // EMP-Modi erstmal nicht laden
    if(obj->GetBotData(BOT_EMP, mode)) continue;
    // Waffe ist nachladbar
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode) - obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode))))
    {
      mode = obj->GetFireMode();
      break;
    }
    // Nächsten Feuermodus prüfen
    if(mode == obj->GetFireMode()) continue;
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
      break;
  }
  // Nix gefunden
  if(!Contents(i)) return();
  // Aha! Waffe wechseln!
  ShiftContents(this(), 0, obj->GetID());
  // Feuermodus wechseln
  obj->SetFireMode(mode);
  // Und Muni reinhauen
  obj->Reload();
  // Klasse
  return(1);
}

/* Inventarbehandlung (wird von KI-Spieler aufgerufen) */

// Inventar behandeln
public func CheckInventory()
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
  {
      // BR-Hack
    if(Contents()->GetID() == GBRB)
    {
      BombingRunBomb();
      break;
    }
    // Wenn man etwas hardcoden möchte, dann einfach die Funktion AI_Inventory im Objekt erstellen und true zurückgeben
    if(obj->~AI_Inventory(this))
      continue;
    // Waffe
    if(obj->~IsWeapon())
      continue;
    // Ausrüstungsgegenstand
    if(obj->~IsHazardGear())
    {
      // Die KI braucht kein Licht und keine doppelten Dinge
	  // Kontext der Auskommentierung: http://cppp.tyron.at/bugtrack.php?shbug=833
	  // KI legt Taschenlampen an damit Spieler sie in der Dunkelheit sehen können
      if(/*obj->GetGearType() == GEAR_Light || */HasGear(obj->GetGearType()))
      {
        DropObject(obj);
        continue;
      }
      // Ausrüstung wird prinzipiell als was gutes angesehen -> anlegen
      ActivateGear(obj);
      continue;
    }
    // Mine
    if(obj->~IsMine())
    {
      // Platzieren
      PlaceMine(obj);
      continue;
    }
    // Upgrade
    if(obj->~IsUpgrade())
    {
      var wpn = GetUpgradeableWeapon(obj);
      // Eine passende Waffe gefunden -> upgraden (Upgrades sind immer gut.)
      if(wpn) UpgradeWeapon(obj, wpn);
      continue;
    }
    // Munition
    if(obj->~IsAmmoPacket())
    {
      ActivateAmmo(obj);
      continue;
    }
    // Objekt ist wertlos für uns
    DropObject(obj);
  }
}

// Objekt fallen lassen (verzögert, damit die Schleife nicht durcheinander kommt
public func DropObject(object pObj)
{
  Schedule(Format("Exit(Object(%d), 0, 10);", ObjectNumber(pObj)), 1, 0, this());
  // Nicht wieder einsammeln
  var effect = AddEffect("CollectionException", pObj, 1, 36);
  EffectVar(0, pObj, effect) = this();
}

// Ausrüstung anlegen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateGear(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Mine platzieren (verzögert, damit die Schleife nicht durcheinander kommt
public func PlaceMine(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Waffe suchen, die man upgraden kann
public func GetUpgradeableWeapon(object pObj)
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsUpgradeable(pObj->GetID()))
      return(obj);
}

// Waffe upgraden (verzögert, damit die Schleife nicht durcheinander kommt
public func UpgradeWeapon(object pObj, object pWeapon)
{
  ScheduleCall(pObj, "UpgradeObject", 1, 0, pWeapon);
}

// Munition aufnehmen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateAmmo(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Hardgecodete Objekte

// Support-Drohne
// Zufällige Waffe für eine Drohne aussuchen
private func GetRandomDroneWeapon()
{
  var aWps = CreateArray();
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsWeapon())
      if(obj->GetFMData(FM_Aim)<=0)
        aWps[GetLength(aWps)] = obj;
  // Zufällig zurückgeben
  if(!GetLength(aWps)) return();
  return(aWps[Random(GetLength(aWps))]);
}

// BR-Bombe
protected func BombingRunBomb()
{
  // nächstes, feindliches Tor suchen
  var nextding, distance = -1;
  for(var pGate in FindObjects(Find_ID(GBRG)))
  {
    if(pGate->GetGoalTeam() == GetPlayerTeam(GetOwner())) continue;
    if(distance == -1)
    {
      nextding = pGate;
      distance = ObjectDistance(pGate);
    }
    else
    {
      if(ObjectDistance(pGate) < distance)
      {
        nextding = pGate;
        distance = ObjectDistance(pGate);
      }
    }
  }
  // Tor ist zu weit weg?
  if(distance > 200)
  {
    distance = -1;
    nextding = false;
  }
  // Freunde suchen, wenn der Timer abläuft
  if(!nextding && Contents()->GetBombTimer() <= 100)
  {
    for(var pFriend in FindObjects(Find_OCF(OCF_CrewMember), Find_Allied(GetOwner()), Find_Exclude(this)))
    {
      if(distance == -1)
      {
        nextding = pFriend;
        distance = ObjectDistance(pFriend);
      }
      else
      {
        if(ObjectDistance(pFriend) < distance)
        {
          nextding = pFriend;
          distance = ObjectDistance(pFriend);
        }
      }
    }
  }
  // Ziel?
  if(nextding)
  {

    // Ziel ist in Reichweite?
    if(distance <= 200 && PathFree(GetX(), GetY(), nextding->GetX(), nextding->GetY()))
    {
      // Winkel bestimmen
      var angle = Angle(GetX(), GetY(), nextding->GetX(), nextding->GetY());

      // Winkel stimmt!
      if(Inside(angle, 70, 110) || Inside(angle, 250, 290))
      {
		if(ReadyToFire()) {
	        if(GetX() < nextding->GetX())
	          SetDir(DIR_Right);
	        else
	          SetDir(DIR_Left);
	        // Feuer frei!
	        Contents()->ControlThrow();
		}
      }
      else
      {
        // Zielen beginnen
        if(!IsAiming()) {
			if(ReadyToSquatAim())
				StartSquatAiming();
		}
        else
        {
          // Feuer frei!
          if(GetDir() == DIR_Left)
            angle = 360-angle;
          angle = BoundBy(angle-10, 0, AIM_Max);
          crosshair->SetAngle(angle);
          UpdateAiming();
          Contents()->ControlThrow();
        }
      }
    }
  }
}


func Schutzanzugan()
{
hatschutzanzug=1;
}

func Schutzanzugaus()
{
hatschutzanzug=0;
}

func HatSchutzAnzugan()
{
if(hatschutzanzug) return(true);
if(!(hatschutzanzug)) return(false);
}


// pinguinanzug unter wasser arbeiten
func CheckObjectAvailability (obj)
{
if(!(pinguin)) return(0);
    // Object is near enough
    if (!Inside (GetX () - GetX (obj), -500, +500)) return ();
    if (!Inside (GetY () - GetY (obj), -250, +250)) return ();
    // Object is not connected to a pipe (for line construction kits)
    if (FindObject (SPIP, 0, 0, 0, 0, 0, "Connect", obj)) return ();
    if (FindObject (DPIP, 0, 0, 0, 0, 0, "Connect", obj)) return ();
    // Must be complete
    if (~GetOCF (obj) & OCF_Fullcon) return ();
    // Doesn't burn
    if (OnFire (obj)) return ();
    // Go!
    return (obj);
}

func GetAvailableObject (def, xobj)
{
if(!(pinguin)) return(0);
  var obj;
  // Next closest normal available or in liquid
  while (obj = FindObject (def, 0, 0, -1, -1, OCF_Available | OCF_InLiquid, 0, 0, 0, obj))
  {
    if (ObjectDistance (obj) > 559) { obj = 0; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (!(Contained (obj) == xobj) || !xobj)
      // We found one
      break;
  }
  if (obj) return (obj);
  // Next closest in container (because containers are sunk here, nothing in them is "available"...)
  while (obj = FindObject (def, 0, 0, -1, -1, OCF_Fullcon, 0, 0, AnyContainer(), obj))
  //while (obj = FindObject (def, 0, 0, -1, -1, OCF_Fullcon(), 0, 0, 0, obj))
  {
    if (ObjectDistance (obj) > 559) { obj = 0; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (Contained (obj) == xobj) continue;
    // The Container must be accessible (Same as for OCF_Available)
    if (GetDefCoreVal ("GrabPutGet", 0, GetID (Contained(obj))) & C4D_GrabGet || GetOCF (Contained(obj)) & OCF_Entrance)
    // We found one
      break;
  }
  return (obj);
}


public func ControlCommandAcquire(target, x, y, target2, def)
{
if(!(pinguin)) return(0);
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return (1, AddCommand (this(), "Get", obj,0,0,0,40));
  return (inherited (target,x,y,target2,def));
}


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

func Pinguinan()
{
pinguin=1;
}

func Pinguinaus()
{
pinguin=0;
}

// Ammogürtel, bekommt die Zahl vom Gürtel geliefert
func GetMoreAmmo(zzaahhll)
{
if(zzaahhll==1) mehrmunition1=1;
if(zzaahhll==2) mehrmunition2=1;
if(zzaahhll==3) mehrmunition3=1;
if(zzaahhll==4) mehrmunition4=1;
if(zzaahhll==5) mehrmunition5=1;
if(zzaahhll==6) mehrmunition6=1;
if(zzaahhll==7) mehrmunition7=1;
if(zzaahhll==8) mehrmunition8=1;
if(zzaahhll==9) mehrmunition9=1;
if(zzaahhll==10) mehrmunition10=1;
if(zzaahhll==11) mehrmunition11=1;
if(zzaahhll==12) mehrmunition12=1;
if(zzaahhll==13) mehrmunition13=1;
if(zzaahhll==14) mehrmunition14=1;
if(zzaahhll==15) mehrmunition15=1;
if(zzaahhll==16) mehrmunition16=1;
if(zzaahhll==17) mehrmunition17=1;
if(zzaahhll==18) mehrmunition18=1;
if(zzaahhll==19) mehrmunition19=1;
if(zzaahhll==20) mehrmunition20=1;
}




func GetNormalAmmo()
{
mehrmunition1=0;
mehrmunition2=0;
mehrmunition3=0;
mehrmunition4=0;
mehrmunition5=0;
mehrmunition6=0;
mehrmunition7=0;
mehrmunition8=0;
mehrmunition9=0;
mehrmunition10=0;
mehrmunition11=0;
mehrmunition12=0;
mehrmunition13=0;
mehrmunition14=0;
mehrmunition15=0;
mehrmunition16=0;
mehrmunition17=0;
mehrmunition18=0;
mehrmunition19=0;
mehrmunition20=0;
// wenn aktuelle Munition mehr ist, als es ohne Gürtel möglich sein sollte:

}

// Projektile klein groß
func MoreAmmo1()
{
if(mehrmunition1==1) return(true);
return(false);
}

func MoreAmmo2()
{
if(mehrmunition2==1) return(true);
return(false);
}

// Energie
func MoreAmmo3()
{
if(mehrmunition3==1) return(true);
return(false);
}

func MoreAmmo4()
{
if(mehrmunition4==1) return(true);
return(false);
}

//Grananten
func MoreAmmo5()
{
if(mehrmunition5==1) return(true);
return(false);
}

func MoreAmmo6()
{
if(mehrmunition6==1) return(true);
return(false);
}

//Benzin
func MoreAmmo7()
{
if(mehrmunition7==1) return(true);
return(false);
}

func MoreAmmo8()
{
if(mehrmunition8==1) return(true);
return(false);
}

//Raketen
func MoreAmmo9()
{
if(mehrmunition9==1) return(true);
return(false);
}

func MoreAmmo10()
{
if(mehrmunition10==1) return(true);
return(false);
}

//BetterBullets
func MoreAmmo11()
{
if(mehrmunition11==1) return(true);
return(false);
}

func MoreAmmo12()
{
if(mehrmunition12==1) return(true);
return(false);
}

// Schrotkugeln
func MoreAmmo13()
{
if(mehrmunition13==1) return(true);
return(false);
}

func MoreAmmo14()
{
if(mehrmunition14==1) return(true);
return(false);
}

//SniperKugeln
func MoreAmmo15()
{
if(mehrmunition15==1) return(true);
return(false);
}

func MoreAmmo16()
{
if(mehrmunition16==1) return(true);
return(false);
}

//Partikel
func MoreAmmo17()
{
if(mehrmunition17==1) return(true);
return(false);
}

func MoreAmmo18()
{
if(mehrmunition18==1) return(true);
return(false);
}


func MoreAmmoAllrounder1()
{
if(mehrmunition19==1) return(true);
return(false);
}

func MoreAmmoAllrounder2()
{
if(mehrmunition20==1) return(true);
return(false);
}


func Klimaschutz(klima)
{
//Kälte
if(klima==1)
{
//wenn schutzkleidung, dann kein Schaden
if(hatschutzanzug || pinguin || hatanorak) return(1);
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, kein Schaden.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_ID(LAGF),Find_ID(TORC))))
{
//nur wenn Feuer brennt
if(GetID(feuer)==LAGF) if(!(GetAction(feuer) eq "Burn")) return(0);
 if(GetID(feuer)==TORC) if(!(GetAction(feuer) eq "Burning")) return(0);
  return(1);}
return(0);
}

//Hitze
if(klima==2)
{
//wenn schutzkleidung, dann kein Schaden
if(hatschutzanzug) return(1);
//Wenn in Flüssigkeit, kein schaden.
if(GetOCF() & OCF_InLiquid()) return(1);
return(0);
}

}




/* Serialisierung */

public func Serialize(array& extra)
{
	var x, ammo;
	while(ammo = Contents(x++,ammobag)) {
		if(!(ammo ->~IsAmmo())) continue;
		extra[GetLength(extra)] = Format("DoAmmo(%i,%d)", GetID(ammo), GetAmmo(GetID(ammo)));
	}
}




local target;

protected func IsBulletTarget() { return(1); } // von kugeln werden alle lebewesen getroffen, egal ob bullettarget oder nicht..
func IsMutant() { return(1); }


func ControlSpecial2()
{
 if (GetProcedure() eq "WALK" && Contents())
  if (Contents()->~IsGrenade())
   SetAction("ThrowGrenade");
 if (GetProcedure() eq "WALK") SetAction("Kick"); 
 return(1);
}

func ThrowGrenade()
{
 if (GetPhase() != 4) return(0);
 var Cont = Contents();
 if (!Cont) return(0);
 Sound("ThrowGrenade");
 Exit(Cont,-5+GetDir()*10,0,0,-5+GetDir()*10,-3,Random(200));
 return(1);
}

func Stab()
{
 if (GetPhase() != 3) return(0);
 for (var prey in FindObjects(Find_Distance(10),Find_OCF(OCF_Alive),Find_Owner(NO_OWNER)))
  Punch(prey,RandomX(12,20));  
 return(1);
}

func Kick()
{
 if (GetPhase() != 3) return(0);
 for (var prey in FindObjects(Find_Distance(5),Find_Exclude(),Find_Or(Find_OCF(OCF_Rotate),Find_OCF(OCF_Living))))
  if (GetAlive(prey)) Fling(prey,-2+GetDir()*4,-1);
   else if (SetXDir(-8+GetDir()*16,prey)) SetYDir(-50,prey);
 return(1);
}


protected func Death()
{
  GameCallEx("Zaehler16");
 SetAction("BigAttack");
 CastParticles("PSpark",Random(40),Random(80),0,0,0,Random(150),RGB(100,0,0));
 Incinerate();
 FadeOut();
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Schedule("Explode(50)",50);
 else Schedule("FakeExplode(50,0,0,50)",50);
 return(1);
}

func ContactLeft() { ChangeDir(); }
func ContactRight() { ChangeDir(); }

func Activity()
{
 if (!GetAlive()) return(0);
 //if(!Random(3)) DoEnergy(+Random(1));
 if (Stuck()) DigFree(GetX(),GetY(),16);
 for (var obj in FindObjects(Find_NoContainer(),Find_Not(Find_Owner(-1)),Find_OCF(OCF_Alive|OCF_CrewMember),
//Find_Func("IsAttackable"),
Find_Distance(300)))
  if (SetCommand(this(),"MoveTo",obj))
   SetDir(GetX()<GetX(obj));
 for (var clonk in FindObjects(Find_NoContainer(),Find_InRect(-200+GetDir()*200,-20,200,20),Find_Not(Find_Owner(-1)),Find_OCF(OCF_CrewMember|OCF_Alive)))
   if (GetProcedure() ne "THROW") 
    if (!Random(8))
     SetAction("Attack");
 if (!Random(16))
  ChangeDir();
 if (!Random(19)) SetComDir(COMD_Stop);
 return(1);
}

func CheckClonks()
{
 for (var clonk in FindObjects(Find_NoContainer(),Find_Not(Find_Owner(-1)),Find_InRect(-80+GetDir()*80,-20,80,20),Find_OCF(OCF_Alive)))
  if (Punch(clonk,Random(35)))
   Fling(clonk,-7+GetDir()*14,-Random(5));
 Sound("MireokAttack");
 return(1);
}

func MakeDestruction()
{
 for (var clonk in FindObjects(Find_Not(Find_Owner(-1)),Find_Distance(100),Find_Exclude(),Find_Or(Find_Category(C4D_Object),Find_Category(C4D_Living))))
 {
  if (Punch(clonk,Random(90)))
   Fling(clonk,-20+GetDir()*40,-Random(5));
  DoDamage(Random(30),clonk);
  SetXDir(RandomX(-10,10),clonk);
  SetXDir(RandomX(-10,10),clonk);
  if (!Random(30)) Incinerate(clonk);
 }
 CreateParticle("Wave",0,0,RandomX(-10,10),RandomX(-10,10),3000,RGB(255,0,255));
 Sound("MireokAttack");
 return(1);
}

public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  if(iType == DMG_Explosion)    return(-20);  
  // Energieschaden
  if(iType == DMG_Energy)     return(50);  
  // Projektilschaden
  if(iType == DMG_Projectile)    return(50);  
  // Feuerschaden
  if(iType == DMG_Fire)    return(-80);  
  // Bioschaden
  if(iType == DMG_Bio)    return(20);  
  // Nahkampfschaden
  if(iType == DMG_Melee)    return(60);  
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)    return(30);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(0);
}

func ChangeDir()
{
 SetYDir(RandomX(-2,10));
 SetDir(!GetDir());
 if (GetDir()) SetComDir(COMD_Right);
 else
 SetComDir(COMD_Left);
 return(1);
}

public func IsThreat() {return(true);}