/*-- Sonne --*/

#strict

local glow,glow2;

protected func Initialize()
{
  SetAction("Shine");
  // Parallaxität: 97%
  Local(0) = 3;
  Local(1) = 3;
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=(daytime*360/TimeResolution())+180;
  iX=512-Sin(iAngle,BoundBy(LandscapeWidth()/30+300,300,512));
  iY=300+Cos(iAngle,300)+75;
  SetPosition(iX,iY);
  SetObjectBlitMode(GFX_BLIT_Additive);
  // Glühen
  glow=CreateObject(LGFL,0,0,-1);
  glow->Init(this,0,0,400,RGB(70,70,70),false,true);
  SetCategory(19922945,glow);
  glow2=CreateObject(LGFL,0,0,-1);
  glow2->Init(this,0,0,100,RGB(250,250,250),false,true);
  SetCategory(19922945,glow2);
  SetR(Random(360));
  SetRDir(1);
  return(1);
}

public func CheckLight()
{
  // Transparenz neu setzen
  SetClrModulation(RGBa(255,BoundBy(155+dayhelligkeit*2,0,255),BoundBy(155+dayhelligkeit*2,0,255),BoundBy(255-dayhelligkeit*5,0,255)));
  glow->Init(this,0,0,dayhelligkeit*8,RGBa(70,20+dayhelligkeit/2,20+dayhelligkeit/2,BoundBy(255-dayhelligkeit*4,0,255)),false,true);
  glow2->Init(this,0,0,dayhelligkeit*3,RGBa(255,200+dayhelligkeit/2,200+dayhelligkeit/2,BoundBy(255-dayhelligkeit*4,0,255)),false,true);
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=(daytime*360/TimeResolution())+180;
  iX=512-Sin(iAngle,BoundBy(LandscapeWidth()/30+300,300,512));
  iY=300+Cos(iAngle,300)+75;
  // So bewegen das die Position in 35 Frames erreicht wird
  SetXDir(-(GetX()-iX),this,200);
  SetYDir(-(GetY()-iY),this,200);
  return(1);
}

public func IsLight()
{
  return true;
}
