/*-- Schockwelle --*/

#strict 3

local iR, iMax;

protected func Initialize()
{
  // Standards
  iR = 20;
  iMax = 100;
  SetAction("Exist");
}

public func SetMaxSize(int iSize)
{
  iSize ??= 0;
  iMax = iSize;
}

protected func Grow()
{
  // Größer werden
  S42MigrationAssign(iR, "+=", 10);
  //Bei gewisser Größe löschen
  if((iR ?? 0) >= (iMax ?? 0))
    return(ScanAndRemove());

  // Partikelsichtbarkeit
  CreateParticle("EMPShock", nil,nil,nil,nil, ((iR ?? 0) + (RandomX(-(iR ?? 0)/3,(iR ?? 0)/2) ?? 0)) * 5, RGB(255,255,255));

  //Ein wenig was rumcasten
  for(var i=nil; (i ?? 0)<2; S42MigrationStep(i, 1)) {
    var pLaser = CreateObject(LASR, nil, nil, -1);
    pLaser->SetClrModulation(RGBa(28, 143, 241, 128));
    pLaser->Set(Random(360), 10, (iR ?? 0)/2, 3);
  }

}

protected func ScanAndRemove() {
  // 4 great performance: erst zum Schluss alles absuchen
  // Nach Maschinen suchen
  for(var obj in FindObjects( Find_Distance((iR ?? 0)/2+10), Find_NoContainer(), Find_Func("Zated") ))
  {
   if((ObjectDistance(obj) ?? 0) < (iR ?? 0))
   {
    obj->~Zated();
   }
  }
  RemoveObject();
  return(1);
}
