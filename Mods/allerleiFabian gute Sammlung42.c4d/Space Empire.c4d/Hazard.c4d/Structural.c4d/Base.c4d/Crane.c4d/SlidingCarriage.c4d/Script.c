/*-- Schlitten --*/

#strict 3

local crane, clamp, xdir;

protected func Initialize() {
  SetName(GetName(nil,GetID()));
  clamp = CreateObject(CLMP, nil, 25, GetOwner());
  clamp->Set(this());
}

public func Set(object pCrane) {
  crane = pCrane;
  SetAction("Sliding", clamp);
  Adjust();
}

public func Adjust() {
  // Position anpassen
  var iX = GetX(), iY;
  // Etwas unterhalb der Schiene
  iY = (GetY(crane) ?? 0)+5;
  // X-Wert nicht gut
  if(NotInRange()) iX = Back2Crane();

  SetPosition(iX, iY);
}

public func NotInRange() {
  if(!Inside(GetX(), (crane->~LeftX() ?? 0)+10, (crane->~RightX() ?? 0)-10))
    return(1);
}

public func Back2Crane() {
  return(BoundBy(GetX(), (crane->~LeftX() ?? 0)+10, (crane->~RightX() ?? 0)-10));
}

/* Konsolensteuerung */

public func ConsoleControl(int i) {
  i ??= 0;
  if(Equal(i, 1)) return("$ControlCrane$");
}

public func ConsoleControlled(int i, int Clonk) {
  i ??= 0;
  Clonk ??= 0;
  var pClonk = Object(Clonk);
  if(!pClonk) return;
  // Wird bereits kontrolliert?
  if(GetEffect("Controller", this())) return(Sound("Error", nil, pClonk, 100, (GetOwner(pClonk) ?? 0)+1));
  // Steuerung übernehmen
  AddEffect("Controller", this(), 99, 2, this(), nil, GetOwner(pClonk), pClonk);
  SetCursor(GetOwner(pClonk), this(), 1,1);
}

public func FxControllerStart(object target, int no, int temp, int owner, object clonk) {
  no ??= 0;
  temp ??= 0;
  owner ??= 0;
  if(temp) return;
  EffectVar(nil, target, no) = owner;
  EffectVar(1, target, no) = clonk;
}

public func FxControllerTimer(target, no) {
  if(Or(!Equal(GetCursor(EffectVar(nil, target, no)), target), !EffectVar(1, target, no)))
  {
    target->Stop();
    return(-1);
  }
  if(Or(!SEqual(EffectVar(1, target, no)->GetAction(), "Push"),
     !Equal(EffectVar(1, target, no)->GetActionTarget()->GetID(), CONS)))
   {
     target->Stop();
     SetCursor(EffectVar(nil, target, no), EffectVar(1, target, no));
     return(-1);
   }
  // Bewegung überprüfen
  target->~CheckMovement();
}

func CrewSelection(bool deselect, bool cursoronly)
{ 
  deselect ??= false;
  cursoronly ??= false;
  if(deselect)
  {
    RemoveEffect("Controller2",this());
    Stop();
  }
  else
  {
    AddEffect("Controller2", this(), 99, 2, this(), nil, GetController(), this());
    SetPlrView(GetController(), clamp);
  }
}

func FxController2Effect(string neweffect)
{
  if(!!SEqual(neweffect, "Controller2"))
    return(-1);
}

func FxController2Timer()
{
  CheckMovement();
}

/* Steuerung */

public func ControlLeft(object obj) {
  if((xdir ?? 0) > -2)
    S42MigrationAssign(xdir, "-=", 2);
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlRight(object obj) {
  if((xdir ?? 0) < 2)
    S42MigrationAssign(xdir, "+=", 2);
    
  SetPlrView(GetController(obj), clamp);
}

public func ControlUp(object obj) {
  clamp->Up();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlDown(object obj) {
  clamp->Down();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlThrow(object obj) {
  clamp->Grab();
  
  SetPlrView(GetController(obj), clamp);
}

public func ControlDig(object obj) {
  xdir = nil;
  
  SetPlrView(GetController(obj), clamp);
}

public func CheckMovement() {
  // Außerhalb der Reichweite
  if(NotInRange()) { Adjust(); xdir=nil; }
  // Der Greifer stößt irgendwo gegen
  if(clamp)
    if(clamp->GBackSolid((xdir ?? 0) * 3, nil))
      xdir = nil;
  // Bei Bewegung Geräusch
  if(!Equal(xdir, nil)) Sound("Elevator", nil, this(), 50, nil, 1);
  else Sound("Elevator", nil, this(), 50, nil, -1);

  SetPosition((GetX() ?? 0) + (xdir ?? 0), GetY());
}

public func CheckY() {
  // Überprüft, ob die Schiene weg ist
  if(!crane) return(RemoveObject());
  if(!Equal(GetY(), (GetY(crane) ?? 0)+10)) Adjust();
  // Die Schiene sollte eigentlich nicht bewegt werden :0
}

public func Stop() {
  xdir = nil;
  Sound("Elevator", nil, this(), 100, nil, -1);
  clamp->LocalN("ydir") = nil;
}

// Maus macht mich zZ kaputt
protected func ControlCommand() { return(1); }

/* Serialisierung */

public func NoSerialize() { return !nil; }

public func Reconfigure(int dx, int dy, object load)
{
	dx ??= 0;
	dy ??= 0;
	SetPosition((GetX() ?? 0)+(dx ?? 0), GetY());
	clamp->SetPosition(GetX(clamp), (GetY(clamp) ?? 0)+(dy ?? 0));
	if (load) {
		clamp->LocalN("load") = load;
		clamp->SetAction("Load");
		clamp->AddEffect("Loading", load, 99, 1, clamp, nil, (GetX(load) ?? 0)-(GetX(clamp) ?? 0), (GetY(load) ?? 0)-(GetY(clamp) ?? 0));
	}
}
