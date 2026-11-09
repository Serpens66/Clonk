#strict
#include SHT1

local glow, time;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  pTrail = CreateObject(TRL6,0,0,GetOwner());
  if(pTrail) {
    pTrail->Set(iSize+2,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  // effect
  time=time+1;
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  CreateParticle("PSpark",0,0,-GetXDir()/4,-GetYDir()/4,RandomX(100,200),
                 RGBa(64,32,255,100+50*GetActTime()/iTime),this());

  if(!Random(3) && time>3)  {
    var angle = RandomX(-30, 30);
    for(var i = 0 ; i < 5 ; i++)
    {
      CreateObject(LASR, 0,0, GetOwner())->Set(angle, 5, 80, 20, this(), this());
      angle += 72;
    }
  }

  return(_inherited());
}

/* Treffer */
private func HitObject(object pObject) {
    LaserStrike(pObject);
}

private func Hit() {
RemoveObject();
}


func LaserStrike(object pObj)
{
  // Schaden!
  DoDmg(40, DMG_Energy, pObj, 1);
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage, DMG_Bio, pObj, iPrec);
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(127,127-iPrg,255,iPrg*2));
}

private func GlowColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(70,50,255-iPrg,50));
}

public func TrailColor(int iATime) { return(Color(iATime)); }
