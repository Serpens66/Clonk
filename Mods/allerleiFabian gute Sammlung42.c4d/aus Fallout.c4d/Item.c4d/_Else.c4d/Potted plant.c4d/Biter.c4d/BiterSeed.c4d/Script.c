/*--- Flint ---*/

#strict

protected func Activate()
{
  CreateObject(EN8Z,0,10,GetOwner(this()));
  RemoveObject();
  return(1);
}