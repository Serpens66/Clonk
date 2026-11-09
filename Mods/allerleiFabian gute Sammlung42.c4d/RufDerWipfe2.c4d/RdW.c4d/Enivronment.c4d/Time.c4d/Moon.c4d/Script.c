/*-- Mond --*/

#strict 3

local phase,glow;

protected func Initialize()
{
  // erstmal unsichtbar
  SetClrModulation(RGBa(128,128,128));
  SetAction("Shine");
  // Phase setzen
  phase = Random(12);
  SetPhase(phase);
  // Parallaxität: 90%
  Local(nil) = 10;
  Local(1) = 10;
  // Glühen
  glow=CreateObject(LGFL,nil,nil,-1);
  glow->Init(this,nil,nil,100,RGB(70,70,70));
  SetCategory(19922945,glow);

  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=(daytime ?? 0)*360/(TimeResolution() ?? 0);
  iX=400-(Sin(iAngle,((LandscapeWidth() ?? 0)/30+400)) ?? 0);
  iY=300+(Cos(iAngle,300) ?? 0)-75;
  SetPosition(iX,iY);
  
  return(1);
}

private func ResetPhase()
{
  SetPhase(phase);
  return(1);
}

public func CheckLight()
{
  // Transparenz neu setzen
  SetClrModulation(RGBa(255,255,255,(dayhelligkeit ?? 0)*4));
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=(daytime ?? 0)*360/(TimeResolution() ?? 0);
  iX=512-(Sin(iAngle,BoundBy((LandscapeWidth() ?? 0)/30+300,300,512)) ?? 0);
  iY=300+(Cos(iAngle,300) ?? 0)+75;
  //DebugLog("Moon iX=%d iY=%d iTX=%d iTY=%d",GetX(),GetY(),iX,iY);
  // So bewegen das die Position in 35 Frames erreicht wird
  SetXDir(-((GetX() ?? 0)-(iX ?? 0)),this,200);
  SetYDir(-((GetY() ?? 0)-(iY ?? 0)),this,200);
  return(1);
}

public func SunRise()
{
  // eine Mondphase weiter
  S42MigrationStep(phase, 1);
  if ((phase ?? 0) >= 12) phase = nil;
  var light=(Abs((phase ?? 0)-6) ?? 0)*12;
  glow->Init(this,nil,nil,(light ?? 0)*4,RGB(light,light,light));
  return(ResetPhase());
}

public func IsLight()
{
  return true;
}

public func GetMoonPhase()
{
  return phase;
}

global func GetMoonLighting()
{
  if(IsNight())
  {
    var pMoon;
    if(pMoon=FindObject(SMON))
      return (Abs((pMoon->GetMoonPhase() ?? 0)-6) ?? 0)*6;
  }
}
