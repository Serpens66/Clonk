/*-- Neues Objekt --*/

#strict 3
local explo;

func Initialize() {
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  return(1);
}

public func SetExplo(int exp)
{ exp ??= 0;
    explo=exp; }

func Remove(...) {
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) {Explode(explo);BlastObjects(GetX(),GetY(),(explo ?? 0)*2);}
  else FakeExplode((explo ?? 0)*2,nil,nil,(explo ?? 0)*2);
  return(nil);
}
