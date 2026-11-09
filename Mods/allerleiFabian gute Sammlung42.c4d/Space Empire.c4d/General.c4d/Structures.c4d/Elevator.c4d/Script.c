/*-- Schwebelift --*/
// (Dieses Script wird von den Liften von SLC und CoAl gemeinsam genutzt)

#strict 3

local AutoMode, AutoMoveDir;    // Dauerbetrieb: an/aus-Schalter, Richtung
local RangeTop, RangeBottom;    // Ober- und Untergrenze
local Liftdock;                 // Zeiger auf Liftdock
local timer,supermode,zeitzahl;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Wait");
  UpdateRangeTop();
  UpdateRangeBottom();
  return(1);
}

public func SetOwnerObject(dock)
{
  Liftdock = dock;
  return(1);
}

private func UpdateRangeTop()
{
  RangeTop = GetY();
  UpdateTransferZone();
  return(1);
}

private func UpdateRangeBottom()
{
  var x = GetX();
  var y = GetY();
  while (And(PathFree(x, y, x, (y ?? 0)+5), ((y ?? 0) < (LandscapeHeight() ?? 0))))
    S42MigrationAssign(y, "+=", 5);
  RangeBottom = (y ?? 0)+5;
  UpdateTransferZone();
  return(1);
}


protected func Timer() 
{
  // kein Liftdock da?
  if (!Liftdock)
    return(Message("kein Lift!"));
  timer = (S42MigrationStep(timer, 1)) % 30;
  // keine Energie und nicht im Liftdock?
  if (Or(HasEMP(Liftdock), And(NoEnergy(), (((GetY() ?? 0)-(RangeTop ?? 0)) > 5))))
    return(SetAction("Idle"));

  // Lore fassen
  GrabLorry();

 // Objekte nach oben 
  var obj; 
  while (obj = FindObject(nil, -23,nil,45,10, OCF_Collectible,nil,nil,NoContainer(),obj))
    SetPosition(GetX(obj),(GetY() ?? 0)+6,obj);

  if (!timer) Activity();
}



/* Timer-Call */

protected func Activity()
{ 
  // zu wartenden Clonks fahren
  if (And(And(!AutoMode, !Passenger()), !GetCommand()))
    MoveToWaitingClonk();
  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
  if (GetComDir())
    if (And(!GetCommand(), (!SEqual(GetAction(), "Drill"))))
      {Halt(); Message("angehalten",this()); }
  // fertig
  return(1);
}

private func MoveToWaitingClonk()
{
  if (HasEMP(Liftdock)) return(1);
  // wartenden Clonk finden
  var clonk = FindObject(nil, -60, (RangeTop ?? 0)-(GetY() ?? 0), 120, (RangeBottom ?? 0)-(RangeTop ?? 0), OCF_CrewMember,nil,nil,NoContainer());
  if (!clonk) return(nil);
  // wartet der Clonk auch?
  if (!Equal(GetComDir(clonk), COMD_Stop)) return(nil);
  if (And(And(And(And(!SEqual(GetAction(clonk), "Walk")
 // && GetAction(clonk) ne "ArmedWalk" // deaktiviert weil sonst nicht zu hazard mit waffe
 ,
      !SEqual(GetAction(clonk), "Push")), !SEqual(GetAction(clonk), "Swim")),
      !SEqual(GetAction(clonk), "Scale")), !SEqual(GetAction(clonk), "Hangle"))) return(nil);
  // nicht bei verfeindeten Clonks
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  // Weg muss frei ist
  if ((GetY(clonk) ?? 0) < (GetY() ?? 0))  if (!PathFree(GetX(), GetY(),    GetX(), GetY(clonk)))    return(nil);
  if ((GetY(clonk) ?? 0) > (GetY() ?? 0))  if (!PathFree(GetX(), (GetY() ?? 0)+(PlateHeight() ?? 0)/2, GetX(), GetY(clonk))) return(nil);
  // Zielposition setzen und starten
  SetMoveTo(GetY(clonk));
  return(1);              
}

