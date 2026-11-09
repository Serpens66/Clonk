/* Kaltes Feuer */

#strict 3

func Activate()
{
  Sound("Magic1");
  Sound("Inflame");
  Sound("Fuse");
  Var(nil)=-60;
  if (GetDir(Par(nil))) Var(nil)=60;
  CastObjects(CFLM,3,20,Var(nil),nil);
  RemoveObject();
  return(1);
}
