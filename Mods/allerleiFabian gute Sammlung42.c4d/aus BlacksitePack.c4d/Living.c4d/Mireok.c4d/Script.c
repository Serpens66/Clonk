/*---- MireokClonk ----*/

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
local bauarbeiter,ausbildungstimer;

local zielSichtweite,zahler, SetSichtweite;

// je nach Einstellung in den variablen Regeln gibt es unterschiedliches Sichtradienverhalten der Clonks
protected func FogWar() 
{
if(GetLength(hashSpielRegeln))
zahler=HashGet(hashSpielRegeln, "SichtradiusVerhalten");
else {zahler=nil;}

if(Equal(zahler, nil)) {
if ((FrameCounter() ?? 0)<5000) {
if(!Random(100))  // zu anfang ab und zu wieder Kriegsnebel deaktivieren, falls er weggeht
for(var i1=nil; (i1 ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i1, 1, true)) {
  SetFoW(nil,i1);} }
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



// Tag, Nacht und Tunnel Sichtradius
func FogWar1()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

var z=nil;
while(!GBackSky(GetX(nil),(GetY(nil) ?? 0)-(z ?? 0))) S42MigrationStep(z, 1);
var himmely = (AbsY(GetY(nil)) ?? 0)-(z ?? 0); // y koordiante über Clonk, wo das erste mal himmer ist

  if(
And((Equal(GetMaterial(), Material("Tunnel")))
 , (!PathFree(GetX(nil),GetY(nil),GetX(nil),himmely)))
) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagTunnel");  else{zielSichtweite = 120;} } // Tunnel und keine direkte Verbindung zum Himmel? Dann eine Sichtweite von 25 anstreben

  if(
And(And((Equal(GetMaterial(), Material("Tunnel"))), IsNight())
 , (!PathFree(GetX(nil),GetY(nil),GetX(nil),himmely)))
 ) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtTunnel"); else{zielSichtweite = 80;} } // Tunnel+ fehlende Verbindung zum  Himmel? UND Nacht?! Dann eine Sichtweite von 15 anstreben


  if((SetSichtweite ?? 0) > (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "-=", 1);
  if((SetSichtweite ?? 0) < (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "+=", 1);

//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

SetPlrViewRange(SetSichtweite,nil,1);
}


// nur Tag Nacht Sichtradius
func FogWar2()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100
  if(IsNight()) {if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusNachtHimmel"); else{zielSichtweite = 200;} }// Nacht? Dann eine Sichtweite von 50 anstreben

  if((SetSichtweite ?? 0) > (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "-=", 1);
  if((SetSichtweite ?? 0) < (zielSichtweite ?? 0)) S42MigrationAssign(SetSichtweite, "+=", 1);

//Message("%d %d",this(),SetSichtweite,zielSichtweite); 

SetPlrViewRange(SetSichtweite,nil,1);
}


// Immer Sichtweite 
func FogWar3()
{
 if(GetLength(hashSpielRegeln)) zielSichtweite = HashGet(hashSpielRegeln, "SichtradiusTagHimmel"); else{zielSichtweite = 400;}  // Standard und am Tag: 100

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



func Klimaschutz(klima)
{
//Kälte
if(Equal(klima, 1))
{
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, kein Schaden.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_ID(LAGF),Find_ID(TORC))))
{
//nur wenn Feuer brennt
if(Equal(GetID(feuer), LAGF)) if(!(!!SEqual(GetAction(feuer), "Burn"))) return(nil);
 if(Equal(GetID(feuer), TORC)) if(!(!!SEqual(GetAction(feuer), "Burning"))) return(nil);
  return(1);}
return(nil);
}

//Hitze
if(Equal(klima, 2))
{
// wenn in Flüssigkeit, kein schaden
if((GetOCF() ?? 0) & (OCF_InLiquid ?? 0)) return(1);
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
// Umweltverhältnisse erstellen:
Pollute();
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
starterfahrung = GetObjectInfoCoreVal("Experience", nil, nil);
}



func Physicsanpassen()
{
// vorher noch kurz einen Effekt erstellen. Denn beim Tod von Clonks werden die Items entfernt, dies lässt sich nicht mit der Deathfunktion unterbinden, weil es schon davor passiert. Vor death werden die Effekte gekillt, das kann man nutzen um schon dann die Items in eine Leiche zu verschieben
 //AddEffect("LeichenDeath", this, 1, 0, this); // stopp zu finden über der death funktion

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
    DolphinJump();
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
  DolphinJump();
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




//Baustoff des Gebäudes wird ab und zu gespawnt beim Bauen, wenn Bauarbeiter
private func Building()
{

  if(FindObject(CNMT))  // nur wenn Baumaterial eingeschaltet ist, dann Material spawnen
  {
  var randomzahl = Random(4);  // damit nicht nur die erste Komponente erstellt wird, sondern bis zu den ersten 4
  if(Equal(bauarbeiter, 1)) {  var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(100), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 5

  if(Equal(bauarbeiter, 2)) {var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(95), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 10

  if(Equal(bauarbeiter, 3)) {var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(90), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 15

  if(Equal(bauarbeiter, 4)) {var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(80), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 20

  if(Equal(bauarbeiter, 5)) {var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(70), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 25

  if(Equal(bauarbeiter, 6)) {var building=GetActionTarget(nil,this());
  var baustoff= GetComponent(nil,randomzahl,building); if(And(!Random(60), baustoff)) CreateObject(baustoff,AbsX(GetX()),AbsY(GetY()),GetOwner()) ;} //-> ca in 20 Frames 30
  }
  // immer wenn Bauarbeiter schneller bauen
  if(Equal(bauarbeiter, 1)) {  var building=GetActionTarget(nil,this());
  if(!Random(4)) DoCon(1,building);}
  if(Equal(bauarbeiter, 2)) {var building=GetActionTarget(nil,this());
  if(!Random(2))DoCon(1,building);}
  if(Equal(bauarbeiter, 3)) {var building=GetActionTarget(nil,this());
  if(!Random(4))DoCon(3,building);}
  if(Equal(bauarbeiter, 4)) {var building=GetActionTarget(nil,this());
  if(!Random(2))DoCon(2,building);}
  if(Equal(bauarbeiter, 5)) {var building=GetActionTarget(nil,this());
  if(!Random(4))DoCon(5,building);}
  if(Equal(bauarbeiter, 6)) {var building=GetActionTarget(nil,this());
  if(!Random(2))DoCon(3,building);}
if (FindObject(SOB_))
{
var building=GetActionTarget(nil,this());
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
 return(1);
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


/*---- HazardClonk ----*/

#strict 3
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
AutoCollectIstAnlokal=nil;
}


public func Chopping()
{
  if (!GetActTime()) return; // Erster Schlag kein Sound. Clonk holt noch aus.
  if(Or(Equal(bauarbeiter, 1), Equal(bauarbeiter, 2))  ) DoDamage(15, GetActionTarget());
  if(Or(Equal(bauarbeiter, 3), Equal(bauarbeiter, 4))  ) DoDamage(30, GetActionTarget());
  if(Or(Equal(bauarbeiter, 5), Equal(bauarbeiter, 6))  ) DoDamage(40, GetActionTarget());  // mehr Schaden -> Baum fällt schneller um
  Sound("Chop*");
  CastParticles("Dust",(Random(3) ?? 0)+1,6,-8+16*(GetDir() ?? 0),1,10,12);
  return(1);
}


protected func Initialize()
{
  //dieser Effekt ist für die Waffen in den Händen zuständig
  wpneffect = AddEffect("ShowWeapon",this(),1,1,this(),HZCK);
  // Magieanzeige ist hier die Munitionsanzeige - nicht mehr
//  SetPhysical("Magic",100000,2);
  //Energie fixen (kA wo das gechanged wird...) UH H4X
  DoEnergy( -(GetPhysical("Energy") ?? 0) / 1000 + 1); //Noch 1 Energie
  SetPhysical("Energy", 150000, 2); //Physical ändern
  DoEnergy( (GetPhysical("Energy") ?? 0) / 100 - 1); // Neu dazugeben
  // Munitionsgürtel
  AmmoStoring();
  GetHUD(this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,nil);

  return(inherited());
}

public func IsThreat() { return(true); }

/* Rekrutierung */

// Anzahl Standardportraits
public func StdPortraitCount() { return(3); }

protected func Recruitment(int iPlr)
{
  // Zur Netzwerk-Sicherheit
  iPlr ??= 0;
  var iRandomPortrait = Random(StdPortraitCount());
  // Mal schauen, was fuer ein Portrait wir haben
  var portrait = GetPortrait(this(), nil, 1);
  var portrait_id = GetPortrait(this(), 1, 1);
  // Ist es ein eigenes Portrait? Dann lassen wir es.
  if(!!SEqual(portrait, "custom")) return(_inherited(iPlr) );
  // Ist eine ID angegeben?
  if(And(portrait_id, !Equal(portrait_id, NONE)))
  {
    // Kein Hazardportrait?
    if(!Equal(portrait_id, GetID()) )
    {
      SetPortrait(Format("%d", 1 + (iRandomPortrait ?? 0)), this(), GetID(), true, nil);
      return(_inherited(iPlr) );
    }
  }
  // Es handelt sich um ein Hazardportrait. Aber ein richtiges?
  if(!WildcardMatch(portrait, "?") )
  {
    // Nein, das war ein Ruestungsportrait oder sowas. Wir wollen aber lieber ein normales.
    SetPortrait(Format("%d", 1 + (iRandomPortrait ?? 0)), this(), GetID(), true, nil);
  }
  return(_inherited(iPlr) );
}

/* Steuerung Richtung (an Inhalt weitergeben, sonst internen Befehl ausführen) */

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(Control2Grab("ControlUpdate", comdir, dig, throw)) return(nil);
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlAim("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);

  return(_inherited(clonk, comdir, dig, throw));
}

protected func ControlUp() {
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlUp");

  if (ControlAim("ControlUp")) return(1);
  if (Control2Grab("ControlUp")) return(nil);
  if (Control2Contents("ControlUp") ) return(1);
  if (ControlLadder("ControlUp") ) return(1);
  if (ControlAgility("ControlUp") ) return(1);
  return(_inherited());
}

protected func ControlUpDouble() {

  if (ControlAim("ControlUp")) return(1);
  if (Control2Grab("ControlUpDouble")) return(nil);
  if (Control2Contents("ControlUpDouble") ) return(1);
  if (ControlJetpack("ControlUpDouble") ) return(1);
  if (ControlAgility("ControlUpDouble") ) return(1);
  return(_inherited());
}

protected func ControlDown() {
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlDown");

  if (ControlAim("ControlDown")) return(1);
  if (Control2Grab("ControlDown")) return(nil);
  if (Control2Contents("ControlDown") ) return(1);
  if (ControlLadder("ControlDown") ) return(1);
  if(Contents(nil))
    if (GetPlrDownDouble(GetOwner()))
		if(ReadyToSquatAim())
    {
         StartSquatAiming(); 
         return(1);
    }
  return(_inherited());
}

private func ReadyToSquatAim() {
	return(	And(And(And(!GetEffect("SquatAimTimeout")
		,	Contents(nil)->~CanAim())
		, !IsAiming())
		, (Or(!!SEqual(GetAction(), "WalkArmed"), !!SEqual(GetAction(), "Walk")))));
}

protected func ControlLeft() {
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlLeft");

  if (ControlAim("ControlLeft")) return(1);
  if (Control2Grab("ControlLeft")) return(nil);
  if (Control2Contents("ControlLeft")) return(1);
  if (ControlLadder("ControlLeft") ) return(1);
  if (ControlJetpack("ControlLeft") ) return(1);
  if (ControlAgility("ControlLeft") ) return(1);

  return(_inherited());
}

protected func ControlRight() {
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlRight");

  if (ControlAim("ControlRight")) return(1);
  if (Control2Grab("ControlRight")) return(nil);
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
  if (Control2Grab("ControlLeftDouble")) return(nil);
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
  if (Control2Grab("ControlRightDouble")) return(nil);
  if (Control2Contents("ControlRightDouble")) return(1);
  if (ControlJetpack("ControlRightDouble") ) return(1);
  if (ControlAgility("ControlRightDouble") ) return(1);
  return(_inherited());
}

protected func ControlDigDouble() {
  if (Or(!!SEqual(GetAction(), "Push"), IsRiding())) {
    GetActionTarget()->~ControlDigDouble(this());
    return(1);
  }
  if (Control2Grab("ControlDigDouble")) return(nil);
  if (Control2Contents("ControlDigDouble")) return(1);
  return(_inherited());
}

protected func ControlDigSingle() {
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlDig");
  if (Control2Grab("ControlDig")) return(nil);
  if (Control2Contents("ControlDig")) return(1);
  if (ControlLadder("ControlDig") ) return(1);
  return(_inherited());
}



protected func ContactBottom() {
if(pinguin) {  if (SEqual(GetAction(), "Swim"))
    SetAction("Walk");}
if(pinguin) {  if (SEqual(GetAction(),"SwimArmed"))
    SetAction("WalkArmed");}

if(!(pinguin)) {  if (!!SEqual(GetAction(), "Swim"))
    SetAction("Walk");}
if(!(pinguin))  { if (!!SEqual(GetAction(), "SwimArmed"))
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
  SetAggroLevel(nil,nil,nil,nil,"ControlThrow");

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

  if (Control2Grab("ControlThrow")) return(nil);
  if (Control2Contents("ControlThrow") ) return(1);
  if (ControlLadder("ControlThrow") ) return(1);

  return(inherited());
}

/* Steuerung Inhalt */

protected func ControlSpecial()
  {
  [$CtrlInventoryDesc$|Image=INVT]
  if(!Contents()) return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Equal(Contents()->GetID(), GBRB))
    return;
  // wenn wir zielen, wollen wir nur Waffen haben
  if(And(IsAiming(), Contents(nil)->~IsWeapon()))
  {
  	// nächste Waffe suchen
        for(var i = 1; (i ?? 0) < (ContentsCount() ?? 0); S42MigrationStep(i, 1, true))
  		if(Contents(i)->~IsWeapon())
  		{
  			// zur Waffe wechseln
                        ShiftContents(nil,nil,Contents(i)->GetID(),true);
  			break;
  		}
  }
  else
	  // Inventory verschieben
        ShiftContents(nil,nil,nil,1);
  UpdateCharge();
  }

protected func Ejection(object pObj)      // Wegwerfen
{
  // Nach Waffe suchen
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(nil)) Contents(nil)->~Selection();
}

protected func Collection2(object pObj)           // Einsammeln
{
//autocollect aktiv?
if (Equal(AutoCollectIstAnlokal, 1)) return(_inherited(pObj));
  // das neue Item nach hinten verschieben (außer es ist Ammo)
  if(Or(!(pObj->~IsAmmoPacket()), NoAmmo()))
    if (Equal(1, ContentsCount(GetID(pObj))))
      if(!((pObj->GetOCF() ?? 0) & (OCF_Living ?? 0))) {
      ShiftContents(nil,nil,nil,nil);
      pObj ->~ OnDeselect();
    }
  UpdateCharge();
_inherited(pObj);
}

public func AmmoTransfered(object pAP) {
//  AddEffect("AmmoCollection",this(),100,10,this(),0,pAP->AmmoID(),pAP->AmmoCount());
//auf neues AmmoRest system umgestellt:
  AddEffect("AmmoCollection",this(),100,10,this(),nil,pAP->AmmoID(),pAP->AmmoCountRest());
}


protected func RejectCollect(id idObj, object pObj)
{
  // Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(Equal(EffectVar(nil, pObj, effect), this()))
      return(1);
  // Spawnpunkt-Hack
  if(Equal(idObj, SPNP)) return;
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return(nil);
  //Waffe?
  if(pObj ->~ IsWeapon()) {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this()))) {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(Or(ContentsCount(idObj), (CustomContentsCount("IsWeapon") ?? 0) >= 3))
        return(1);  //Ja, nicht aufnehmen
      else
        return(nil);
    }
    else
      return(nil);
  }
  // Einsammellimit für Ausrüstung
  if(And(pObj ->~ IsEquipment(), (ContentsCount(idObj) ?? 0) > 0))
    return(1);

//Einsammellimit für Munitionskisten bei Bauarbeitern, maximal 2.
//if(bauarbeiter!=0){
//if(pObj ->~ IsAmmoPacket() && ContentsCount(idObj) > 1)
//    return(1);}

  //Wieviel haben wir denn schon im inventar? Gilt für alle Objekte, die keine Waffen/ausrüstung sind
  if((ContentsCount() ?? 0) - (CustomContentsCount("IsWeapon") ?? 0) >= (ObjectCollectionLimit() ?? 0))
    return(1);
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(Equal(GetID(gear), idObj))
        return(1);
  
  // Ok
  return(nil);
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
    return;
  if(And(!WildcardMatch(GetAction(),"*Scale*"), !WildcardMatch(GetAction(), "*Hangle*")))
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
      if((c->GetFMData(FM_Aim) ?? 0) > 0)
        c->PauseReload();
}

public func ResumeReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if((c->GetFMData(FM_Aim) ?? 0) > 0)
        c->ResumeReload();
}

public func CustomContentsCount(string funcname)
{
  // Alle Inventarobjekte durchlaufen und Waffen zählen
  for(var i=nil,j,obj ; obj = Contents(i) ; S42MigrationStep(i, 1, true))
    if(ObjectCall(obj, funcname))
      S42MigrationStep(j, 1, true);
  // Waffen gezählt
  return(j);
}



// max. Waffen im Inventar. 
public func WeaponCollectionLimit() { return(3); } 

// max. Objekte im Inventar. Ein Bauarbeiter kann mehr tragen.
public func ObjectCollectionLimit() { 
if(Or(Equal(bauarbeiter, 2), Equal(bauarbeiter, 3))) return(3);
if(Or(Equal(bauarbeiter, 4), Equal(bauarbeiter, 5))) return(4);
if(Equal(bauarbeiter, 6)) return(5);
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
  if(idTarget->~NoControlContents()) return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents())
    if(Equal(Contents()->GetID(), GBRB))
      return(1);

  // controlel Aim -> aufhören zu zielen außer die nächste Waffe kann es
  // Siehe ChangeWeapon
  if(ControlAim("ControlContents",idTarget)) return(1);

  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(nil)) Contents(nil)->~Deselection(this());
  // Rotieren
  if (!ShiftContents(nil, nil, idTarget)) return(1);
  // Waffe ziehen/wegstecken
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(nil)) Contents(nil)->~Selection(this());
}

private func ChangeWeapon(object pTarget) {
    // Zielaktion anpassen
    if(pTarget->~IsWeapon())  {
      var phase = GetPhase();
      if(Or(Equal(pTarget->~GetFMData(FM_Aim), 2), Equal(pTarget->~GetFMData(FM_Aim), -1)))
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
  if(!!SEqual(GetAction(), "Push"))
    return(nil);
  // Pause Reload: nicht wieder anfangen ey!!!
//  if(command S= "ControlThrow") {
//	if(WildcardMatch(GetAction(),"Scale*") || GetAction() S= "Hangle")
//	  return(1);
//  }
	
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return(1);
  return(nil);
}

/* Und mit angefassten Objekten auch noch */

private func Control2Grab ()
{
  // Haben wir was angefasst?
  if(!!SEqual(GetAction(), "Push"))
    return(1);

}

/* Spezielles Leiterverhalten */

public func ReleaseLadderUp() {
	//nach oben abspringen
	ReleaseLadder(-15*((GetDir() ?? 0)*2-1),-25);
}

/* Maussteuerung */

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{
  ClearMacroCommands();
  SetAggroLevel(nil,nil,nil,nil,"ControlCommand");

  // Context -> Keine Sonderbehandlung 
  if(!!SEqual(szCommand, "Context")) { return(nil); }

  if (!!SEqual(szCommand, "MoveTo")) {

    if (Control2Grab("ControlCommand")) return(nil);
  
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
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", nil);
  Sound("Click", 1, nil,nil, (GetOwner() ?? 0)+1);
}

protected func ContextHelpMessagesOff(object pCaller) {
  [$CtxHelpMessagesOff$|Image=CXIN|Condition=HelpMessagesOn]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", true);
  Sound("Click", 1, nil,nil, (GetOwner() ?? 0)+1);
}

public func HelpMessagesOn() { return(!GetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg")); }
public func HelpMessagesOff() { return(!HelpMessagesOn()); }

protected func ContextGuard(object pCaller) {
  [$CtxGuard$|Image=FLA2|Condition=Outside]
  CreateMenu(FLA2, pCaller, this(),nil,nil,nil,1);
//AddMenuItem("Nur schießen", "Attttack", FLA2, pCaller, 0, pCaller,"Schießt auf alle Feinde, ohne das vorherige Kommando abzubrechen");
//AddMenuItem("Schießen und ein Stück verfolgen", "Fooollow", FLA2, pCaller, 0, pCaller,"Schißet auf alle Feinde und verfolgt sie ein Stück");
//AddMenuItem("Position bewachen", "Beewachenradius", FLA2, pCaller, 0, pCaller,"Schießt und bewegt sich ein Stück, kehrt aber zu Ausgangspunkt zurück");
AddMenuItem("Bewachungsradius 100", "Beewachen", FLA2, pCaller, nil, 100,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 200", "Beewachen", FLA2, pCaller, nil, 200,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 300", "Beewachen", FLA2, pCaller, nil, 300,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 400", "Beewachen", FLA2, pCaller, nil, 400,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 500", "Beewachen", FLA2, pCaller, nil, 500,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 600", "Beewachen", FLA2, pCaller, nil, 600,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 700", "Beewachen", FLA2, pCaller, nil, 700,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 800", "Beewachen", FLA2, pCaller, nil, 800,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 900", "Beewachen", FLA2, pCaller, nil, 900,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
AddMenuItem("Bewachungsradius 1000", "Beewachen", FLA2, pCaller, nil, 1000,"Schießt auf alles innerhalb des Radius, beachte dabei den Schussradius deiner Waffe!");
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
	iReason ??= 0;
	if(Equal(iReason, 4)) return -1;
}

private func DeathAnnounce(int plr, object clonk, int killplr) {
  plr ??= 0;
  killplr ??= 0;
  if(GetEffect("NoAnnounce", this)) return;

  if(Equal(killplr, -1))
    return(inherited(plr, clonk, killplr));
  
  if(!clonk)
  	return(inherited(plr, clonk, killplr));
  
  //Selfkill?
  if(Equal(plr, killplr))
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
  return;
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
   if(!!SEqual(a, "Push"))
   	if(GetActionTarget()->~IsWeapon())
   		return(true);

   // Nur beim Laufen, Schwimmen oder Springen
   if(Or(Or(!!SEqual(a, "WalkArmed")
   , !!SEqual(a, "SwimArmed"))
   , !!SEqual(a, "JumpArmed")))
		return(true);

   // wenn Waffe die nur im Zielen abgefeuert werden kann, ist die Aktion ohne "armed" und daher bisher unberücksichtigt
   if(Contents()->~IsWeapon())
     if((Contents()->GetFMData(FM_Aim) ?? 0)>0)
       if(Or(!!SEqual(a, "Walk"), !!SEqual(a, "None")))    // nicht wenn waffe zum zielen aber schwimmend oder springend
       return(true);
   
   // JetpackFlight fehlt noch, sowie Aim*... das steht in den Funktionalitäten
   return(_inherited());
}

private func CheckArmed() {     // Bewaffnung prÃ¼fen

  if (!!SEqual(GetAction(), "Walk")) if (IsArmed2()) return(SetAction("WalkArmed"));
  if (!!SEqual(GetAction(), "Jump")) if (IsArmed2()) return(SetAction("JumpArmed"));
  if (!!SEqual(GetAction(), "Swim")) if (IsArmed2()) return(SetAction("SwimArmed"));
  if (!!SEqual(GetAction(), "WalkArmed")) if (!IsArmed2()) return(SetAction("Walk"));
  if (!!SEqual(GetAction(), "JumpArmed")) if (!IsArmed2()) return(SetAction("Jump"));
  if (!!SEqual(GetAction(), "SwimArmed")) if (!IsArmed2()) return(SetAction("Swim"));
}

// Der Clonk ist bewaffnet (auch mit einer Waffe die nicht angezeigt wird)
// also Waffen mit denen man Zielen muss
public func IsArmed() {
  if(!Contents()) return;
  if(Contents()->~IsWeapon())
    return(true);
  return(nil);
}
// Der Clonk hat einen Gegenstand in der Hand, der gezeichnet wird
public func IsArmed2() {
  if(!Contents()) return;
  if(Or(Contents()->~IsDrawable(),
    (And(IsArmed(), ((Contents()->~GetFMData(FM_Aim) ?? 0) <= 0)))))
    return(true);
  return(nil);

}

public func JumpStart(bool bBackflip)
{
  // bewaffnet?
  bBackflip ??= false;
  CheckArmed();

  _inherited(bBackflip);
}

protected func ComNone()
  {
  SetComDir(COMD_None);
  }

private func IsJumping() {
  if(!!SEqual(GetAction(), "Dive"))
    return(true);
  return(WildcardMatch(GetAction(),"Jump*"));
}

public func StartSquatAiming() {     // Anfangen in der Hocke zu zielen
  // Abbrechen
  if(Contained()) return(1);
  if(And(!SEqual(GetAction(), "WalkArmed"), !SEqual(GetAction(), "Walk")))
      return(1);

  SetXDir(nil);
  if(Or(Equal(Contents(nil)->~GetFMData(FM_Aim), 2), Equal(Contents(nil)->~GetFMData(FM_Aim), -1)))
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(nil)) Contents(nil)->~AimStart();
}

public func IsSquatAiming () { return(WildcardMatch(GetAction(), "AimSquat*")); }

public func AimOverride () {
	if(Contained()) return(Contained()->~AimOverride());
	
	return(_inherited());
}

public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
  // Parameter zurechtstutzen
  iAngle ??= 0;
  iMaxAngle ??= 0;
  iRange ??= 0;
  while((iAngle ?? 0)<0) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>360) S42MigrationAssign(iAngle, "-=", 360);
  if(!iMaxAngle) iMaxAngle = nil;
  if(!iRange) iRange = 350;

  // Landschaft durchsuchen
  // es wird nur auf Lebewesen gezielt
  var targets = FindTargets(this, iRange, iMaxAngle, iAngle, nil);
  var target;
  
  // nächstes Ziel finden...
	for(var t in targets) {
		if(!target) {
			target = t;
			continue;
		}
		if((ObjectDistance(t) ?? 0) < (ObjectDistance(target) ?? 0)) {
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
  if(!Equal(GetCursor(GetOwner()), this())) return;

  if((GetOwner() ?? 0) < -1) return;

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
  if(!!SEqual(GetAction(), "Push"))
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
    DoEnergy(-(damage ?? 0), obj, true);
    AddEffect("Sparkleschadenz", obj, 1, 1,nil,GetID());
    if (And(!Random(20), obj->~IsClonk())) Sound("Hurt*");
  }
}


private func FxSparklezTimer(pTarget, iEffectNumber, iTime)
{

  pTarget->CastParticles("MaterialSpark", 1, 50, nil, nil, 10, 30, RGB(200, 200, 200), RGB(100, 255, 100));
}


private func FxSparkleschadenzTimer(pTarget, iEffectNumber, iTime)
{
 // if (iTime > 1 * 1) return -1;
  pTarget->CastParticles("PxSpark", 1, 50, nil, nil, 10, 30, RGB(nil, nil, 100), RGB(100, 100, 255));
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
  SetComDir(COMD_Stop);
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

  if(Contained()) return(nil);

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=nil; y=nil; r=nil;

  // hält Waffe in der Hand
  if(!!SEqual(a, "WalkArmed")) {
    x=nil;
    y=1000;
    return(1);
  }
  
  else if(!!SEqual(a, "JumpArmed")) {
    x=500-125*(p ?? 0);
    y=1000;
    return(1);
  }
  else if(!!SEqual(a, "SwimArmed")) {
    x=4000;
    return(1);
  }
  //Jetpack
  else if(!!SEqual(a, "JetpackFlight")) {
  
    x=4000;
    r=35;
    return(1);
  }


  // zielt mit der Waffe
  else if(IsAiming()) {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = ((Abs(crosshair->GetAngle()) ?? 0)-90);
    x=-(Sin(65+(p ?? 0)*20,4000) ?? 0)-1500;
    y=-(Sin(55+(p ?? 0)*10,5000) ?? 0);
    if(!!SEqual(a, "AimLow"))
      S42MigrationAssign(y, "+=", 4000);
    if(!!SEqual(a, "AimSquat"))
    {
      S42MigrationAssign(x, "+=", 5000);
      S42MigrationAssign(y, "+=", 4500);
    }
    if(!!SEqual(a, "AimSquatLow"))
    {
      S42MigrationAssign(x, "+=", 5000);
      S42MigrationAssign(y, "+=", 6000);
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
    else */return(nil);

/*    return(1);
  }*/
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  x = (EffectVar(2, this(), number) ?? 0)/1000;
  y = (EffectVar(3, this(), number) ?? 0)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  x = (EffectVar(4, this(), number) ?? 0)/1000;
  y = (EffectVar(5, this(), number) ?? 0)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  return(EffectVar(1, this(), number));
}

/* Munitionsaufsammelanzeige */

func FxAmmoCollectionStart(object pTarget, int iEffectNumber, int iTemp, id idAmmo, int iAmmoCount) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iAmmoCount ??= 0;
  if(iTemp)
    return(1);
  EffectVar(nil,pTarget,iEffectNumber) = 100;
  EffectVar(1,pTarget,iEffectNumber) = idAmmo;
  EffectVar(2,pTarget,iEffectNumber) = iAmmoCount;
}

func FxAmmoCollectionEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, id idAmmo, int iAmmoCount) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  iAmmoCount ??= 0;
  if(!!SEqual(szNewEffectName, "AmmoCollection"))
    //return(-2);
    // wird nich addiert: alten löschen
    RemoveEffect(nil,pTarget,iEffectNumber);
  return(nil);
}

func FxAmmoCollectionAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id idAmmo, int iAmmoCount) {
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  iAmmoCount ??= 0;
  var x=1;
  while(EffectVar(x,pTarget,iEffectNumber)) {
    if(Equal(EffectVar(x,pTarget,iEffectNumber), idAmmo))
      break;
    else
      S42MigrationStep(x, 1, true);
  }
  EffectVar(S42MigrationStep(x, 1, true),pTarget,iEffectNumber) = idAmmo;
  S42MigrationAssign(EffectVar(x,pTarget,iEffectNumber), "+=", (iAmmoCount ?? 0));

  S42MigrationAssign(EffectVar(nil,pTarget,iEffectNumber), "+=", 100);
  return(1);
}

func FxAmmoCollectionTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var x=1, string = " ";
  while(EffectVar(x,pTarget,iEffectNumber))
    string = Format("%s|{{%i}} %4d",string,EffectVar(S42MigrationStep(x, 1, true),pTarget,iEffectNumber),EffectVar(S42MigrationStep(x, 1, true),pTarget,iEffectNumber));
  
  HelpMessage(GetOwner(pTarget),string,pTarget);
  if((iEffectTime ?? 0) >= (EffectVar(nil,pTarget,iEffectNumber) ?? 0))
    return(-1);
}

public func Incineration() {
  // Hazardclonks brennen ncht wirklich ;)
  Extinguish();
if(!brennt){ // damit bei Brandbomben nicht sofort tot
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);brennt=1;ScheduleCall(nil,"Feueristaus",20,nil,nil);}
}

func Feueristaus()
{brennt=nil;}


public func OnHit(int iDmg, int iType, object pFrom)
{
  iDmg ??= 0;
  iType ??= 0;
  _inherited(iDmg, iType, pFrom);
  
  if(!GetAlive()) return;
  
  // Besondere Effekte
  if(Equal(iType, DMG_Bio))
    AddFireEffect(this(),10,RGB(20,255,30));
	
  // hurt
  if((iDmg ?? 0) > 12)
	  Hurt();
	
  // Aggro-Check
  if(pFrom)
    if(LocalN("shooter", pFrom))
      if((GetAggroLevel() ?? 0) > (Aggro_Nothing ?? 0))
        if(!GetAggroTarget())
          if(CheckTarget(LocalN("shooter", pFrom)))
            SetAggroTarget(LocalN("shooter", pFrom));
}

public func LastDamageType(int type) {
  type ??= 0;
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
  iX ??= 0;
  iY ??= 0;
  iDelay ??= 0;
  iAggro ??= 0;
  if(!szCommand) return;
  if(And(And(And(!SEqual(szCommand, "MoveTo"), !SEqual(szCommand, "Follow")), !SEqual(szCommand, "Wait")), !SEqual(szCommand, "None")))
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return;
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(!!SEqual(szCommand, "MoveTo"))
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(!!SEqual(szCommand, "Follow"))
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return;
    }
  // Aktuelle Beschäftigung beenden
  var currCom = GetMacroCommand(nil,nil);
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
  var iEffect = AddEffect("MacroCommand", this(), 300, nil, this());
  aMacroCommandList[0] = iEffect;
  EffectVar(nil, this(), iEffect) = szCommand;
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
  if(!Equal(szCommand, "None"))
    ScheduleCall(this(), Format("MacroCom%s", szCommand), 1);
  else
  {
    ClearMacroCommands();
    if(!Equal(iAggro, -1)) SetAggroLevel(iAggro,nil,nil,nil,"SetMacroCommand");
  }
  return(1);
}

