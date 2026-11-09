#strict
#appendto FLNT
#appendto SFLN
#appendto EFLN
#appendto METO
#appendto TRP1
// #appendto _MBM
#appendto _ICM

public func Destroy()
{
  CastPXS("FlyAshes",25,25);
  ProtectedCall(this(),"Hit");
}

public IsProjectile: return(1);
