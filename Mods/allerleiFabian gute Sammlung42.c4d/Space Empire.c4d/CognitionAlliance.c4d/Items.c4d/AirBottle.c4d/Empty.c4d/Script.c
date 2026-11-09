/*-- leere Pressluftflasche --*/

#strict 3

local fill;

protected func Initialize()
{
  Message("Init aufgerufen!",this());
  return(1);
}

protected func Hit()
{
  Sound("MetalHit2");
  return(1);
}

protected func CheckFill()
{  
  if (And(!Contained(), !InLiquid()))
  {
    S42MigrationAssign(fill, "+=", 5);
    ChangeDef(CABO,this()); 
    return(1);
  }
  
  if (And(Contained(), !InLiquid(Contained())))
  {
    S42MigrationAssign(fill, "+=", 5);
    ChangeDef(CABO,this()); 
  }
  return(1);
}

/* Infos */

public func GetAmmoPercent(...) { return((fill ?? 0)*100/500);
}

protected func SellTo() { return(CABO); }
