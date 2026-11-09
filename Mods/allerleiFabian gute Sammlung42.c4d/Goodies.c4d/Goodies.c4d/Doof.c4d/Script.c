#strict

local zaeehler;

Initialize:
  SetLocal(0,-59);
  return(1);


func Hit()
{
 ++ zaeehler;
 if(zaeehler<=10)
{
 var stein = CreateObject(ROCK);
 FakeExplode(5,stein,0,5);
  if(Not(GBackSolid(0,-2))) SetYDir(Local());
  if(GBackSolid(-2,0)) SetXDir(Mul(GetXDir(),-1));
  if(GBackSolid(2,0))  SetXDir(Mul(GetXDir(),-1));
  if(GreaterThan(Local(),-3)){SetYDir();SetPosition(GetX(),Sum(GetY(),1));SetLocal(0,-59);} //And(SetYDir(),SetPosition(GetX(),Sum(GetY(),1)),SetLocal(0,-59));
}
else
{
 Sound("RavenHurt");
Explode(10);
 return(1);
}
}

Activate:
 AssignVar(0,Contained());
 Incinerate(Var(0));
 return();