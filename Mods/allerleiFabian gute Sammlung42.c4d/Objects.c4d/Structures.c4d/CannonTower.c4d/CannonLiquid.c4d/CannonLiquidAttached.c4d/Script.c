/*--- Flüssigkeitsgeschütz (angeschraubt) ---*/

#strict 3

#include CTW2

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(LICN); }

public func ComFire(object clonk)
{
  Sound("Click");

  inherited(clonk, ...);

  return(1);
}

protected func DoMenu(object clonk, int iSelection)
{
  iSelection ??= 0;
  var tower = GetActionTarget();
  CreateMenu(tower->GetID(), clonk, this(), nil, "$TxtNoammo$",nil,nil,nil,CannonShootMenuID());
  
  // Fässer suchen
  var i, obj, last_id, barrel_cnt, type_cnt;
  while (obj = tower->Contents(S42MigrationStep(i, 1, true))) {
    var obj_id = obj->GetID();
    if (IsFilledBarrel(obj)) {
      S42MigrationStep(barrel_cnt, 1, true);
      // Erstes Objekt des Typs?
      if (!Equal(obj_id, last_id)) {
        S42MigrationStep(type_cnt, 1, true);
        last_id = obj_id;
        AddMenuItem("$TxtShoots$", "Shoot", obj_id, clonk, tower->ContentsCount(obj_id), nil, nil, 4, obj);
      }
    }
  }

  if ((type_cnt ?? 0) > 1)
    AddMenuItem("$TxtShootsAll$", "Shoot", BARL, clonk, barrel_cnt);

  SelectMenuItem(iSelection, clonk);
  return(1);
}

private func IsFilledBarrel(object obj)
{
  return(And(obj->~GetAmount(), obj->~BarrelMaterialName()));
}

private func Shoot(id barrel_type)
{
  var tower = GetActionTarget();
  var r = GetR();

  if (Equal(barrel_type, BARL))
    barrel_type = nil;

  var i, obj, n;
  for(var obj in FindObjects(Find_Container(GetActionTarget()))) {
    if(!IsFilledBarrel(obj)) continue;
    if (And(barrel_type, !Equal(barrel_type, obj->GetID()))) continue;

    var mat = obj->BarrelMaterial();
    var amt = obj->GetAmount();
    obj->BarrelDoFill(-(amt ?? 0));
    obj->ChangeDef(obj->EmptyBarrelID());

    for (var j = nil; (j ?? 0) < (amt ?? 0); S42MigrationStep(j, 1, true)) {
      S42MigrationStep(n, 1, true);
      var f = (n ?? 0) / 50;
      var dist = 20 + (f ?? 0);
      var angle = (r ?? 0) + (RandomX((f ?? 0) / -3, (f ?? 0) / 3) ?? 0);
      InsertMaterial(mat, Sin(angle, dist), -(Cos(angle, dist) ?? 0), Sin(r, 200), -(Cos(r, 200) ?? 0));
    }
  }

  Sound("Splash*");
}