// Befehl an den Anfang setzen
public func AddMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{//Log("%s #%d: AddMacroCom", GetName(), ObjectNumber());
  // Befehlsliste zwischenspeichern
  iX ??= 0;
  iY ??= 0;
  iDelay ??= 0;
  iAggro ??= 0;
  var aComs = aMacroCommandList;
  // Und leeren
  var currCom = GetMacroCommand(nil,nil);
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
        aMacroCommandList[(GetLength(aMacroCommandList) ?? 0)] = iCom;
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
  iX ??= 0;
  iY ??= 0;
  iDelay ??= 0;
  iAggro ??= 0;
  if(!szCommand) return;
  if(And(And(And(!SEqual(szCommand, "MoveTo"), !SEqual(szCommand, "Follow")), !SEqual(szCommand, "Wait")), !SEqual(szCommand, "None")))
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return;
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(!!SEqual(szCommand, "MoveTo"))
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(!!SEqual(szCommand, "Follow"))
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return;
    }
  // Befehl eintragen
  var iEffect = AddEffect("MacroCommand", this(), 300, nil, this());
  aMacroCommandList[(GetLength(aMacroCommandList) ?? 0)] = iEffect;
  EffectVar(nil, this(), iEffect) = szCommand;
  EffectVar(1, this(), iEffect) = pTarget;
  EffectVar(2, this(), iEffect) = iX;
  EffectVar(3, this(), iEffect) = iY;
  EffectVar(4, this(), iEffect) = iDelay;
  EffectVar(5, this(), iEffect) = pCallback;
  EffectVar(6, this(), iEffect) = iAggro;
  // Starten?
  if(Equal(GetLength(aMacroCommandList), 1))
  {
    if(!Equal(szCommand, "None"))
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
  iCommandNum ??= 0;
  iElement ??= 0;
  if(Or((iCommandNum ?? 0) < 0, (GetLength(aMacroCommandList) ?? 0) <= (iCommandNum ?? 0))) return;
  // Element zurückgeben
  return(EffectVar(iElement, this(), aMacroCommandList[(iCommandNum ?? 0)]));
}

