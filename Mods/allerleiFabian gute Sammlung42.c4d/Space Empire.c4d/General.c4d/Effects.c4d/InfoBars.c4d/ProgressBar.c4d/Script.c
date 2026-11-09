/*-- Fortschritts-Balken --*/

#strict
#include IBRS   // dasselbe wie im Status-Balken


private FindLowerBar:
  return(0);

private FindHigherBar:
  var obj = FindObject(IBRS, -1,-1,3,3, 0,0,Par(0));
  if (!obj)
      obj = FindObject(IBRE, -1,-1,3,3, 0,0,Par(0));
  return(obj);

private func StdInfoBarText(v1, v2)
{
  return(Format("@Produktion:  %d%", v1*100/v2));
}

