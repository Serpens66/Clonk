/*-- Nebelbeweger --*/

#strict

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
{AddEffect ("IntNebelPush", this(), 50, 1, this(), GetID(), 35*(3+Random(10)));}
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
  EffectVar(0,pTarget,iEffectNumber) = duration;
}

public func FxIntNebelPushTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var duration = EffectVar(0,pTarget,iEffectNumber);

  if(iEffectTime >= duration) return -1;

   beschleunigeen = ((GetWind(0,0,1)/10)-windvorherr);

  if(iEffectTime < duration/3)
  {
    vel+=2;
      PushParticles("Nebel",+beschleunigeen);
      PushParticles("Nebel2",+beschleunigeen);
  }
  else
  {
    vel--;
    PushParticles("Nebel",+beschleunigeen);
    PushParticles("Nebel2",+beschleunigeen);
  }
  windvorherr = GetWind(0,0,1)/10;
 //if(!Random(100)) Log(Format("beschleunigen: %v",beschleunigeen));
}

public func FxIntNebelPushStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
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
