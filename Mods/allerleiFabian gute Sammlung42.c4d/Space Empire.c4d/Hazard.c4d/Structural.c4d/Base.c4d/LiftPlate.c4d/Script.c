/*-- Groﬂe Schwebeplatte --*/

#strict 3

/* Lokale Variablen */

local Mode; // 0: vertikal 1: horizontal 2: beides
local UpperY, LowerY, WaitComDir, WaitY;
local rLight, lLight;
local Limited;

// Begrenzen
public func SetLimits(int iUY, int iLY)
{
	iUY ??= 0;
	iLY ??= 0;
	if(iUY) UpperY = iUY;
	if(iLY) LowerY = iLY;
	Limited = true;
}

public func SetAutoLimits() {
	Limited = true;
	UpdateTransferZone();
}

/* Steuerung */

public func ControlCommand (string command, object target, int x, int y) {
  // Bewegungskommando vertikal
  x ??= 0;
  y ??= 0;
  if (!!SEqual(command, "MoveTo")) {
    if (Vertical())
    if (Inside((x ?? 0) - (GetX() ?? 0),-16,+16)) {
      if ((y ?? 0) < (GetY() ?? 0))
        return(SetComDir(COMD_Up));
      if ((y ?? 0) > (GetY() ?? 0))
        return(SetComDir(COMD_Down));
    }
  // Bewegungskommando horizontal
    if (Horizontal())
    if (Inside((x ?? 0) - (GetY() ?? 0),-16,+16)) {
      if ((x ?? 0) < (GetX() ?? 0))
        return(SetComDir(COMD_Left));
      if ((x ?? 0) > (GetX() ?? 0))
        return(SetComDir(COMD_Right));
    }
  }
  // Andere Kommandos nicht auswerten
}

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  // Remove horizontal Comdir component
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(!Horizontal())
  {
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
  }

  // Remove vertical ComDir component
  if(!Vertical())
  {
    if(ComDirLike(comdir, COMD_Up)) comdir = ComDirTransform(comdir, COMD_Down);
    if(ComDirLike(comdir, COMD_Down)) comdir = ComDirTransform(comdir, COMD_Up);
  }

  if(!Equal(comdir, GetComDir()))
    Sound("Click");

  // Stop without input
  if(Equal(comdir, COMD_Stop)) return(Stop());

  // Clear X/Y-Dir if necessary
  if(And(!ComDirLike(comdir, COMD_Right), !ComDirLike(comdir, COMD_Left)))
    SetXDir(nil);
  if(And(!ComDirLike(comdir, COMD_Up), !ComDirLike(comdir, COMD_Down)))
    SetYDir(nil);

  // Ab geth.
  SetComDir(comdir);
  SetAction("Travel");
  return(1);
}

public func ControlUp (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    if (!Vertical()) return(Stop());
    SetComDir(COMD_Up);
    SetXDir(nil);
    SetAction("Travel");
    return(1);
  }
}

public func ControlDownSingle (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    if (!Vertical()) return(Stop());
    SetComDir(COMD_Down);
    SetXDir(nil);
    SetAction("Travel");
    return(1);
  }
}

public func ControlLeft (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    if (!Horizontal()) return(Stop());
    SetComDir(COMD_Left);
    SetYDir(nil);
    SetAction("Travel");
    return(1);
  }
  else
  {
    AddCommand(caller, "Call", this(), nil,nil,nil,nil, "ContMoveLeft");
    AddCommand(caller, "UnGrab", this());
    return(1);
  }
}

public func ControlRight (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    if (!Horizontal()) return(Stop());
    SetComDir(COMD_Right);
    SetYDir(nil);
    SetAction("Travel");
    return(1);
  }
  else
  {
    AddCommand(caller, "Call", this(), nil,nil,nil,nil, "ContMoveRight");
    AddCommand(caller, "UnGrab", this());
    return(1);
  }
}

public func ContMoveLeft(object caller) { caller->SetComDir(COMD_Left); }
public func ContMoveRight(object caller) { caller->SetComDir(COMD_Right); }

/* Kontakt */

protected func ContactTop () {
  if (!Vertical()) return(Stop());
  Wait(COMD_Down);
  return(1);
}

protected func ContactBottom () {
  UnstuckClonks();

  if (!Vertical()) return(Stop());
  if (And(Horizontal(), Vertical())) return(Stop());
  Wait(COMD_Up);
  return(1);
}

protected func ContactLeft () {
  if (And(Horizontal(), Vertical())) return(Stop());
  if (!Horizontal()) return(Stop());
  Wait(COMD_Right);
  return(1);
}

