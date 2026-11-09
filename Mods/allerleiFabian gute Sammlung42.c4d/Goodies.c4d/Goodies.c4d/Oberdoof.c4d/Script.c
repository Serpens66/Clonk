#strict 3

func Hit(...) {
 CastObjects(DO59,5,100);
 Explode(1);
 Sound("Catapult");
 return(1);
}
func Activate(...) {
 AssignVar(nil,Contained());
 Fling(Var(nil),nil,-20,);
 Sound("Fuse");
 Smoke(nil,nil,9);
 RemoveObject();
 return;
}
