#strict

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

func Initialize()
{
  SetAction("Travel");
}


private func Traveling()
{
  SetXDir(0);
  SetYDir(0);
  CreateParticle("Thrust",0,0,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(0,50,250,60),0,0);
  return(1);
}

  
/* Treffer */

func Hit() {
  HitObject();
}

private func HitObject() {
  Sound("EMP");
  CreateObject(SHCK, 0, 0, GetOwner())->SetMaxSize(15);
  RemoveObject();
}

public func Damage() {
  if(GetDamage())
    Hit();
}
