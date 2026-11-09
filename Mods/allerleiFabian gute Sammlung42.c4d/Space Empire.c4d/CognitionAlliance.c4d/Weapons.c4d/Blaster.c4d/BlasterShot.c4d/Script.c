/*-- Blasterschuss --*/

#strict 3


local XDir, YDir, count;

/* Initialisierung */

protected func Initialize()
{
  return(1);
}

public func Launch()
{
  Sound("Lasershot4");
  // Richtung und Geschwindigkeit festlegen
  if (Equal(Par(nil), DIR_Left)) XDir = -100;
  else (XDir = 100);
  YDir = nil;
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  SetAction("Flying");
  return(1);
}

protected func Hit()
{
  Explode(5);
  return(1);
}

protected func Flight()
{
  // schon lange unterwegs?
  if ((GetActTime() ?? 0) > 55) return(SetAction("Fadeout"));
  
  // nach Zielen suchen
  var target = FindObject(nil,-5,-10,10,20,OCF_Alive,"",nil,NoContainer());
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit1");
  DoEnergy(-(4+(Random(2) ?? 0)),target);
  RemoveObject();  
  return(1);
}

protected func Flight2()
{
  // schon zu lange unterwegs?
  if (((GetActTime() ?? 0)*25) > 250) RemoveObject();
  // nach Zielen suchen
  var target = FindObject(nil,-5,-10,10,20,OCF_Alive,"",nil,NoContainer());
  // langsam ausfaden
  SetClrModulation(RGBa(255,255,255,250-(GetActTime() ?? 0)*25));
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit1");
  DoEnergy(-(4+(Random(2) ?? 0)),target);
  RemoveObject();  
  return(1);
}
