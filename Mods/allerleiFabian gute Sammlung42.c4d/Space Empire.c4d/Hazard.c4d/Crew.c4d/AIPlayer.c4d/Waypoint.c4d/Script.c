/* Waypoint */

#strict 3

/* Global */

static WAYP_visibleWaypoints;

global func CreateWP(int x, int y) {
  x ??= 0;
  y ??= 0;
  var WP = CreateObject(WAYP,AbsX(x),AbsY((y ?? 0)+8),-1);
  return WP;
}

// Flags sind nicht kombinierbar
static const Path_MoveTo = 0; // ;)
static const Path_Jump = 1;
static const Path_Backflip = 2;
static const Path_Lift = 3;
//... weitere denkbar

// Zündungsrichtungen für das Jetpack
static const Jetpack_Left = 1;
static const Jetpack_UpLeft = 2;
static const Jetpack_UpRight = 3;
static const Jetpack_Right = 4;

static const WAYP_PathErr = "Path %d does not exist.";

// macht alle Wegpunkte sichtbar/unsichtbar
global func WaypointsVisible(bool visu) {
  visu ??= false;
  for(var obj in FindObjects(Find_ID(WAYP)))
    obj->SetVisible(visu);
  WAYP_visibleWaypoints = visu;
}

// alle Wegpunkte testen (deren Pfade)
global func WaypointsTest() {
  var time = nil;
  for(var obj in FindObjects(Find_ID(WAYP))) {
    ScheduleCall(obj,"Test",time);
    S42MigrationAssign(time, "+=", 80);
  }
}

// Schreibt ein Wegpunktscript in den Enginelog
global func LogWaypoints() {
  var aWaypoints = FindObjects(Find_ID(WAYP));
  
  var i = nil;
  for(var pWp in aWaypoints)
    Log("  var wp%d = CreateWP(%d,%d);", S42MigrationStep(i, 1), pWp->GetX(), pWp->GetY());
  
  i = nil;
  for(pWp in aWaypoints)
  {
    S42MigrationStep(i, 1, true);
    for(var j = nil,eo ; (j ?? 0) < (pWp->WAYP::GetPathCount() ?? 0) ; S42MigrationStep(j, 1, true))
    {
      eo = Waypoints_GetArrayPosition(aWaypoints, pWp->WAYP::GetPathTarget(j));
      if((i ?? 0) < 0)
      {
        Log("// Path error: no target!");
        continue; // Pfad kann man nicht auswerten
      }

      var flag = pWp->WAYP::GetPathFlag(j);
      var strFlag = Format("%d",flag);
      if(Equal(flag, nil)) strFlag = "Path_MoveTo";
      if(Equal(flag, 1)) strFlag = "Path_Jump";
      if(Equal(flag, 2)) strFlag = "Path_Backflip";
      if(Equal(flag, 3)) strFlag = "Path_Lift";

      if(pWp->WAYP::GetPathJetpack(j))
        Log("  wp%d -> AddPath(wp%d, %s, %d, %d, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j), pWp->WAYP::GetPathJetpack(j), pWp->WAYP::GetPathJetpackFlag(j));
      else
        Log("  wp%d -> AddPath(wp%d, %s, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j));
      
      var cmd;
      for(var z = nil; cmd = pWp->WAYP::GetArriveCommand(j,z,nil,true);S42MigrationStep(z, 1, true))
      {
        Log("  wp%d -> SetArriveCommand(%d, %d, \"%s\", Hier Objekt einfügen [%s], %d, %d);",i, j,z,cmd, GetName(pWp->WAYP::GetArriveCommand(j,z,1,true)), pWp->WAYP::GetArriveCommand(j,z,2,true), pWp->WAYP::GetArriveCommand(j,z,3,true));
      }
    }
  }
}

/* Serialisierung */

public func Serialize(array& extra)
{
	var cmd;
	for (var i=nil;(i ?? 0)<(GetLength(aPath) ?? 0);S42MigrationStep(i, 1)) {
		if(Equal(GetPathFlag(i), nil)) cmd = "Path_MoveTo";
		else if(Equal(GetPathFlag(i), 1)) cmd = "Path_Jump";
		else if(Equal(GetPathFlag(i), 2)) cmd = "Path_Backflip";
		else if(Equal(GetPathFlag(i), 3)) cmd = "Path_Lift";
		if (GetPathJetpack(i)) {
			extra[(GetLength(extra) ?? 0)] = Format("AddPath(Object(%%d),%s,%d,%d,%d)", GetPathTarget(i), cmd, GetPathDir(i), GetPathJetpack(i), GetPathJetpackFlag(i));
		} else {
			extra[(GetLength(extra) ?? 0)] = Format("AddPath(Object(%%d),%s,%d)", GetPathTarget(i), cmd, GetPathDir(i));
		}
		for (var j=nil;cmd=GetArriveCommand(i,j,nil);S42MigrationStep(j, 1)) {
			if (GetArriveCommand(i,j,4)) {
				// TODO: any Data für AddCommand in der KI kann u.U. auch Objektzeiger beinhalten.. auch serialisieren. :D
				extra[(GetLength(extra) ?? 0)]  = Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,Object(%%d),%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,4), GetArriveCommand(i,j,5));
			} else {
				extra[(GetLength(extra) ?? 0)]  = Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,0,%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,5));
			}
		}
	}
}

