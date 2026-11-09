/*-- Maschinengewehr --*/

#strict 3


local ammocount, restshots;

protected func Initialize()
{
  SetAction("Reload");  
  return(1);
}

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(nil);
  if (!ammocount) return(LoadAmmo());
  S42MigrationStep(ammocount, -1, true);
  restshots=5;
  SetAction("Fireing");
  return(1);
}

protected func FireBullet()
{ 
  // Kugel abfeuern
  var bullet = CreateObject(C_S5,(GetDir(Contained()) ?? 0)*24-12,(Random(4) ?? 0)-2);
  bullet->Launch(GetDir(Contained()));
  
  // Patronenhülse abgeben
  var hull = CreateContents(CSH2); 
  if (Equal(GetDir(Contained()), DIR_Left)) Exit(hull, nil, nil, nil,  (Random(2) ?? 0)+1, -(Random(3) ?? 0)-2, (Random(21) ?? 0)+30);
                                    else Exit(hull, nil, nil, nil, -(Random(2) ?? 0)-1, -(Random(3) ?? 0)-2, (Random(21) ?? 0)+30);
  S42MigrationStep(restshots, -1);
  // Ammo-Anzeige des Clonks
  Contained()->~RefreshAmmoInfo();                                  
  if (restshots) return(1);
  SetAction("Reload");
  return(1);
}

protected func LoadAmmo()
{
  // nach Munition suchen
  if (ammocount) return(1);
  if (!Contained()) return(nil);
  var ammo = FindContents(CAM2,Contained());
  if (!ammo) return(Sound("NoAmmo2")); 
  
  // nachladen
  RemoveObject(ammo);
  ammocount = 5;
  Sound("Buckle1");
  ShiftContents(Contained(),nil,GetID());
  return(1);
}

/* Infos */

public func GetAmmoPercent(...) { return((ammocount ?? 0)*20+(restshots ?? 0)*4);
}

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }

