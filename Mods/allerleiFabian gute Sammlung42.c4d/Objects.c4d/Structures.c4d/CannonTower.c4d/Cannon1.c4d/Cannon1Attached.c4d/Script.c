/*--- Kampfgeschütz (angeschraubt) ---*/

#strict 3


/* Überladbar für andere Kanonen */

protected func RotationSpeed() { return(5); }
protected func CannonMobileID() { return(CTW3); }
protected func CannonAmmo(object obj) { return((GetOCF(obj) ?? 0) & (OCF_Collectible ?? 0) ); }
protected func CannonPowderNeeded(object obj) { return(1); }
protected func CannonPower(object obj) { return(12); }
protected func CannonSound(object obj) { return("Blast3"); }
protected func CannonSmoke(object obj) { return(1); }
protected func CannonShootMenuID() { return(GetID()); }
protected func CannonShootMenuName(object pPotentialProjectile) { return Format("$TxtShoots$",GetName(pPotentialProjectile)); }

protected func Destruction() {
  CloseMenus();
}

private func CloseMenus() {
  var tower = GetActionTarget();

  for(var obj in FindObjects(Find_Container(tower)))
  {
    if(!obj->~IsClonk()) continue;
    if(!Equal(obj->GetMenu(), CannonShootMenuID())) continue;
    obj->CloseMenu();
  }
}

/* Turm weg? */

protected func AttachTargetLost()
{
  ComStopDouble();
}

/* Kommandos aus dem Turm */

public func ComLeft(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(-(RotationSpeed() ?? 0) );
  return(1);
}

public func ComRight(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(RotationSpeed() );
  return(1);
}

public func ComStop(object pClonk)
{
  SetAction("Attaching",GetActionTarget());
  SetRDir(nil);
  return(1);
}

public func ComStopDouble(object pClonk)
{
  CloseMenus();

  var r = ((GetR() ?? 0) + 270) % 360;
  SetR(r);
  ChangeDef(CannonMobileID() );
  return(1);
}

public func ComEnter()
{
  ComLeave();
}

public func ComLeave()
{
  var tower = GetActionTarget();
  for(var obj in FindObjects(Find_Container(tower)))
  {
    if(!obj->~IsClonk()) continue;
    if(!Equal(obj->GetMenu(), CannonShootMenuID())) continue;

    var sel = GetMenuSelection(obj);
    obj->CloseMenu();
    DoMenu(obj, sel);
  }
}

public func ComFire(object pClonk)
{
  SetAction("Attaching",GetActionTarget());
  SetRDir(nil);

  DoMenu(pClonk, nil);
}

private func DoMenu(object pClonk, int iSelection)
{
  iSelection ??= 0;
  CreateMenu(GetID(GetActionTarget()), pClonk, this(), nil, "$TxtNoammo$", nil, nil, nil, CannonShootMenuID());

  var i, obj;
  while(obj = Contents(S42MigrationStep(i, 1, true), GetActionTarget()) )
    if(!Equal(GetID(obj), GUNP))
      if(CannonAmmo(obj) )
        AddMenuItem(CannonShootMenuName(obj), Format("Shoot(%i, Object(%d), Object(%d))", obj->GetID(), ObjectNumber(obj), ObjectNumber(pClonk)), nil, pClonk, nil, nil, nil, 4, obj);
  SelectMenuItem(iSelection, pClonk);
  return(1);
}

private func Shoot(id defFoo, object pProjectile, object pShooter)
{
  if (!pProjectile) return(nil);
  if (!Equal(pProjectile->Contained(), GetActionTarget())) return(nil);

  var powder = CannonPowderNeeded(pProjectile);
  if((ContentsCount(GUNP, GetActionTarget()) ?? 0) < (powder ?? 0))
  {
    Sound("Error");
    Message("$TxtNotenoughgunpowder1r$", GetActionTarget(), powder);
  }
  else
  {
    for(var i = nil; (i ?? 0) < (powder ?? 0); S42MigrationStep( i, 1))
      RemoveObject(FindContents(GUNP, GetActionTarget()) );

    if(CannonSmoke(pProjectile) )
    {
      Smoke(Sin(GetR(), 13), -(Cos(GetR(), 13) ?? 0), 20);
      Smoke(Sin(GetR(), 18), -(Cos(GetR(), 23) ?? 0), 17);
      Smoke(Sin(GetR(), 22), -(Cos(GetR(), 32) ?? 0), 14);
      Smoke(Sin(GetR(), 25), -(Cos(GetR(), 40) ?? 0), 11);
    }
    
    // Controller setzen (Killverfolgung)
    pProjectile->SetController(pShooter->GetOwner());
    Shoot();

    Exit(pProjectile, Sin(GetR(), 13), -(Cos(GetR(), 13) ?? 0), GetR(), Sin(GetR(), CannonPower(pProjectile)), -(Cos(GetR(), CannonPower(pProjectile)) ?? 0), 20);
    if((GetOCF(pProjectile) ?? 0) & (OCF_CrewMember ?? 0) ) ObjectSetAction(pProjectile, "Tumble");
    Sound(CannonSound(pProjectile) );
    SetPlrView(pShooter->GetOwner(), pProjectile);
  }
}