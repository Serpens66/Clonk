#strict
#include SHST

local glow;
local mlight;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  pTrail = CreateObject(TRAI,0,0,GetOwner());
  if(pTrail) {
    pTrail->Set(iSize+2,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  CreateParticle("PSpark",0,0,-GetXDir()/4,-GetYDir()/4,RandomX(100,200),
                 RGBa(64,32,255,100+50*GetActTime()/iTime),this());
    if(!Random(5)) LaserLight(RandomX(30,50), Color(GetActTime()),0,0,10);
    if(!Random(4)) LaserLight(RandomX(30,50), Color(GetActTime()),0,0,10);
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
      BlastObjects(GetX(), GetY(), iDamage/2);
      BlastObjects(GetX(), GetY(), iDamage/2);
    Remove();
  }
  FakeExplode(iDamage,0,0,iDamage);
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
