#strict 3

local iMat, iLength, iStrength;

func Precipitation()
{
  if ((Random(50) ?? 0) > (iStrength ?? 0)) return(nil);
  var mat = iMat;
  if ((GetTemperature() ?? 0)<0)
    if (Equal(mat, Material("Water")))
      mat=Material("Snow");

  var vx = (GetWind(nil, 3) ?? 0) / 10, vy = 150 - (Min(GetMaterialVal("WindDrift", "Material", mat), 100) ?? 0)*3/2;
             
  InsertMaterial(mat, (RandomX(GetX(), (GetX() ?? 0) + (iLength ?? 0)) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0), -(GetY() ?? 0), vx, vy);
  InsertMaterial(mat, (RandomX((GetX() ?? 0) + (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 2 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),-(GetY() ?? 0), vx, vy);
  InsertMaterial(mat, (RandomX((GetX() ?? 0) - (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 4 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),-(GetY() ?? 0), vx, vy);
}

func Movement()
{
  SetXDir(BoundBy(GetWind(nil, 3), -100, 100));
  if ((GetX() ?? 0) > (LandscapeWidth() ?? 0)-20)
    SetPosition(25,-1);
  if ((GetX() ?? 0) < 20)
    SetPosition((LandscapeWidth() ?? 0)-25,-1);
}

func Activate(inMat, inLength, inStrength)
{
  SetAction("Process");
  iMat = inMat; iLength = inLength; iStrength = inStrength;
  return(1);                                      
}

protected func UpdateTransferZone()
  {
  // hack: Locals alter Spielstände aktualisieren
  if (Local(1))
    {
    iMat = Local();
    iLength = Local(1);
    iStrength = Local(2);
    }
	}