private func GrabLorry()
{
  // Lore anfassen
  if (!SEqual(GetAction(), "Wait")) return(nil);
  var lorry = FindObject(LORY, -20,-20, 40,40);
  if (!lorry) return(nil);
  
  // Wenn ein Clonk dabei ist, nichts tun
  if (FindObject(nil,-(PlateWidth() ?? 0)/2,-(PlateHeight() ?? 0),PlateWidth(),PlateHeight(),OCF_CrewMember,"Push",FindObject(LORY))) return(nil);
  // Lore positionieren
  SetPosition(GetX(), (GetY() ?? 0)-1, lorry);
  SetR(nil, lorry);    SetRDir(nil, lorry);
  SetXDir(nil, lorry); SetYDir(nil, lorry);
  return(1);
}

private func SetMoveTo(position)
{
  // keine Energie
  if (Or(NoEnergy(), HasEMP(Liftdock))) return(1);
  // los geht's
  SetAction("Travel"); 
  var Dir = nil;
  if ((position ?? 0) > (GetY() ?? 0))  Dir = +4;
  if ((position ?? 0) < (GetY() ?? 0))  Dir = -5;
  SetCommand(this(),"MoveTo", nil, GetX(), (Max(position, RangeTop) ?? 0)+(Dir ?? 0));
  SetYDir(nil);
  return(1);
}

public func Halt()
{
  // kein Liftdock da?
  if (!Liftdock)
    return(SetAction("Idle"));
  // keine Energie und nicht im Liftdock?
  if (Or(HasEMP(Liftdock), And(NoEnergy(), !Inside((GetY() ?? 0)-(GetY(Liftdock) ?? 0), -4, -1))))
    return(SetAction("Idle"));
  // anhalten
  SetAction("Wait");
  SetCommand(this(),"None");
  SetComDir(COMD_Stop);
  SetYDir(nil);
  if ((GetY() ?? 0) > (RangeBottom ?? 0)) UpdateRangeBottom();
  return(1);
}


/* Action-Calls */

private func Drilling()
{
  if (HasEMP(Liftdock)) Halt();
if(And((!GBackLiquid(GetX(),(GetY() ?? 0)+5)), FindContents(ENAP,Liftdock)))
//Überall durchbohren, wenn keine flüssigkeit unter fahrstuhl (flüssigkeit klappt noch nicht)
    {FreeRect((GetX() ?? 0) - ((PlateWidth() ?? 0)/2), (GetY() ?? 0) + ((PlateHeight() ?? 0)/2), PlateWidth(), PlateHeight());RemoveObject(FindContents(ENAP,Liftdock));}
  // Steuermann verloren?
  if (And(!FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,"Push",this()),
      !FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,"Push",FindObject(LORY))))
    Halt();
  return(1);
}

private func Waiting()
{
  // Dauerbetrieb: selbständig losfahren
  if (Equal(AutoMode, 1))
    SelfMovement();
  return(1);
}

private func SelfMovement()
{
  if (HasEMP(Liftdock)) return(1);
  // Selbständig rauf- bzw. runterfahren
  if (AutoMoveDir)  SetMoveTo(RangeTop);
  if (!AutoMoveDir) SetMoveTo((LandscapeHeight() ?? 0)-12);
  AutoMoveDir = !AutoMoveDir;
  return(1);
}

/* Commands auswerten */

protected func ControlCommand(Cmd, Target, Tx, Ty)
{
  if (HasEMP(Liftdock)) return(nil);
  if (!Liftdock) return(nil);
  // nur Bewegungskommando auswerten
  if (!SEqual(Cmd, "MoveTo"))
    return(nil);
  // Ziel einstellen
  if (Inside(Tx, (GetX() ?? 0)-15, (GetX() ?? 0)+15))
    return(SetMoveTo(Ty));
  return(nil);
}

protected func ControlCommandFinished(...) {
  return(Halt());
}


/* Steuerung */

public func ControlDownSingle(clonk)
{
  [Abwärts]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(nil);
  if (Or(NoEnergy(), HasEMP(Liftdock))) return(nil);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  // nach unten fahren
  SetMoveTo((LandscapeHeight() ?? 0)-12);
  return(1);
}

public func ControlUp(clonk)
{
  [Auswärts]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(nil);
  if (Or(NoEnergy(), HasEMP(Liftdock))) return(nil);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  // nach oben fahren
  SetMoveTo(RangeTop);
  return(1);
}

