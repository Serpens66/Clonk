/* Marke */

#strict 3

local time;

func Initialize()
{
  SetVisibility(VIS_Owner);
  return(1);
}

func Disappear()
{
  S42MigrationStep(time , 1, true);

  if ((time ?? 0) >= 15)
  {
    RemoveObject();
    return;
  }
  return(1);
}