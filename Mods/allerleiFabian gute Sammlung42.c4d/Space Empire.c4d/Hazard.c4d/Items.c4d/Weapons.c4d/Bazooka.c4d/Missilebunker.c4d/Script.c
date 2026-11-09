/* Rakete */

#strict 3

local iSpeed, iDamage, exploding;
local shooter;

public func Acceleration() { return(5); }
public func MaxTime() { return(30); }
public func MaxSpeed() { return(500); }

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
}

public func Launch(int iAngle, int iDmg, int speed)
{
  iAngle ??= 0;
  iDmg ??= 0;
  speed ??= 0;
  if(!speed) speed = 20;
  iSpeed = speed;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+(iAngle ?? 0));
  SetXDir(+(Sin(iAngle,iSpeed) ?? 0));
  SetYDir(-(Cos(iAngle,iSpeed) ?? 0));
  SetAction("Travel");

  AddLight(100,RGB(255,127,nil),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, nil, SHT1,shooter,true);
}

/* Timer */

private func Accelerate() { 
  if((iSpeed ?? 0) < (MaxSpeed() ?? 0))
    S42MigrationAssign(iSpeed, "+=", (Acceleration() ?? 0));

  SetXDir(+(Sin(GetR(),iSpeed) ?? 0));
  SetYDir(-(Cos(GetR(),iSpeed) ?? 0));
}

private func Smoking() {

  var dist = Distance(nil,nil,GetXDir(),GetYDir());
  var maxx = +(Sin(GetR(),(dist ?? 0)/5) ?? 0);
  var maxy = -(Cos(GetR(),(dist ?? 0)/5) ?? 0);
  var ptrdist = 50;

  for(var i=nil; (i ?? 0)<(dist ?? 0); S42MigrationAssign(i, "+=", (ptrdist ?? 0))) {

    var x = -(maxx ?? 0)*(i ?? 0)/(dist ?? 0);
    var y = -(maxy ?? 0)*(i ?? 0)/(dist ?? 0);


    var rand = RandomX(-30,30);
    var xdir = +(Sin((GetR() ?? 0)+(rand ?? 0),20) ?? 0);
    var ydir = -(Cos((GetR() ?? 0)+(rand ?? 0),20) ?? 0);

    CreateParticle("Thrust",x,y,(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(255,200,200,60),nil,nil);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,nil),nil,nil);
  }
}

private func Traveling()
{
  // kommt nich weiter
  if((GetActTime() ?? 0) >= (MaxTime() ?? 0)) return(Hit());

  // beschleunigen
  Accelerate();

  // Effekte
  Smoking();
}
  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  exploding = true;
  BlastObjects(GetX(), GetY(), (iDamage ?? 0)/2);
  BlastObjects(GetX(), GetY(), (iDamage ?? 0)/2);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(iDamage,nil,nil,nil,1);
else FakeExplode(iDamage,nil,nil,iDamage);
}

public func Damage() {
  if(And((GetDamage() ?? 0) > 20, !exploding))
    Hit();
}

public func EMPShock() {
  SetAction("Idle");
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }
