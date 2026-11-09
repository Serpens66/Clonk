/*-- Flashlight --*/

#strict 3
#include GEAR

local light;
local user;


/* Status */

public func GetGearType()	{ return(GEAR_Light); }
public func IsEquipment()	{ return(true); }
public func IsLamp() 		{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  user = pClonk;

}

public func GearEffectReset(object pClonk)
{
  user = nil;
}


private func CheckRotation() {
  light->ChangeR(user->~AimAngle(20,180));
}


/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(light) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConUseable() {
  // need for nightvision
    return(true);
}

public func ConUse(object pHazard) {

if(!light)
{
  light = AddLightCone(1000,RGBa(255,255,220,50),user);
  light->ChangeSizeXY(3000,7000);
  light->ChangeOffset(nil,nil, true);
  SetAction("On");
  Sound("Click");
  }

  // turn off
  else {
  RemoveObject(light);
  SetAction("Idle");
  Sound("Click");
  }
  Sound("Click"); 
}


