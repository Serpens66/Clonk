/*-- Flammenfeuerball der Verdammnis!! --*/

#strict 3

#include _FLM

func Initialize() {
  AddLight(400,RGBa(255,160,100,50),this());
  Sound("Fire", nil,nil, 75, nil, 1);
  Incinerate(); 
}

protected func Flying() {
//  SetXDir(GetXDir()*9/10);
  SetYDir((GetYDir(nil,1000) ?? 0)+(GetGravity() ?? 0)*2,nil,1000);

  for(var x; (x ?? 0) < 2+(Random(3) ?? 0); S42MigrationStep(x, 1, true))
    CreateObject(_FLM,nil,1)->Launch((GetXDir() ?? 0)/5,(GetYDir() ?? 0)/5,nil, iDamage);

  if((GetActTime() ?? 0) > 50)
    Release();
}

protected func Hit() {
  Release();
}

protected func Release() {
  for(var x=10+(Random(5) ?? 0); (x ?? 0) > 0; S42MigrationStep(x, -1, true)) {
    CreateParticle("FrSprk",nil,nil,(GetXDir() ?? 0)+(RandomX(-15,15) ?? 0),RandomX(-15,15),50+(Random(30) ?? 0),RGB(70+(Random(20) ?? 0),30+(Random(20) ?? 0),nil));
    CreateParticle("PxSpark",nil,nil,(GetXDir() ?? 0)+(RandomX(-15,15) ?? 0),RandomX(-15,15),50+(Random(30) ?? 0),RGB(70+(Random(20) ?? 0),30+(Random(20) ?? 0),nil));
  }
  for(var x; (x ?? 0) < 10+(Random(5) ?? 0); S42MigrationStep(x, 1, true)) {
    CreateObject(_FLM,nil,-2)->Launch(RandomX(-25,25),RandomX(-25,25),nil, iDamage);
  }
  CreateObject(_FLM,nil,-1)->Launch(nil,nil,nil,iDamage);
  Sound("Inflame");
  RemoveObject();
}

protected func HitObject(object pObj) {
//  Explode(20,CreateObject(WIPF));
//  AddFireEffect(pObj,100);
  DoDmg(40, DMG_Fire, pObj);
  CreateParticle("Blast",nil,nil,nil,nil,100);
  Release();
}

protected func CastFireEffects(int time) {
  time ??= 0;
  CreateParticle("FireBlast",nil,nil,nil,-(Random(2) ?? 0),50+(Random(30) ?? 0),RGB(70+(Random(20) ?? 0),50+(Random(30) ?? 0),nil),this());
  CreateParticle("FrSprk",nil,nil,nil,-(Random(2) ?? 0),100+(Random(30) ?? 0),RGB(70+(Random(20) ?? 0),50+(Random(30) ?? 0),nil));
  CastParticles("PxSpark",5+(Random(3) ?? 0),30+(Random(20) ?? 0),nil,nil,40,80,RGB(70,50),RGB(100,80));
}
