/*-- Neues Script --*/

#strict 3

//Explode mit Effekt und tollen Extraparametern für performance-/effektmäßiges Zeug
global func FakeExplode(int iLevel, object pObj, bool bNoShots, iblast) {
  iLevel ??= 0;
  bNoShots ??= false;
  if(!pObj)
    if(!(pObj=this()))
      return;

  var x = (GetX(pObj) ?? 0)-(GetX() ?? 0);
  var y = (GetY(pObj) ?? 0)-(GetY() ?? 0);

  // Sound
  var grade = BoundBy(((iLevel ?? 0)/10)-1,1,3);
  Sound(Format("Blast%d",grade),nil,pObj,100);

  
  // Effects
  ShakeViewPort((iLevel ?? 0)*8, pObj);
//////////
  var i=nil, count = 3+(iLevel ?? 0)/8, angle = Random(360);
  while(And((count ?? 0) > 0, S42MigrationStep(i, 1) < (count ?? 0)*10)) {
    S42MigrationAssign(angle, "+=", (RandomX(40,80) ?? 0));
  
    if(!bNoShots)
      if(GetName(nil,SHT1)) { //Gibt es das Objekt? (HAX)
        var ammo = CreateObject(SHT1,x,y,GetController(pObj));
        ammo->Launch(angle,100+(iLevel ?? 0)*3,nil,4);
      }
    //Rauch
    var smokex = +(Sin(angle,RandomX((iLevel ?? 0)/4,(iLevel ?? 0)/2)) ?? 0);
    var smokey = -(Cos(angle,RandomX((iLevel ?? 0)/4,(iLevel ?? 0)/2)) ?? 0);
    if(GBackSolid((x ?? 0)+(smokex ?? 0),(y ?? 0)+(smokey ?? 0)))
      continue;
     var level = (iLevel ?? 0) + (Random((iLevel ?? 0)/5) ?? 0);
    CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    S42MigrationStep(count, -1, true);
  }

  CreateParticle("Blast",x,y,nil,nil,(iLevel ?? 0)*10,RGBa(255,200,100,50));
//  CastParticles("FSpark",iLevel/5+1,iLevel,x,y,10*iLevel/2,10*iLevel,RGBa(239,0,0,0),RGBa(255,16,16,255));

  CreateParticle("Smoke3", x, y, nil, nil, (iLevel ?? 0)*10, RGBa(75,65,55,20));
  CreateBurnMark(x,y,iLevel,20+(iLevel ?? 0)/2);
  /////////////
CreateParticle("Blast", x,y, nil,nil, (iLevel ?? 0)*10);
      
       CastParticles("FSpark", (iLevel ?? 0)/5+1, iLevel, x,y, (iLevel ?? 0)*5+10,(iLevel ?? 0)*10+10, 0x00ef0000,0xffff1010);
  // wenn iblast, dann tatsächlich explosionsschaden machen, der aber keine erde zerstört
var ix,iy,owner;
owner= GetOwner(pObj);
ix=GetX(pObj);
iy=GetY(pObj);
  // Objekt löschen
  RemoveObject(pObj);
// jetzt an der stelle wo es war alles außer Erde in die Luft jagen (erst wenn objekt weg ist, da es sich sonst auch nochmal selbst trifft)
if(iblast)
BlastObjects(ix,iy,iblast,nil,(owner ?? 0)+1);
}
