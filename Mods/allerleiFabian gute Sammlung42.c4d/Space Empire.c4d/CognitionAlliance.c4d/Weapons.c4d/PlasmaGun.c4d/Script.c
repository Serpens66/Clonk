/*-- Plasmagewehr --*/

#strict


local ammocount;

protected func Initialize()
{
  SetAction("Reload");
  return(1);
}

public func Fire()
{
  if (!ActIdle()) return(0); 
  if (!ammocount) return(LoadAmmo());
  if (!Contained()) return(1);
  var x = GetDir(Contained())*28-14;
  var bullet = CreateObject(CPS1,GetX()+x,GetY()+2,GetOwner(Contained()));
  bullet->Launch(GetX(),GetY(),GetDir(Contained())*150-75,0,270-GetDir(Contained())*180);
  ammocount--;
  Sound("Lasershot2");
  SetAction("Reload");
  return(1);
}

protected func LoadAmmo()
{
  // nach Munition suchen
  if (ammocount) return(1);
  if (!Contained()) return(0);
  var ammo = FindContents(CNBF,Contained());
  if (!ammo) return(Sound("NoAmmo2"));
   
  // nachladen  
  ammo->Empty();
  ammocount = 8;
  Sound("Buckle1");
  // Waffe wieder auswählen:
  ShiftContents(Contained(),0,GetID());
  return(1);
}

/* Infos */

public GetAmmoPercent: return(ammocount*100/8);

/* Eigenschaften */

public func IsWeapon() { return(1); }
public func IsWeaponProduct() { return(1); }