// Beenden (wenn, dann nur extern gebraucht)
public func FinishMacroCommand(bool fNoCallback, bool fNoSuccess, int iCommandNum)
{
  // An der Stelle gibt es nichts?
  fNoCallback ??= false;
  fNoSuccess ??= false;
  iCommandNum ??= 0;
  if(Or((iCommandNum ?? 0) < 0, (GetLength(aMacroCommandList) ?? 0) <= (iCommandNum ?? 0))) return;
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
  if(And(GetMacroCommand(), !iCommandNum))
    if(!Equal(GetMacroCommand(), "None"))
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
  if(!SEqual(GetMacroCommand(nil,nil), "MoveTo")) return;
  // Kein Ziel? (bewirkt, dass der Clonk nicht nach 0,0 laufen kann!)
  if(And(!GetMacroCommand(nil,2), !GetMacroCommand(nil,3)))
    if(!GetMacroCommand(nil,4))
      return(MacroComSuccessFailed(nil, Macro_MoveToNoTarget));
  // Pfad suchen
  aPath = CreateArray();
  var start, end;
  start = FindWaypoint(GetX(), GetY());
  end = FindWaypoint(GetMacroCommand(nil,2), GetMacroCommand(nil,3));
  if(Or(!start, !end)) return(MacroComSuccessFailed(nil, Macro_NoPath));
  aPath = FindPath(start, end, true);
  // Kein Pfad konnte gefunden werden?
  if(!aPath) return(MacroComSuccessFailed(nil, Macro_NoPath));

  // ist der Endpunkt ein Transportpunkt? (ein Wegpunkt mit nur einem Ausgang
  // und dieser Ausgang zeigt nicht zum Eingang)
  if(Equal(end->GetPathCount(), 1)) {
    var before = nil;
    var leng = GetLength(aPath);
    if(!Equal(start, end))
      before = aPath[(leng ?? 0)-2];
    if(!Equal(end->GetPathTarget(nil), before)) {
      // Ziel ändern (Regel: Pfad darf nicht auf Transportpunkt enden)
      aPath[(leng ?? 0)] = end->GetPathTarget(nil);
      // Ziel ändern auf folgenden Wegpunkt, Position entsprechend.
      EffectVar(1, this(), aMacroCommandList[0]) = end->GetPathTarget(nil);
      EffectVar(2, this(), aMacroCommandList[0]) = GetX(end->GetPathTarget(nil));
      EffectVar(3, this(), aMacroCommandList[0]) = GetY(end->GetPathTarget(nil));
    }
  }
  // In Bewegung setzen (der erste Wegpunkt sollte durch MoveTo erreichbar sein, wenn nicht -> doof
  // Spezialhack: reitet!
  if(IsRiding()) SetAction("Walk");
  SetCommand(this(), "MoveTo", aPath[0]);
  AppendCommand(this(), "Call", this(), 1,nil,nil,nil, "MacroComMoveToStep");
  // Automatischer Abbruch
  ScheduleCall(this(), "MacroComSuccessFailed", 500, nil, nil, Macro_PathImpossible);
  // Falls Startwegpunkt = Jumppadpunkt
  if(And((GetLength(aPath) ?? 0) > 1, Equal(aPath[0]->GetPathCount(), 1)))
    ScheduleCall(this(), "JumppadCheck", 5, nil, aPath[1], 2);
  // Aggrolevel
  if(!Equal(GetMacroCommand(nil, 6), -1))
    SetAggroLevel(GetMacroCommand(nil, 6),nil,nil,nil,"MacroComMoveTo");
  //Log("%s #%d: MacroComMoveTo", GetName(), ObjectNumber());
}

