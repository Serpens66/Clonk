/*-- Gewehrkugel --*/

#strict 3


local XDir, YDir;

/* Initialisierung */

protected func Initialize()
{
  return(1);
}

public func Launch()
{
  if (Equal(Par(nil), DIR_Left)) XDir = -125;
  else (XDir = +125);
  YDir = nil;
  
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
  if ((GetActTime() ?? 0) > 30)
    if (!Random(60-(GetActTime() ?? 0)))
      return(Hit());
  
  var target = FindObject(nil,-8,-10,16,20,OCF_Alive,"",nil,NoContainer());
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit2");
  DoEnergy(-(10+(Random(4) ?? 0)),target);
  RemoveObject();  
  return(1);
}
