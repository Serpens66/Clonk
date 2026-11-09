/* Marke */

#strict

local time;

func Initialize()
{
  SetVisibility(VIS_Owner);
  return(1);
}

func Disappear()
{
  Message("Jumper",this());
  time ++;

  if (time >= 11)
  {
    RemoveObject();
    return;
  }
  return(1);
}