protected func MacroComMoveToStep(object dummy, int iStep) // MoveTo-Schritt
{
  // Erstes Kommando ist gar nicht MoveTo?
  iStep ??= 0;
  if(!SEqual(GetMacroCommand(nil,nil), "MoveTo")) return;
  // Wird haben das Ende des Pfades erreicht?
  if(Equal(GetLength(aPath), iStep))
  {//Log("%s #%d: MacroComMoveTo finished", GetName(), ObjectNumber());
    // Zum Ziel laufen und beenden
    var x = GetMacroCommand(nil,2), y = GetMacroCommand(nil,3);
    SetCommand(this(), "MoveTo", nil, x,y);
    AppendCommand(this(), "Call", this(), nil,nil,nil,nil, "MacroComSuccess");
    return(1);
  }
  // Aus irgendeinem Grund ist kein Wegpunkt da?
  if(!aPath[(iStep ?? 0)]) return(MacroComSuccessFailed(nil, Macro_PathBroken));
  if(!aPath[(iStep ?? 0)-1]) return(MacroComSuccessFailed(nil, Macro_PathBroken));
  // Nächsten Wegpunkt begehen
  MoveAlongPath(aPath[(iStep ?? 0)-1], aPath[(iStep ?? 0)], (iStep ?? 0)+1);
  AppendCommand(this(), "Call", this(), (iStep ?? 0)+1,nil,nil,nil, "MacroComMoveToStep");
  // Automatischer Abbruch
  ClearScheduleCall(this(), "MacroComSuccessFailed");
  // 200 Frames pro 100 Pixel Abstand sind angesetzt
  var breaktime = Max((ObjectDistance(aPath[(iStep ?? 0)-1], aPath[(iStep ?? 0)]) ?? 0) / 100 * 200, 100);
  ScheduleCall(this(), "MacroComSuccessFailed", breaktime, nil, nil, Macro_PathImpossible);
  //Log("%s #%d: MacroComMoveToStep", GetName(), ObjectNumber());
  return(1);
}

protected func MacroComMoveToStepFailed() // MoveTo-Schritt fehlgeschlagen
{//Log("%s #%d: MacroComMoveToStepFailed", GetName(), ObjectNumber());
  // Erstes Kommando ist gar nicht MoveTo?
  if(!SEqual(GetMacroCommand(nil,nil), "MoveTo")) return;
  // Fehlschlag auf ganzer Linie
  MacroComSuccessFailed(nil, Macro_PathImpossible);
  return(1);
}

