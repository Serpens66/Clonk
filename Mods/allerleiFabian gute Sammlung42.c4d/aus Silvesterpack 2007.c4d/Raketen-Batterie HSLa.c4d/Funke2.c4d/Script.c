/*-- Funke --*/

#strict 3

local R,G,B,a,i;

protected func Initialize() {
  return(StartIt());
}

func StartIt() {
  R = Random(256);
  G = 255;
  B = 128;
  SetAction("i");
}

private func Effect() {
var zufall1,zufall2;
zufall1=RandomX(40,60);
zufall2=Random(50);
  CreateParticle("EMSpark",nil,nil,nil,nil,zufall1,HSLa(R,G,B,zufall2));
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(1000,1500),HSLa(R,G,B,180));
  S42MigrationStep(i, 1);
  Sound("Fuse");
  if((i ?? 0)>=45){
    return(Finshes());
    }
  return(1);
}

protected func Hit() {
 return(RemoveObject(this()));
}

private func Finshes() {
 return(RemoveObject(this()));
}
