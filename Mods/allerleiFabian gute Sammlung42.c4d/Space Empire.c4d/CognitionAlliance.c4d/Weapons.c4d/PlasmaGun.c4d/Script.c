/*-- Plasmagewehr --*/

#strict 3


local ammocount;

protected func Initialize()
{
  SetAction("Reload");
  return(1);
}

public func Fire()
{
  if (!ActIdle()) return(nil);
  if (!ammocount) return(LoadAmmo());
  if (!Contained()) return(1);
  var x = (GetDir(Contained()) ?? 0)*28-14;
  var bullet = CreateObject(CPS1,(GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+2,GetOwner(Contained()));
  bullet->Launch(GetX(),GetY(),(GetDir(Contained()) ?? 0)*150-75,nil,270-(GetDir(Contained()) ?? 0)*180);
  S42MigrationStep(ammocount, -1, true);
  Sound("Lasershot2");
  SetAction("Reload");
  return(1);
}

protected func LoadAmmo()
{
  // nach Munition suchen
  if (ammocount) return(1);
  if (!Contained()) return(nil);
  var ammo = FindContents(CNBF,Contained());
  if (!ammo) return(Sound("NoAmmo2"));
   
  // nachladen  
  ammo->Empty();
  ammocount = 8;
  Sound("Buckle1");
  // Waffe wieder auswählen:
  ShiftContents(Contained(),nil,GetID());
  return(1);
}

/* Infos */

public func GetAmmoPercent(...) { return((ammocount ?? 0)*100/8);
}

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }
