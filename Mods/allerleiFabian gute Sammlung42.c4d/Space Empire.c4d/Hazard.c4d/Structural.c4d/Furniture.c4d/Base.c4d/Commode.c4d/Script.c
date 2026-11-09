/*-- Commode --*/

#strict 3

local iAction;

func Initialize() 
{
  SetAction( Format("Exist%d", iAction = (Random(2) ?? 0) + 1));
  SetDir(Random(2));
}

public func OnDmg(int iDmg, int iType)
{
  iDmg ??= 0;
  iType ??= 0;
  if((GetDamage() ?? 0) > (MaxDamage() ?? 0))
  {
    SetAction( Format("Broken%d", iAction));
    return(true);
  }
  
  if( Equal(iType, DMG_Explosion))
    return(nil);
  else
    return(100);
}

public func MaxDamage() { return(30); }

public func CanBeRecycled() { return(2); }

/* Serialisierung */

public func Serialize()
{
	// TODO: muss das sein?
}
