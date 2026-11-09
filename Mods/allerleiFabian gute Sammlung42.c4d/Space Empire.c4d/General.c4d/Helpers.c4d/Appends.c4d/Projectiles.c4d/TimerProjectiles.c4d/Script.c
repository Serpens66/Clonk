#strict
#appendto TFLN
#appendto STFN

public func Destroy()
{
  if (GetAction() eq "Activated")
  {
    CastPXS("FlyAshes",25,25);
    SetAction("TimeOver");
  }
  else
    ProtectedCall(this(),"Hit");
}

public IsProjectile: return(1);
