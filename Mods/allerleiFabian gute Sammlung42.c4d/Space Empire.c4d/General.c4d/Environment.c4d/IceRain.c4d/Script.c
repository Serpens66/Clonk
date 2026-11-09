/*-- Eishagel --*/

#strict

local iStrength,iIgnoretemp;


protected func Initialize()
{
  SetPosition(0,0);
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
  if (GetTemperature() > -60 && !iIgnoretemp)  return(1);  // ingame gehts nur von -50 bis 50, daher entspricht das dann -30°C

  if (GetTemperature() <= -60 && GetTemperature() > -80 || iIgnoretemp)
{
  if ((Random(20)) >= iStrength) return(1); 
//wenn iStr==1 , dann kommt nur mit 1 zu 20 ein EisMeteor. wenn es aber 10 ist, kommt immmer ein Meteor. Alle 20 Frames.
  var obj = CreateObject(_ICM, Random(LandscapeWidth()), -20, -1);
  SetXDir(Random(51)-25+GetWind()/10, obj);
  SetYDir(Random(25), obj);
  SetRDir(Random(21)-10, obj);}

  if (GetTemperature() <= -80)
{
  if ((Random(10)) >= iStrength) return(1); 
//wenn iStr==1 , dann kommt nur mit 1 zu 10 ein EisMeteor. wenn es aber 10 ist, kommt immmer ein Meteor. Alle 10 Frames.
  var obj = CreateObject(_ICM, Random(LandscapeWidth()), -20, -1);
  SetXDir(Random(51)-25+GetWind()/10, obj);
  SetYDir(Random(25), obj);
  SetRDir(Random(21)-10, obj);}
}

