/* Mörser*/

#strict
  local iDamage, exploding, sound;
  
public func Initialize()
{
  sound=0;
  iDamage = 20;
  SetAction("Travel");
  AddLight(100,RGB(255,127,0),this(),GLOW);
  AddEffect("HitCheck", this(), 1,1, 0, SHT1,GetOwner());
}

private func Traveling()
{
  var aspeed=Sqrt(GetXDir()*GetXDir()+GetYDir()*GetYDir());
  var phi=ArcSin(GetXDir(),aspeed); 
  SetR(phi);
  Smoking();
  SetPlrView(GetOwner(), this());
  if(GetYDir()>40 && !sound)
  {
  Sound("Mortar");
  sound=1;
  }
  if(!PathFree(GetX(), GetY(), GetX()+GetXDir(),GetY()+GetYDir())) Hit();
}

private func Smoking() {

  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,20);
    var ydir = -Cos(GetR()+rand,20);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
  }
}
  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  for(var i=RandomX(7,10); i>0; --i)
  {
    var obj=CreateObject(CGFR, 0, 0, GetController());
    var xdir = GetXDir()*1/4+RandomX(-40,40);
    var ydir = GetYDir()*1/4+RandomX(-40,40);
    var angle = Angle(0,0,xdir,ydir);

    obj->SetExplodeSize(iDamage);
    SetSpeed(xdir, ydir, obj);
    if(!Random(2))
      CreateSmokeTrail(RandomX(20,30),angle+RandomX(-30,30),0,0,this());
  }
  Explode(20);
}

public func Damage() {
  if(GetDamage() > 20 && !exploding)
    Hit();
}

public func EMPShock() {
  SetAction("Idle");
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }
