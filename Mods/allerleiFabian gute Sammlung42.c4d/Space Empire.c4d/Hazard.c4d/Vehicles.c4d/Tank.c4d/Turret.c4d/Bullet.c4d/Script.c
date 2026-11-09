#strict 3

local shooter;
local explo;
local nospeed;

/* Bullet */

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
	
  SetExplo(30);
}

public func Launch(int xdir, int ydir)
{
  xdir ??= 0;
  ydir ??= 0;
  AddEffect("HitCheck", this(), 1, 1, nil, SHT1,shooter,true);
  SetSpeed(xdir, ydir);
  SetAction("Travel");
}

public func SetExplo(int ex) {
  ex ??= 0;
  explo = ex;
}

public func Hit() {
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(explo);
else FakeExplode(explo,nil,nil,explo);
}

public func HitObject(obj) { Hit(); }

func Traveling () {
  SetR(Angle(nil,nil,GetXDir(),GetYDir()));
  
  // wie, liegt das rum?! kaputtmachen
  if(And(!GetXDir(), !GetYDir())) {
	if(!nospeed) {
		nospeed = true;
	}
	else {
		Hit();
	}
  } else {
	nospeed = nil;
  }

  CreateParticle("Smoke2", nil, nil, RandomX(-15, 15), -5,
                           RandomX(80, 160), RGBa(100,100,100,60)); 
}