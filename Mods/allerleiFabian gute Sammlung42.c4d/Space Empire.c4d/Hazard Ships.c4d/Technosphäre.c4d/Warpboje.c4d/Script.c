#strict 3

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

func Initialize()
{
  SetAction("Travel");
}


private func Traveling()
{
  SetXDir(nil);
  SetYDir(nil);
  CreateParticle("Thrust",nil,nil,(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(nil,50,250,60),nil,nil);
  return(1);
}

  
/* Treffer */

func Hit() {
  HitObject();
}

private func HitObject() {
  Sound("EMP");
  CreateObject(SHCK, nil, nil, GetOwner())->SetMaxSize(15);
  RemoveObject();
}

public func Damage() {
  if(GetDamage())
    Hit();
}
