#strict

func Hit()
{
  SetVar(0,70);
  while(DecVar()) CreateObject(HS39,1,1,GetOwner());
  //BlastObjects(GetX(),GetY(),160);
  Sound("Blast3", this());
  Explode(1);
return(1);
}