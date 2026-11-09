/*-- Funke --*/

#strict

local R,G,B,a,i;

protected func Initialize() {
  R=0;
  G=0;
  B=0;
  return(1);
}

func StartIt() {
  R = Random(256);
  G = 255;
  B = 128;
  Sound("Rocket_Start1");
  SetXDir(RandomX(-10,10));
  SetYDir(RandomX(-60,-100));
}

private func Effect() {
var zufall1,zufall2;
zufall1=RandomX(40,60);
zufall2=Random(50);
  CreateParticle("EMSpark",0,0,0,0,zufall1,HSLa(R,G,B,zufall2));
  CreateParticle("PSpark",0,0,0,0,RandomX(1000,1500),HSLa(R,G,B,180));
  ++i;
  if(i>=50){
    return(Finshes());
    }
  return(1);
}

protected func Hit() {
 return(RemoveObject(this()));
}

private func Finshes() {
 // Sound("Rocket_Blast*");
  var iSearchRadius = RandomX(-180, 180);
  var angle = RandomX(-180, 180);
  CastObjects(HSL2,4,5);
 return(RemoveObject(this()));
}
