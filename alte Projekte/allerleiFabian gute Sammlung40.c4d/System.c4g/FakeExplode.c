/*-- Neues Script --*/

#strict

//Explode mit Effekt und tollen Extraparametern für performance-/effektmäßiges Zeug
global func FakeExplode(int iLevel, object pObj, bool bNoShots, iblast) {
  if(!pObj)
    if(!(pObj=this()))
      return();

  var x = GetX(pObj)-GetX();
  var y = GetY(pObj)-GetY();

  // Sound
  var grade = BoundBy((iLevel/10)-1,1,3);
  Sound(Format("Blast%d",grade),0,pObj,100);

  
  // Effects
  ShakeViewPort(iLevel*8, pObj);
//////////
  var i=0, count = 3+iLevel/8, angle = Random(360);
  while(count > 0 && ++i < count*10) {
    angle += RandomX(40,80);
  
    if(!bNoShots)
      if(GetName(0,SHT1)) { //Gibt es das Objekt? (HAX)
        var ammo = CreateObject(SHT1,x,y,GetController(pObj));
        ammo->Launch(angle,100+iLevel*3,0,4);
      }
    //Rauch
    var smokex = +Sin(angle,RandomX(iLevel/4,iLevel/2));
    var smokey = -Cos(angle,RandomX(iLevel/4,iLevel/2));
    if(GBackSolid(x+smokex,y+smokey))
      continue;
     var level = iLevel + Random(iLevel/5);
    CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    count--;
  }

  CreateParticle("Blast",x,y,0,0,iLevel*10,RGBa(255,200,100,50));
//  CastParticles("FSpark",iLevel/5+1,iLevel,x,y,10*iLevel/2,10*iLevel,RGBa(239,0,0,0),RGBa(255,16,16,255));

  CreateParticle("Smoke3", x, y, 0, 0, iLevel*10, RGBa(75,65,55,20));
  CreateBurnMark(x,y,iLevel,20+iLevel/2);
  /////////////
CreateParticle("Blast", x,y, 0,0, iLevel*10);
      
       CastParticles("FSpark", iLevel/5+1, iLevel, x,y, iLevel*5+10,iLevel*10+10, 0x00ef0000,0xffff1010);
  // wenn iblast, dann tatsächlich explosionsschaden machen, der aber keine erde zerstört
var ix,iy,owner;
owner= GetOwner(pObj);
ix=GetX(pObj);
iy=GetY(pObj);
  // Objekt löschen
  RemoveObject(pObj);
// jetzt an der stelle wo es war alles außer Erde in die Luft jagen (erst wenn objekt weg ist, da es sich sonst auch nochmal selbst trifft)
if(iblast)
BlastObjects(ix,iy,iblast,0,owner+1);
}
