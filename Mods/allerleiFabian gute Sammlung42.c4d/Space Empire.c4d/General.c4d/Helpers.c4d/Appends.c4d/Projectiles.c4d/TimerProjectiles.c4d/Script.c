#strict 3
#appendto TFLN
#appendto STFN

public func Destroy()
{
  if (!!SEqual(GetAction(), "Activated"))
  {
    CastPXS("FlyAshes",25,25);
    SetAction("TimeOver");
  }
  else
    ProtectedCall(this(),"Hit");
}

public func IsProjectile(...) { return(1);
}
