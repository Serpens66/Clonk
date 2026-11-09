/*-- Verbrannter Seilbahnkorb --*/

#strict 2

local pLeft, pRight;  // Seilbahnstation links und Rechts

public func Removing(int iFire)
{
  // Graphik entfernen
  SetGraphics(0,this(),0,1);
  SetGraphics(0,this(),0,2);
  // Seilbahn benachrichtigen
  if(pLeft) {
    LocalN("pCase",pLeft) = 0;
    pLeft->~RemoveCase(1);
  }
  if(pRight) {
    var pObj = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pRight));
    if(pObj) {
      LocalN("pCase",pObj) = 0;
      pObj->~RemoveCase(1);
    }
  }
  // Fertig
  return(true);
}

protected func Incineration()   { Removing(); return(_inherited(...)); }
protected func IncinerationEx() { Removing(); return(_inherited(...)); }
protected func Destruction()    { Removing(); return(_inherited(...)); }