// Follow
protected func MacroComFollow(bool fStarted)
{
  // Sonderbehandlung: wurde schon gestartet
  fStarted ??= false;
  if(fStarted)
  {
    DebugLog(Format("%s #%d: MacroComFollow timer", GetName(), ObjectNumber()), "MacroComFollow");
    // Prüfen, ob an der Stelle iPos in der Kette "Follow" steht
    if(And(!SEqual(GetMacroCommand(1), "Follow"), !SEqual(GetMacroCommand(nil), "Follow")))
      // Steht nicht da, ergo muss sich ein anderes Kommando reingeschaltet haben -> nichts tun
      return;
    // Wir haben kein MoveTo mehr? Dann sind wir angekommen (wir starten einfach neu)
    if(!SEqual(GetMacroCommand(), "MoveTo")) return(MacroComFollow());
    // Unser Ziel ist irgendwie weg?
    if(!GetMacroCommand(1,1))
    {
      DebugLog(Format("%s #%d: MacroComFollow target lost", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      RemoveMacroCommand();
      // Fehlschlag ausgeben
      return(MacroComSuccessFailed(nil, Macro_FollowTargetLost));
    }
    // Schauen, ob unser Bewegungsziel noch ok ist
    var x,y,target;
    x = GetMacroCommand(nil,2);
    y = GetMacroCommand(nil,3);
    target = GetMacroCommand(nil,1);
    // Distanz > 30 Pixel?
    if((Distance(AbsX(x), AbsY(y), AbsX(target->GetX()), AbsY(target->GetY())) ?? 0) > 30)
    {
      DebugLog(Format("%s #%d: MacroComFollow new MoveTo", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      if(!!SEqual(GetMacroCommand(), "MoveTo")) RemoveMacroCommand();
      // Neues Kommando setzen
      AddMacroCommand(nil, "MoveTo", GetMacroCommand(nil,1), nil, nil, nil, GetMacroCommand(nil,6));
    }
    // Follow Weiterlaufen lassen
    return(ScheduleCall(this(), "MacroComFollow", 30, nil, true));
  }
  DebugLog(Format("%s #%d: MacroComFollow started", GetName(), ObjectNumber()), "MacroComFollow");
  // Erstes Kommando ist gar nicht Follow?
  if(!SEqual(GetMacroCommand(nil,nil), "Follow")) return;
  // Kein Ziel? -> Fehlschlag
  if(!GetMacroCommand(nil,1))
    return(MacroComSuccessFailed());
  // Wir sind dem Ziel noch nicht nahe genug?
  if((ObjectDistance(GetMacroCommand(nil,1)) ?? 0) > 30)
    // Follow bedeutet viele MoveTos -> loslaufen bitte
    AddMacroCommand(nil, "MoveTo", GetMacroCommand(nil,1), nil, nil, nil, GetMacroCommand(nil, 6));
  // Aggrolevel
  //if(GetMacroCommand(0, 6) != -1)
    //SetAggroLevel(GetMacroCommand(0, 6));
  // Alle 30 Frames aktualisieren wir
  return(ScheduleCall(this(), "MacroComFollow", 30, nil, true));
}

// Wait
protected func MacroComWait(bool fEnd)
{
  // Erstes Kommando ist gar nicht Wait?
  fEnd ??= false;
  if(!SEqual(GetMacroCommand(nil,nil), "Wait")) return;
  // Beenden?
  if(fEnd)
    return(MacroComSuccess(nil));
  // Aggrolevel
  if(!Equal(GetMacroCommand(nil, 6), -1))
    SetAggroLevel(GetMacroCommand(nil, 6),nil,nil,nil,"MacroComWait");
  // Dann warten wir jetzt eine Runde
  return(ScheduleCall(this(), "MacroComWait", GetMacroCommand(nil,4), nil, true));
}

/* Makro-Funktionen */

// Makrobefehl an der Stelle iCommand ist erfolgreich
protected func MacroComSuccess(iCommand, int iCmd2)
{//Log("%s #%d: MacroComSuccess", GetName(), ObjectNumber());
  // Für Aufrufe aus SetCommand(... "Call");
  iCmd2 ??= 0;
  if(Equal(GetType(iCommand), C4V_C4Object)) iCommand = iCmd2;
  // An der Stelle gibt es nichts?
  if(Or((iCommand ?? 0) < 0, (GetLength(aMacroCommandList) ?? 0) <= (iCommand ?? 0))) return;
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
  MacroComSuccessFailed(nil, Macro_PathImpossible);
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
  iCommand ??= 0;
  iReason ??= 0;
  if(Or((iCommand ?? 0) < 0, (GetLength(aMacroCommandList) ?? 0) <= (iCommand ?? 0))) return;
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
  iCommand ??= 0;
  if(Or((iCommand ?? 0) < 0, (GetLength(aMacroCommandList) ?? 0) <= (iCommand ?? 0))) return;
  // Entfernen
  for(var i = nil, iEff, checked ; (i ?? 0) < (GetEffectCount("MacroCommand", this()) ?? 0) ; S42MigrationStep(i, 1, true))
  {
    iEff = GetEffect("MacroCommand", this(), i);
    if(Equal(iEff, aMacroCommandList[(iCommand ?? 0)]))
    {
      RemoveEffect("MacroCommand", this(), i);
      continue;
    }
    if(GetEffect("MacroCommandLocked", this())) continue;
    for(var mac in aMacroCommandList)
      if(Equal(mac, iEff))
        checked = true;
    if(!checked) RemoveEffect("MacroCommand", this(), i, true);
    checked = nil;
  }
/*  if(RemoveEffect(0, this(), aMacroCommandList[iCommand]))
    Log("Effect removed");*/
  var aNewMacroComList = CreateArray();
  i = nil;
  for(var iCom in aMacroCommandList)
  {
    if(Equal(i, iCommand)) continue;
    aNewMacroComList[(GetLength(aNewMacroComList) ?? 0)] = iCom;
    S42MigrationStep(i, 1, true);
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
  fJetpack ??= false;
  var iterationLimit = ObjectCount(WAYP);
  
  var ex = GetX(pEnd);
  var ey = GetY(pEnd);

  var pCurrent = pStart;

  var aWaypoints = CreateArray();
  
  var jetp = nil;
  var ammoload = DefinitionCall(JTPK,"GetFMData",FM_AmmoLoad);

  for(var j=nil; (j ?? 0)< (iterationLimit ?? 0); S42MigrationStep(j, 1)) {

    aWaypoints[(j ?? 0)] = pCurrent;

    // fertig!
    if(Equal(pCurrent, pEnd))
      return(aWaypoints);

    // ansonsten...
    var cx = GetX(pCurrent);
    var cy = GetY(pCurrent);
    var cangle = Angle(cx,cy,ex,ey);

    var pNext = nil;
    var pathcount = (pCurrent->WAYP::GetPathCount());
    var aBest = CreateArray();
    var good = nil;
    var neutral = nil;
    var bad = nil;

    // alle Nachbarknoten...
    for(var i=nil; (i ?? 0)<(pathcount ?? 0); S42MigrationStep(i, 1)) {
      pNext = (pCurrent->WAYP::GetPathTarget(i));

      if(!Check4Jetpack(pCurrent,i,fJetpack,jetp,ammoload))
        continue;

      // Fuer die richtige Richtung zaehlt nicht der Winkel des naechsten Knotens, sondern
      // wenn pNext nur einen Nachfolger hat, zaehlt die Richtung in die der Nachfolger geht
      // (was rekursiv fortsetzbar ist)
      var jetpgedacht = nil;
      while(Equal((pNext->WAYP::GetPathCount()), 1)) {
        // Endknoten? Wir brauchen nicht weiteriterieren
        if(Equal(pNext, pEnd)) break;
        // für den gedachten Pfad muss noch gecheckt werden, ob da nicht Jetpack benötigt wird
        var check = Check4Jetpack(pNext,nil,fJetpack,(jetp ?? 0)+(jetpgedacht ?? 0),ammoload);
        if(check)
          pNext = (pNext->WAYP::GetPathTarget(nil));
        if(Equal(check, 2))
          S42MigrationAssign(jetpgedacht, "+=", (ammoload ?? 0));
      }

      // Endknoten? Super, fertig!
      if(Equal(pNext, pEnd)) {
        aBest[0] = i;
        good = 1;
        break;
      }

      // ansonsten
      var nx = GetX(pNext);
      var ny = GetY(pNext);
      var nangle = Angle(cx,cy,nx,ny);
 
      // diffangle: Nimmt Werte von 0 (genau richtig) bis 180 (genau falsch) an
      var diffangle = Abs(Normalize((nangle ?? 0)-(cangle ?? 0),-180));
      // Schummeln: der WP von dem man grad gekommen ist, wird als "bad" eingestuft
      if((j ?? 0)>0)
        if(Equal(pNext, aWaypoints[(j ?? 0)-1]))
          diffangle = 180;
      // möglichst ein Array aus Pfaden zusammenstellen, die etwa in die gewünschte Richtung
      // führen... (Differenz <= 55°)
      if((diffangle ?? 0) <= 55) {
        aBest[(good ?? 0)] = i;
        S42MigrationStep(good, 1);
      }
      // dann vielleicht nicht ganz so zielführende...? (nur wenn kein Pfad in richtige Richtung führt)
      if(And(And((diffangle ?? 0) <= 110, (diffangle ?? 0) > 55), !good)) {
        aBest[(neutral ?? 0)] = i;
        S42MigrationStep(neutral, 1);
      }
      // ansonsten dann ein Array aus nicht-zielführenden...
      if(And(And((diffangle ?? 0) > 110, !neutral), !good)) {
        aBest[(bad ?? 0)] = i;
        S42MigrationStep(bad, 1);
      }
    }
    // Debug
    if(Debug("waypoints")) {
      var dbg = nil;
      var str = " ";
      var wayp = nil;
      if(good) { str = "good"; dbg = good; }
      else if(neutral) { str = "neutral"; dbg = neutral; }
      else { str = "bad"; dbg = bad; }
      if((dbg ?? 0)>1) {
        DebugLog("  Choosing path from %s...","waypoints",str);
        for(var d = nil; (d ?? 0)<(dbg ?? 0); S42MigrationStep(d, 1)) {
          wayp = (pCurrent->WAYP::GetPathTarget(aBest[(d ?? 0)]));
          DebugLog("  waypoint (%d), angle %d","waypoints",ObjectNumber(wayp),Abs(Normalize((Angle(GetX(pCurrent),GetY(pCurrent),GetX(wayp),GetY(wayp)) ?? 0)-(cangle ?? 0),-180)));
        }
      }
    }
    
    // zufällig aus möglichen Pfaden auswählen
    var chosenone = -1;
    if(good)			{ chosenone = aBest[(Random(good) ?? 0)]; }
    else if(neutral)	{ chosenone = aBest[(Random(neutral) ?? 0)]; }
    else if(bad)		{ chosenone = aBest[(Random(bad) ?? 0)]; }
    else {
      DebugLog("Failure: Selected waypoint leads into a dead end.","waypoints");
      return(nil);
    }
    // ansonsten ok
    pNext = (pCurrent->WAYP::GetPathTarget(chosenone));

    // geschätzter Benzinverbrauch bei Jetpack hochzählen
    if(pCurrent->WAYP::GetPathJetpack(chosenone))
      S42MigrationAssign(jetp, "+=", (ammoload ?? 0));

    DebugLog("Next %s waypoint (%d), angle %d","waypoints",str,ObjectNumber(pNext),Abs(Normalize((Angle(GetX(pCurrent),GetY(pCurrent),GetX(pNext),GetY(pNext)) ?? 0)-(cangle ?? 0),-180)));

    // Fertig
    if(Equal(pNext, pEnd)) {
      DebugLog("Success: Complete path consists of %d waypoints.","waypoints",GetLength(aWaypoints));
    }    

    // kommt bei naechster Iteration ins Array
    pCurrent = pNext;
  }
  DebugLog("Failure: No path found after %d iterations.","waypoints",iterationLimit);
  return(nil);
}

// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
private func FindWaypoint(int iX, int iY)
{
  iX ??= 0;
  iY ??= 0;
  var wp;
  while(wp = FindObject(WAYP, AbsX(iX), AbsY(iY), -1, -1, nil,nil,nil,nil, wp)) {
    if(Or(PathFree(iX, iY, wp->GetX(), wp->GetY()), GBackSolid(wp->GetX(), wp->GetY())))
      return(wp);
  }
  // Nix gefunden
  return;
}

// Vorbedingung für Jetpack prüfen
private func Check4Jetpack(object pCurrent, int path, bool fJetpack, int jetp, int ammoload)
{
  // kein Jetpackpath... dann ist ja gut
  path ??= 0;
  fJetpack ??= false;
  jetp ??= 0;
  ammoload ??= 0;
  if(!(pCurrent->WAYP::GetPathJetpack(path)))
    return(1);
  // ansonsten Vorbedingungen checken
  if(fJetpack)
    if(HasJetpack())
      //if(ammoload <= GetAmmo(GSAM)-jetp)
        return(2);
  return(nil);
}

// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
  // Rausfinden, welchen Weg wir gehen müssen
  iNextStep ??= 0;
  for(var i = nil, path = -1 ; (i ?? 0) < (pCurrentWp->GetPathCount() ?? 0) ; S42MigrationStep(i, 1, true))
    if(Equal(pCurrentWp->GetPathTarget(i), pNextWp))
    {
      path = i;
      break;
    }
  // Kein Weg? -> Fehler
  if((path ?? 0) < 0) return(MacroComSuccessFailed(nil, Macro_PathBroken));
  // Sonderbehandlung für Zielwegpunkte, die nur einen Pfad von sich wegführen haben (da könnten Jumppads o.ä. sein)
  ClearScheduleCall(this(), "JumppadCheck"); // Zur Sicherheit
  if(And((GetLength(aPath) ?? 0) > (iNextStep ?? 0), Equal(pNextWp->GetPathCount(), 1)))
    ScheduleCall(this(), "JumppadCheck", 5, nil, pNextWp, iNextStep);
  // Jetpackfliegen
  if(pCurrentWp->GetPathJetpack(path))
    ScheduleCall(this(), "StartJetpack", pCurrentWp->GetPathJetpack(path), nil, pCurrentWp->GetPathJetpackFlag(path));
  // Flag rausfinden und entsprechend agieren
  var flag = pCurrentWp->GetPathFlag(path);
  if(Equal(flag, Path_MoveTo))
  {
    // Hinlaufen
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(Equal(flag, Path_Jump))
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if((dir ?? 0) < 0) dir = nil;
    SetDir(dir);
    Jump();
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(Equal(flag, Path_Backflip))
  {
    // Backflippen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if((dir ?? 0) < 0) dir = nil;
    SetDir(!dir);
    var iEff = AddEffect("ControlStack", this, 110, 5, this);
		if(Equal(GetDir(), DIR_Left))
		  EffectVar(nil, this, iEff) = COMD_Right;
		else
		  EffectVar(nil, this, iEff) = COMD_Left;
    Jump();
    ScheduleCall(nil, "JumpStart", 1, 1, true);

    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(Equal(flag, Path_Lift))
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(nil, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
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
  iNextStep ??= 0;
  if((GetLength(aPath) ?? 0) <= (iNextStep ?? 0)) return;
  var pNextWp = aPath[(iNextStep ?? 0)];
  // Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
  if(!Equal(aPath[(iNextStep ?? 0)-1], pTargetWp))
    return;
  // Schauen, ob wir näher an pNextWp dran sind
  if((ObjectDistance(pTargetWp) ?? 0) > (ObjectDistance(pNextWp) ?? 0))
    // Wegpunkt wird übersprungen
    return(MacroComMoveToStep(nil, iNextStep));
  // Weiterlaufen
  ScheduleCall(this(), "JumppadCheck", 5, nil, pTargetWp, iNextStep);
}

protected func ClimbLadder()
{
  if(And(!GetMacroCommand(), !GetCommand())) return;
  // Testen, ob wir noch klettern
  if(!SEqual(GetAction(), "ScaleLadder")) return;
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
  
  if((targety ?? 0) < (GetY() ?? 0)) SetComDir(COMD_Up);
  if((targety ?? 0) > (GetY() ?? 0)) SetComDir(COMD_Down);
  
  // 2do: Wir müssen irgendwie feststellen, ob die Leiter aus ist. :(
  
  
  // Wegpunkt oben? früher abspringen.
  if(And(Inside((GetY() ?? 0)-(targety ?? 0),5,15), (Abs((GetX() ?? 0)-(targetx ?? 0)) ?? 0) < 30))
  {
    var comd = GetComDir();
    if((targetx ?? 0) < (GetX() ?? 0))
    {
    	ControlLadder("ControlLeft");
        if(Equal(GetDir(), DIR_Left))
    	  SetComDir(comd);
   	}
    if((targetx ?? 0) > (GetX() ?? 0))
    {
    	ControlLadder("ControlRight");
        if(Equal(GetDir(), DIR_Right))
    	  SetComDir(comd);
   	}
  }
    
  // Sind wir ungefähr da?
  if(And(Inside(GetY(), (targety ?? 0)-5, (targety ?? 0)+5), PathFree(GetX(), GetY(), targetx, targety)))
  {
    // Wir versuchen mal, abzuspringen (das klappt spätestens beim 2. Mal(theoretisch))
    
    
    //wenn Ziel weiter oben: immer nach oben abspringen
    if((GetY() ?? 0) > (targety ?? 0))
      SetComDir(COMD_Up);
    //Ziel weiter unten? Nach unten abspringen.
    else if((Abs((GetY() ?? 0)-(targety ?? 0)) ?? 0) > 50)
      SetComDir(COMD_Down);
    //Ziel weiter weg? Backflip! :D
    else if((Abs((GetX() ?? 0)-(targetx ?? 0)) ?? 0) > 80)
      SetComDir(COMD_Stop);
    
    var comd = GetComDir();
    if((targetx ?? 0) < (GetX() ?? 0))
    {
    	ControlLadder("ControlLeft");
        if(Equal(GetDir(), DIR_Left))
    	  SetComDir(comd);
   	}
    if((targetx ?? 0) > (GetX() ?? 0))
    {
    	ControlLadder("ControlRight");
        if(Equal(GetDir(), DIR_Right))
    	  SetComDir(comd);
   	}
  }
  // Weiter checken
  ScheduleCall(this(), "ClimbLadder", 10);
}

protected func StartJetpack(int iDir)
{
  // Kein Jetpack mehr? oO
  iDir ??= 0;
  var jetpack = HasJetpack();
  if(!jetpack) return;
  // Richtung zünden
  if(Equal(iDir, Jetpack_Left)) // Links
  {
    SetDir(DIR_Left);
    jetpack->ControlLeftDouble(this());
  }
  if(Equal(iDir, Jetpack_UpLeft)) // Oben links
  {
    SetDir(DIR_Left);
    jetpack->ControlUpDouble(this());
  }
  if(Equal(iDir, Jetpack_UpRight)) // Oben rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlUpDouble(this());
  }
  if(Equal(iDir, Jetpack_Right)) // Rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlRightDouble(this());
  }
}

protected func LiftControl(object dummy, int pCurrentWp, int pNextWp)
{
  // Fässt den Lift schon an?
  pCurrentWp ??= 0;
  pNextWp ??= 0;
  if(!!SEqual(GetAction(), "Push"))
  {
    var lift = GetActionTarget();
    if(!lift) return;
    // Nah genug am Ziel? Absteigen
    if((ObjectDistance(Object(pNextWp)) ?? 0) <= 50)
      return(AddCommand(this(), "MoveTo", Object(pNextWp)));
    // Liftplatten befehligen
    lift->~ControlCommand("MoveTo",nil, Object(pNextWp)->GetX());
    // Warten
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, nil,nil, "LiftControl");
    AddCommand(this(), "Wait", nil,nil,nil,nil,nil, 15);
    return(1);
  }
  // Liftplatte suchen
  var x1 = Object(pCurrentWp)->GetX();
  var x2 = Object(pNextWp)->GetX();
  if((x1 ?? 0) > (x2 ?? 0)) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
  var lift = FindObject2(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, (x2 ?? 0)-(x1 ?? 0), 50));
  // Kein Lift? -> Fehlschlag
  if(!lift) return(MacroComSuccessFailed(nil, Macro_PathBroken));
  // Lift nah genug? -> Einsteigen
  if((ObjectDistance(lift) ?? 0) <= 50)
  {
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, nil,nil, "LiftControl");
    AddCommand(this(), "Grab", lift);
    return(1);
  }
  // Warten
  AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, nil,nil, "LiftControl");
  
  AddCommand(this(), "Wait", nil,nil,nil,nil,nil, 15);
  return(1);
}

// Alles was der Wegpunkt als ArriveCommand hat
protected func AddSpecialCommands(object pCurrentWp, int path)
{
  path ??= 0;
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= (pCurrentWp->GetArriveCommandCount(path) ?? 0) - 1,command ; (i ?? 0) > -1 ; S42MigrationStep(i, -1, true))
    AddCommand(this(), pCurrentWp->GetArriveCommand(path, i), 
                       pCurrentWp->GetArriveCommand(path, i, 1),
                       pCurrentWp->GetArriveCommand(path, i, 2),
                       pCurrentWp->GetArriveCommand(path, i, 3),
                       pCurrentWp->GetArriveCommand(path, i, 4), nil,
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
  iLevel ??= 0;
  iDist ??= 0;
  iX ??= 0;
  iY ??= 0;
  if((iLevel ?? 0) > 3) return;
  // Wir kommen von > 2
  var target;
  if(And((GetAggroLevel() ?? 0) >= 2, (iLevel ?? 0) < 2))
    if(target = EffectVar(1, this(), GetEffect("Aggro", this())))
      if(Equal(GetMacroCommand(1, 1), target))
      {
        FinishMacroCommand(1,nil,1);
        FinishMacroCommand(1);
      }
  // 0
  if(!iLevel) return(RemoveEffect("Aggro", this()));
  // 1, 2 und 3
  var effect = GetEffect("Aggro", this());
  if(!effect) effect = AddEffect("Aggro", this(), 50, 10, this());
  EffectVar(nil, this(), effect) = iLevel;
  // Parameter
  if(!iDist)
    {iDist = 500;}
  //Log(Format("distanz=%v",iDist));
  EffectVar(2, this(), effect) = iDist;
  // iX und iY, Default = GetX() & GetY()
  if(And(And(!iX, !iY), Equal(iLevel, 3)))
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
  if(!effect) return;
  return(EffectVar(nil, this(), effect));
}

// Setzt sofort das Angriffsziel
public func SetAggroTarget(object pTarget)
{
  if(Equal(GetAggroLevel(), Aggro_Nothing)) return;
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
 no ??= 0;
 if(geblendet) return;
  // keine Waffe geladen? Dann waffen durchchecken  
// Waffenstatus durchgehen, vorher zählen
  var waffenanzahl = CustomContentsCount("IsWeapon");
   var o = nil;
   for(var u = nil; (u ?? 0) < (ContentsCount() ?? 0); S42MigrationStep(u, 1, true))
       if(Contents(u)->~IsWeapon())
  		{
       if(!Contents(u)->~GetCharge()) S42MigrationStep(o, 1);
       if(Contents(u)->~GetCharge()) break;
  		}
  // wenn o nun gleich waffenanzahl, dann heißt das, dass alle Waffen leer sind, dann nachladen.
   if(Equal(o, waffenanzahl))
   return(CheckIdleWeapon());

  // Wir haben ein Ziel?
  if(EffectVar(1, this(), no)) { EffectCall(this(), no, "Fire"); return(1); }
  // Zielen beenden
  if(IsAiming()) StopAiming();
//  Message("@No target", this());
  // Ziel suchen
  var dist = EffectVar(2, this(), no);
  var dir = (GetDir() ?? 0)*2-1;
  // Vorne
  var target = GetTarget(90*(dir ?? 0), 90,dist);
  // Hinten
  if(!target)
    if(Or((And(!GetCommand(), !GetMacroCommand())), !Equal(EffectVar(nil, this(), no), 1)))
      target = GetTarget(-90*(dir ?? 0), 90,dist);
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
    return;
  }
  // Super
  EffectVar(1, this(), no) = target;
  EffectVar(99,this(), no) = true; // wir haben ein Ziel \o/
}

