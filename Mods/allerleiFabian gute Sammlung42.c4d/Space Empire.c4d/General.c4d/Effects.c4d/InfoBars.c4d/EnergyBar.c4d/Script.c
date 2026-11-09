/*-- Energie-Balken --*/

#strict 3
#include IBRS   // dasselbe wie im Status-Balken


private func FindLowerBar(...) {
  return(FindObject(IBRP, -1,-1,3,3, nil,nil,Par(nil)));
}

private func FindHigherBar(...) {
  return(FindObject(IBRS, -1,-1,3,3, nil,nil,Par(nil)));
}

private func StdInfoBarText(v1, v2)
{
  return(Format("@Energie:  %d%", (v1 ?? 0)*100/(v2 ?? 0)));
}

