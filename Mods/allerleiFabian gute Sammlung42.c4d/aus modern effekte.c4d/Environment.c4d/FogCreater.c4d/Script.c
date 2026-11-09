/*-- Nebliger Wind --*/

#strict 3

local vel,windvorher,beschleunigen;


/* Initialisierung */

protected func Initialize()
{
  SetPosition();
  vel = 30;

  //Nebel auf Landschaft plazieren
  for(var i = (LandscapeWidth() ?? 0)/10; (i ?? 0) > 0; S42MigrationStep(i, -1, true))
   DoFog((i ?? 0)*10);
}

/* Nebelerstellung */

protected func Timer()
{
  //Effektmanager beachten
  if(!GetEffectData(EFSM_Fog)) return;

  if((GetWind(nil,nil,1) ?? 0)<0)  // wenn windrichtung nach links, dann am rechten Rand erzeugen.
  DoFog(LandscapeWidth ());
  if((GetWind(nil,nil,1) ?? 0)>0)
  DoFog();

  if(!Random(100))
    if(!GetEffect("IntPush",this))
      AddEffect ("IntPush", this, 50, 1, this, GetID(), 35*(3+(Random(10) ?? 0)));
}

protected func DoFog(x)
{
  //Atmosphärischen Nebel an den Kartenrändern erstellen
  var grey = 100+(Random(155) ?? 0);

  var obj = this;
  if(!Random(15))
    obj = nil;
 
var erstellen = 110-(Abs(GetWind(nil,nil,1)) ?? 0); // je schneller der Wind (bis maximal 100), desto mehr partikel werden erzeugt.
  if(!Random((erstellen ?? 0)/10))
  CreateParticle ("Fog", x, Random(LandscapeHeight()), windvorher, nil, 300+(Random(600) ?? 0), RGBa(grey,grey,grey,Random(100)), obj, nil);
}

public func FxIntPushStart(object pTarget, int iEffectNumber, int iTemp, duration)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = duration;
}

public func FxIntPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var duration = EffectVar(nil,pTarget,iEffectNumber);

  if((iEffectTime ?? 0) >= (duration ?? 0)) return -1;

   beschleunigen = (((GetWind(nil,nil,1) ?? 0)/6)-(windvorher ?? 0));

  if((iEffectTime ?? 0) < (duration ?? 0)/3)
  {
    S42MigrationAssign(vel, "+=", 2);
    //PushParticles("Fog",+2);
      PushParticles("Fog",+(beschleunigen ?? 0));
  }
  else
  {
    S42MigrationStep(vel, -1, true);
    //PushParticles("Fog",-1);
    PushParticles("Fog",+(beschleunigen ?? 0));
  }
  windvorher = (GetWind(nil,nil,1) ?? 0)/6;
}

public func FxIntPushStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  vel = 30;
}


func Destruction()
{
ClearParticles("Fog");
}