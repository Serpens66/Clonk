/*---- Jaffa ----*/

#strict 3
#include CLNK
#include HZCK

public func Zated()
{
  if(!!SEqual(GetAction(), "Lie"))
  {
   SetAction("Lie");
   EMPShockEffect(50);
   return(1);
  }
  SetAction("Zatfall");
  EMPShockEffect(50);
  return(1);
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);

  if(IsAiming()) {
    AimUpdate(this(), comdir, 1, "ControlConf");
    return(1);
  }

  return(_inherited());
}