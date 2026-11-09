/* Marke */

#strict

local time;
local name;

func Initialize()
{
  SetVisibility(VIS_Owner);
  time = 11;
  return(1);
}

func Disappear()
{
  Message("%v",this(),GetName(name));
  
  time --;
  if (time == 0)
  {
    RemoveObject();
    return();
  }
  return(1);
}