protected func ContactRight () {
  if (And(Horizontal(), Vertical())) return(Stop());
  if (!Horizontal()) return(Stop());
  Wait(COMD_Left);
  return(1);
}

/* Konfiguration */

public func EnableHorizontal () {
  if (Equal(nil, Mode)) Mode = 2; else Mode = 1;
  UpdateTransferZone();
}

public func DisableHorizontal () {
  Mode = nil;
  UpdateTransferZone();
}

public func EnableVertical () {
  if (Equal(1, Mode)) Mode = 2; else Mode = nil;
  UpdateTransferZone();
}

public func DisableVertical () {
  Mode = 1;
  UpdateTransferZone();
}

private func Horizontal () {
  return(!Equal(Mode, nil));
}

private func Vertical () {
  return(!Equal(Mode, 1));
}

public func SetYTop (int iNewTop) {
  iNewTop ??= 0;
  return(UpperY = iNewTop);
}
  
private func YTop () {
  return(UpperY);
}

public func SetYBottom (int iNewBottom) {
  iNewBottom ??= 0;
  return(LowerY = iNewBottom);
}
  
private func YBottom () {
  return(LowerY);
}

/* Kommandos */

private func Stop () {
  SetAction("Travel");
  SetComDir(COMD_Stop);
  SetSpeed();
  return(1);
}

private func Wait (int iComDir) {
  iComDir ??= 0;
  Stop();
  // Aktion Wait ruft als EndCall ResumeTravel auf
  SetAction("Wait"); 

  WaitComDir = iComDir;
  return(1);
}

private func ResumeTravel () { // EndCall von Aktion Wait
  SetAction("Travel");
  SetComDir(WaitComDir);
  return(1);
}

private func Automatic () { // EndCall von Aktion Travel
  // Keine Automatik, wenn ein Clonk mitf‰hrt
  if (Passenger()) return(nil);
  // Zu einem wartenden Clonk hinfahren
  if (SpeedToVertical()) return(1);
  // Eigenst‰ndige Bewegung beginnen
  if (SelfTravel()) return(1);
  return(nil);
}

private func SelfTravel () {
  // Nur wenn angehalten
  if (!Equal(GetComDir(), COMD_Stop)) return(nil);
  // Eigenst‰ndige Bewegung je nach zugelassener Richtung beginnen
  if (Equal(1, Mode)) SetComDir(COMD_Left);
  if (Equal(nil, Mode)) SetComDir(COMD_Up);
  if (Equal(2, Mode)) SetComDir(COMD_Down);
  return(1);
}

private func SpeedToVertical () {
  if (!Equal(Mode, nil)) return(nil);
  
  var WaitClonk;
  var upperdistance = AbsY((UpperY ?? 0)-20);
  var lowerdistance = AbsY((LowerY ?? 0)+20);

  for(WaitClonk in FindObjects(	Find_InRect(-50,upperdistance,100,-(upperdistance ?? 0)+(lowerdistance ?? 0)),
								Find_OCF(OCF_CrewMember),
								Find_NoContainer())) {

	if (!Equal(GetComDir(WaitClonk), COMD_Stop)) continue;

	SetAction("SpeedToY");
	WaitY = Max(GetY(WaitClonk), (UpperY ?? 0)-10);
	
	return(1);
  }
  
  return(nil);
}

/* Aktion */

private func CheckYTop () { // PhaseCall von Travel
  if ((GetY() ?? 0) < (YTop() ?? 0)-10) {
	SetPosition(GetX(), (YTop() ?? 0)-10);
	ContactTop();
  }
  return(1);
}

private func SpeedToY () { // StartCall von SpeedToY
  SetYDir(BoundBy((WaitY ?? 0) - (GetY() ?? 0), -50, 50));
  if (Inside((WaitY ?? 0)-(GetY() ?? 0),-4,+2)) Stop();
  return(1);
}

private func SpeedToX () { // StartCall von SpeedToX
  SetYDir(nil);
  SetXDir(BoundBy((WaitY ?? 0)-(GetX() ?? 0),-50,+50));
  if (Inside((WaitY ?? 0) - (GetX() ?? 0),-10,+10)) Stop();
  return(1);
}

private func UnstuckClonks () { // Sucht Clonks in der SolidMask und setzt sie auf die Platte
  var pClonk;
  while(pClonk = FindObject(nil,-25,-2,50,20, nil, nil,nil, NoContainer(), pClonk))
    if(Stuck(pClonk))
      SetPosition(GetX(pClonk), GetY(), pClonk);
}

/* Status */

