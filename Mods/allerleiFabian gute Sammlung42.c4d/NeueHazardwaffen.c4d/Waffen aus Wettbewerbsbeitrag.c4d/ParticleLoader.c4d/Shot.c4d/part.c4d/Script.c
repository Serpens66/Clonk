/*-- Neues Objekt --*/

#strict

func Initialize() {
  CreateParticle("PSpark",0,0,0,0,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  return(1);
}

Remove:
  CreateParticle("PSpark",0,0,0,0,RandomX(100,200),
                 RGBa(64,255,32,100+50),this());
  RemoveObject();
  return(0);
