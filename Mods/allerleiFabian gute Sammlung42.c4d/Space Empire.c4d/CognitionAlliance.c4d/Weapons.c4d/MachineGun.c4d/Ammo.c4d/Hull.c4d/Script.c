/*-- Patronenhülse --*/

#strict 3


protected func Initialize()
{
  SetAction("Erasing");
  return(1);
}

protected func Selfremove()
{ return(RemoveObject()); }
