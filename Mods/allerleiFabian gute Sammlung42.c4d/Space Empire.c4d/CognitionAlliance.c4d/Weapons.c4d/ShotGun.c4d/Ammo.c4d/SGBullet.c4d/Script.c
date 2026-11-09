/*-- Gewehrkugel --*/

#strict


local XDir, YDir;

/* Initialisierung */

protected func Initialize()
{
  return(1);
}

public func Launch()
{
  if (Par(0) == DIR_Left()) XDir = -125;
  else (XDir = +125);
  YDir = 0;
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  SetAction("Flying");
  return(1);
}

protected func Hit()
{
  Explode(10);
  return(1);
}

protected func Flight()
{
  if (GetActTime() > 30)
    if (!Random(60-GetActTime()))
      return(Hit());
  
  var target = FindObject(0,-8,-10,16,20,OCF_Alive(),"",0,NoContainer());
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit2");
  DoEnergy(-(10+Random(4)),target);
  RemoveObject();  
  return(1);
}
