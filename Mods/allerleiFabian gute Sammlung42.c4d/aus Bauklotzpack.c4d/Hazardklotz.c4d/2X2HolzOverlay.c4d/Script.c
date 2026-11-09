/*-- 2x2 Holzoverlay --*/

#strict

local iDmg;

func Initialize() {
  iDmg = 0;
  return(1);
}

public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  if(iType == DMG_Explosion)
  {
   return(0);
  }
  // Energieschaden
  if(iType == DMG_Energy)
  {
   return(50);
  }
  // Projektilschaden
  if(iType == DMG_Projectile)
  {
   return(100);
  }
  // Feuerschaden
  if(iType == DMG_Fire)
  {
   return(-100);
  }
  // Bioschaden
  if(iType == DMG_Bio)
  {
   return(100);
  }
  // Nahkampfschaden
  if(iType == DMG_Melee)
  {
   return(50);
  }
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(90);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(50);
}

public func IsBulletExtraTarget(id idBullet, object pBullet, object pShooter)
{
  return(true);
}


protected func Damage() 
{
  if(!Local(0)) RemoveObject();
  DoDamage((GetDamage()-iDmg)/2,Local(0));
  iDmg = GetDamage();
 return(1);
}