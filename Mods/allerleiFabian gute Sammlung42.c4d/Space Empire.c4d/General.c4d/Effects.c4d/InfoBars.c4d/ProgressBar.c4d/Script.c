/*-- Fortschritts-Balken --*/

#strict 3
#include IBRS   // dasselbe wie im Status-Balken


private func FindLowerBar(...) {
  return(nil);
}

private func FindHigherBar(...) {
  var obj = FindObject(IBRS, -1,-1,3,3, nil,nil,Par(nil));
  if (!obj)
      obj = FindObject(IBRE, -1,-1,3,3, nil,nil,Par(nil));
  return(obj);
}

private func StdInfoBarText(v1, v2)
{
  return(Format("@Produktion:  %d%", (v1 ?? 0)*100/(v2 ?? 0)));
}

