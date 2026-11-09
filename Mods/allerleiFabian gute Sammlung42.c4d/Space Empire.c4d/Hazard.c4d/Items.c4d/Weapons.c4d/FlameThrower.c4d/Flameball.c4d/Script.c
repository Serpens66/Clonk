/*-- Flammenfeuerball der Verdammnis!! --*/

#strict

#include FFLM

func Initialize() {
  AddLight(400,RGBa(255,160,100,50),this());
  Sound("Fire", 0,0, 75, 0, 1);
  Incinerate(); 
}

protected func Flying() {
//  SetXDir(GetXDir()*9/10);
  SetYDir(GetYDir(0,1000)+GetGravity()*2,0,1000);

  for(var x; x < 2+Random(3); x++)
    CreateObject(FFLM,0,1)->Launch(GetXDir()/5,GetYDir()/5,0, iDamage);

  if(GetActTime() > 50)
    Release();
}

protected func Hit() {
  Release();
}

protected func Release() {
  for(var x=10+Random(5); x > 0; x--) {
    CreateParticle("FrSprk",0,0,GetXDir()+RandomX(-15,15),RandomX(-15,15),50+Random(30),RGB(70+Random(20),30+Random(20),0));
    CreateParticle("PxSpark",0,0,GetXDir()+RandomX(-15,15),RandomX(-15,15),50+Random(30),RGB(70+Random(20),30+Random(20),0));
  }
  for(var x; x < 10+Random(5); x++) {
    CreateObject(FFLM,0,-2)->Launch(RandomX(-25,25),RandomX(-25,25),0, iDamage);
  }
  CreateObject(FFLM,0,-1)->Launch(0,0,0,iDamage);
  Sound("Inflame");
  RemoveObject();
}

protected func HitObject(object pObj) {
//  Explode(20,CreateObject(WIPF));
//  AddFireEffect(pObj,100);
  DoDmg(40, DMG_Fire, pObj);
  CreateParticle("Blast",0,0,0,0,100);
  Release();
}

protected func CastFireEffects(int time) {
  CreateParticle("FireBlast",0,0,0,-Random(2),50+Random(30),RGB(70+Random(20),50+Random(30),0),this());
  CreateParticle("FrSprk",0,0,0,-Random(2),100+Random(30),RGB(70+Random(20),50+Random(30),0));
  CastParticles("PxSpark",5+Random(3),30+Random(20),0,0,40,80,RGB(70,50),RGB(100,80));
}