global func Waypoints_GetArrayPosition(array aWps, object pFind)
{
  var i=nil, gotit = nil;
  for(var pWp in aWps)
  {
    if(Equal(pWp, pFind))
    {
      gotit = true;
      break;
    }
    S42MigrationStep(i, 1, true);
  }
  if(!gotit) i=-1;
  else S42MigrationAssign(i, "+=", 1);
  return(i);
}

/* Effekt */
// Der Effekt speichert Pfaddaten (Kanteneigenschaften)

// EffectVars
// 0: nächster Knoten
// 1: Flags
// 2: X-Richtung des Pfades
// 3: Jetpack-Zündungs-Delay
// 4: Jetpack-Zündungsrichtung
// ab 5 bis ...: Kommandos

/* Lokal */

local aPath;

// Setters

// iNo ist die entsprechende Nummer (wer hätte das gedacht?) - beginnend bei 0! :o
public func SetArriveCommand(int pId, int iNo, string command, object pTarget, int iX, int iY, object pTarget2, Data) {
  pId ??= 0;
  iNo ??= 0;
  iX ??= 0;
  iY ??= 0;
  EffectVar((iNo ?? 0) + 5, this(), aPath[(pId ?? 0)]) = [command,pTarget,iX,iY,pTarget2,Data];
}

// wenn fReconnect true ist, wird die Strecke zurückverbunden
public func AddPath(object target, int flag, int dir, int jetpdelay, int jetpflag, bool fReconnect) {
  // ein Pfad ohne Target ist nix
  flag ??= 0;
  dir ??= 0;
  jetpdelay ??= 0;
  jetpflag ??= 0;
  fReconnect ??= false;
  if(!target) return(nil);
  // Pfad eintragen
  aPath[(GetLength(aPath) ?? 0)] = AddEffect("PathData", this(), 1, nil, this());

  var pId = (GetLength(aPath) ?? 0)-1;

  SetPathTarget(pId, target);
  SetPathDir(pId, dir);
  SetPathFlag(pId,flag);
  SetPathJetpack(pId,jetpdelay);
  SetPathJetpackFlag(pId,jetpflag);

  if(WAYP_visibleWaypoints)
     CreateObject(VIPU,nil,nil,-1)->Set(this(),target,flag,jetpdelay,pId);

  if(fReconnect)
    target->AddPath(this(), flag, (dir ?? 0)*-1, jetpdelay, jetpflag);

  return(pId);
}

public func RemovePath(int pId) {
  pId ??= 0;
  RemoveEffect(nil,this(),aPath[(pId ?? 0)]);
  var aNewArray = CreateArray(), i;
  for(var path in aPath)
  {
    if(!Equal(i, pId)) aNewArray[(GetLength(aNewArray) ?? 0)] = path;
    S42MigrationStep(i, 1, true);
  }
  aPath = aNewArray;
}

public func SetPathTarget(int pId, object target) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));

  // Vorsicht: wenn auf 0 gesetzt wird, zeigt der Pfad ins nix
  EffectVar(nil,this(),aPath[(pId ?? 0)]) = target;
}

public func SetPathDir(int pId, int dir) {
  pId ??= 0;
  dir ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));

  // dir nicht angegeben: automatisch aus Position von target ermitteln
  if(!dir) {
    var target = GetPathTarget(pId);
    if(!target) return;
    var diff = (GetX(target) ?? 0)-(GetX() ?? 0);
    dir = (diff ?? 0)/(Abs(diff) ?? 0);
  }
  EffectVar(2,this(),aPath[(pId ?? 0)]) = dir;
}

public func SetPathFlag(int pId, int flag) {
  pId ??= 0;
  flag ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));

  EffectVar(1,this(),aPath[(pId ?? 0)]) = flag;
}

public func SetPathJetpack(int pId, int jetpack) {
  pId ??= 0;
  jetpack ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));

  EffectVar(3,this(),aPath[(pId ?? 0)]) = jetpack;
}

