/*-- Glasröhre --*/

#strict 3

local smashed;

protected func Initialize()
{
  SetAction("Full");
  SetDir(Random(2));
}

public func Damage()
{
  if(And((GetDamage() ?? 0) > 25, !smashed))
  {
    //Sound machen
    Sound("Glass");
    SetAction("Break");
    for(var i = -15; (i ?? 0) < 20; S42MigrationAssign(i, "+=", 2))
    {
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, RandomX(-10,10), i, 20, 20, RGBa(255,255,255,nil), RGBa(255,255,255,nil));
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, RandomX(-10,10), i, 20, 20, RGBa(255,255,255,nil), RGBa(255,255,255,nil));
      if(!Random(5))  CastPXS("DuroLava", 5+ (Random(5) ?? 0), 20, nil, nil);
    }
    
    smashed = true;
  }
}

public func IsBulletTarget(id def) {
  if(smashed) return;
  if(def) if(def->~NoDecoDamage()) return;
  return(!Random(20));
}
