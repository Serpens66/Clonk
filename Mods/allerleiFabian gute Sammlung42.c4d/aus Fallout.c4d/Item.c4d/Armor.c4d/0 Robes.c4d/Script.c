/*-- Rüstung --*/

#strict
#include GEAR

local lkjsdfnldnlvsndlksdf,derclonk;  // magical physics

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("Munch1"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  derclonk=pClonk;
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 3; 
  var strGraphics = "Assassin";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, S_RB, 0, 0);
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

else
{  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}

// falls fähigkeit an, deaktivieren
if(lkjsdfnldnlvsndlksdf!=0) ConUse(pClonk);
  Sound("Connect");
return();
}


/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(1);     // 
  //Projectile
  if(iType == DMG_Projectile)
    return(1);     // 
  //Flame
  if(iType == DMG_Fire)
    return(-40);   // 
  //Explosion
  if(iType == DMG_Explosion)
    return(-10);     // 
  //Energy
  if(iType == DMG_Energy)
    return(20);    // 
  //Plazma
  if(iType == DMG_Bio)
    return(70);     // 
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(1);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}



/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(lkjsdfnldnlvsndlksdf) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConUseable() {return(true);}

public func ConUse(object pHazard) {

  // turn on

  if(lkjsdfnldnlvsndlksdf==0) {


lkjsdfnldnlvsndlksdf=1;
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pHazard) -50000, PHYS_StackTemporary, pHazard);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pHazard) +60000, PHYS_StackTemporary, pHazard);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pHazard) +40000, PHYS_StackTemporary, pHazard);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pHazard) -20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pHazard) +10000, PHYS_StackTemporary, pHazard);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);

 }

  // turn off
  else {
Log("manuell deaktiviert");
lkjsdfnldnlvsndlksdf=0;
  ResetPhysical(pHazard, "Fight");
  ResetPhysical(pHazard, "Throw");
  ResetPhysical(pHazard, "Walk");
  ResetPhysical(pHazard, "Scale");
  ResetPhysical(pHazard, "Hangle");
  ResetPhysical(pHazard, "Jump");
  ResetPhysical(pHazard, "Swim");
  ResetPhysical(pHazard, "Breath");
  ResetPhysical(pHazard, "Dig");
  ResetPhysical(pHazard, "Energy");
  };
  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
return();
}

func Schaden()
{
if(lkjsdfnldnlvsndlksdf==1)
{   DoEnergy(-60,derclonk,1);};}





public func IsAnvilProduct() { return(1); }