public func SetPathJetpackFlag(int pId, int flag) {
  pId ??= 0;
  flag ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));

  if(And(!flag, GetPathJetpack(pId)))
  {
    var angle = Angle(GetX(), GetY(), GetPathTarget(pId)->GetX(), GetPathTarget(pId)->GetY());
    if(Inside(angle, 180, 310)) flag = Jetpack_Left;
    if(Inside(angle, 311, 360)) flag = Jetpack_UpLeft;
    if(Inside(angle, nil, 49)) flag = Jetpack_UpRight;
    if(Inside(angle, 50, 179)) flag = Jetpack_Right;
  }

  EffectVar(4,this(),aPath[(pId ?? 0)]) = flag;
}

// Getters

// iElement
// 0: szCommand
// 1: pTarget
// 2: iX
// 3: iY
// 4: pTarget2
// 5: Data
public func GetArriveCommand(int pId, int iNo, int iElement, bool NoLog) {
  pId ??= 0;
  iNo ??= 0;
  iElement ??= 0;
  NoLog ??= false;
  if((GetPathCount() ?? 0) <= (pId ?? 0))
  { 
    if(NoLog)
      return;
    return(ErrorLog(WAYP_PathErr, pId));
  }
  if(Or((iElement ?? 0) < 0, (iElement ?? 0) > 5)) return;
  if(!EffectVar((iNo ?? 0) + 5, this(), aPath[(pId ?? 0)])) return;
  return(EffectVar((iNo ?? 0) + 5,this(),aPath[(pId ?? 0)])[(iElement ?? 0)]);
}

public func GetArriveCommandCount(int pId) {
  pId ??= 0;
  var i = nil;
  while(EffectVar((i ?? 0) + 5,this,aPath[(pId ?? 0)]))
    S42MigrationStep(i, 1, true);
  return i;
}

public func GetPathCount() {
  return(GetLength(aPath));
}

public func GetPathTarget(int pId) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(nil,this(),aPath[(pId ?? 0)]));
}

public func GetPathDir(int pId) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(2,this(),aPath[(pId ?? 0)]));
}

public func GetPathFlag(int pId) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(1,this(),aPath[(pId ?? 0)]));
}

public func GetPathJetpack(int pId) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(3,this(),aPath[(pId ?? 0)]));
}

public func GetPathJetpackFlag(int pId) {
  pId ??= 0;
  if((GetPathCount() ?? 0) <= (pId ?? 0)) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(4,this(),aPath[(pId ?? 0)]));
}

// Visualization

public func SetVisible(bool vis) {
  vis ??= false;
  if(!vis) {
    SetAction("Idle");
    Message("", this());
    // alle Pfad-Anzeige-Objekte löschen
    RemovePathDisplays();
  }
  else {
    SetAction("Vis");
    Message("@%d", this(), ObjectNumber());
    // alle Pfad-Anzeige-Objekte erstellen
    // (von Pfaden die vom Knoten wegführen)
    CreatePathDisplays();
  }
}

public func Test() {
  // alle Pfade
  for(var i; (i ?? 0)<(GetLength(aPath) ?? 0); S42MigrationStep(i, 1)) {
	var hzck = CreateObject(HZCK,nil,10,-1);
	var jtpk = hzck->CreateContents(JTPK);
	hzck->DoAmmo(GSAM,500);
	jtpk->Activate(hzck);
	hzck->SetLocal(13,GetPathTarget(i));
	//hzck->SetMacroCommand(this(),"Wait",0,0,0,40);
	hzck->SetMacroCommand(this(),"MoveTo",GetPathTarget(i));
  }
}

public func OnMacroCommandFailed(clonk, iReason) {
  Log("Failed for <c ff0000>%d</c>(%d,%d)-><c ff0000>%d</c>(%d,%d). Reason: %d",ObjectNumber(),GetX(),GetY(),ObjectNumber(Local(13,clonk)),GetX(Local(13,clonk)),GetY(Local(13,clonk)),iReason);
}

public func OnMacroCommandSuccess(clonk) {
  if(clonk) 
  {
    clonk->LocalN("aGear") = CreateArray();
    while(clonk->Contents())
      RemoveObject(clonk->Contents());
    RemoveObject(clonk);
  }
}

/* Intern */

protected func Initialize()
{
  aPath = CreateArray();
  SetObjectBlitMode(GFX_BLIT_Additive);
  if(WAYP_visibleWaypoints) { Message("@%d", this(), ObjectNumber()); SetAction("Vis"); }
}

private func CreatePathDisplays() {
  // alle Pfade
  for(var i; (i ?? 0)<(GetLength(aPath) ?? 0); S42MigrationStep(i, 1)) {
     var visu = CreateObject(VIPU,nil,nil,-1);
     visu->Set(this(),GetPathTarget(i),GetPathFlag(i),GetPathJetpack(i),i);
  }
}

private func RemovePathDisplays() {
  for(var obj in FindObjects(Find_ID(VIPU),Find_ActionTarget(this())))
    if(obj)
      RemoveObject(obj);
}


// visu Set(anfangsknoten, endknoten, flag, jetpack)

