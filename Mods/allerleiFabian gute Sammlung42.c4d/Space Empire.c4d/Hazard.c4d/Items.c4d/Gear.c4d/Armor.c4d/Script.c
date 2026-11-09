/*-- Rüstung --*/

#strict
#include GEAR


local pNightVision;	// saves the nightvision-light


/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  var iPortraitCount = 3; 
  var strGraphics = "Armor";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, GetID(pClonk), 0, 0);
  Sound("Connect");
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pClonk) +5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pClonk) - 500, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pClonk) - 500, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pClonk) - 500, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pClonk) - 500, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pClonk) - 2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pClonk) - 500, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pClonk) - 2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pClonk) +5000, PHYS_StackTemporary, pClonk);
DoEnergy(5000,pClonk,true);

}

public func GearEffectReset(object pClonk)
{
  // reset graphics and portrait
  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);
  if(pNightVision) RemoveObject(pNightVision);
  Sound("Connect");
  ResetPhysical(pClonk, "Fight");
  ResetPhysical(pClonk, "Walk");
  ResetPhysical(pClonk, "Scale");
  ResetPhysical(pClonk, "Hangle");
  ResetPhysical(pClonk, "Jump");
  ResetPhysical(pClonk, "Swim");
  ResetPhysical(pClonk, "Breath");
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
    return(30);     // 60% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(30);     // 40% Projektile
  //Flame
  if(iType == DMG_Fire)
    return(5);
  //Explosion
  if(iType == DMG_Explosion)
    return(10);     // 20% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(-20);    // -20% Energy (!)
  //Bio
  if(iType == DMG_Bio)
    return(5);     
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(5);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}


/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(pNightVision) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConUseable() {
  // need for nightvision
    return(true);
}

public func ConUse(object pHazard) {

  // turn on
  if(!pNightVision) {
    pNightVision = AddLight2(2200,RGBa(50,255,50,45),pHazard);
    if(!pNightVision)
      return();
    pNightVision->SetVisibility(VIS_Owner);
  }

  // turn off
  else {
    RemoveObject(pNightVision);
  }


  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
}



public func IsRefineryProduct() { return(1); }
