#strict
#include SHT1

local glow;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  }


private func Traveling()
{
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  CreateParticle("PSpark",0,0,-GetXDir()/4,-GetYDir()/4,RandomX(100,200),
                 RGBa(255,32,64,100+50*GetActTime()/iTime),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),0,0,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)  {
    var clnk=CreateObject(CLNK);
    clnk->GrabContents(pObj);
    clnk->RemoveObject(0,1);
    Fling(pObj,GetXDir()/100,-2); 
    Sound("Electric");   
    }
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,127-iPrg,64,iPrg*2));
}

private func GlowColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255-iPrg,50,64,50));
}

public func TrailColor(int iATime) { return(Color(iATime)); }


func TurnLightOn()
{
  var iColor;
  iColor = RGBa(150,50,190,50);
  mlight = AddLight3(300, iColor, this());
  return(1);
}