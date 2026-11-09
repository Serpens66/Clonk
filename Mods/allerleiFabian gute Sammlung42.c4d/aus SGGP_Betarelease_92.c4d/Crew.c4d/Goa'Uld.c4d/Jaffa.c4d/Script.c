/*---- Jaffa ----*/

#strict
#include CLNK
#include HZCK

public func Zated()
{
  if(GetAction() eq "Lie")
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
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);

  if(IsAiming()) {
    AimUpdate(this(), comdir, 1, "ControlConf");
    return(1);
  }

  return(_inherited());
}