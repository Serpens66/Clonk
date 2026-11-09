#strict 3

// #appendto FXP1  // ist jetzt alles direkt in den wolkenskripten drin
// #appendto FX22

local iMat;

func Precipitation()
{
  if(And((Or((And(Equal(iMat, Material("Water")), (GetTemperature() ?? 0) < 0)), Equal(iMat, Material("Snow")))), FindObject(SNOR))) // wenn schneeeffekt regel aktiv, dann geben wolken keinen schnee, statdessen wird er von der regel gemacht
    return(nil);
  return(_inherited());
}
