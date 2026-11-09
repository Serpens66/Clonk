/*-- Soundkulisse --*/

#strict 3

local soundset,interval,variation,falloff,globalsound;


/* Erstellung */

public func Set(string szSoundSet, int iInterval, int iVariation, int iFalloff)
{
  iInterval ??= 0;
  iVariation ??= 0;
  iFalloff ??= 0;
  soundset = szSoundSet;
  if(!soundset) soundset = "Wave*.ogg";

  interval = iInterval;
  //if(!interval) interval = 100;

  variation = iVariation;
  //if(!variation) variation = 40;
  
  falloff = iFalloff;

  if(iInterval)
    StartSoundEffect();
  else
    StartDuroSound();
}

/* Soundsteuerung */

protected func StartDuroSound()
{
  var f,g;
  if(S42MigrationEqual(falloff, -1))
  {
    f = nil;
    g = true;
  }
  else
  {
    f = falloff;
    g = nil;
  }
  Sound(soundset,g,this,nil,nil,+1,nil,f);
}

protected func StartSoundEffect()
{
  var f,g;
  if(S42MigrationEqual(falloff, -1))
  {
    f = nil;
    g = true;
  }
  else
  {
    f = falloff;
    g = nil;
  }
  Sound(soundset,g,this,nil,nil,nil,nil,f);

  AddEffect("IntDoSound",this,50,
                (interval ?? 0)+(RandomX(-(variation ?? 0),+(variation ?? 0)) ?? 0),
                this,nil,soundset);
}

public func FxIntDoSoundStart(object pTarget, int iEffectNumber, int iTemp,szsound)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = szsound;
  return 1;
}

public func FxIntDoSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  pTarget->StartSoundEffect();
  return -1;
}