#strict 3
#include SHT1

local glow;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(255,20,20),this(),GLOW);
  
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(TRAI,nil,nil,-1);
  if(pTrail) {
    pTrail->Set((iSize ?? 0)+2,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  //CreateParticle("PSpark",0,0,-GetXDir()/4,-GetYDir()/4,RandomX(100,200)*GetCon()/100,
  //               RGBa(64,32,255,100+50*GetActTime()/iTime),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(!!SEqual(GetAction(pObject), "Dead")) return(1);
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),nil,nil,10);
    Remove();
  }
}

public func BulletStrike(object pObj) 
{
  if(pObj)
  {
   pObj->~Zated();
  }
  return(1);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(nil,255,nil,(iPrg ?? 0)*2));
}

private func GlowColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(nil,255,nil,130));
}

public func TrailColor(int iATime) { iATime ??= 0;
    return(Color(iATime)); }
