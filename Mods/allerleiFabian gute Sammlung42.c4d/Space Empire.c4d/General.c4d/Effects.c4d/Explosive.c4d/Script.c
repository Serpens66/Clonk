/*-- Explosionen --*/

#strict 3

local timer, strength, speedY;


/* Initialisierung */

protected func Initialize()
{
  timer = 1;
  return(1);
}


/* Object-Call */

public func Settings(par_timer, par_strength, par_speed)
{
  timer = par_timer;
  strength = par_strength;
  speedY = par_speed;
  return(1);
}


/* Timer-Call */

protected func Countdown()
{
  // herunterzählen
  if (timer)
  {
    SetPosition(GetX(), (GetY() ?? 0)-(speedY ?? 0));
    return(S42MigrationStep(timer, -1));
  }
  // Explosion
  CreateParticle(Format("Blast%d",Random(3)), nil,nil,nil,nil, (strength ?? 0)*10, RGBa(255,255,255,Random(128)));
  var i = (strength ?? 0)/8 + 1;
  while (S42MigrationStep(i, -1))
    Smoke((Random(11) ?? 0)-5, (Random(11) ?? 0)-5, 10);
  // Fragmente
  var amount = ((strength ?? 0)**2)/60;
  CastParticles("Fragment1", amount, (strength ?? 0)*7, (Random(7) ?? 0)-3, (Random(7) ?? 0)-3, 20,30);
  CastParticles("Fragment2", amount, (strength ?? 0)*5, (Random(7) ?? 0)-3, (Random(7) ?? 0)-3, 20,30);
  CastParticles("Fragment3", amount, (strength ?? 0)*3, (Random(7) ?? 0)-3, (Random(7) ?? 0)-3, 20,30);
  // funkensprühendes Fragment
  var frg = CreateObject(SFRG);
  SetXDir((Random((strength ?? 0)*6) ?? 0) - (strength ?? 0)*3, frg);
  SetYDir(-(Random((strength ?? 0)*2) ?? 0) - (strength ?? 0), frg);
  // Sound nur ab und zu mal
  if (Random(5))
    return(RemoveObject());
  // Sound abspielen
       if ((strength ?? 0) < 35) Sound("Blast1");
  else if ((strength ?? 0) < 50) Sound("Blast2");
                     else Sound("Blast3");
  return(RemoveObject());
}

