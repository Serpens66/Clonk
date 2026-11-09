/*-- Sichtradius appendto alte Gebäude --*/


#strict 2

#appendto ANVL
#appendto BRDG
#appendto CTW0
#appendto CST1
#appendto CST2
#appendto CST3
#appendto CHEM
#appendto DRCK
#appendto ELEV
#appendto FNDR
#appendto GIDL
#appendto HUT1
#appendto HUT2
#appendto HUT3
#appendto IDOL
#appendto IGLO
#appendto POWR
#appendto PUMP
#appendto RSRC
#appendto SAWM
#appendto SGNL
#appendto TWR1
#appendto TWR2
#appendto WMIL
#appendto WTWR
#appendto WRKS
#appendto DH8Z
#appendto D_8Z
#appendto F_EL
#appendto ELE_
#appendto AIRL
#appendto CHLP
#appendto SLP1
#appendto AIAL
#appendto FH9I
#appendto GRS1
#appendto GRS3
#appendto LUPU
#appendto _U2A
#appendto _X1A
#appendto ELEV // fahrstuhl
#appendto RW4V // Seilbahn
#appendto DTFF // Flüssigkeits Tank
#appendto C97Z // Wetterstation
#appendto DTBQ  // Tiefbrunnen
#appendto QBII // schneestatue gro
#appendto QIID  // kleine schneestatue
#appendto TAUC // UAV Station
#appendto MU5C // munitioinsfabrik
#appendto TAMB // mittlere bassis
#appendto TAGB // große basis
#appendto DTBA // Bergwerk
#appendto TABA // kleine basis
#appendto _SH5 // auftragshaus
#appendto ST_4 // Bahnhof
// #appendto WAL4 // versteck Wand
#appendto TR33 // Baumversteck
#appendto ST_1 // Feldweiche
#appendto CAVE // Schacht
#appendto FDRS // kleiner hochofen
#appendto FRGE // Esse
#appendto CHPI // Kohlenmeiler
#appendto CHBU // köhlerei
#appendto _USW // Umspannwerk
// #appendto 
// #appendto 
// #appendto 
// #appendto 
// #appendto 
// #appendto 
// #appendto 
// #appendto 


#appendto GSTA  //Gastank damit mit schuss zur explosion gebracht werden kann


func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(FindObject(SSSI)) return(70); // nur wenn Sicht von Dingen aktiviert ist
    return(0);
}

public func Completion(a,b,c,d)
{
  SetPlrViewRange(FogViewRange(),this(),1); 
return(_inherited(a,b,c,d));
}

// damit die Gebäude bei genug Schaden auch ohne Explosion zerstört werden (z.b durch Projektile)
public func Damage(a,b,c,d,e,f,g)
{
var hp;
var schaden;
hp = GetDefCoreVal("BlastIncinerate","DefCore");
schaden = GetDamage();
if (schaden >= hp) {Incinerate();};
_inherited(a,b,c,d,e,f,g);
}


// genauer spezialisieren auf holz/ stein gebäude
/*
// zeug für hazardschaden resistenzen
public func OnDmg(int iDamage, int iType)
{
  // Explosionsschaden
  if(iType == DMG_Explosion)
  {
   return(-10);
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
   return(-200);
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
*/

public func IsBulletExtraTarget(id idBullet, object pBullet, object pShooter)
{
  return(true);
}

