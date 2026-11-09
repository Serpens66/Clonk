/*-- Blasterschuss --*/

#strict


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
  if (Par(0) == DIR_Left()) XDir = -100;
  else (XDir = 100);
  YDir = 0;
  
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
  if (GetActTime() > 55) return(SetAction("Fadeout"));
  
  // nach Zielen suchen
  var target = FindObject(0,-5,-10,10,20,OCF_Alive(),"",0,NoContainer());
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit1");
  DoEnergy(-(4+Random(2)),target);
  RemoveObject();  
  return(1);
}

protected func Flight2()
{
  // schon zu lange unterwegs?
  if ((GetActTime()*25) > 250) RemoveObject();
  // nach Zielen suchen
  var target = FindObject(0,-5,-10,10,20,OCF_Alive(),"",0,NoContainer());
  // langsam ausfaden
  SetClrModulation(RGBa(255,255,255,250-GetActTime()*25));
  
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!target) return(1);  
  Sound("Hit1");
  DoEnergy(-(4+Random(2)),target);
  RemoveObject();  
  return(1);
}