public func FxAggroFire(object pTarget, int no)
{

 //Wenn geblendet, dann nichts machen
 no ??= 0;
 if(geblendet) return;

  // Zusatzhack: BR-Bombe!
  if(Equal(GetID(Contents()), GBRB))
    // Nichts tun :C
    return;
  var y = EffectVar(4, this(), no);
  var x = EffectVar(3, this(), no);
  var dist = EffectVar(2, this(), no);
  var target = EffectVar(1, this(), no);
  var level = EffectVar(nil, this(), no);
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
      return(AddCommand(this(), "Exit", nil,nil,nil,nil,nil,nil,nil, C4CMD_SilentSub));
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
  if(Equal(level, 3)) {
    if((Distance(GetX(), GetY(), x, y) ?? 0) > (dist ?? 0))
    {
      if(Equal(GetMacroCommand(1, 1), target))
      {
        FinishMacroCommand(1,nil,1);
        FinishMacroCommand(1);
      }
       // Log("zu weit von Wachposition entfernt");
      AddMacroCommand(nil, "MoveTo", nil, x,y, nil, level);
      EffectVar(1, this(), no) = nil;
	  //      Message("@Returning to guarded position", this());
      return;
    }
  }
  
  var maxdist = dist;
  if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(Equal(level, 1)) maxdist = nil;
    if((level ?? 0) >= 2) maxdist = (dist ?? 0)/2;
    pathfree = nil;
  }
  //Log("kurz vor ziel weg");
  // Ziel irgendwie weg?
  // (Pathfree wurde schon gecheckt)...  reicht nicht, da zielen und schießen nicht aufhört, wenn was zwischen gegner, aber noch in Radius, und daher wird auch nicht nachgeladen -.-
  if(Or(!CheckTarget(target,this,maxdist,nil,nil,true), !PathFree(GetX(), GetY(), target->GetX(), target->GetY())))
    {
      EffectVar(1, this(), no) = nil;
      if(Equal(EffectVar(nil, this(), no), 2))
        ClearMacroCommands();
      if(!IsAiming()) if(Contents(nil)) Contents(nil)->~AimStop(); // damit auch das Autofeuer aufhört
      if(IsAiming())
        StopAiming();     
        //Log("Ziel iwie weg");
      return;
    }
  // Ich hab nix? °-°
  if(!Contents()) return; // Lauf, Forest, lauf!
  //Log("nach ich hab nix");
  // Waffe in die Hand nehmen
  if(!SelectWeapon(level, target, nil))
    // Evtl. Feuermodus wechseln (dann muss erst nachgeladen werden, aber besser als nichts)
    if(!SelectWeapon(level, target, true))
    {
      // Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
      if(Equal(GetAggroLevel(), Aggro_Follow))
      {
//      Message("@Searching for weapons / ammo", this());
        // Waffen auffrischen?
        if((CustomContentsCount("IsWeapon") ?? 0) <= 1)
          return(SearchWeapon(Aggro_Shoot));
        // Munition auffrischen
        return(SearchAmmo(Aggro_Shoot));
      }
      // ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
        //Log("Balrog?");
        return;
    }
  // Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

  if(!ReadyToFire()) return;

  // Schaue ich in die richtige Richtung?
  if((GetX() ?? 0) < (target->GetX() ?? 0))
  {
    if(!Equal(GetDir(), DIR_Right))
      SetDir(DIR_Right);
  }
  else
  {
    if(!Equal(GetDir(), DIR_Left))
      SetDir(DIR_Left);
  }
     //Log("kurz vor zielen muss auch mal sein");
  // Zielen, muss auch mal sein
 if(Or(Or((And(!GetCommand(), !GetMacroCommand())), !Equal(level, 1)), IsAiming()))
 {
  if(And(pathfree, (Contents()->GetBotData(BOT_Range) ?? 0) > 30)) // Weg frei und keine Nahkampfwaffe?
  {
    var angle = Angle(GetX(), GetY(), target->GetX(), target->GetY());
    if((Or((And(!Inside(angle, 70, 120), !Inside(angle, 250, 290))), (Contents()->GetFMData(FM_Aim) ?? 0)>0))
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
          S42MigrationAssign(ty, "-=", 15);
		
		DoMouseAiming(tx, ty);
      }
    }
    else
      if(IsAiming())
        StopAiming();
  }
	if(And(IsAiming(), !CheckAmmo(Contents()->GetFMData(FM_AmmoID), Contents()->GetFMData(FM_AmmoLoad), Contents(), this())))
	 	StopAiming();
 }
 
   // Gut. Feuern wir bereits?
  if(Or(Contents()->IsRecharging(), Contents()->IsShooting())) return;
 
  // Feuer!
  if(And(!Equal(maxdist, 300), pathfree)) Control2Contents("ControlThrow");
