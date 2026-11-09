/*-- Labdoor --*/

#strict 3

#include GBDR

local Status, bBackground;

//Einstellungem

public func Initialize()
{
   SetDir(DIR_Right);
   SetAction("Close");
   
   UpdateSolidMask();
   _inherited();
   SetMaxDamage(100);
}


public func OnDmg(int iDmg, int iType) 
{
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Projectile))   return(40);
  if(Equal(iType, DMG_Melee))    return(20);
  if(Equal(iType, DMG_Energy))   return(70);
  if(Equal(iType, DMG_Explosion))    return(-10);

  return(100);
}

public func OnOpen()
{
  if( bBackground) return(nil);
    
  if( Or(!!SEqual(GetAction(), "Open"), Status)) return(nil);
  
  SetAction("Open");
  SetSolidMask(nil);
  Status = true;
  //Sound
  Sound("LabDoorOpen");
}

public func OnClose()
{
  if( bBackground) return(nil);
  
  if( Or(!!SEqual(GetAction(), "Close"), !Status)) return(nil);
  SetAction("Close");
  Status = nil;
  UpdateSolidMask();
  Sound("LabDoorClose");
}

public func OnDestroyed(iType)
{
    SetAction("Destroyed");
}

public func ShowFront()
{
   bBackground = true;
   SetAction("FrontSight");
   SetSolidMask();
   
   return(bBackground);
}

public func UpdateSolidMask()
{
  if(Equal(GetDir(), DIR_Left))
    return(SetSolidMask(14,nil,1,30,14,nil) );
  else
    return(SetSolidMask(14,nil,1,30,1,nil) );
}

public func IsBulletTarget() { 
  if(bBackground) return(nil);
  return(_inherited());

}
public func CanBeLaserRecycled() { return(2); }

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (destroyed)
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Destroyed\")";
	if (Equal(GetDir(), DIR_Left)) {
		extra[(GetLength(extra) ?? 0)] = "SetDir(DIR_Left)";
		extra[(GetLength(extra) ?? 0)] = "UpdateSolidMask()";
	}
	if (bBackground)
		extra[(GetLength(extra) ?? 0)] = "ShowFront()";
}
