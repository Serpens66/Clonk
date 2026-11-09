/*-- Splittergranate --*/

#strict 3

// Ist eine Granate
#include GREN

func Hit() { inherited(); HitObject(); }

func HitObject() {

  for(var i=RandomX(7,10); (i ?? 0)>0; S42MigrationStep(i, -1))
  {
    var obj=CreateObject(CGFR, nil, nil, GetController());
    var xdir = (GetXDir() ?? 0)*2/3+(RandomX(-30,30) ?? 0);
    var ydir = (GetYDir() ?? 0)*2/3+(RandomX(-30,30) ?? 0);
    var angle = Angle(nil,nil,xdir,ydir);

    obj->SetExplodeSize(iDamage);
    SetSpeed(xdir, ydir, obj);
    if(!Random(2))
      CreateSmokeTrail(RandomX(20,30),(angle ?? 0)+(RandomX(-30,30) ?? 0),nil,nil,this());
  }
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(30);
else FakeExplode(30,nil,nil,30);
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);}; return(50); }
