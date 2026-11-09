/*-- Granate --*/

#strict 3

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
 ScheduleCall(nil,"GrenadeSound",50);
 SetAction("Activated");
}

func GrenadeSound()
{
Sound("ThrowGrenade");
}


private func Boom() 
{
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) {Explode(30);BlastObjects(GetX(),GetY(),30*2);}
else FakeExplode(30,nil,nil,60);
}

public func IsGrenade()
{ return(true); }