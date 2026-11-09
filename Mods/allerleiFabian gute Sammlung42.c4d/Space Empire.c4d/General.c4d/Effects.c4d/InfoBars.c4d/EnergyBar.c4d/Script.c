/*-- Energie-Balken --*/

#strict
#include IBRS   // dasselbe wie im Status-Balken


private FindLowerBar:
  return(FindObject(IBRP, -1,-1,3,3, 0,0,Par(0)));

private FindHigherBar:
  return(FindObject(IBRS, -1,-1,3,3, 0,0,Par(0)));

private func StdInfoBarText(v1, v2)
{
  return(Format("@Energie:  %d%", v1*100/v2));
}

