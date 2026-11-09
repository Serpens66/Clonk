/*-- Rüstung --*/

#strict
#include GEAR

local pTarget;

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

public func Activate(object pClonk)
{
  [$CtrlActivateDesc$]
  if(GetID(Contained()) != KULL) { return(0); }
  // Can the clonk use this?
  if (!(pClonk->~CanUse(GetID()))) return(0);
  // Clonk doesn't already use gear of that type 
  if (pClonk->~HasGear(this()->GetGearType())) {
    PlayerMessage(GetOwner(pClonk),"$AlreadyWears$",pClonk,GetName(pClonk));
    return(0);
  }
  // Buckle
  if(pClonk->~EquipGear(this()))
    GearBuckle(pClonk);
  return(1);
}

public func GearEffect(object pClonk)
{
  if(pClonk) pTarget = pClonk;
  // Change graphics and portrait
  var iPortraitCount = 3; 
  var strGraphics = "Armor";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, GetID(pClonk), 0, 0);
  Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  pTarget = 0;
  // reset graphics and portrait
  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);
  Sound("Connect");
}

/* Brath */

public func GearGiveBreath()
{
  if(pTarget)
  {
   if(GetBreath(pTarget) < 70)
   {
    pTarget->DoBreath(29);
    Sound("MakeBreath",0,pTarget);
    Schedule("pTarget->~Bubble(0,-12)",1,20);
   }
  }
  return(1);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  
  if(iType == DMG_Melee)
    return(100);
    
  if(iType == DMG_Projectile)
    return(100);
    
  if(iType == DMG_Fire)
    return(999);

  if(iType == DMG_Explosion)
    return(100);

  if(iType == DMG_Energy)
    return(100);

  if(iType == DMG_Bio)
    return(110);
}