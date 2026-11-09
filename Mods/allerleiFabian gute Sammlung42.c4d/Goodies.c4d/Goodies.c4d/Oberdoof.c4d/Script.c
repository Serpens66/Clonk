#strict

Hit:
 CastObjects(DO59,5,100);
 Explode(1);
 Sound("Catapult");
 return(1);
Activate:
 AssignVar(0,Contained());
 Fling(Var(0),0,-20,);
 Sound("Fuse");
 Smoke(0,0,9);
 RemoveObject();
 return();
