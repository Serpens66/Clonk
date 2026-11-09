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
  time ++;

  if (time >= 15)
  {
    RemoveObject();
    return;
  }
  return(1);
}