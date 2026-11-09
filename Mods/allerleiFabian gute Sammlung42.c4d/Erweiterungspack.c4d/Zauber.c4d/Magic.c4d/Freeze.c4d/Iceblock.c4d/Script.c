#strict 3

local alpha;

func Destruct(){ return(S42MigrationAssign(alpha, "+=", 10));}

func NoAction() //Keine Äktschn!
{
 if(!ActIdle(GetActionTarget())) ObjectSetAction(GetActionTarget(),"Idle");
 if(alpha) Destructing();
 return(1);
}

func Destructing()
{
  SetClrModulation(RGBa(255,255,255,alpha));
  S42MigrationAssign(alpha, "+=", 5);
  if((alpha ?? 0)>200) {ObjectSetAction(GetActionTarget(),"Walk"); RemoveObject();}
  return(1);
}
