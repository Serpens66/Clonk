/*--- Flint ---*/

#strict 3

protected func Activate()
{
  CreateObject(EN8Z,nil,10,GetOwner(this()));
  RemoveObject();
  return(1);
}