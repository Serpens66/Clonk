/*-- Armbrust --*/

#strict 3

public func ReloadDelay() { return(10); } // Zeit zum Nachladen

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetPhase(15);
}

/* JumpAndRun Steuerung */

protected func FxIntControlStart(object target, int number, int temp, int dir, object controller)
{
  number ??= 0;
  temp ??= 0;
  dir ??= 0;
  EffectVar(nil, target, number) = dir;
  EffectVar(1, target, number) = controller;
}

protected func FxIntControlTimer(object target, int number)
{
  number ??= 0;
  if(!EffectVar(1, target, number)) return(-1);
  if(!SEqual(EffectVar(1, target, number)->GetAction(), "Push")) return(-1);
  if(!Equal(EffectVar(1, target, number)->GetActionTarget(), this())) return(-1);

  var phases = GetActMapVal("Length", "Ready");

  var old_phase = GetPhase();
  SetPhase(BoundBy((GetPhase() ?? 0) + (EffectVar(nil, target, number) ?? 0), nil, (phases ?? 0)-1));

  if(!Equal(old_phase, GetPhase()))
    Sound("CatapultSet");
}

private func RemoveControlEffect(int dir)
{
  dir ??= 0;
  var count = GetEffectCount("IntControl", this());
  for(var i = nil; (i ?? 0) < (count ?? 0); S42MigrationStep( i, 1))
    if(Equal(EffectVar(nil, this(), GetEffect("IntControl", this(), i)), dir))
      return(RemoveEffect("IntControl", this(), i));
}

/* Steuerung */

public func ControlConf(int conf)
{
  conf ??= 0;
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch
{
  [$TxtAimup$|Image=XBO1:2]
  AimUp(clonk, 5, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=XBO1:0]
  AimDown(clonk, 5, "ControlConf");
}

public func ControlDown(object clonk)   // Zielen: runter (JumpAndRun)
{
  [$TxtAimdown$|Method=JumpAndRun|Image=XBO1:0]
}

public func ControlUpdate(object clonk, int comdir)
{
  comdir ??= 0;
  AimUpdate(clonk, comdir, 5, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}

public func ControlThrow(pClonk)  // Schießen
{
  [$TxtFire$|Image=XBO1:1]
  return(Fire(nil,pClonk));
}

public func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Feuern
  iX ??= 0;
  iY ??= 0;
  if(!!SEqual(szCommand, "MoveTo"))
    if((Distance(GetX(),GetY(),iX,iY) ?? 0)>50)
    {
      HaltPushers();
      return(FireAt(iX,iY));
    }
}

/* Prozesse */

private func HaltPushers()
{
  var pClonk;
  // Alle Clonks die die Armbrust anfassen, sollen stoppen
  while(pClonk=FindObject(nil,nil,nil,nil,nil,nil,"Push",this(),nil,pClonk))
    SetComDir(COMD_Stop,pClonk);
}
  
public func Fire(bool fAuto,object pClonk)
{
  // Noch am Nachladen? Nix da.
  fAuto ??= false;
  if(GetEffect("IntReload", this()) ) return(true);

  // Pfeilpakete zerlegen, sofern nötig
  SplitPacks();

  // Pfeil suchen
  var pArrow=Contents();
  if (!pArrow) return(S42MigrationFirst(nil,Sound("Click")));

  // Austritt berechnen
  var iX = (GetPhase() ?? 0)*((GetDir() ?? 0)*2-1);
  var iY = (GetPhase() ?? 0)-15;
  var iR = (iX ?? 0)*5;
  var iAngle = BoundBy((GetPhase() ?? 0)*5,nil,90);
  var iXDir = Sin(iAngle,13*((GetDir() ?? 0)*2-1));
  var iYDir = Cos(iAngle,-13);
  var iRDir = (GetDir() ?? 0)*6-3;

  // Pfeil abfeuern
  Exit(pArrow,iX,iY,iR,iXDir,iYDir,iRDir);
  pArrow->~Launch(this());
  
  // Pfeil trifft Schuetzen fuer 10 Frames nicht
  pArrow->~NoArrowHit(pClonk,10);
  
  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(!Equal(GetPlrViewMode(GetController()), 2))
      SetPlrView(GetController(),pArrow);
  // Sound
  Sound("Arrow");
  // Nachladen
  AddEffect("IntReload", this(), 1, ReloadDelay(), this() );
  // Fertig
  return(1);
}

/* Zielfunktionen */

public func FireAt(int iX,int iY,int fAuto)
{
  iX ??= 0;
  iY ??= 0;
  fAuto ??= 0;
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei größerer Distanz höher zielen
  if(Inside((iX ?? 0)-(GetX() ?? 0),+1,+300))
     S42MigrationAssign(iAngle, "-=", (Abs((iX ?? 0)-(GetX() ?? 0)) ?? 0)/12);
  if(Inside((iX ?? 0)-(GetX() ?? 0),-300,-1))
     S42MigrationAssign(iAngle, "+=", (Abs((iX ?? 0)-(GetX() ?? 0)) ?? 0)/12);
  // Zielen
  AimToAngle(iAngle);
  // Feuern
  return(Fire(fAuto));
}

public func AimToAngle(int iAngle)
{
  // Winkel anpassen
  iAngle ??= 0;
  while((iAngle ?? 0) > 180) S42MigrationAssign(iAngle, "-=", 360);
  // Richtung
  if((iAngle ?? 0) > 0) SetDir(DIR_Right);
  if((iAngle ?? 0) < 0) SetDir(DIR_Left);
  // Zielrichtung
  SetPhase(BoundBy( 20*(Abs(iAngle) ?? 0)/90, nil,19));
}

/* Laden */

protected func Collection(pObj)
{
  // Sound
  Sound("Connect");
  // Pakete zerlegen
  if(pObj->~IsArrowPack() )
    if(!pObj->~Unpack(this()) )
      Split2Components(pObj);
  SplitPacks();
  return(1);
}

private func SplitPacks()
{
  var i, obj;
  while(obj = Contents(S42MigrationStep(i, 1, true)) )
    if(obj->~IsArrowPack() )
      if(!obj->~Unpack(this()) )
        Split2Components(obj);
}

/* Nur Pfeile und Pfeilpakete annehmen */

protected func RejectCollect(idDef, pObj)
{
  // Maximal nur 10 Pfeile aufnehmen
  return( Or((And((!pObj->~IsArrow()), (!pObj->~IsArrowPack()))), ((GetArrowCount() ?? 0) >= 10)) );
}

/* Pfeile zaehlen */

private func GetArrowCount()
{
  var i, obj, count;
  while(obj = Contents(S42MigrationStep(i , 1, true)) )
  {
    if(obj->~IsArrow() ) S42MigrationStep(count , 1, true);
    if(obj->~IsArrowPack() ) S42MigrationAssign(count, "+=", (obj->PackCount() ?? 0));
  }
  return(count);
}

