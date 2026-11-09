/*-- Leather Armor --*/

#strict
#include GEAR

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("Chuff"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 0; 
  var strGraphics = "Agent";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1) , pClonk,S_L2, 0, 0);
}

public func GearEffectReset(object pClonk)
{
// Grafik je nach Stufe zurücksetzen
 var stuufe=pClonk->~AktuelleStufe();
if(stuufe)
{

if(stuufe>=1 && stuufe<=9)
{SetGraphics("Normal",pClonk);  //Hazardclonk mit Clonkgrafik
  var iPortraitCount = 5; 
  var strGraphics = "";
SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, CLNK, 0, 0);}

if(stuufe>=10 && stuufe<=14 && !(pClonk->~IstBauarbeiter()))
{SetPortrait("random", pClonk, S_XX); SetGraphics("SG1",pClonk);}  //Hazardclonk mit SG1 Grafik

// Bisher nur eine Grafik für alle Stufen ab 10
if(stuufe>=10 && stuufe<=20 && pClonk->~IstBauarbeiter())
{ SetPortrait("random", pClonk, X_12); 
SetGraphics("Bauer",pClonk);}  //Hazardclonk mit Arbeiter Grafik

if(stuufe>=15 && stuufe<=19 && !(pClonk->~IstBauarbeiter()))
{SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}  //Hazardclonk Grafik

if(stuufe==20 && !(pClonk->~IstBauarbeiter()))
{SetGraphics("Future",pClonk);
SetPortrait("FutureP", pClonk, HZCK);} //Hazardclonk Future Grafik, Future Portrait
}

  // reset graphics and portrait
else{  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}
  Sound("Connect");
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(2);     // 25% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(5);     // 20% Projektile
  //Flame
  if(iType == DMG_Fire)
    return(70);   // 25% Flame
  //Explosion
  if(iType == DMG_Explosion)
    return(50);     // 25% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(-10);    // 40% Energy
  //Plazma
  if(iType == DMG_Bio)
    return(-10);     // 10% Plazma
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(0);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

public func IsAnvilProduct() { return(1); }