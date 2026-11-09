#strict

//Fundament
func Initialize()
{
SetLocal(8);
}

protected func Destruction() 
 {
  if (Local(9)) RemoveObject(Local(9));
  return(1);
 }
