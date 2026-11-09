/*-- leere Pressluftflasche --*/

#strict

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
  if (!Contained() && !InLiquid())
  {
    fill+=5;
    ChangeDef(CABO,this()); 
    return(1);
  }
  
  if (Contained() && !InLiquid(Contained()))
  {
    fill+=5;
    ChangeDef(CABO,this()); 
  }
  return(1);
}

/* Infos */

public GetAmmoPercent: return(fill*100/500);

protected func SellTo() { return(CABO); }
