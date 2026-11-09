/*-- Sonne --*/

#strict 3

local glow,glow2;

protected func Initialize()
{
  SetAction("Shine");
  // Parallaxität: 97%
  Local(nil) = 3;
  Local(1) = 3;
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=((daytime ?? 0)*360/(TimeResolution() ?? 0))+180;
  iX=512-(Sin(iAngle,BoundBy((LandscapeWidth() ?? 0)/30+300,300,512)) ?? 0);
  iY=300+(Cos(iAngle,300) ?? 0)+75;
  SetPosition(iX,iY);
  SetObjectBlitMode(GFX_BLIT_Additive);
  // Glühen
  glow=CreateObject(LGFL,nil,nil,-1);
  glow->Init(this,nil,nil,400,RGB(70,70,70),nil,true);
  SetCategory(19922945,glow);
  glow2=CreateObject(LGFL,nil,nil,-1);
  glow2->Init(this,nil,nil,100,RGB(250,250,250),nil,true);
  SetCategory(19922945,glow2);
  SetR(Random(360));
  SetRDir(1);
  return(1);
}

public func CheckLight()
{
  // Transparenz neu setzen
  SetClrModulation(RGBa(255,BoundBy(155+(dayhelligkeit ?? 0)*2,nil,255),BoundBy(155+(dayhelligkeit ?? 0)*2,nil,255),BoundBy(255-(dayhelligkeit ?? 0)*5,nil,255)));
  glow->Init(this,nil,nil,(dayhelligkeit ?? 0)*8,RGBa(70,20+(dayhelligkeit ?? 0)/2,20+(dayhelligkeit ?? 0)/2,BoundBy(255-(dayhelligkeit ?? 0)*4,nil,255)),nil,true);
  glow2->Init(this,nil,nil,(dayhelligkeit ?? 0)*3,RGBa(255,200+(dayhelligkeit ?? 0)/2,200+(dayhelligkeit ?? 0)/2,BoundBy(255-(dayhelligkeit ?? 0)*4,nil,255)),nil,true);
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=((daytime ?? 0)*360/(TimeResolution() ?? 0))+180;
  iX=512-(Sin(iAngle,BoundBy((LandscapeWidth() ?? 0)/30+300,300,512)) ?? 0);
  iY=300+(Cos(iAngle,300) ?? 0)+75;
  // So bewegen das die Position in 35 Frames erreicht wird
  SetXDir(-((GetX() ?? 0)-(iX ?? 0)),this,200);
  SetYDir(-((GetY() ?? 0)-(iY ?? 0)),this,200);
  return(1);
}

public func IsLight()
{
  return true;
}
