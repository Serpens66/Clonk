/*-- Schwebelift --*/

#strict

local AutoMode, AutoMoveDir;    // Dauerbetrieb: an/aus-Schalter, Richtung
local RangeTop, RangeBottom;    // Ober- und Untergrenze
local Liftdock;                 // Zeiger auf Liftdock


/* Initialisierung */

protected func Initialize()
{
  SetAction("Wait");
  UpdateRangeTop();
  UpdateRangeBottom();
  return(1);
}

public func Init(dock)
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
  var y = GetY() + 13;
  while (PathFree(x, y, x, y+5) && (y < LandscapeHeight()))
    y += 5;
  RangeBottom = y+5;
  UpdateTransferZone();
  return(1);
}


/* Timer-Call */

protected func Activity()
{
  // kein Liftdock da?
  if (!Liftdock)
    return(SetAction("Idle"));
  // keine Energie und nicht im Liftdock?
  if (NoEnergy() && !Inside(GetY()-GetY(Liftdock), -3, 0))
    return(SetAction("Idle"));
  // Lore fassen
  GrabLorry();
  // Objekte nach oben 
  if (GetComDir()==COMD_Stop())
  {
    var obj; 
    while (obj = FindObject(0, -24,7,48,6, OCF_Collectible(),0,0,NoContainer(),obj))
      SetPosition(GetX(obj),GetY()+6,obj);
  }
  // zu wartenden Clonks fahren
  if (!AutoMode && !Passenger() && !GetCommand())
    MoveToWaitingClonk();
  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
  if (GetComDir())
    if (!GetCommand() && (GetAction() ne "Drill"))
      Halt();
  // fertig
  return(1);
}

private func MoveToWaitingClonk()
{
  // wartenden Clonk finden
  var clonk = FindObject(0, -60, RangeTop-GetY(), 120, 6000, OCF_CrewMember(), 0, 0, NoContainer());
  if (!clonk) return(0);
  // wartet der Clonk auch?
  if (GetComDir(clonk) != COMD_Stop()) return(0);
  if (GetAction(clonk) ne "Walk"  && GetAction(clonk) ne "ArmedWalk" &&
      GetAction(clonk) ne "Push"  && GetAction(clonk) ne "Swim" &&
      GetAction(clonk) ne "Scale" && GetAction(clonk) ne "Hangle") return(0);
  // nicht bei verfeindeten Clonks
  if (Hostile(GetController(clonk), GetOwner())) return(0);
  // Weg muss frei ist
  if (GetY(clonk) < GetY())  if (!PathFree(GetX(), GetY(),    GetX(), GetY(clonk)))    return(0);
  if (GetY(clonk) > GetY())  if (!PathFree(GetX(), GetY()+16, GetX(), GetY(clonk))) return(0);
  // Zielposition setzen und starten
  SetMoveTo(GetY(clonk));
  return(1);              
}

private func GrabLorry()
{
  // Lore anfassen
  if (GetAction() ne "Wait") return(0);
  var lorry = FindObject(LORY, -12,-12, 25,25);
  if (!lorry) return(0);
  // Lore schon angefasst?
  if (GetX(lorry) == GetX() &&
      GetY(lorry) == GetY()-3) return(0);
  // Lore positionieren
  SetPosition(GetX(), GetY()-3, lorry);
  SetR(0, lorry);    SetRDir(0, lorry);
  SetXDir(0, lorry); SetYDir(0, lorry);
  return(1);
}

private func SetMoveTo(position)
{
  // keine Energie
  if (NoEnergy()) return(1);
  // los geht's
  SetAction("Travel"); 
  var Dir = 0;
  if (position > GetY())  Dir = +3;
  if (position < GetY())  Dir = -4;
  SetCommand(this(),"MoveTo", 0, GetX(), Max(position, RangeTop) + Dir);
  SetYDir(0);
  return(1);
}

private func Halt()
{
  // kein Liftdock da?
  if (!Liftdock)
    return(SetAction("Idle"));
  // keine Energie und nicht im Liftdock?
  if (NoEnergy() && !Inside(GetY()-GetY(Liftdock), -4, -1))
    return(SetAction("Idle"));
  // anhalten
  SetAction("Wait");
  SetCommand(this(),"None");
  SetComDir(COMD_Stop());
  SetYDir(0);
  if (GetY() > RangeBottom) UpdateRangeBottom();
  return(1);
}


/* Action-Calls */

