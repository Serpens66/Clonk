/*-- Pressluftflasche --*/

#strict

local fill;

protected func Initialize()
{
  fill = 500;
  return(1);
}

protected func Hit()
{
  Sound("MetalHit2");
  return(1);
}

protected func CheckBreath()
{
  // nicht ganz volle Flaschen an Land wieder auffüllen  
  if (!InLiquid() && !Contained())
  {
    if (fill < 496) fill+=10;
    return(1);
  }
  if (!Contained()) return(1);
  if (!InLiquid(Contained()))
  {
    if (fill < 496) fill+=10;
    return(1);
  }
  
  // unter Wasser Clonk mit Luft versorgen

//if(Inside(pClonk->GetBreath(), 1, pClonk->GetPhysical("Breath")/1000-1))

 if (Inside(GetBreath(Contained()),1,GetPhysical("Breath",0,Contained())/1000-1) && (fill > 4)) {
    fill-=10;
    DoBreath(2,Contained());
    if (fill <= 0) {
      fill=0;
      ChangeDef(CAB1,this());
    }
  }  
  return(1);
}

// Füllstandsanzeige mittels Munitionsbalken

public func GetAmmoPercent() { return(fill*100/500); }

// Eigenschaften
public func IsRefineryProduct() { return(1); }
public func HasSpecialBar() { return(1); }
