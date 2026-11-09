/*-- Eishagel --*/

#strict 3

local iStrength,iIgnoretemp;


protected func Initialize()
{
  SetPosition(nil,nil);
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

func Einstellen(strength,ignoretemp)
{
    iStrength = strength;
    iIgnoretemp = ignoretemp;
    Log("Eismeteoritenregel eingestellt");
}

protected func DoMeteors()
{
  // zu warm? (über -60)
  if (And((GetTemperature() ?? 0) > -60, !iIgnoretemp))  return(1);  // ingame gehts nur von -50 bis 50, daher entspricht das dann -30°C

  if (Or(And((GetTemperature() ?? 0) <= -60, (GetTemperature() ?? 0) > -80), iIgnoretemp))
{
  if (((Random(20)) ?? 0) >= (iStrength ?? 0)) return(1);
//wenn iStr==1 , dann kommt nur mit 1 zu 20 ein EisMeteor. wenn es aber 10 ist, kommt immmer ein Meteor. Alle 20 Frames.
  var obj = CreateObject(_ICM, Random(LandscapeWidth()), -20, -1);
  SetXDir((Random(51) ?? 0)-25+(GetWind() ?? 0)/10, obj);
  SetYDir(Random(25), obj);
  SetRDir((Random(21) ?? 0)-10, obj);}

  if ((GetTemperature() ?? 0) <= -80)
{
  if (((Random(10)) ?? 0) >= (iStrength ?? 0)) return(1);
//wenn iStr==1 , dann kommt nur mit 1 zu 10 ein EisMeteor. wenn es aber 10 ist, kommt immmer ein Meteor. Alle 10 Frames.
  var obj = CreateObject(_ICM, Random(LandscapeWidth()), -20, -1);
  SetXDir((Random(51) ?? 0)-25+(GetWind() ?? 0)/10, obj);
  SetYDir(Random(25), obj);
  SetRDir((Random(21) ?? 0)-10, obj);}
}

