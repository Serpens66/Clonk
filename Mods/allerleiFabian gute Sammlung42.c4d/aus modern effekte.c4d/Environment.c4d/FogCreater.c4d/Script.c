/*-- Nebliger Wind --*/

#strict 2

local vel,windvorher,beschleunigen;


/* Initialisierung */

protected func Initialize()
{
  SetPosition();
  vel = 30;

  //Nebel auf Landschaft plazieren
  for(var i = LandscapeWidth()/10; i > 0; i--)
   DoFog(i*10);
}

/* Nebelerstellung */

protected func Timer()
{
  //Effektmanager beachten
  if(!GetEffectData(EFSM_Fog)) return;

  if(GetWind(0,0,1)<0)  // wenn windrichtung nach links, dann am rechten Rand erzeugen.
  DoFog(LandscapeWidth ());
  if(GetWind(0,0,1)>0)
  DoFog();

  if(!Random(100))
    if(!GetEffect("IntPush",this))
      AddEffect ("IntPush", this, 50, 1, this, GetID(), 35*(3+Random(10)));
}

protected func DoFog(x)
{
  //Atmosphärischen Nebel an den Kartenrändern erstellen
  var grey = 100+Random(155);

  var obj = this;
  if(!Random(15))
    obj = 0;
 
var erstellen = 110-Abs(GetWind(0,0,1)); // je schneller der Wind (bis maximal 100), desto mehr partikel werden erzeugt.
  if(!Random(erstellen/10))
  CreateParticle ("Fog", x, Random(LandscapeHeight()), windvorher, 0, 300+Random(600), RGBa(grey,grey,grey,Random(100)), obj, false); 
}

public func FxIntPushStart(object pTarget, int iEffectNumber, int iTemp, duration)
{
  EffectVar(0,pTarget,iEffectNumber) = duration;
}

public func FxIntPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var duration = EffectVar(0,pTarget,iEffectNumber);

  if(iEffectTime >= duration) return -1;

   beschleunigen = ((GetWind(0,0,1)/6)-windvorher);

  if(iEffectTime < duration/3)
  {
    vel+=2;
    //PushParticles("Fog",+2);
      PushParticles("Fog",+beschleunigen);
  }
  else
  {
    vel--;
    //PushParticles("Fog",-1);
    PushParticles("Fog",+beschleunigen);
  }
  windvorher = GetWind(0,0,1)/6;
}

public func FxIntPushStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  vel = 30;
}


func Destruction()
{
ClearParticles("Fog");
}