public func ControlLeft(clonk)
{
  [Anhalten]
  return(DoControlStop(clonk));
}

public func ControlRight(clonk)
{
  [Anhalten]
  return(DoControlStop(clonk));
}

private func DoControlStop(clonk)
{
  // nicht reagieren, wenn kein Liftdock da
  if (!Liftdock) return(nil);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  // anhalten
  return(Halt());
}

public func ControlDigDouble(clonk)
{
  [Schachtbohrung]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(nil);
  if (Or(NoEnergy(), HasEMP(Liftdock))) return(nil);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  // kurz anhalten
  SetYDir(nil);
  SetCommand(this(),"None");
  // jetzt wird gegraben
  SetAction("Drill");
  SetComDir(COMD_Down);
  return(1);
}

public func ControlThrow(clonk)
{
  [Dauerbetrieb]
  // ohne Liftdock geht da nix
  if (!Liftdock) return(nil);
  if (HasEMP(Liftdock)) return(Sound("Error"));
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(nil);
  AutoMode = !AutoMode;
  if (AutoMode) Message("Dauerbetrieb an", this());
           else Message("Dauerbetrieb aus", this());
  return(1);
}


/* Kontakt */

protected func ContactBottom()
{
  Halt();
  UpdateRangeBottom();
  return(1);
}

protected func ContactTop(...) {
  return(Halt());
}

private func Passenger(...) {
  return(FindObject(nil, -(PlateWidth() ?? 0)/2+2,-(PlateHeight() ?? 0)/2, (PlateWidth() ?? 0)-4,PlateHeight(), OCF_CrewMember,nil,nil,NoContainer()));
}

private func NoEnergy(...) {
  return(!GetEnergy(Liftdock));
}


/* Transfer */

protected func UpdateTransferZone()
{         
  if (!Liftdock)
    return(SetTransferZone());
  var dockY = GetY(Liftdock);
  SetTransferZone(-((PlateHeight() ?? 0)-2), (dockY ?? 0)-(GetY() ?? 0), PlateWidth(), (RangeBottom ?? 0)-(dockY ?? 0)+12);
  return(1);
}

protected func ControlTransfer(clonk, Tx, Ty)  
{
  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if (Inside((GetY(clonk) ?? 0)-(RangeBottom ?? 0), -10, +10))
    if (Inside((Ty ?? 0)-(RangeBottom ?? 0), -10, +10))
      return(nil);
  // Maximale Obergrenze Zielposition durch Fahrstuhl
  if (!Liftdock) return(nil);
  Ty = Max(Ty, (GetY(Liftdock) ?? 0)+15);
  // Fahrstuhl an Zielposition: erledigt
  if (Inside((Ty ?? 0)-(GetY() ?? 0), -5, +5))
    return(nil);
  // Fahrstuhl fährt: warten
  if (!SEqual(GetAction(), "Wait"))
    return(1);
  // Fahrstuhl noch nicht beim Clonk: warten
  if (!Inside((GetY() ?? 0)-(GetY(clonk) ?? 0), -5, +5))
    return(1);
  // Fahrstuhl nur in der Mitte anfassen
  if (!Inside((GetX() ?? 0)-(GetX(clonk) ?? 0), -5, +5))
    return(AddCommand(clonk,"MoveTo",this()));
  // Fahrstuhl anfassen
  if (Or(!SEqual(GetAction(clonk), "Push"), !Equal(GetActionTarget(nil,clonk), this())))
    return(AddCommand(clonk,"Grab",this()));
  // Fahrstuhl zur Zielposition steuern
  return(SetMoveTo(Ty));
}


/* Schaden */

protected func Damage()
{
  if ((GetDamage() ?? 0) < (GetMaxDamage() ?? 0)) return(1);
  return(Destroy());
}

protected func Hit2()
{
  // Aufprall auf den Boden
  if (NoEnergy()) return(Destroy());
  return(nil);
}

public func Destroy()
{
  // Explosion
  Explode(20);
  return(1);
}

/* zu überladende Funktionen für individuelle Objektdaten*/

protected func PlateHeight()
{
  return(1);
}

protected func PlateWidth()
{
  return(1);
}

protected func GetMaxDamage()
{
  return(1);
}

/* Eigenschaften */

public func IsElevator() { return(1); }
