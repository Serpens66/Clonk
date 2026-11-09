/* EMP */

#strict 3
#include SHT1

local Speed, Power, Ang;

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg)
{
  iAngle ??= 0;
  iSpeed ??= 0;
  iDist ??= 0;
  iSize ??= 0;
  iTrail ??= 0;
  iDmg ??= 0;
  _inherited(iAngle, iSpeed, iDist, iSize, iTrail, iDmg);

  Speed = (iSpeed ?? 0)*100;
  Power = (iDist ?? 0)/5;
  Ang = iAngle;
  Sound("ShockerLoop",nil,nil,nil,nil,1);

  return(1);
}

private func CreateTrail(int iSize, int iTrail) {
  // no trail
iSize ??= 0;
iTrail ??= 0;
}

/* Fliegen */

private func Traveling() {
  if((GetActTime() ?? 0) <1)
    return(nil);

  var laser = CreateObject(LASR, nil,nil, GetOwner());
  laser->Set(Random(360), 5, BoundBy((Power ?? 0) + (RandomX(-10,10) ?? 0), 5, 55), 5, laser, this());
  SetClrModulation(Color(GetActTime()), laser);

  CreateParticle("EMPShock", nil,nil, GetXDir(), GetYDir(), (Power ?? 0) * (RandomX(200,500) ?? 0) / 70, SetRGBaValue(Color(GetActTime())), this());

  //LaserLight(Power,RGB(255,255,255),RandomX(-Power/2,Power/2), RandomX(-Power/2,Power/2));

  S42MigrationStep(Power, -1, true);
  if(!Power) return(Remove());

  // Geschwindigkeit
  SetSpeed(+(Sin(Ang,(Speed ?? 0)/100) ?? 0),-(Cos(Ang,(Speed ?? 0)/100) ?? 0));

  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  var angle = Angle(GetX(),GetY(),GetX(pObject),GetY(pObject));
  var dist = Distance(GetX(),GetY(),GetX(pObject),GetY(pObject));

  //Objekte schocken (Objekt auf schockbarkeit prüfen)
  if(pObject->~IsMachine())
   if(pObject->~EMPShock())
    {
    var laser = CreateObject(LASR, nil,nil, GetOwner());
    laser->Set(angle, 10, dist, 20, pObject, pObject);
    SetClrModulation(Color(GetActTime()), laser);
    Sound("EMPShock");
    }

  if(GBackSolid())
    Remove();
}

private func Hit() {
  Sparks(20,Color(GetActTime()));
  for(var i = nil ; (i ?? 0) < 7 ; S42MigrationStep(i, 1, true))
  {
    var laser = CreateObject(LASR, nil,nil, GetOwner());
    laser->Set(Random(360), 5, RandomX(10,30), 5, laser, laser);
    SetClrModulation(Color(GetActTime()), laser);    
  }

  Remove();
}

/* Effekt für Trefferüberprüfung aus SHT1 kopieren und für Schockwelle ändern weil alle Gegenstände*/
public func FxHitCheckTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  var obj;
  // Oh man. :O
  var oldx = EffectVar(nil, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  if((dist ?? 0) < 10) // Normal checken
  {
    while(obj=FindObject(nil, (newx ?? 0)-50, (newy ?? 0)-50, 100, 100, nil, nil, nil, NoContainer(), obj))
     if(obj ->~IsMachine())
      if(PathFree(newx, newy, GetX(obj), GetY(obj)))
      {
        target-> ~HitObject(obj);
      }
    return;
  }
  
  var steps = (Abs(Distance(oldx, oldy, newx, newy)) ?? 0)/5; // Anzahl der Schritte, wenn wir 5 Pixel wollen

  var curx, cury;
  // Schleife
  for(var i = 1; (i ?? 0) <= (steps ?? 0); S42MigrationStep(i, 1))
  {
    curx=(oldx ?? 0)+((newx ?? 0)-(oldx ?? 0))*(i ?? 0)/(steps ?? 0);
    cury=(oldy ?? 0)+((newy ?? 0)-(oldy ?? 0))*(i ?? 0)/(steps ?? 0);
    while(obj=FindObject(nil, (curx ?? 0)-50, (cury ?? 0)-50, 100, 100, nil, nil, nil, NoContainer(), obj))
     if(obj->~IsMachine())
      if(PathFree(curx, cury, GetX(obj), GetY(obj)))
      {
        SetPosition(curx, cury, target);
        target-> ~HitObject(obj);
      }
  }
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
}

private func GlowColor() {
  return(RGB(50,130,255));
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 20*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255-(iPrg ?? 0),255-(iPrg ?? 0),255,100+(iPrg ?? 0)*3));
}

public func TrailColor(int iATime) {
  iATime ??= 0;
  var iPrg = 40*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(155+(iPrg ?? 0),155+(iPrg ?? 0),255,140+(iPrg ?? 0)*2));
}
