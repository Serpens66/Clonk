#strict

// #appendto FXP1  // ist jetzt alles direkt in den wolkenskripten drin
// #appendto FX22

local iMat;

func Precipitation()
{
  if(((iMat == Material("Water") && GetTemperature() < 0)|| iMat == Material("Snow")) && FindObject(SNOR)) // wenn schneeeffekt regel aktiv, dann geben wolken keinen schnee, statdessen wird er von der regel gemacht
    return(0);
  return(_inherited());
}
