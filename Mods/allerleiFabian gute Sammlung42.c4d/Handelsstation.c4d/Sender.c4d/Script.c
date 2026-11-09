/*-- Sender --*/

local min, sec;

#strict 3

func Hit(...) {
  Sound("RockHit*");
  return(1);
}


//Zeitverlauf:

func Initialize(...) {
  min=6;
  sec=nil;
  Message("@%d:%d",this(),min,sec);
  return(1);
}

func Testen(...) {
  if(!sec)
{
    S42MigrationStep(min, -1, true);
//5 Minuten abgelaufen:
    if(!min)
      return(Weg());
    sec=60;
}
  S42MigrationStep(sec, -1, true);
  Message("@%d:%d",this(),min,sec);
  return(1);
}

func Weg(...) {
  RemoveObject();
  return(1);
}