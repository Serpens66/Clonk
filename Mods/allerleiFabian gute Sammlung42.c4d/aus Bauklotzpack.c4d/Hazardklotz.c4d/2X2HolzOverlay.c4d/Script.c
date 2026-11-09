/*-- 2x2 Holzoverlay --*/

#strict 3

local iDmg;

func Initialize() {
  iDmg = nil;
  return(1);
}

public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  iDamage ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Explosion))
  {
   return(nil);
  }
  // Energieschaden
  if(Equal(iType, DMG_Energy))
  {
   return(50);
  }
  // Projektilschaden
  if(Equal(iType, DMG_Projectile))
  {
   return(100);
  }
  // Feuerschaden
  if(Equal(iType, DMG_Fire))
  {
   return(-100);
  }
  // Bioschaden
  if(Equal(iType, DMG_Bio))
  {
   return(100);
  }
  // Nahkampfschaden
  if(Equal(iType, DMG_Melee))
  {
   return(50);
  }
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(90);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(50);
}

public func IsBulletExtraTarget(id idBullet, object pBullet, object pShooter)
{
  return(true);
}


protected func Damage() 
{
  if(!Local(nil)) RemoveObject();
  DoDamage(((GetDamage() ?? 0)-(iDmg ?? 0))/2,Local(nil));
  iDmg = GetDamage();
 return(1);
}