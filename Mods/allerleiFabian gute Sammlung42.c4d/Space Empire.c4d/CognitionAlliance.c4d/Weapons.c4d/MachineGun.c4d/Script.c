/*-- Maschinengewehr --*/

#strict


local ammocount, restshots;

protected func Initialize()
{
  SetAction("Reload");  
  return(1);
}

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);  
  if (!ammocount) return(LoadAmmo());
  ammocount--;
  restshots=5;
  SetAction("Fireing");
  return(1);
}

protected func FireBullet()
{ 
  // Kugel abfeuern
  var bullet = CreateObject(C_S5,GetDir(Contained())*24-12,Random(4)-2);
  bullet->Launch(GetDir(Contained()));
  
  // Patronenhülse abgeben
  var hull = CreateContents(CSH2); 
  if (GetDir(Contained()) == DIR_Left()) Exit(hull, 0, 0, 0,  Random(2)+1, -Random(3)-2, Random(21)+30);
                                    else Exit(hull, 0, 0, 0, -Random(2)-1, -Random(3)-2, Random(21)+30);
  --restshots;
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
  if (!Contained()) return(0);
  var ammo = FindContents(CAM2,Contained());
  if (!ammo) return(Sound("NoAmmo2")); 
  
  // nachladen
  RemoveObject(ammo);
  ammocount = 5;
  Sound("Buckle1");
  ShiftContents(Contained(),0,GetID());
  return(1);
}

/* Infos */

public GetAmmoPercent: return(ammocount*20+restshots*4);

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }

