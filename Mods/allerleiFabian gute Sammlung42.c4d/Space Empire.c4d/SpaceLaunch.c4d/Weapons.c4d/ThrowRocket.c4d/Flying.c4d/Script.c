/*-- fliegende Rakete --*/

#strict

local dir;


/* Start */

public func Init(direction)
{
  dir = direction;
  SetAction("Init");
  SetPlrView(GetOwner(),this());
}

private func Launch()
{
  // auch im Freien?
  if (Contained())
  {
    SetAction("Idle");
    ChangeDef(STRK);
    return(1);
  }
  // Rakete starten
  var speed = 40+Random(11);
  SetXDir(dir*2*speed - speed);
  SetYDir(-speed);
  SetRDir(0);
  if (dir) SetR(+45);
      else SetR(-45);
}


/* Fliegt */

private func CheckTargets()
{
  // Flugrichtung setzen
  var angle = Angle(0,0,GetXDir(),GetYDir());
  SetR(angle);
  Smoke(0,3,1);
  // Beschleunigung
  SetXDir(GetXDir() + Sin(angle,5)/2);
  SetYDir(GetYDir() - Cos(angle,5)/2);
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(0,1,0,0,0, OCF_Exclusive()))                    // Gebäude
    if (Hostile(GetOwner(obj), GetOwner()))
      return(Hit());
  if (obj = FindObject(0,1,0,0,0, OCF_Entrance(),0,0,NoContainer()))   // Fahrzeuge
    if (GetCategory(obj) & C4D_Vehicle())
      if (Hostile(GetOwner(obj), GetOwner()))
        return(Hit());
}


/* Explosion */

protected func Hit()
{
  var obj = CreateObject(SFRX,0,0,GetOwner());
  obj->Settings(1, 40);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(30+Random(6));
else FakeExplode(30+Random(6),0,0,30+Random(6));
}

public Destroy: return(Hit());


/* Eigenschaften */

public IsProjectile: return(1);