//  Message("@My target: %s @%d/%d with level %d", this(), target->GetName(), target->GetX(), target->GetY(), level);
  // Stufe 2 - verfolgen!
  if((EffectVar(nil, this(), no) ?? 0) >= 2)
    if(Or(!SEqual(GetMacroCommand(1), "Follow"), !Equal(GetMacroCommand(1, 1), target)))
      if(Or(!SEqual(GetMacroCommand(nil), "Follow"), !Equal(GetMacroCommand(nil,1), target)))
      {
        DebugLog("FxAggroFire - Adding Follow command","aggro");
        AddMacroCommand(nil, "MoveTo", nil, GetX(),GetY(), nil, level);
        AddMacroCommand(nil, "Follow", target, nil, nil, nil, level);
      }
  //Log("sollte gefeuert haben?");
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  //Log("Selectweapon wird geprüft");
  // Entfernung zum Ziel
  iLevel ??= 0;
  fFireModes ??= false;
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return;
  // Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  // Alle durchgehen und passende prüfen
  for(var i=nil,obj,fav,mode,favmode ; obj = Contents(i) ; S42MigrationStep(mode, 1, true))
  {
    // Nix Waffe
    if(!(obj->~IsWeapon())) { S42MigrationStep(i, 1, true); mode = -1; continue; }
    // Feuermodus
    if(And(mode, !fFireModes)) { S42MigrationStep(i, 1, true); mode = -1; continue; }
    if(!(obj->GetFMData(FM_Name, mode))) { S42MigrationStep(i, 1, true); mode = -1; continue; }
    if(And(Equal(mode, obj->GetFireMode()), mode)) continue;
    // Nix gut... Hier wird jetzt auch die Aim Action von Bazooka und Co erkannt, weil damit sonst nicht geschossen wird -.-
    if((obj->GetFMData(FM_Aim, mode) ?? 0)>0)
      if(Or(Equal(iLevel, 1), (And(!WildcardMatch(GetAction(), "*Walk*"), !WildcardMatch(GetAction(), "Aim*")))))
        continue;
    // Keine Munition dafür?
    if(And(!(obj->GetCharge()), !GetAmmo(obj->GetFMData(FM_AmmoID, mode))))
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
      if((fav->GetBotData(BOT_Range, favmode) ?? 0) < (dist ?? 0))
      {
        // Neue Waffe hat mehr
        if((obj->GetBotData(BOT_Range, mode) ?? 0) > (dist ?? 0))
        {
          fav = obj;
          type = obj->GetBotData(BOT_DmgType, mode);
          favmode = mode;
        }
      }
      else
      {
        // Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
        if(And((pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) ?? 0) < (pTarget->~OnDmg(type) ?? 0),
          // Allerdings darf die Waffe nicht zu schwach sein
          (fav->GetBotData(BOT_Power, favmode) ?? 0)-1 <= (obj->GetBotData(BOT_Power, mode) ?? 0)))
        {
          // Neuer Favorit
          fav = obj;
          type = fav->GetBotData(BOT_DmgType);
          favmode = mode;
        }
        else
        {
          // Stärke der neuen Waffe ist größer oder Favorit ist ein Langlader
          if(Or((fav->GetBotData(BOT_Power, favmode) ?? 0) < (obj->GetBotData(BOT_Power, mode) ?? 0),
             (And(Equal(fav->GetBotData(BOT_Power, favmode), BOT_Power_LongLoad), (Or(fav->IsReloading(), !(fav->GetCharge())))))))
          {
            // Waffe hat keine extralange Nachladezeit
            if(!Equal(obj->GetBotData(BOT_Power, mode), BOT_Power_LongLoad))
            {
              // Neuer Favorit
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
            // Waffe sollte nicht nachladen und nicht leer sein
            else if(And(!Equal(obj->GetCharge(), nil), !(obj->IsReloading())))
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
      if((fav->GetBotData(BOT_Range, favmode) ?? 0) >= (dist ?? 0))
        // Schadenstyp auch
        if(Equal(preftype, type))
          // Stärke auch
            if((fav->GetBotData(BOT_Power, favmode) ?? 0) >= (BOT_Power_3 ?? 0))
              break;
    }
  }
  // Auswählen
  if(!fav) return;
  // Feuermodus wechseln?
  if(fFireModes)
    if(And(favmode, !Equal(favmode, fav->GetFireMode())))
      fav->SetFireMode(favmode);
  if(Equal(ContentsCount(), 1)) return(1);
  return(ShiftContents(nil,nil,fav->GetID()));
}

public func GetPrefDmgType(object pTarget)
{
  var min = pTarget->~OnDmg(DMG_Projectile);
  var type = DMG_Projectile;
  if((pTarget->~OnDmg(DMG_Melee) ?? 0) < (min ?? 0))
  {
    type = DMG_Melee;
    min = pTarget->~OnDmg(type);
  }
  if((pTarget->~OnDmg(DMG_Fire) ?? 0) < (min ?? 0))
  {
    type = DMG_Fire;
    min = pTarget->~OnDmg(type);
  }
  if((pTarget->~OnDmg(DMG_Explosion) ?? 0) < (min ?? 0))
  {
    type = DMG_Explosion;
    min = pTarget->~OnDmg(type);
  }
  if((pTarget->~OnDmg(DMG_Energy) ?? 0) < (min ?? 0))
  {
    type = DMG_Energy;
    min = pTarget->~OnDmg(type);
  }
  if((pTarget->~OnDmg(DMG_Bio) ?? 0) < (min ?? 0))
    type = DMG_Bio;
    
  return(type);
}

// Sucht nach Waffen und läuft dorthin
public func SearchWeapon(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  iAggro ??= 0;
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(),this()))
        	// Hinlaufen
                return(SetMacroCommand(nil, "MoveTo", pSpawn, nil,nil,nil, iAggro));
}

// Sucht nach Munition und läuft dorthin
public func SearchAmmo(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  iAggro ??= 0;
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(),this()))
      	// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
        return(SetMacroCommand(nil, "MoveTo", pSpawn, nil,nil,nil, iAggro));
}

/* Waffenbehandlung wenn nicht im Kampf */

public func CheckIdleWeapon()
{
  if(Contents())
    // Hack - mit BR-Bombe tut er gar nichts
    if(Equal(Contents()->GetID(), GBRB)) return;
  // Keine Waffen im Inventar
  if(!CustomContentsCount("IsWeapon")) return;
  // nachladende Waffe in der Hand
  if(Contents()->~IsWeapon())
    if(Contents()->IsReloading())
      return;
     //Log("Inventar nach waffe zum laden durchsuchen");
  // Inventar nach Waffe durchsuchen, die man Nachladen könnte
  for(var i=nil, mode=1, obj, favmode ; obj = Contents(i) ; S42MigrationStep(mode, 1, true))
  {
    // Keine Waffe
    if(!(obj->~IsWeapon()))
    {
     //Log("keine Waffe");
      S42MigrationStep(i, 1, true);
      mode = nil;
      continue;
    }
    // Waffe hat gar nicht so viele Modi
    if(!(obj->GetFMData(FM_Name, mode)))
    {
      //Log("Waffe hat nicht genug modi");
      S42MigrationStep(i, 1, true);
      mode = nil;
      continue;
    }
    // Waffe ist voll geladen, hier geändert dass nicht /2, sondern auf andrer Seite der gleichung *2, weil 1/2==0 wäre.
    if((obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode)) ?? 0)*2 >= (obj->GetFMData(FM_AmmoLoad, mode) ?? 0))
    {
      //Log("waffe ist geladen");
      S42MigrationStep(i, 1, true);
      mode = nil;
      continue;
    }
    // EMP-Modi erstmal nicht laden
    if(obj->GetBotData(BOT_EMP, mode)) continue;
    // Waffe ist nachladbar
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), (obj->GetFMData(FM_AmmoLoad, mode) ?? 0) - (obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode)) ?? 0)))
    {
      mode = obj->GetFireMode();
      break;
    }
    // Nächsten Feuermodus prüfen
    if(Equal(mode, obj->GetFireMode())) continue;
    if(CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
      break;
  }
  // Nix gefunden
  if(!Contents(i)) return;
  // Aha! Waffe wechseln!
  ShiftContents(this(), nil, obj->GetID());
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
  for(var i=nil,obj ; obj = Contents(i) ; S42MigrationStep(i, 1, true))
  {
      // BR-Hack
    if(Equal(Contents()->GetID(), GBRB))
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
  Schedule(Format("Exit(Object(%d), 0, 10);", ObjectNumber(pObj)), 1, nil, this());
  // Nicht wieder einsammeln
  var effect = AddEffect("CollectionException", pObj, 1, 36);
  EffectVar(nil, pObj, effect) = this();
}

// Ausrüstung anlegen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateGear(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, nil, this());
}

// Mine platzieren (verzögert, damit die Schleife nicht durcheinander kommt
public func PlaceMine(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, nil, this());
}

// Waffe suchen, die man upgraden kann
public func GetUpgradeableWeapon(object pObj)
{
  for(var i=nil,obj ; obj = Contents(i) ; S42MigrationStep(i, 1, true))
    if(obj->~IsUpgradeable(pObj->GetID()))
      return(obj);
}

// Waffe upgraden (verzögert, damit die Schleife nicht durcheinander kommt
public func UpgradeWeapon(object pObj, object pWeapon)
{
  ScheduleCall(pObj, "UpgradeObject", 1, nil, pWeapon);
}

// Munition aufnehmen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateAmmo(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, nil, this());
}

// Hardgecodete Objekte

// Support-Drohne
// Zufällige Waffe für eine Drohne aussuchen
private func GetRandomDroneWeapon()
{
  var aWps = CreateArray();
  for(var i=nil,obj ; obj = Contents(i) ; S42MigrationStep(i, 1, true))
    if(obj->~IsWeapon())
      if((obj->GetFMData(FM_Aim) ?? 0)<=0)
        aWps[(GetLength(aWps) ?? 0)] = obj;
  // Zufällig zurückgeben
  if(!GetLength(aWps)) return;
  return(aWps[(Random(GetLength(aWps)) ?? 0)]);
}

