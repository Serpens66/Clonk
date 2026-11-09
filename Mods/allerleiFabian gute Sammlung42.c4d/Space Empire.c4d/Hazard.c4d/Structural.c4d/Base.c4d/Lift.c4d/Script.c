/*-- Lift --*/

#strict 3

// Lokale Variable: Feuerfarbe :')
local UpperY, LowerY, WaitComDir, WaitY;
local rgb;

protected func Initialize()
{
  AddEffect("Sparks", this(), 1, 4, this());
  rgb=RGB(40+(Random(120) ?? 0), 40+(Random(120) ?? 0), 40+(Random(120) ?? 0));
  SetAction("Travel");
  UpdateTransferZone();
  return(1);
}

public func FxSparksTimer(target, number, time)
{
	// Funken! :D
	CreateParticle("PSpark", nil, -20, RandomX(-1,1), (RandomX(-1,1) ?? 0)-(GetYDir() ?? 0)/4, 50+(Random(80) ?? 0), rgb, this(), true);
	CreateParticle("PSpark", nil, +20, RandomX(-1,1), (RandomX(-1,1) ?? 0)-(GetYDir() ?? 0)/4, 50+(Random(80) ?? 0), rgb, this(), true);
}

func FlareUp()
{
	for(var i=3; (i ?? 0)>0; S42MigrationStep(i, -1))
		CreateParticle("PSpark", nil, -20, RandomX(-4,4), RandomX(-2,-4), 130+(Random(80) ?? 0), rgb, this(), true);
}

func FlareDown()
{
	for(var i=3; (i ?? 0)>0; S42MigrationStep(i, -1))
		CreateParticle("PSpark", nil, +20, RandomX(-4,4), RandomX(2,4), 130+(Random(80) ?? 0), rgb, this(), true);
}

/* Steuerung */

public func ControlCommand (string command, object target, int x, int y) {
  // Bewegungskommando vertikal
  x ??= 0;
  y ??= 0;
  if (!!SEqual(command, "MoveTo")) {
    if (Inside((x ?? 0) - (GetX() ?? 0),-16,+16)) {
      if ((y ?? 0) < (GetY() ?? 0))
        return(SetComDir(COMD_Up));
      if ((y ?? 0) > (GetY() ?? 0))
        return(SetComDir(COMD_Down));
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
  if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
  if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);

  if(!Equal(comdir, GetComDir()))
    Sound("Click");

  // Clear X/Y-Dir
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
    SetComDir(COMD_Up);
    SetXDir(nil);
    SetAction("Travel");
  }
  return(1);
}

public func ControlDownSingle (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Down);
    SetXDir(nil);
    SetAction("Travel");
  }
  return(1);
}

public func ControlLeft (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Stop);
    SetYDir(nil);
    SetAction("Travel");
  }
  return(1);
}

public func ControlRight (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Stop);
    SetYDir(nil);
    SetAction("Travel");
  }
  return(1);
}

// Without HorizontalFix=1, these should prevent that the lift moves horizontally,
// but they do not. They still move after having pressed double left/right and
// waiting a few frames.
/*public func ControlLeftSingle (object caller) {
  [|Method=None]
  return(1);
}

public func ControlLeftDouble (object caller) {
  [|Method=None]
  return(1);
}

public func ControlRightSingle (object caller) {
  [|Method=None]
  return(1);
}

public func ControlRightDouble (object caller) {
  [|Method=None]
  return(1);
}*/

public func ControlLeftReleased (object caller) {
  [|Method=None]
  return(1);
}

/* Kontakt */

protected func ContactTop () {
  Wait(COMD_Down);
  return(1);
}

protected func ContactBottom () {
  UnstuckClonks();
  Wait(COMD_Up);
  return(1);
}

protected func ContactLeft () {
  Stop();
  return(1);
}

protected func ContactRight () {
  Stop();
  return(1);
}

public func SetYTop (int iNewTop) {
  iNewTop ??= 0;
  return(UpperY = iNewTop);
}
  
private func YTop () {
  return(UpperY);
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
  // Keine Automatik, wenn ein Clonk mitfährt
  if (Passenger()) return(nil);
  // Zu einem wartenden Clonk hinfahren
  if (SpeedToVertical()) return(1);
  // Eigenständige Bewegung beginnen
  if (SelfTravel()) return(1);
  return(nil);
}

private func SelfTravel () {
  // Nur wenn angehalten
  if (!Equal(GetComDir(), COMD_Stop)) return(nil);
  // Eigenständige Bewegung je nach zugelassener Richtung beginnen
  SetComDir(COMD_Up);
  return(1);
}

private func SpeedToVertical () {
  var WaitClonk;
  if(!(WaitClonk=FindObject(nil, -50,-1000,100,2000, OCF_CrewMember,nil,nil,NoContainer()))) return;
  if(Inside(WaitClonk->GetX(), (GetX() ?? 0) - 12, (GetX() ?? 0) + 12))
    if(Inside(WaitClonk->GetY(), (GetY() ?? 0) - 40, (GetY() ?? 0) - 20))
      return;
  if (!Equal(GetComDir(WaitClonk), COMD_Stop)) return;
  if(!Inside(GetY(WaitClonk), (UpperY ?? 0)-50, (LowerY ?? 0)+50)) return;
  SetAction("SpeedToY");
  WaitY = Max(GetY(WaitClonk), (YTop() ?? 0)-10);
  return(1);
}

/* Aktion */

private func CheckYTop () { // PhaseCall von Travel
  if ((GetY() ?? 0) > (YTop() ?? 0)-10) return(nil);
  SetPosition(GetX(), (YTop() ?? 0)-10);
  ContactTop();
  return(1);
}

private func SpeedToY () { // StartCall von SpeedToY
  SetYDir(BoundBy((WaitY ?? 0) - (GetY() ?? 0), -50, 50));
  if (Inside((WaitY ?? 0)-(GetY() ?? 0),-4,+2)) return(Stop());
  if((GetYDir() ?? 0) < 0) FlareDown();
  else FlareUp();
  return(1);
}

private func UnstuckClonks () { // Sucht Clonks in der SolidMask und setzt sie auf die Platte
  var pClonk;
  while(pClonk = FindObject(nil,-25,nil,50,25, OCF_CrewMember, nil,nil, NoContainer(), pClonk))
    if(Stuck(pClonk))
      SetPosition(GetX(pClonk), GetY(), pClonk);
}

/* Status */

private func Passenger () {
  return( FindObject(nil, -24,-13,48,16, OCF_CrewMember,nil,nil,NoContainer()) );
}

/* Transfer */

protected func UpdateTransferZone () {
  // Obergrenze ermitteln
  var y1 = -20, y2 = +20;
  while(And((GetY() ?? 0) + (y1 ?? 0) > 5, !GBackSolid(nil, y1)))
    S42MigrationAssign(y1, "-=", 5);
  // Untergrenze ermitteln
  while(And((GetY() ?? 0) + (y2 ?? 0) < (LandscapeHeight() ?? 0)-5, !GBackSolid(nil, (y2 ?? 0)+15)))
    S42MigrationAssign(y2, "+=", 5);
  // Transferzone setzen
  SetTransferZone(-23,y1,48,(y2 ?? 0)-(y1 ?? 0) +10);
  // Ober- und Untergrenze speichern
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
  if(!Inside((GetY() ?? 0) - (GetY(pObj) ?? 0) -15, +15))
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
	extra[(GetLength(extra) ?? 0)] = Format("LocalN(\"rgb\")=%d", rgb);
}