private func Drilling()
{
  // Steuermann verloren?
  if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this()) &&
      !FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",FindObject(LORY)))
    Halt();
  return(1);
}

private func Waiting()
{
  // Dauerbetrieb: selbständig losfahren
  if (AutoMode == 1)
    SelfMovement();
  return(1);
}

private func SelfMovement()
{
  // Selbständig rauf- bzw. runterfahren
  if (AutoMoveDir)  SetMoveTo(RangeTop);
  if (!AutoMoveDir) SetMoveTo(LandscapeHeight()-12);
  AutoMoveDir = !AutoMoveDir;
  return(1);
}


/* Commands auswerten */

protected func ControlCommand(Cmd, Target, Tx, Ty)
{
  if (!Liftdock) return(0);
  // nur Bewegungskommando auswerten
  if (Cmd ne "MoveTo")
    return(0);
  // Ziel einstellen
  if (Inside(Tx, GetX()-15, GetX()+15))
    return(SetMoveTo(Ty));
  return(0);
}

protected ControlCommandFinished:
  return(Halt());


/* Steuerung */

public func ControlDownSingle(clonk)
{
  [Abwärts]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(0);
  if (NoEnergy()) return(0);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(0);
  // nach unten fahren
  SetMoveTo(LandscapeHeight()-12);
  return(1);
}

public func ControlUp(clonk)
{
  [Auswärts]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(0);
  if (NoEnergy()) return(0);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(0);
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
  if (!Liftdock) return(0);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(0);
  // anhalten
  return(Halt());
}

public func ControlDigDouble(clonk)
{
  [Schachtbohrung]
  // kein Liftdock oder keine Energie?
  if (!Liftdock)  return(0);
  if (NoEnergy()) return(0);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(0);
  // kurz anhalten
  SetYDir(0);
  SetCommand(this(),"None");
  // jetzt wird gegraben
  SetAction("Drill");
  SetComDir(COMD_Down());
  return(1);
}

public func ControlThrow(clonk)
{
  [Dauerbetrieb]
  // ohne Liftdock geht da nix
  if (!Liftdock) return(0);
  Sound("Click");
  // verfeindet?
  if (Hostile(GetController(clonk), GetOwner())) return(0);
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

protected ContactTop:
  return(Halt());

private Passenger:
  return(FindObject(0, -24,-13, 48,20, OCF_CrewMember(),0,0,NoContainer()));

private NoEnergy:
  return(!GetEnergy(Liftdock));


/* Transfer */

protected func UpdateTransferZone()
{         
  if (!Liftdock)
    return(SetTransferZone());
  var dockY = GetY(Liftdock);
  SetTransferZone(-24, dockY-GetY(), 48, RangeBottom-dockY+12);
  return(1);
}

protected func ControlTransfer(clonk, Tx, Ty)  
{
  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if (Inside(GetY(clonk)-RangeBottom, -10, +10))
    if (Inside(Ty-RangeBottom, -10, +10))
      return(0);
  // Maximale Obergrenze Zielposition durch Fahrstuhl
  if (!Liftdock) return(0);
  Ty = Max(Ty, GetY(Liftdock)+15);
  // Fahrstuhl an Zielposition: erledigt
  if (Inside(Ty-GetY(), -5, +5))
    return(0);
  // Fahrstuhl fährt: warten
  if (GetAction() ne "Wait")
    return(1);
  // Fahrstuhl noch nicht beim Clonk: warten
  if (!Inside(GetY()-GetY(clonk), -5, +5))
    return(1);
  // Fahrstuhl nur in der Mitte anfassen
  if (!Inside(GetX()-GetX(clonk), -5, +5))
    return(AddCommand(clonk,"MoveTo",this()));
  // Fahrstuhl anfassen
  if (GetAction(clonk) ne "Push" || GetActionTarget(0,clonk) != this())
    return(AddCommand(clonk,"Grab",this()));
  // Fahrstuhl zur Zielposition steuern
  return(SetMoveTo(Ty));
}


/* Schaden */

protected func Damage()
{
  if (GetDamage() < GetMaxDamage()) return(1);
  return(Destroy());
}

public func Destroy()
{
  // Fragmente
  var obj, i;
  for (i = 0; i < 5; ++i)
  {
    obj = CreateObject(SFRX, Random(50)-25, Random(8), GetOwner());
    obj->Settings(i);
  }
  // Explosion
  Explode(20);
  return(1);
}


/* Eigenschaften */

public IsElevator: return(1);
public GetMaxDamage: return(60);
public Repairable: return(1);