private func Passenger () {
  return( FindObject(nil, -24,-13,48,16, OCF_CrewMember,nil,nil,NoContainer()) );
}

public func IsLift() { return(1); }

/* Initialisierung */

protected func Initialize()
{
//  AddLightAmbience(100, this());
  lLight = AddLightCone(1000,RGBa(255,255,220,20),this());
  rLight = AddLightCone(1000,RGBa(255,255,220,20),this());

  lLight->ChangeSizeXY(700, 500);
  rLight->ChangeSizeXY(700, 500);
  lLight->ChangeOffset(-11,-1);
  rLight->ChangeOffset(+11,-1);
  lLight->ChangeR(-130);
  rLight->ChangeR(+130);

  AddEffect("LightEffects", this(), 1, 5, this());
  SetAction("Travel");
  UpdateTransferZone();
  return(1);
}

public func FxLightEffectsTimer(target, number, time)
{
  // Funken! :D
  CreateParticle("PSpark", RandomX(-8,8), 15, RandomX(-1,1), RandomX(2,5), 150+(Random(70) ?? 0), RGB(50,50,25), this(), true);
  // Lichter
/*  var r = LocalN("iR", rLight);
  // Nach unten drehen
  if(GetComDir() == COMD_Down())
    {
    if(r < 120)
      r+=5;
    else
      return();
    }
  // Nach oben drehen
  if(GetComDir() == COMD_Up())
    {
    if(r > 10)
      r-=5;
    else
      return();
    }
  // mittig ausrichten
  if(GetComDir() == COMD_Left() && GetComDir() == COMD_Right() && GetComDir() == COMD_Stop())
    {
    if(r > 90)
      r-=5;
    else
      if(r < 90)
        r+=5;
      else
        return();
    }
  rLight->ChangeR(+r);
  lLight->ChangeR(-r);*/
}

/* Transfer */

protected func UpdateTransferZone () {
  // Nur vertikale Schwebeplatten
  if (Mode) return(SetTransferZone());
  // Obergrenze ermitteln
  var y1, y2;
  while(And((GetY() ?? 0) + (y1 ?? 0) > 5, !GBackSolid(nil, y1)))
    S42MigrationAssign(y1, "-=", 5);
  // Untergrenze ermitteln
  while(And((GetY() ?? 0) + (y2 ?? 0) + 15 < (LandscapeHeight() ?? 0)-5, !GBackSolid(nil, (y2 ?? 0)+15)))
    S42MigrationAssign(y2, "+=", 5);
  // Transferzone setzen
  SetTransferZone(-23,y1,48,(y2 ?? 0)-(y1 ?? 0) +10);
  // Ober- und Untergrenze speichern
  if(!Limited) return;
  UpperY = (GetY() ?? 0) + (y1 ?? 0);
  LowerY = (GetY() ?? 0) + (y2 ?? 0);
  return(1);
}

protected func ControlTransfer (object pObj, int iX, int iY) {
  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  iX ??= 0;
  iY ??= 0;
  if(Inside((GetY(pObj) ?? 0) - (LowerY ?? 0), -10, +10))
    if(Inside((iY ?? 0) - (LowerY ?? 0), -10, +10))
      return;

  // Zielposition
  // Maximale Obergrenze
  iY = Max(iY, UpperY);

  // Fahrstuhl mit Clonk an Zielposition: anhalten, erledigt
  if(Inside((iY ?? 0) - (GetY() ?? 0), -5, +5))
    if(Passenger())
      return(S42MigrationFirst(nil,Stop()));

  // Fahrstuhl noch nicht beim Clonk: warten
  if(!Inside((GetY() ?? 0) - (GetY(pObj) ?? 0), -20, +20))
    return(1);

  // Fahrstuhl nur in der Mitte anfassen
  if(!Inside((GetX() ?? 0) - (GetX(pObj) ?? 0), -15, +15))
    return(AddCommand(pObj,"MoveTo",this(),nil,nil,nil,15));

  // Fahrstuhl anfassen
  if(Or(!SEqual(GetAction(pObj), "Push"), !Equal(GetActionTarget(nil, pObj), this())))
    return(AddCommand(pObj,"Grab",this()));

  // Fahrstuhl zur Zielposition steuern
  if((iY ?? 0) < (GetY() ?? 0))
    return(SetComDir(COMD_Up));
  return(SetComDir(COMD_Down));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetYTop(%d)", UpperY);
	if (Equal(Mode, 1))
		extra[(GetLength(extra) ?? 0)] = "DisableVertical()";
	if (Equal(Mode, 2))
		extra[(GetLength(extra) ?? 0)] = "EnableHorizontal()";
}
