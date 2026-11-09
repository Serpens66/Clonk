/*-- Granate --*/

#strict

func Hit()
{
  Sound("greandehit");
  return(1);
}

public func Activate()
{
 [$TxtFuse$]
 if(!ActIdle()) return(1);
 Sound("pin");
 ScheduleCall(0,"GrenadeSound",50);
 SetAction("Activated");
}

func GrenadeSound()
{
Sound("ThrowGrenade");
}


private func Boom() 
{
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) {Explode(30);BlastObjects(GetX(),GetY(),30*2);}
else FakeExplode(30,0,0,60);
}

public func IsGrenade()
{ return(true); }