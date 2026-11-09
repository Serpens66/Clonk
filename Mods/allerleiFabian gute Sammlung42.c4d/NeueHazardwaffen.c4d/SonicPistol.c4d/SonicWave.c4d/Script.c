#strict
#include SHT1

local glow;

/* Schuss */

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize)
{
  // Standardwerte setzen
  if(!iSize)    iSize = 8;
  if(!iGlowSize)
    iGlowSize = iSize;
  if(!iTrail)   iTrail = 300;
  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  // und zuweisen
  iSize = Min(iSize+2,GetDefWidth());
  iPrec = iDmgPrec;

  // Positionieren
  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;
  
  if(!iTime)
    return(RemoveObject());

  var self = this();

  if(!self) return();   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  if(GetXDir()>0)  SetAction("TravelRight");
  else SetAction("TravelRight");
  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  // Tolles Leuchten erzeugen
  
  SetObjDrawTransform(100+35*iGlowSize,0,0, 0,100+35*iGlowSize,0, this(),1);
  SetClrModulation(GlowColor(1),this(),1);

  AddEffect("HitCheck", this(), 1,1, 0, GetID(),shooter);
}

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  pTrail = CreateObject(TRL5,0,0,-1);
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
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(pObject==shooter) return();
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),0,0,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
  if(pObj!=shooter)  // nicht sich selbst treffen!
 {
    Fling(pObj,GetXDir()/50,-4); Sound("DePressurize"); }
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


func TurnLightOn()
{
  var iColor;
  iColor = RGBa(150,50,190,50);
  mlight = AddLight3(300, iColor, this());
  return(1);
}