/*-- Patronenhülse --*/

#strict


protected func Initialize()
{
  SetAction("Erasing");
  return(1);
}

protected func Remove() { return(RemoveObject()); }
