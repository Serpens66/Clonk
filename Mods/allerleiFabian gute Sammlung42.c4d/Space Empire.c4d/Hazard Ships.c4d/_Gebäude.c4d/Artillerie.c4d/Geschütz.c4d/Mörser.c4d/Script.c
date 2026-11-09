/* Mörser*/

#strict 3
  local iDamage, exploding, sound;
  
public func Initialize()
{
  sound=nil;
  iDamage = 20;
  SetAction("Travel");
  AddLight(100,RGB(255,127,nil),this(),GLOW);
  AddEffect("HitCheck", this(), 1,1, nil, SHT1,GetOwner());
}

private func Traveling()
{
  var aspeed=Sqrt((GetXDir() ?? 0)*(GetXDir() ?? 0)+(GetYDir() ?? 0)*(GetYDir() ?? 0));
  var phi=ArcSin(GetXDir(),aspeed); 
  SetR(phi);
  Smoking();
  SetPlrView(GetOwner(), this());
  if(And((GetYDir() ?? 0)>40, !sound))
  {
  Sound("Mortar");
  sound=1;
  }
  if(!PathFree(GetX(), GetY(), (GetX() ?? 0)+(GetXDir() ?? 0),(GetY() ?? 0)+(GetYDir() ?? 0))) Hit();
}

private func Smoking() {

  var dist = Distance(nil,nil,GetXDir(),GetYDir());
  var maxx = +(Sin(GetR(),(dist ?? 0)/10) ?? 0);
  var maxy = -(Cos(GetR(),(dist ?? 0)/10) ?? 0);
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
  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  for(var i=RandomX(7,10); (i ?? 0)>0; S42MigrationStep(i, -1))
  {
    var obj=CreateObject(CGFR, nil, nil, GetController());
    var xdir = (GetXDir() ?? 0)*1/4+(RandomX(-40,40) ?? 0);
    var ydir = (GetYDir() ?? 0)*1/4+(RandomX(-40,40) ?? 0);
    var angle = Angle(nil,nil,xdir,ydir);

    obj->SetExplodeSize(iDamage);
    SetSpeed(xdir, ydir, obj);
    if(!Random(2))
      CreateSmokeTrail(RandomX(20,30),(angle ?? 0)+(RandomX(-30,30) ?? 0),nil,nil,this());
  }
  Explode(20);
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
