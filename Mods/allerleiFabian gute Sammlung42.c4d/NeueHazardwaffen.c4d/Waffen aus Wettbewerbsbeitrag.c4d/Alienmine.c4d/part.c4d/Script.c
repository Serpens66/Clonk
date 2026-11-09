/*-- Neues Objekt --*/

#strict
local explo;

func Initialize() {
  CreateParticle("PSpark",0,0,0,0,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  return(1);
}

public func SetExplo(int exp)
{ explo=exp; }

Remove:
  CreateParticle("PSpark",0,0,0,0,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) {Explode(explo);BlastObjects(GetX(),GetY(),explo*2);}
  else FakeExplode(explo*2,0,0,explo*2);
  return(0);
