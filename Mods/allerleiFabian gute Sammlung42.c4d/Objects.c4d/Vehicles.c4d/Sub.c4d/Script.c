/*-- U-Boot --*/

#strict 3

local turn_end_dir;

/* Initialisierung */

protected func Completion()
{
  SetAction("Walk");
  SetComDir(COMD_Stop);
  turn_end_dir = -1;
}

/* Steuerung */

private func TurnEnd()
{
  if(!Equal(turn_end_dir, -1))
  {
    SetDirection(turn_end_dir);
    turn_end_dir = -1;
  }
}

public func SetDirection(int comdir)
{
  comdir ??= 0;
  ClearCommand();

  if(Or(!!SEqual(GetAction(), "Turn"), !!SEqual(GetAction(), "Grapple")))
  {
    // Set direction when Turn action has finished
    turn_end_dir = comdir;
    return;
  }

  if(!SEqual(GetAction(), "Swim")) return;

  SetComDir(comdir);

  if(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right)))
    SetYDir(nil);
  if(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_Down)))
    SetXDir(nil);

  if(ComDirLike(comdir, COMD_Right))
  {
    if(Equal(GetDir(), DIR_Left))
    {
      SetAction("Turn");
      SetDir(DIR_Right);
    }
  }

  if(ComDirLike(comdir, COMD_Left))
  {
    if(Equal(GetDir(), DIR_Right))
    {
      SetAction("Turn");
      SetDir(DIR_Left);
    }
  }
}

protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  SetDirection(comdir);
}

protected func ContainedUp(object clonk)
{
  [$TxtComUp$|Image=SSY1:1]
  CloseAirlock();

  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Up));

  return(1);
}

protected func ContainedDown(object clonk)
{
  [$TxtComDown$|Image=SSY1:0]
  CloseAirlock();

  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Down));

  return(1);
}

protected func ContainedRight(object clonk)
{
  [$TxtComRight$|Image=SSY1:4]
  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Right));

  return(1);
}

protected func ContainedLeft(object clonk)
{
  [$TxtComLeft$|Image=SSY1:3]
  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetDirection(ComDirTransform(GetComDir(), COMD_Left));

  return(1);
}

protected func ContainedUpDouble(object pByObject)
{
  [$TxtExit$|Image=SSY1:5]
  OpenAirlock();
  Exit(pByObject);

  SetDirection(COMD_Stop);
}

protected func ContainedDig(object clonk)
{
  [$TxtFire$|Image=SSY1:2]
  var i, obj;
  for(var i = nil; obj = Contents(i); S42MigrationStep( i, 1))
    if(obj->~IsTorpedo())
      break;

  if(obj) SetCommand(obj, "Exit");
  else Sound("Error");

  return(1);
}

protected func ControlCommand(string szCommand, object pTarget, int iX,int iY)
{
  // Bewegungskommando
  iX ??= 0;
  iY ??= 0;
  if (!!SEqual(szCommand, "MoveTo"))
  {
    // Reset manual movement
    return(SetCommand(this(),szCommand,pTarget,iX,iY));
  }
}

/* Einsammeln & Eingang */

protected func RejectCollect(id idObject, object pObject)
{
  // Anderes Objekt nicht im Freien
  if (Contained(pObject)) return(nil);
  // Torpedotreffer
  if (pObject->~IsTorpedo())
    return(TorpedoHit(pObject));
  // U-Boot sammelt an Land nichts ein
  if (!SEqual(GetAction(), "Swim")) return(1);
  // Objekt befindet sich nicht im Bereich des Greifers
  if (!Inside((GetX(pObject) ?? 0) - (GetX() ?? 0) + 30 - (GetDir() ?? 0) * 60, -8, +8)) return(1);
  if (!Inside((GetY(pObject) ?? 0) - (GetY() ?? 0) - 14, -8, +8)) return(1);
  // Einsammeln okay
  return(nil);
}

protected func Ejection(object obj)
{
  if (obj->~IsTorpedo())
    obj->Launch(this());
}

protected func Collection(object pObject, bool fPut)
{
  fPut ??= false;
  if (fPut) return(1);
  SetAction("Grapple");
  Sound("Grapple");
}

protected func ActivateEntrance(object pByObject)
{
  return(OpenAirlock());
}

/* Kontakt */

protected func ContactLeft()
{
  if(!SEqual(GetAction(), "Swim")) return(nil);

  if (ComDirLike(GetComDir(), COMD_Left))
    SetDirection(COMD_Stop);
}
  
protected func ContactRight()
{
  if(!SEqual(GetAction(), "Swim")) return(nil);

  if (ComDirLike(GetComDir(), COMD_Right))
    SetDirection(COMD_Stop);
}

/* Funktionen */

private func ClearCommand()
{
  SetCommand(this(), "None");
}

private func CheckCommandAction()
{
  // Wenden nach rechts
  if(GetCommand())
    if(ComDirLike(GetComDir(), COMD_Right))
      if(Equal(GetDir(), DIR_Left))
      {
        SetAction("Turn");
        SetDir(DIR_Right);
      }

  // Wenden nach links
  if (GetCommand())
    if (ComDirLike(GetComDir(), COMD_Left))
      if (Equal(GetDir(), DIR_Right))
      {
        SetAction("Turn");
        SetDir(DIR_Left);
      }
}

private func SwimProcess()
{
  Bubbling();
  CheckCommandAction();
}

private func Bubbling()
{
  Bubble(nil, Random(4));
  Bubble(nil, -5);
  Bubble(nil, 5);
}

private func TorpedoHit(object pTorpedo)
{
  if(!SEqual(GetAction(pTorpedo), "Swim")) return(1);
  if((GetActTime(pTorpedo) ?? 0) < 25) return(1);
  pTorpedo->~Hit();
  return(1);
}

private func OpenAirlock()
{
  if (GetEntrance()) return(nil);
  SetEntrance(1);
  Sound("Airlock1");
  return(1);
}

private func CloseAirlock()
{
  if(!GetEntrance()) return(nil);
  SetEntrance(nil);
  Sound("Airlock2");
  return(1);
}

private func Damage()
{
  var i=nil;
  if ((GetDamage() ?? 0)<150) return(nil);
  ChangeDef(SUBB);
  SetAction("Wrecked");
  DoCon(-5);
  while(S42MigrationStep(i, 1, true)<14) Bubble(Random(30),Random(20));
}

/* Forschung */

public func IsDeepSeaResearch() { return(1); }
