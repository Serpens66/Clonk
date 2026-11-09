/*-- Verbrannte Seilbahn --*/

#strict 2

#include BAS1

// Der Korb und die Leitung
local pCase, pWire;

public func RemoveCase(int iSide, int iFire)
{
  var pRopeway;
  // iSide: -1 = Links; 1 = Rechts; Sonst: Beide
  if(iSide >= 0) {
    if(pWire) RemoveObject(pWire);
    if(pCase) {
      if(iFire)
        Incinerate(pCase);
      else
        RemoveObject(pCase);
      pCase = 0;
    }
  }
  if(iSide <= 0)
    if(pRopeway = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this())))
      pRopeway->~RemoveCase(1,iFire);
  return(true);
}

protected func Incineration()   { RemoveCase(0,1); return(_inherited(...)); }
protected func IncinerationEx() { RemoveCase(0,1); return(_inherited(...)); }
protected func Destruction()    { RemoveCase(0); return(_inherited(...)); }