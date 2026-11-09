/*-- Mond --*/

#strict

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
  Local(0) = 10;
  Local(1) = 10;
  // Glühen
  glow=CreateObject(LGFL,0,0,-1);
  glow->Init(this,0,0,100,RGB(70,70,70));
  SetCategory(19922945,glow);

  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=daytime*360/TimeResolution();
  iX=400-Sin(iAngle,(LandscapeWidth()/30+400));
  iY=300+Cos(iAngle,300)-75;
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
  SetClrModulation(RGBa(255,255,255,dayhelligkeit*4));
  // Neue Position ausrechnen
  var iX,iY,iAngle;
  iAngle=daytime*360/TimeResolution();
  iX=512-Sin(iAngle,BoundBy(LandscapeWidth()/30+300,300,512));
  iY=300+Cos(iAngle,300)+75;
  //DebugLog("Moon iX=%d iY=%d iTX=%d iTY=%d",GetX(),GetY(),iX,iY);
  // So bewegen das die Position in 35 Frames erreicht wird
  SetXDir(-(GetX()-iX),this,200);
  SetYDir(-(GetY()-iY),this,200);
  return(1);
}

public func SunRise()
{
  // eine Mondphase weiter
  ++phase;
  if (phase >= 12) phase = 0;
  var light=Abs(phase-6)*12;
  glow->Init(this,0,0,light*4,RGB(light,light,light));
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
      return Abs(pMoon->GetMoonPhase()-6)*6;
  }
}
