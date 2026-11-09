/*-- Gewehr --*/

#strict


local ammocount;

protected func Initialize()
{
  SetAction("Reload");
  return(1);
}

public func Fire()
{
  // Beim Nachladen?
  if (!ActIdle()) return(0); 
  // keine Munition?
  if (!ammocount) return(LoadAmmo());
   
  // Schuss abgeben
  var ammo = CreateObject(C_S3, GetDir(Contained())*24-12, 0, GetOwner(Contained()));
  ammo->Launch(GetDir(Contained()));
  ammocount--;
  
  // Kugelhülse abgeben
  var hull = CreateContents(CSH1);
  if (GetDir(Contained()) == DIR_Left()) Exit(hull, 0, 0, 0,  Random(2)+1, -Random(3)-2, Random(21)+30);
                                    else Exit(hull, 0, 0, 0, -Random(2)-1, -Random(3)-2, Random(21)+30);
  Sound("AutoOnce");  
  SetAction("Reload");  
  return(1);
}

protected func LoadAmmo()
{
  if (ammocount) return(1); 
  if (!Contained()) return(0);
  // nach Munition suchen
  var munition = FindContents(CAM1,Contained());
  if (!munition) return(Sound("NoAmmo2")); // keine weiteren Munitionspacks vorhanden ?
  
  // nachladen
  RemoveObject(munition);
  ammocount = 5;
  Sound("Buckle1");
  ShiftContents(Contained(),0,GetID());
  return(1);
}

/* Infos */

public GetAmmoPercent: return(ammocount*20);

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }
