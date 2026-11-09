/*-- Sender --*/

local min, sec;

#strict

Hit:
  Sound("RockHit*");
  return(1);


//Zeitverlauf:

Initialize:
  min=6;
  sec=0;
  Message("@%d:%d",this(),min,sec);
  return(1);

Testen:
  if(!sec)
{
    min--;
//5 Minuten abgelaufen:
    if(!min)
      return(Weg());
    sec=60;
}
  sec--;
  Message("@%d:%d",this(),min,sec);
  return(1);

Weg:
  RemoveObject();
  return(1);