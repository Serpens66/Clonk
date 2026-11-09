/*-- Schild --*/

#strict

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
  if (GetR()==-2) SetR(0);
  return(1);
}