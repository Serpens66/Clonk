/*-- Eishagel --*/

#strict

local iStrength;


protected func Initialize()
{
  // schon ein Steuerungsobjekt da?
  iStrength = 1;
  SetPosition(0,0);
  var obj = FindObject(GetID());
  if (!obj) return(1);
  // sich selbst dazuzählen
  obj->AddOne();
  RemoveObject();
}

public func AddOne()
{
  ++iStrength;
}

protected func DoMeteors()
{
  if ((Random(5)+5) >= iStrength) return(1);
  var obj = CreateObject(METO, Random(LandscapeWidth()), -20, -1);
  SetXDir(Random(51)-25+GetWind()/10, obj);
  SetYDir(Random(25), obj);
  SetRDir(Random(21)-10, obj);
}

