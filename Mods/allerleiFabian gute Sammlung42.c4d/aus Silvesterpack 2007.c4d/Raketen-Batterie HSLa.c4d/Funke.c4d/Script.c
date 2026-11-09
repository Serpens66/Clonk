/*-- Funke --*/

#strict 3

local R,G,B,a,i;

protected func Initialize() {
  R=nil;
  G=nil;
  B=nil;
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
  CreateParticle("EMSpark",nil,nil,nil,nil,zufall1,HSLa(R,G,B,zufall2));
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(1000,1500),HSLa(R,G,B,180));
  S42MigrationStep(i, 1);
  if((i ?? 0)>=50){
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
