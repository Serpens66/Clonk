/*-- Nebelbeweger --*/

#strict 3

local windvorherr,beschleunigeen;
local vel;  // überflüssiges überbleibsel

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  return(1);
}



/* Eventuell Nebel bewegen */
private func BewegNebel()
{
  if(!Random(100))
if(!GetEffect("IntNebelPush",this()))
{AddEffect ("IntNebelPush", this(), 50, 1, this(), GetID(), 35*(3+(Random(10) ?? 0)));}
}


// eventuell so ändern, dass schon bei windstärkenberechnung der Wind durch 2 geteilt wird, so würde man eine halbe geschwindigkeit erreichen



func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Effekt der die Nebelpartikel je nach Windstärke bewegt

public func FxIntNebelPushStart(object pTarget, int iEffectNumber, int iTemp, duration)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = duration;
}

public func FxIntNebelPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var duration = EffectVar(nil,pTarget,iEffectNumber);

  if((iEffectTime ?? 0) >= (duration ?? 0)) return -1;

   beschleunigeen = (((GetWind(nil,nil,1) ?? 0)/10)-(windvorherr ?? 0));

  if((iEffectTime ?? 0) < (duration ?? 0)/3)
  {
    S42MigrationAssign(vel, "+=", 2);
      PushParticles("Nebel",+(beschleunigeen ?? 0));
      PushParticles("Nebel2",+(beschleunigeen ?? 0));
  }
  else
  {
    S42MigrationStep(vel, -1, true);
    PushParticles("Nebel",+(beschleunigeen ?? 0));
    PushParticles("Nebel2",+(beschleunigeen ?? 0));
  }
  windvorherr = (GetWind(nil,nil,1) ?? 0)/10;
 //if(!Random(100)) Log(Format("beschleunigen: %v",beschleunigeen));
}

public func FxIntNebelPushStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  vel = 30;
}




/*

public func FxIntNebelPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
   beschleunigeen = (GetWind(0,0,1)-windvorherr);
    PushParticles("Nebel",+beschleunigeen);
    PushParticles("Nebel2",+beschleunigeen/4);
  windvorherr = GetWind(0,0,1);
 if(!Random(100)) Log(Format("beschleunigen: %v",beschleunigeen));
}

*/
