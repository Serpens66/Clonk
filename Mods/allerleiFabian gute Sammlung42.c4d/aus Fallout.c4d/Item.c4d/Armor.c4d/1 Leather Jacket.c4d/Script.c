/*-- Rüstung --*/

#strict
#include GEAR

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("Munch1"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  var iPortraitCount = 2; 
  var strGraphics = "Wooza";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, S_LJ, 0, 0);
  Sound("Connect");
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pClonk) +20000, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pClonk) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pClonk) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pClonk) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pClonk) - 2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pClonk) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pClonk) +5000, PHYS_StackTemporary, pClonk);
DoEnergy(5000,pClonk,true);
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
  ResetPhysical(pClonk, "Fight");
  ResetPhysical(pClonk, "Walk");
  ResetPhysical(pClonk, "Scale");
  ResetPhysical(pClonk, "Hangle");
  ResetPhysical(pClonk, "Swim");
  ResetPhysical(pClonk, "Dig");
  ResetPhysical(pClonk, "Energy");
DoEnergy(-5000,pClonk,true);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(50);     // 20% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(5);     // 20% Projektile
  //Flame
  if(iType == DMG_Fire)
    return(5);   // 10% Flame
  //Explosion
  if(iType == DMG_Explosion)
    return(5);     // 20% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(1);    // 30% Energy
  //Plazma
  if(iType == DMG_Bio)
    return(1);     // 10% Plazma
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(1);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

public func IsAnvilProduct() { return(1); }