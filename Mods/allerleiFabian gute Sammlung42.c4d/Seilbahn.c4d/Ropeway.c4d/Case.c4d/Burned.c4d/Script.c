/*-- Verbrannter Seilbahnkorb --*/

#strict 3

local pLeft, pRight;  // Seilbahnstation links und Rechts

public func Removing(int iFire)
{
  // Graphik entfernen
  iFire ??= 0;
  SetGraphics(nil,this(),nil,1);
  SetGraphics(nil,this(),nil,2);
  // Seilbahn benachrichtigen
  if(pLeft) {
    LocalN("pCase",pLeft) = nil;
    pLeft->~RemoveCase(1);
  }
  if(pRight) {
    var pObj = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pRight));
    if(pObj) {
      LocalN("pCase",pObj) = nil;
      pObj->~RemoveCase(1);
    }
  }
  // Fertig
  return(true);
}

protected func Incineration()   { Removing(); return(_inherited(...)); }
protected func IncinerationEx() { Removing(); return(_inherited(...)); }
protected func Destruction()    { Removing(); return(_inherited(...)); }