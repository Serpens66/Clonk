#strict 3

local zaeehler;

func Initialize(...) {
  SetLocal(nil,-59);
  return(1);
}


func Hit()
{
 S42MigrationStep( zaeehler, 1);
 if((zaeehler ?? 0)<=10)
{
 var stein = CreateObject(ROCK);
 FakeExplode(5,stein,nil,5);
  if(Not(GBackSolid(nil,-2))) SetYDir(Local());
  if(GBackSolid(-2,nil)) SetXDir(Mul(GetXDir(),-1));
  if(GBackSolid(2,nil))  SetXDir(Mul(GetXDir(),-1));
  if(GreaterThan(Local(),-3)){SetYDir();SetPosition(GetX(),Sum(GetY(),1));SetLocal(nil,-59);} //And(SetYDir(),SetPosition(GetX(),Sum(GetY(),1)),SetLocal(0,-59));
}
else
{
 Sound("RavenHurt");
Explode(10);
 return(1);
}
}

func Activate(...) {
 AssignVar(nil,Contained());
 Incinerate(Var(nil));
 return;
}