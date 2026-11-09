/*-- Neues Objekt --*/

#strict 3

func Initialize() {
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  return(1);
}

func Remove(...) {
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  RemoveObject();
  return(nil);
}
