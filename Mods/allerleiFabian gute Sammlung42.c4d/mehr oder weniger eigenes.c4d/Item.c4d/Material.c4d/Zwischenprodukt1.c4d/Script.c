/*-- Plutonium1 --*/

#strict 2

local mlight;


public func RadioActiv()
{
if(!Random(10))
{
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), Random(40)+30);
}
}


func Initialize()
{
  // rufe die Effektfunktion in diesem Objekt alle fünf Frames auf
  AddEffect("Radiate", this, 1, 20, this);
TurnLightOn();
}

func FxRadiateTimer(pTarget, iEffectNumber, iTime)
{
  var maximum_distance = 120;
  // suche nach Zielen und mache denen dann Schaden je nachdem wie weit sie weg sind
  for (var obj in FindObjects(Find_Distance(maximum_distance), Find_OCF(OCF_Alive),Find_Not(Find_Func("HatSchutzAnzugan"))))
  {
    var distance = Distance(GetX(), GetY(), obj->GetX(), obj->GetY());
    var distance_factor = 100 - (100 * distance / maximum_distance); // in Prozent!
    var damage = distance_factor * 8;
    DoEnergy(-damage, obj, true);
    AddEffect("Sparkle", obj, 1, 1, 0, GetID());
    if (!Random(4) && obj->~IsClonk()) Sound("Hurt*");
  }
}

public func Hit()
{
  Sound ("RockHit*");
}

// ein NoGet Objekt entsteht, welches für die Dauer des Umwandlungsprozesses weiterhin Schaden macht. Nach dem Prozess wird es vernichtet.  Heißt auch, dass Plutonium nur auf diese Weise zerstörbar sein darf
func Destruction(a,b,c,d,e,f)
{
Enter(Contained(),CreateObject(PL02,0,0,GetOwner()));
return(_inherited(a,b,c,d,e,f));
}



func FxSparkleTimer(pTarget, iEffectNumber, iTime)
{
  if (iTime > 1 * 1) return -1;
  pTarget->CastParticles("PxSpark", 30, 50, 0, 0, 10, 50, RGB(0, 100, 0), RGB(100, 255, 100));
}

func Incineration()
{
  // Explosion mit Verstrahlung
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), Random(150)+80);
  Explode(15);
  return(1);
}


private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(150,235,10,70);
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