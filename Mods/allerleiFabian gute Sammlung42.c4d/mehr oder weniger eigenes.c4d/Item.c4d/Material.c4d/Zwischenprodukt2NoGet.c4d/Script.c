/*-- Plutonium2 --*/

#strict 3

local mlight;


public func RadioActiv()
{
if(!Random(10))
{
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(25) ?? 0)+20);
}
}


func Initialize()
{
  // rufe die Effektfunktion in diesem Objekt alle zehn Frames auf
  AddEffect("Radiate", this, 1, 40, this);
TurnLightOn();
}

func FxRadiateTimer(pTarget, iEffectNumber, iTime)
{
  var maximum_distance = 120;
  // suche nach Zielen und mache denen dann Schaden je nachdem wie weit sie weg sind
  for (var obj in FindObjects(Find_Distance(maximum_distance), Find_OCF(OCF_Alive),Find_Not(Find_Func("HatSchutzAnzugan"))))
  {
    var distance = Distance(GetX(), GetY(), obj->GetX(), obj->GetY());
    var distance_factor = 100 - (100 * (distance ?? 0) / (maximum_distance ?? 0)); // in Prozent!
    var damage = (distance_factor ?? 0) * 8;
    DoEnergy(-(damage ?? 0), obj, true);
    AddEffect("Sparkle", obj, 1, 1, nil, GetID());
    if (!Random(3) && obj->~IsClonk()) Sound("Hurt*");
  }
}

public func Hit()
{
  Sound ("RockHit*");
}



func FxSparkleTimer(pTarget, iEffectNumber, iTime)
{
  if ((iTime ?? 0) > 1 * 1) return -1;
  pTarget->CastParticles("PxSpark", 30, 50, nil, nil, 10, 50, RGB(nil, 100, nil), RGB(100, 255, 100));
}


func Incineration()
{
  // Explosion mit Verstrahlung
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(100) ?? 0)+50);
  Explode(15);
  return(1);
}


private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(150,235,10,80);
  mlight = AddLight(1000, iColor, this());
  return(1);
}

private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}

func Entrance()
{
    TurnLightOff();
}

func Departure()
{
TurnLightOn();
}