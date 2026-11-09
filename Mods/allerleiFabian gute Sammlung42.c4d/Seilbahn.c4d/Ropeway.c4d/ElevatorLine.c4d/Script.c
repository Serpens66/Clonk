/*-- Seilbahnleitung --*/

#strict 2
// Für "LineBreak" und "BreakMessage"
#include PWRL

protected func Initialize()
{
  Local(0) = 23;
  Local(1) = 21;
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  return(true);
}

// Das Verbinden der beiden Seilbahnen
public func Connect(object pTarget1, object pTarget2, object pKit, object pLine, object pTo)
{
  // Ist das, woran gerade angeschlossen wird, überhaupt eine Seilbahn?
  if(GetID(pTo) != RW4V) {
    // Nö, also tschüss
    Sound("Error",0,pKit);
    Message("$MsgNoConnectType$",pKit,GetName(pLine),GetName(pTo));
    return(false);
  }
  var pLeft, pRight, iSide;
  if(GetX(pTarget1) < GetX(pTarget2)) { pLeft = pTarget1; pRight = pTarget2; iSide = 1; }
  else                                { pLeft = pTarget2; pRight = pTarget1; iSide = 0; }
  var iX, iY;
  iX = (GetX(pRight)+pRight->~GetCaseOffset(0,0))-(GetX(pLeft)+pLeft->~GetCaseOffset(1,0));
  iY = (GetY(pRight)+pRight->~GetCaseOffset(0,1))-(GetY(pLeft)+pLeft->~GetCaseOffset(1,1));
  // Sind die Koordinaten dafür überhaupt geeignet?
//  if(!Inside(AbsRotation(Angle(GetX(pTarget1),GetY(pTarget1),GetX(pTarget2),GetY(pTarget2)))%180,45,135)) {
  if(!Inside(AbsRotation(Angle(0,0,iX,iY))%180,45,135))
    // Wäre hierfür nicht die andere Aussage geeigneter?
    if(!(iX < 26 && Distance(iX,iY) < 30)) {
      // Das ist zu steil, dafür ist der Fahrstuhl geeignet!
      Sound("Error",0,pKit);
      Message("$MsgTooDeep$",pKit);
      return(false);
    }
  // Liegen sie nicht zu nah beieinander?
  if(iX < 26) {
    // So nah beieinander bringt die Seilbahn auch wiederum nichts!
    Sound("Error",0,pKit);
    Message("$MsgTooNear$",pKit);
    return(false);
  }
  // Wurde keine der beiden Bahnen von der jeweiligen Seite bereits irgendwie angeschlossen?
  if(LocalN("pLeadTo",pLeft) || FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pRight))) {
    // Soetwas führt zu Unbequemlichkeiten und Fehlern wodurch dies nicht gestattet wird
    Sound("Error",0,pKit);
    Message("$MsgAlreadyConnected$",pKit);
    return(false);
  }
  // Alle Vorgaben wurden eingehalten: Seilbahnen miteinander verbinden
  pLeft->~ConnectWith(pRight,iSide);
  Message("$MsgConnect$",Contained(pKit),GetName(pLine),GetName(pTo));
  Sound("Connect",0,Contained(pKit));
  RemoveObject(pLine);
  RemoveObject(pKit);
  return(true);
}