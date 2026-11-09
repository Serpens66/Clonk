/*-- Schild --*/

#strict 3

func Initialize()
{
  SetRDir(2);
  return(1);
}

func Explosion()
{
  RemoveObject();
  return(1);
}

func Rotate()
{
  if (Equal(GetR(), -2)) SetR(nil);
  return(1);
}