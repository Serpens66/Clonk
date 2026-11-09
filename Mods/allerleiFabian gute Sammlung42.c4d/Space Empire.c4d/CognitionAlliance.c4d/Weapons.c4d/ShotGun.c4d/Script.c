/*-- Gewehr --*/

#strict 3


local ammocount;

protected func Initialize()
{
  SetAction("Reload");
  return(1);
}

public func Fire()
{
  // Beim Nachladen?
  if (!ActIdle()) return(nil);
  // keine Munition?
  if (!ammocount) return(LoadAmmo());
   
  // Schuss abgeben
  var ammo = CreateObject(C_S3, (GetDir(Contained()) ?? 0)*24-12, nil, GetOwner(Contained()));
  ammo->Launch(GetDir(Contained()));
  S42MigrationStep(ammocount, -1, true);
  
  // Kugelhülse abgeben
  var hull = CreateContents(CSH1);
  if (Equal(GetDir(Contained()), DIR_Left)) Exit(hull, nil, nil, nil,  (Random(2) ?? 0)+1, -(Random(3) ?? 0)-2, (Random(21) ?? 0)+30);
                                    else Exit(hull, nil, nil, nil, -(Random(2) ?? 0)-1, -(Random(3) ?? 0)-2, (Random(21) ?? 0)+30);
  Sound("AutoOnce");  
  SetAction("Reload");  
  return(1);
}

protected func LoadAmmo()
{
  if (ammocount) return(1); 
  if (!Contained()) return(nil);
  // nach Munition suchen
  var munition = FindContents(CAM1,Contained());
  if (!munition) return(Sound("NoAmmo2")); // keine weiteren Munitionspacks vorhanden ?
  
  // nachladen
  RemoveObject(munition);
  ammocount = 5;
  Sound("Buckle1");
  ShiftContents(Contained(),nil,GetID());
  return(1);
}

/* Infos */

public func GetAmmoPercent(...) { return((ammocount ?? 0)*20);
}

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }
