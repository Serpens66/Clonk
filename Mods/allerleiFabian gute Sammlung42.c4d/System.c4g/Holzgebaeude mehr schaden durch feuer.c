/*-- Holzgebäude --*/

//bekommen hohen Schaden durch HazardFeuer
// weniger schaden durch etwas geht nicht, weil zb. 90% von 1 durch abrunden 0 ist und dann garkein schaden mehr gemacht würde

#strict

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
  if(iType == DMG_Explosion)
  {
   return(-20);
  }
  // Energieschaden
  if(iType == DMG_Energy)
  {
   return(0);
  }
  // Projektilschaden
  if(iType == DMG_Projectile)
  {
   return(0);
  }
  // Feuerschaden
  if(iType == DMG_Fire)
  {
   return(-100);
  }
  // Bioschaden
  if(iType == DMG_Bio)
  {
   return(0);
  }
  // Nahkampfschaden
  if(iType == DMG_Melee)
  {
   return(0);
  }
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(0);     // Stahlmantelkugeln durchschlagen Rüstungen einfach

 return(0);
}