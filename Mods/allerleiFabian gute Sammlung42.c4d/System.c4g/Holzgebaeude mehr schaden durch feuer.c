/*-- Holzgebäude --*/

//bekommen hohen Schaden durch HazardFeuer
// weniger schaden durch etwas geht nicht, weil zb. 90% von 1 durch abrunden 0 ist und dann garkein schaden mehr gemacht würde

#strict 3

#appendto BRDG
#appendto DRCK
#appendto ELEV
#appendto HUT1
#appendto HUT2
#appendto IGLO
#appendto PUMP
#appendto RSRC
#appendto SAWM
#appendto SGNL
#appendto WMIL
#appendto WRKS
#appendto DH8Z
#appendto D_8Z
#appendto F_EL
#appendto ELE_
#appendto CHLP
#appendto SLP1
#appendto FH9I
#appendto GRS1
#appendto GRS3
#appendto LUPU
#appendto _U2A
#appendto _X1A



// zeug für hazardschaden resistenzen/schwächen
public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  iDamage ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Explosion))
  {
   return(-20);
  }
  // Energieschaden
  if(Equal(iType, DMG_Energy))
  {
   return(nil);
  }
  // Projektilschaden
  if(Equal(iType, DMG_Projectile))
  {
   return(nil);
  }
  // Feuerschaden
  if(Equal(iType, DMG_Fire))
  {
   return(-100);
  }
  // Bioschaden
  if(Equal(iType, DMG_Bio))
  {
   return(nil);
  }
  // Nahkampfschaden
  if(Equal(iType, DMG_Melee))
  {
   return(nil);
  }
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(nil);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(nil);
}