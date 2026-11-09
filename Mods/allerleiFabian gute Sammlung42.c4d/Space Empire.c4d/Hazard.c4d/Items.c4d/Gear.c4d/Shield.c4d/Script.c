/*-- Schildgenerator --*/

#strict 3
#include GEAR
#include KLAS

protected func Activate(object pByObj) {
  [$CtrlActivateDesc$]
  
  var upgrade = GetUpgradeable();
  if(GetLength(upgrade)) {
	  // create menu
	  CreateMenu(GetID(),pByObj,this(),4,"$NothingUpgradeable$",nil,nil,nil);

	  for(var pObj in upgrade) {
	    var szName = Format("$Upgrade$",GetName(pObj));
	    // fill the menu with menuitems
	    AddMenuItem(szName,"UpgradeObj",GetID(pObj),pByObj,nil,pObj,pObj->IsUpgradeable(GetID()));
	  }
	  return(1);
  }
  
  var pClonk = pByObj;
  
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

/* Status */

public func GetGearType()	{ return(GEAR_Shield); }
protected func Hit()		{ Sound("MetalHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  AddEffect("HazardShield", pClonk, 99, 3, nil, HSHD,200);
  Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  if(GetEffect("HazardShield", pClonk))
    return(RemoveEffect("HazardShield", pClonk));
  Sound("Connect");
}

public func FxHazardShieldStart(object pTarget, int iNo, int iTemp, var1) {
	iNo ??= 0;
	iTemp ??= 0;
	EffectVar(1,pTarget,iNo) = var1;
}

public func FxHazardShieldTimer(object pTarget, int iNo, int iTime)
{
  iNo ??= 0;
  iTime ??= 0;
  var boom = Equal(EffectVar(nil,pTarget,iNo), 2);
  var alpha = 115;
  if(boom) alpha = 15;
  var angle = Random(360);
  CreateParticle("FxShield", AbsX(GetX(pTarget)), AbsY(GetY(pTarget)), Sin(angle,250), -(Cos(angle,250) ?? 0), (EffectVar(1,pTarget,iNo) ?? 0)+((boom ?? 0)*30), RGBa(255,255,255, alpha), pTarget);

  if((EffectVar(nil,pTarget,iNo) ?? 0)>0)
    S42MigrationStep(EffectVar(nil,pTarget,iNo), -1, true);
}

public func FxHazardShieldBulletHit(object pTarget, int iNo)
{
  iNo ??= 0;
  if(Equal(EffectVar(nil,pTarget,iNo), nil))
  EffectVar(nil,pTarget,iNo) = 2;
  Sound("Shield");
}

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  //Energy
  iDamage ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Energy))
    return(60);    // 60% Energie
}

public func OnClonkHit(int iDmg, int iType, object pClonk)
{
  iDmg ??= 0;
  iType ??= 0;
  if(!pClonk) return;

  // nur Energie
  if(!Equal(iType, DMG_Energy)) return(nil);

  // # keine Energie abziehen, kann wie Rüstung getragen werden bis man tot ist
  // -> betatest für balancing
  // if(!DoAmmo(ENAM, -BoundBy(iDmg/3, 1, 500), pClonk) && !NoAmmo()) return();

  // Schild flackert auf
  EffectCall(pClonk, GetEffect("HazardShield", pClonk), "BulletHit");
  // Sound
  Sound("Shield");
}
