/*-- Verbrannte Seilbahn --*/

#strict 3

#include BAS1

// Der Korb und die Leitung
local pCase, pWire;

public func RemoveCase(int iSide, int iFire)
{
  iSide ??= 0;
  iFire ??= 0;
  var pRopeway;
  // iSide: -1 = Links; 1 = Rechts; Sonst: Beide
  if((iSide ?? 0) >= 0) {
    if(pWire) RemoveObject(pWire);
    if(pCase) {
      if(iFire)
        Incinerate(pCase);
      else
        RemoveObject(pCase);
      pCase = nil;
    }
  }
  if((iSide ?? 0) <= 0)
    if(pRopeway = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this())))
      pRopeway->~RemoveCase(1,iFire);
  return(true);
}

protected func Incineration()   { RemoveCase(nil,1); return(_inherited(...)); }
protected func IncinerationEx() { RemoveCase(nil,1); return(_inherited(...)); }
protected func Destruction()    { RemoveCase(nil); return(_inherited(...)); }