#strict 3
#include SHT1

local glow;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(TRL5,nil,nil,GetOwner());
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
  CreateParticle("PSpark",nil,nil,-(GetXDir() ?? 0)/4,-(GetYDir() ?? 0)/4,RandomX(100,200),
                 RGBa(64,32,255,100+50*(GetActTime() ?? 0)/(iTime ?? 0)),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),nil,nil,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage, DMG_Bio, pObj, iPrec);
  return(1);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(127,127-(iPrg ?? 0),255,(iPrg ?? 0)*2));
}

private func GlowColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(70,50,255-(iPrg ?? 0),50));
}

public func TrailColor(int iATime) { iATime ??= 0;
    return(Color(iATime)); }
