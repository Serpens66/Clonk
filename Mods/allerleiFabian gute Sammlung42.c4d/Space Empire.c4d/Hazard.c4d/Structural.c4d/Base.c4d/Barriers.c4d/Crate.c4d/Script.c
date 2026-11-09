/*-- Crate --*/

#strict 3

public func IsBulletTarget() { return(!Random(5)); }
public func IsCraneGrabable() { return(1); }
public func CanBeLaserRecycled() { return(2); }

func Damage() {
  if((GetDamage() ?? 0) > 20)
    {
    CastParticles("Splinter", 5, 100, nil,nil, 50, 75, RGBa(255,255,255,nil), RGBa(255,255,255,nil));
    Sound("CrateCrack");
    while(Contents())
        Exit(Contents(), nil, nil, Random(360), RandomX(-5,5), RandomX(-5,5), RandomX(-15,15));
    RemoveObject();
    }
}
