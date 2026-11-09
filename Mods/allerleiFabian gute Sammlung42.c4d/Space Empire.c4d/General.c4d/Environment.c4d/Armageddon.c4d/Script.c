/*-- Eishagel --*/

#strict 3

local iStrength;


protected func Initialize()
{
  // schon ein Steuerungsobjekt da?
  iStrength = 1;
  SetPosition(nil,nil);
  var obj = FindObject(GetID());
  if (!obj) return(1);
  // sich selbst dazuzählen
  obj->AddOne();
  RemoveObject();
}

public func AddOne()
{
  S42MigrationStep(iStrength, 1);
}

protected func DoMeteors()
{
  if (((Random(5) ?? 0)+5) >= (iStrength ?? 0)) return(1);
  var obj = CreateObject(METO, Random(LandscapeWidth()), -20, -1);
  SetXDir((Random(51) ?? 0)-25+(GetWind() ?? 0)/10, obj);
  SetYDir(Random(25), obj);
  SetRDir((Random(21) ?? 0)-10, obj);
}

