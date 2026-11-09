/*-- Rüstung --*/

#strict 3
#include GEAR

local pTarget;

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

public func Activate(object pClonk)
{
  [$CtrlActivateDesc$]
  if(!Equal(GetID(Contained()), KULL)) { return(nil); }
  // Can the clonk use this?
  if (!(pClonk->~CanUse(GetID()))) return(nil);
  // Clonk doesn't already use gear of that type 
  if (pClonk->~HasGear(this()->GetGearType())) {
    PlayerMessage(GetOwner(pClonk),"$AlreadyWears$",pClonk,GetName(pClonk));
    return(nil);
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
  SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, GetID(pClonk), nil, nil);
  Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  pTarget = nil;
  // reset graphics and portrait
  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);
  Sound("Connect");
}

/* Brath */

public func GearGiveBreath()
{
  if(pTarget)
  {
   if((GetBreath(pTarget) ?? 0) < 70)
   {
    pTarget->DoBreath(29);
    Sound("MakeBreath",nil,pTarget);
    Schedule("pTarget->~Bubble(0,-12)",1,20);
   }
  }
  return(1);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  iDamage ??= 0;
  iType ??= 0;
  if(!pClonk) return;
  
  if(Equal(iType, DMG_Melee))
    return(100);
    
  if(Equal(iType, DMG_Projectile))
    return(100);
    
  if(Equal(iType, DMG_Fire))
    return(999);

  if(Equal(iType, DMG_Explosion))
    return(100);

  if(Equal(iType, DMG_Energy))
    return(100);

  if(Equal(iType, DMG_Bio))
    return(110);
}