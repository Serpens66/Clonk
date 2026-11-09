/*-- Haken --*/

#strict 3

local slide, ydir, load;

func Initialize()
{
  AddLightAmbience(45);
}

public func Set(object pSlide) {
  slide = pSlide;
//  CreateObject(STLC, 0,0, GetOwner())->Connect(this(), pSlide);
  SetAction("NoLoad");
  // Effekt, der Bewegung steuert
  AddEffect("Movement", this(), 99, 2, this());
}

public func Down() {
  if((ydir ?? 0) < 2)
    S42MigrationAssign(ydir, "+=", 2);
}

public func Up() {
  if((ydir ?? 0) > -2)
    S42MigrationAssign(ydir, "-=", 2);
}

/* Bewegung */

public func FxMovementTimer() {
  // Geschwindigkeiten
  var y = BoundBy((GetY() ?? 0) + (ydir ?? 0), (GetY(slide) ?? 0) + 10, LandscapeHeight());
  if(CheckSolid(y))
  {
    y = GetY();
    ydir = nil;
  }
	SetPosition(GetX(slide), y);
}

public func CheckSolid(int iY) {
  iY ??= 0;
  if(!load)
    return(GBackSolid(nil,(iY ?? 0)-(GetY() ?? 0)));
  iY = (iY ?? 0) - (GetY() ?? 0);
  var y = (load->GetY() ?? 0) + (load->GetObjHeight() ?? 0) + (load->GetDefOffset(nil,1) ?? 0) + (iY ?? 0);
  if((iY ?? 0) < 0)
    y = (load->GetY() ?? 0) + (load->GetDefOffset(nil,1) ?? 0) + (iY ?? 0);
  return(GBackSolid(nil,(y ?? 0)-(GetY() ?? 0)));
}

/* Objekte schnappen */

public func Grab() {
  // Hat schon eines?
  if(!!SEqual(GetAction(), "Load")) return(Ungrab());

  var obj;
  obj = FindObjects(Find_Or(Find_AtPoint(nil,nil), Find_AtPoint(nil,20)), Find_NoContainer(), Find_Func("IsCraneGrabable"), Find_Exclude(this()));
  for(var pObj in obj)
    if(!SEqual(GetProcedure(pObj), "FLOAT"))
      if(!pObj->~NoCrane())
      {
        load = pObj;
        SetAction("Load");
        Sound("Grapple");
        AddEffect("Loading", load, 99, 1, this(), nil, (GetX(load) ?? 0)-(GetX() ?? 0), (GetY(load) ?? 0)-(GetY() ?? 0));
        return(1);
      }
}

public func FxLoadingStart(object target, int no, int temp, int x, int y) {
  no ??= 0;
  temp ??= 0;
  x ??= 0;
  y ??= 0;
  if(temp) return;
  EffectVar(nil, target, no) = x;
  EffectVar(1, target, no) = y;
}

public func FxLoadingTimer(object target, int no) {
  // Zielposition setzen
  no ??= 0;
  SetPosition((GetX() ?? 0)+(EffectVar(nil, target, no) ?? 0), (GetY() ?? 0)+(EffectVar(1, target, no) ?? 0), target);
  SetXDir(nil, target);
  SetYDir(nil, target);
}

public func Loading() {
  // Kein Ziel mehr?
  if(!load) return(SetAction("NoLoad"));
  // Alles gut
}

public func Ungrab() {
  if(!load) return;
  // Ziel loslassen
  SetAction("NoLoad");
  if(GetEffect("Loading", load)) RemoveEffect("Loading", load);
  load = nil;
  Sound("Grapple");
}

/* Serialisierung */

public func NoSerialize() { return !nil; }