// BR-Bombe
protected func BombingRunBomb()
{
  // nächstes, feindliches Tor suchen
  var nextding, distance = -1;
  for(var pGate in FindObjects(Find_ID(GBRG)))
  {
    if(Equal(pGate->GetGoalTeam(), GetPlayerTeam(GetOwner()))) continue;
    if(Equal(distance, -1))
    {
      nextding = pGate;
      distance = ObjectDistance(pGate);
    }
    else
    {
      if((ObjectDistance(pGate) ?? 0) < (distance ?? 0))
      {
        nextding = pGate;
        distance = ObjectDistance(pGate);
      }
    }
  }
  // Tor ist zu weit weg?
  if((distance ?? 0) > 200)
  {
    distance = -1;
    nextding = nil;
  }
  // Freunde suchen, wenn der Timer abläuft
  if(And(!nextding, (Contents()->GetBombTimer() ?? 0) <= 100))
  {
    for(var pFriend in FindObjects(Find_OCF(OCF_CrewMember), Find_Allied(GetOwner()), Find_Exclude(this)))
    {
      if(Equal(distance, -1))
      {
        nextding = pFriend;
        distance = ObjectDistance(pFriend);
      }
      else
      {
        if((ObjectDistance(pFriend) ?? 0) < (distance ?? 0))
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
    if(And((distance ?? 0) <= 200, PathFree(GetX(), GetY(), nextding->GetX(), nextding->GetY())))
    {
      // Winkel bestimmen
      var angle = Angle(GetX(), GetY(), nextding->GetX(), nextding->GetY());

      // Winkel stimmt!
      if(Or(Inside(angle, 70, 110), Inside(angle, 250, 290)))
      {
		if(ReadyToFire()) {
	        if((GetX() ?? 0) < (nextding->GetX() ?? 0))
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
          if(Equal(GetDir(), DIR_Left))
            angle = 360-(angle ?? 0);
          angle = BoundBy((angle ?? 0)-10, nil, AIM_Max);
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
hatschutzanzug=nil;
}

func HatSchutzAnzugan()
{
if(hatschutzanzug) return(true);
if(!(hatschutzanzug)) return(nil);
}


// pinguinanzug unter wasser arbeiten
func CheckObjectAvailability (obj)
{
if(!(pinguin)) return(nil);
    // Object is near enough
    if (!Inside ((GetX () ?? 0) - (GetX (obj) ?? 0), -500, +500)) return ;
    if (!Inside ((GetY () ?? 0) - (GetY (obj) ?? 0), -250, +250)) return ;
    // Object is not connected to a pipe (for line construction kits)
    if (FindObject (SPIP, nil, nil, nil, nil, nil, "Connect", obj)) return ;
    if (FindObject (DPIP, nil, nil, nil, nil, nil, "Connect", obj)) return ;
    // Must be complete
    if (~(GetOCF (obj) ?? 0) & (OCF_Fullcon ?? 0)) return ;
    // Doesn't burn
    if (OnFire (obj)) return ;
    // Go!
    return (obj);
}

func GetAvailableObject (def, xobj)
{
if(!(pinguin)) return(nil);
  var obj;
  // Next closest normal available or in liquid
  while (obj = FindObject (def, nil, nil, -1, -1, (OCF_Available ?? 0) | (OCF_InLiquid ?? 0), nil, nil, nil, obj))
  {
    if ((ObjectDistance (obj) ?? 0) > 559) { obj = nil; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (Or(!(Equal(Contained (obj), xobj)), !xobj))
      // We found one
      break;
  }
  if (obj) return (obj);
  // Next closest in container (because containers are sunk here, nothing in them is "available"...)
  while (obj = FindObject (def, nil, nil, -1, -1, OCF_Fullcon, nil, nil, AnyContainer(), obj))
  //while (obj = FindObject (def, 0, 0, -1, -1, OCF_Fullcon(), 0, 0, 0, obj))
  {
    if ((ObjectDistance (obj) ?? 0) > 559) { obj = nil; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (Equal(Contained (obj), xobj)) continue;
    // The Container must be accessible (Same as for OCF_Available)
    if (Or((GetDefCoreVal ("GrabPutGet", nil, GetID (Contained(obj))) ?? 0) & (C4D_GrabGet ?? 0), (GetOCF (Contained(obj)) ?? 0) & (OCF_Entrance ?? 0)))
    // We found one
      break;
  }
  return (obj);
}


public func ControlCommandAcquire(target, x, y, target2, def)
{
if(!(pinguin)) return(nil);
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return (S42MigrationFirst(1, AddCommand (this(), "Get", obj,nil,nil,nil,40)));
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
pinguin=nil;
}

// Ammogürtel, bekommt die Zahl vom Gürtel geliefert
func GetMoreAmmo(zzaahhll)
{
if(Equal(zzaahhll, 1)) mehrmunition1=1;
if(Equal(zzaahhll, 2)) mehrmunition2=1;
if(Equal(zzaahhll, 3)) mehrmunition3=1;
if(Equal(zzaahhll, 4)) mehrmunition4=1;
if(Equal(zzaahhll, 5)) mehrmunition5=1;
if(Equal(zzaahhll, 6)) mehrmunition6=1;
if(Equal(zzaahhll, 7)) mehrmunition7=1;
if(Equal(zzaahhll, 8)) mehrmunition8=1;
if(Equal(zzaahhll, 9)) mehrmunition9=1;
if(Equal(zzaahhll, 10)) mehrmunition10=1;
if(Equal(zzaahhll, 11)) mehrmunition11=1;
if(Equal(zzaahhll, 12)) mehrmunition12=1;
if(Equal(zzaahhll, 13)) mehrmunition13=1;
if(Equal(zzaahhll, 14)) mehrmunition14=1;
if(Equal(zzaahhll, 15)) mehrmunition15=1;
if(Equal(zzaahhll, 16)) mehrmunition16=1;
if(Equal(zzaahhll, 17)) mehrmunition17=1;
if(Equal(zzaahhll, 18)) mehrmunition18=1;
if(Equal(zzaahhll, 19)) mehrmunition19=1;
if(Equal(zzaahhll, 20)) mehrmunition20=1;
}




func GetNormalAmmo()
{
mehrmunition1=nil;
mehrmunition2=nil;
mehrmunition3=nil;
mehrmunition4=nil;
mehrmunition5=nil;
mehrmunition6=nil;
mehrmunition7=nil;
mehrmunition8=nil;
mehrmunition9=nil;
mehrmunition10=nil;
mehrmunition11=nil;
mehrmunition12=nil;
mehrmunition13=nil;
mehrmunition14=nil;
mehrmunition15=nil;
mehrmunition16=nil;
mehrmunition17=nil;
mehrmunition18=nil;
mehrmunition19=nil;
mehrmunition20=nil;
// wenn aktuelle Munition mehr ist, als es ohne Gürtel möglich sein sollte:

}

// Projektile klein groß
func MoreAmmo1()
{
if(Equal(mehrmunition1, 1)) return(true);
return(nil);
}

func MoreAmmo2()
{
if(Equal(mehrmunition2, 1)) return(true);
return(nil);
}

// Energie
func MoreAmmo3()
{
if(Equal(mehrmunition3, 1)) return(true);
return(nil);
}

func MoreAmmo4()
{
if(Equal(mehrmunition4, 1)) return(true);
return(nil);
}

//Grananten
func MoreAmmo5()
{
if(Equal(mehrmunition5, 1)) return(true);
return(nil);
}

func MoreAmmo6()
{
if(Equal(mehrmunition6, 1)) return(true);
return(nil);
}

//Benzin
func MoreAmmo7()
{
if(Equal(mehrmunition7, 1)) return(true);
return(nil);
}

func MoreAmmo8()
{
if(Equal(mehrmunition8, 1)) return(true);
return(nil);
}

//Raketen
func MoreAmmo9()
{
if(Equal(mehrmunition9, 1)) return(true);
return(nil);
}

func MoreAmmo10()
{
if(Equal(mehrmunition10, 1)) return(true);
return(nil);
}

//BetterBullets
func MoreAmmo11()
{
if(Equal(mehrmunition11, 1)) return(true);
return(nil);
}

func MoreAmmo12()
{
if(Equal(mehrmunition12, 1)) return(true);
return(nil);
}

// Schrotkugeln
func MoreAmmo13()
{
if(Equal(mehrmunition13, 1)) return(true);
return(nil);
}

func MoreAmmo14()
{
if(Equal(mehrmunition14, 1)) return(true);
return(nil);
}

//SniperKugeln
func MoreAmmo15()
{
if(Equal(mehrmunition15, 1)) return(true);
return(nil);
}

func MoreAmmo16()
{
if(Equal(mehrmunition16, 1)) return(true);
return(nil);
}

//Partikel
func MoreAmmo17()
{
if(Equal(mehrmunition17, 1)) return(true);
return(nil);
}

func MoreAmmo18()
{
if(Equal(mehrmunition18, 1)) return(true);
return(nil);
}


func MoreAmmoAllrounder1()
{
if(Equal(mehrmunition19, 1)) return(true);
return(nil);
}

func MoreAmmoAllrounder2()
{
if(Equal(mehrmunition20, 1)) return(true);
return(nil);
}


func Klimaschutz(klima)
{
//Kälte
if(Equal(klima, 1))
{
//wenn schutzkleidung, dann kein Schaden
if(Or(Or(hatschutzanzug, pinguin), hatanorak)) return(1);
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, kein Schaden.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_ID(LAGF),Find_ID(TORC))))
{
//nur wenn Feuer brennt
if(Equal(GetID(feuer), LAGF)) if(!(!!SEqual(GetAction(feuer), "Burn"))) return(nil);
 if(Equal(GetID(feuer), TORC)) if(!(!!SEqual(GetAction(feuer), "Burning"))) return(nil);
  return(1);}
return(nil);
}

//Hitze
if(Equal(klima, 2))
{
//wenn schutzkleidung, dann kein Schaden
if(hatschutzanzug) return(1);
//Wenn in Flüssigkeit, kein schaden.
if((GetOCF() ?? 0) & (OCF_InLiquid ?? 0)) return(1);
return(nil);
}

}




/* Serialisierung */

public func Serialize(array& extra)
{
	var x, ammo;
	while(ammo = Contents(S42MigrationStep(x, 1, true),ammobag)) {
		if(!(ammo ->~IsAmmo())) continue;
		extra[(GetLength(extra) ?? 0)] = Format("DoAmmo(%i,%d)", GetID(ammo), GetAmmo(GetID(ammo)));
	}
}




local target;

protected func IsBulletTarget() { return(1); } // von kugeln werden alle lebewesen getroffen, egal ob bullettarget oder nicht..
func IsMutant() { return(1); }


func ControlSpecial2()
{
 if (And(!!SEqual(GetProcedure(), "WALK"), Contents()))
  if (Contents()->~IsGrenade())
   SetAction("ThrowGrenade");
 if (!!SEqual(GetProcedure(), "WALK")) SetAction("Kick");
 return(1);
}

func ThrowGrenade()
{
 if (!Equal(GetPhase(), 4)) return(nil);
 var Cont = Contents();
 if (!Cont) return(nil);
 Sound("ThrowGrenade");
 Exit(Cont,-5+(GetDir() ?? 0)*10,nil,nil,-5+(GetDir() ?? 0)*10,-3,Random(200));
 return(1);
}

func Stab()
{
 if (!Equal(GetPhase(), 3)) return(nil);
 for (var prey in FindObjects(Find_Distance(10),Find_OCF(OCF_Alive),Find_Owner(NO_OWNER)))
  Punch(prey,RandomX(12,20));  
 return(1);
}

func Kick()
{
 if (!Equal(GetPhase(), 3)) return(nil);
 for (var prey in FindObjects(Find_Distance(5),Find_Exclude(),Find_Or(Find_OCF(OCF_Rotate),Find_OCF(OCF_Living))))
  if (GetAlive(prey)) Fling(prey,-2+(GetDir() ?? 0)*4,-1);
   else if (SetXDir(-8+(GetDir() ?? 0)*16,prey)) SetYDir(-50,prey);
 return(1);
}


protected func Death()
{
  GameCallEx("Zaehler16");
 SetAction("BigAttack");
 CastParticles("PSpark",Random(40),Random(80),nil,nil,nil,Random(150),RGB(100,nil,nil));
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
 if (!GetAlive()) return(nil);
 //if(!Random(3)) DoEnergy(+Random(1));
 if (Stuck()) DigFree(GetX(),GetY(),16);
 for (var obj in FindObjects(Find_NoContainer(),Find_Not(Find_Owner(-1)),Find_OCF((OCF_Alive ?? 0)|(OCF_CrewMember ?? 0)),
//Find_Func("IsAttackable"),
Find_Distance(300)))
  if (SetCommand(this(),"MoveTo",obj))
   SetDir((GetX() ?? 0)<(GetX(obj) ?? 0));
 for (var clonk in FindObjects(Find_NoContainer(),Find_InRect(-200+(GetDir() ?? 0)*200,-20,200,20),Find_Not(Find_Owner(-1)),Find_OCF((OCF_CrewMember ?? 0)|(OCF_Alive ?? 0))))
   if (!SEqual(GetProcedure(), "THROW"))
    if (!Random(8))
     SetAction("Attack");
 if (!Random(16))
  ChangeDir();
 if (!Random(19)) SetComDir(COMD_Stop);
 return(1);
}

func CheckClonks()
{
 for (var clonk in FindObjects(Find_NoContainer(),Find_Not(Find_Owner(-1)),Find_InRect(-80+(GetDir() ?? 0)*80,-20,80,20),Find_OCF(OCF_Alive)))
  if (Punch(clonk,Random(35)))
   Fling(clonk,-7+(GetDir() ?? 0)*14,-(Random(5) ?? 0));
 Sound("MireokAttack");
 return(1);
}

func MakeDestruction()
{
 for (var clonk in FindObjects(Find_Not(Find_Owner(-1)),Find_Distance(100),Find_Exclude(),Find_Or(Find_Category(C4D_Object),Find_Category(C4D_Living))))
 {
  if (Punch(clonk,Random(90)))
   Fling(clonk,-20+(GetDir() ?? 0)*40,-(Random(5) ?? 0));
  DoDamage(Random(30),clonk);
  SetXDir(RandomX(-10,10),clonk);
  SetXDir(RandomX(-10,10),clonk);
  if (!Random(30)) Incinerate(clonk);
 }
 CreateParticle("Wave",nil,nil,RandomX(-10,10),RandomX(-10,10),3000,RGB(255,nil,255));
 Sound("MireokAttack");
 return(1);
}

public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  iDamage ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Explosion))    return(-20);
  // Energieschaden
  if(Equal(iType, DMG_Energy))     return(50);
  // Projektilschaden
  if(Equal(iType, DMG_Projectile))    return(50);
  // Feuerschaden
  if(Equal(iType, DMG_Fire))    return(-80);
  // Bioschaden
  if(Equal(iType, DMG_Bio))    return(20);
  // Nahkampfschaden
  if(Equal(iType, DMG_Melee))    return(60);
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))    return(30);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(nil);
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