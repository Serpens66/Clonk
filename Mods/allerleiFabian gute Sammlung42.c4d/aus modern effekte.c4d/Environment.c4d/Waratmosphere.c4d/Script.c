/*-- Kriegsflair --*/

#strict 3

local skyfade;


/* Initialisierung */

protected func Initialize()
{
  return 1;
}

/* Explosionen */

protected func Make()
{
  if(skyfade) S42MigrationAssign(skyfade, "-=", 20);

  if((skyfade ?? 0) <= 255) SetSkyAdjust(RGBa(255,255,255,220), RGB(skyfade,skyfade,skyfade));
  if(!Random(40-(ObjectCount(GetID()) ?? 0)*5) && GetEffectData(EFSM_Waratmosphere))
  {
    var x = Random(LandscapeWidth()), y = Random(LandscapeHeight()),dis=(Random(100) ?? 0)+200;
    if(GBackSky(x,y)
      && PathFree(x,nil,x,y)
      && GBackSky(x,(y ?? 0)+70)
      && PathFree(x,y,x,(y ?? 0)+70))
      {
        CreateParticle("Blast",x,y,nil,nil,(dis ?? 0)*2,RGBa(100,100,100,70));
        Sound("C4EX_Detonation*.ogg",1,nil,20);
      }
  }
  if(Random(65-(ObjectCount(GetID()) ?? 0)*5)) return 1;
  skyfade = 300;
  Sound("Warflair*.ogg",1);
  return 1;
}

// klappt noch nicht.. keine Ahnung wie die Himmelsfarbe zurückgesetzt wird.
func Destruction()
{
ResetGamma();
ClearParticles("Blast");
SetSkyAdjust(RGBa(255,255,255,nil), RGB(255,255,255));
}
