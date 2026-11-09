/* Marke */

#strict 3

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
  
  S42MigrationStep(time , -1, true);
  if (Equal(time, nil))
  {
    RemoveObject();
    return;
  }
  return(1);
}