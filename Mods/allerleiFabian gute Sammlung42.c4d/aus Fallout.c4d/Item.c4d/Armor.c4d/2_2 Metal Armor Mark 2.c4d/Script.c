/*-- Rüstung --*/

#strict
#include GEAR

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pClonk) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pClonk) -5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pClonk) - 12000, PHYS_StackTemporary, pClonk);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pClonk) +40000, PHYS_StackTemporary, pClonk);
DoEnergy(40000,pClonk,true);
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 0; 
  var strGraphics = "BlackKnight";
  SetGraphics(strGraphics,pClonk);
  SetPortrait("BlackKnight1", pClonk, S_M2, 0, 0);
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
  ResetPhysical(pClonk, "Throw");
  ResetPhysical(pClonk, "Walk");
  ResetPhysical(pClonk, "Scale");
  ResetPhysical(pClonk, "Hangle");
  ResetPhysical(pClonk, "Jump");
  ResetPhysical(pClonk, "Swim");
  ResetPhysical(pClonk, "Breath");
  ResetPhysical(pClonk, "Dig");
  ResetPhysical(pClonk, "Energy");
DoEnergy(-40000,pClonk,true);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(30);     // 30% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(90);     // 75% Projektile
  //Flame
  if(iType == DMG_Fire)
    return(-20);   // 10% Flame
  //Explosion
  if(iType == DMG_Explosion)
    return(1);     // 25% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(-20);    // 10% Energy
  //Plazma
  if(iType == DMG_Bio)
    return(-10);     // 20% Plazma
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(50);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

public func IsAnvilProduct() { return(1); }