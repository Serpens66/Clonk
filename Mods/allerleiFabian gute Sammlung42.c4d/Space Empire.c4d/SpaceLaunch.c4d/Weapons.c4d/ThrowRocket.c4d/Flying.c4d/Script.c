/*-- fliegende Rakete --*/

#strict 3

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
  var speed = 40+(Random(11) ?? 0);
  SetXDir((dir ?? 0)*2*(speed ?? 0) - (speed ?? 0));
  SetYDir(-(speed ?? 0));
  SetRDir(nil);
  if (dir) SetR(+45);
      else SetR(-45);
}


/* Fliegt */

private func CheckTargets()
{
  // Flugrichtung setzen
  var angle = Angle(nil,nil,GetXDir(),GetYDir());
  SetR(angle);
  Smoke(nil,3,1);
  // Beschleunigung
  SetXDir((GetXDir() ?? 0) + (Sin(angle,5) ?? 0)/2);
  SetYDir((GetYDir() ?? 0) - (Cos(angle,5) ?? 0)/2);
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Exclusive))                    // Gebäude
    if (Hostile(GetOwner(obj), GetOwner()))
      return(Hit());
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Entrance,nil,nil,NoContainer()))   // Fahrzeuge
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      if (Hostile(GetOwner(obj), GetOwner()))
        return(Hit());
}


/* Explosion */

protected func Hit()
{
  var obj = CreateObject(SFRX,nil,nil,GetOwner());
  obj->Settings(1, 40);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(30+(Random(6) ?? 0));
else FakeExplode(30+(Random(6) ?? 0),nil,nil,30+(Random(6) ?? 0));
}

public func Destroy(...) { return(Hit());
}


/* Eigenschaften */

public func IsProjectile(...) { return(1);
}

