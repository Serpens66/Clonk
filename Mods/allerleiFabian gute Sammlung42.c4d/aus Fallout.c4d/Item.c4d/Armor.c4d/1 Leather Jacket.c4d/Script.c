/*-- Rüstung --*/

#strict 3
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
  SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, S_LJ, nil, nil);
  Sound("Connect");
  SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, pClonk) ?? 0) +20000, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, pClonk) ?? 0) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, pClonk) ?? 0) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, pClonk) ?? 0) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, pClonk) ?? 0) - 2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, pClonk) ?? 0) +1000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, pClonk) ?? 0) +5000, PHYS_StackTemporary, pClonk);
DoEnergy(5000,pClonk,true);
}

public func GearEffectReset(object pClonk)
{

// Grafik je nach Stufe zurücksetzen
 var stuufe=pClonk->~AktuelleStufe();
if(stuufe)
{

if(And((stuufe ?? 0)>=1, (stuufe ?? 0)<=9))
{SetGraphics("Normal",pClonk);  //Hazardclonk mit Clonkgrafik
  var iPortraitCount = 5; 
  var strGraphics = "";
SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, CLNK, nil, nil);}

if(And(And((stuufe ?? 0)>=10, (stuufe ?? 0)<=14), !(pClonk->~IstBauarbeiter())))
{SetPortrait("random", pClonk, S_XX); SetGraphics("SG1",pClonk);}  //Hazardclonk mit SG1 Grafik

// Bisher nur eine Grafik für alle Stufen ab 10
if(And(And((stuufe ?? 0)>=10, (stuufe ?? 0)<=20), pClonk->~IstBauarbeiter()))
{ SetPortrait("random", pClonk, X_12); 
SetGraphics("Bauer",pClonk);}  //Hazardclonk mit Arbeiter Grafik

if(And(And((stuufe ?? 0)>=15, (stuufe ?? 0)<=19), !(pClonk->~IstBauarbeiter())))
{SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);}  //Hazardclonk Grafik

if(And(Equal(stuufe, 20), !(pClonk->~IstBauarbeiter())))
{SetGraphics("Future",pClonk);
SetPortrait("FutureP", pClonk, HZCK);} //Hazardclonk Future Grafik, Future Portrait
}
  // reset graphics and portrait
else{  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);}
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
  iDamage ??= 0;
  iType ??= 0;
  if(!pClonk) return;
  //Melee
  if(Equal(iType, DMG_Melee))
    return(50);     // 20% Melee
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(5);     // 20% Projektile
  //Flame
  if(Equal(iType, DMG_Fire))
    return(5);   // 10% Flame
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(5);     // 20% Explosion
  //Energy
  if(Equal(iType, DMG_Energy))
    return(1);    // 30% Energy
  //Plazma
  if(Equal(iType, DMG_Bio))
    return(1);     // 10% Plazma
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(1);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

public func IsAnvilProduct() { return(1); }