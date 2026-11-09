/* Zelttasche */

#strict

func Activate(clonk)
{
  var x = 0;
  var y = Min(11, LandscapeHeight() - GetY() + 1);

  var tent = CreateObject(TN5B, x, y, GetOwner(clonk));
  
  tent -> Unpack();
  AddCommand(clonk, "Grab", tent);
  
  RemoveObject();
  return(1);
}

func Hit()
{
  Sound("WoodHit*");
  return(1);
}