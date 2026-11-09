/*-- Haubitze --*/

#strict 3

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetPhase(10);
}

/* Steuerung */

protected func ControlConf(int conf)
{
  conf ??= 0;
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN5:2]
  AimUp(clonk, 4, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN5:0]
  AimDown(clonk, 4, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN5:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
  comdir ??= 0;
  AimUpdate(clonk, comdir, 4, "ControlConf");
}

public func ControlDownSingle() // Sicht zur¸cksetzen
{
  [$TxtResetview$|Method=None]
}

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  [$TxtFire$|Image=CAN5:1]
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing;
  if (pThing = pClonk->Contents())
    if (AllowLoad(GetID(pThing)))
      return(nil);
  // Abfeuern wenn mˆglich, sonst Objekt herausnehmen
  return(Fire());
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
  // Alle Clonks die die Haubitze anfassen, sollen stoppen
  while(pClonk=FindObject(nil,nil,nil,nil,nil,nil,"Push",this(),nil,pClonk))
    SetComDir(COMD_Stop,pClonk);
}

public func Fire(bool fAuto)
{
  // Schieﬂpulver und Projektil suchen
  fAuto ??= false;
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  // Schieﬂpulver oder Projektil fehlt
  if (Or(!pGunpowder, !pProjectile))
  {
    Sound("Click");
    return(nil);
  }
  // Schieﬂpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var iX = ((GetPhase() ?? 0)+3)*((GetDir() ?? 0)*2-1);
  var iY = (GetPhase() ?? 0)-14;
  var iAngle = BoundBy((GetPhase() ?? 0)*5-5,nil,90);
  var iXDir = Sin(iAngle,32*((GetDir() ?? 0)*2-1));
  var iYDir = Cos(iAngle,-18);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(!Equal(GetPlrViewMode(GetController()), 2))
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch
  for (var i = nil; (i ?? 0) < 6; S42MigrationStep(i, 1))
    Smoke((iX ?? 0)+(RandomX(-5,+5) ?? 0),(iY ?? 0)+(RandomX(-5,+5) ?? 0),RandomX(5,12));
  // Erfolgreich geschossen
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
  // Bei grˆﬂerer Distanz hˆher zielen
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
  SetPhase(BoundBy( 19*(Abs(iAngle) ?? 0)/90, nil,18));
}
    
/* Laden */

protected func Collection() { Sound("Connect"); }

protected func RejectCollect(id idObj, object pObj)
{
  return(!AllowLoad(idObj));
}

private func AllowLoad(id idObj)
{
  // Noch jede Menge Platz
  if ((ContentsCount() ?? 0) < 9) return(1);
  // Niemals mehr als 10 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if ((ContentsCount() ?? 0) >= 10) return(nil);
  // Nicht mehr als 9 Schieﬂpulver laden (Idiotenschutz)
  if (Equal(idObj, GUNP))
  {
    if (Equal(ContentsCount(GUNP), 9))
      return(nil);
  }
  // Nicht mehr als 9 Projektile laden, wenn noch Platz f¸r Pulver gebraucht wird
  else
  {
    if (Equal(ContentsCount(GUNP), nil))
      return(nil);
  }
  // Laden o.k.
  return(1);
}

/* Forschung */

public func GetResearchBase() { return(CATA); }
