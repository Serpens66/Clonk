/*-- Seilbahnleitung --*/

#strict 3
// Für "LineBreak" und "BreakMessage"
#include PWRL

protected func Initialize()
{
  Local(nil) = 23;
  Local(1) = 21;
  SetVertex(nil,nil,GetX()); SetVertex(nil,1,GetY());
  SetVertex(1,nil,GetX()); SetVertex(1,1,GetY());
  return(true);
}

// Das Verbinden der beiden Seilbahnen
public func Connect(object pTarget1, object pTarget2, object pKit, object pLine, object pTo)
{
  // Ist das, woran gerade angeschlossen wird, überhaupt eine Seilbahn?
  if(!S42MigrationEqual(GetID(pTo), RW4V)) {
    // Nö, also tschüss
    Sound("Error",nil,pKit);
    Message("$MsgNoConnectType$",pKit,GetName(pLine),GetName(pTo));
    return(nil);
  }
  var pLeft, pRight, iSide;
  if((GetX(pTarget1) ?? 0) < (GetX(pTarget2) ?? 0)) { pLeft = pTarget1; pRight = pTarget2; iSide = 1; }
  else                                { pLeft = pTarget2; pRight = pTarget1; iSide = nil; }
  var iX, iY;
  iX = ((GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0))-((GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0));
  iY = ((GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0))-((GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0));
  // Sind die Koordinaten dafür überhaupt geeignet?
//  if(!Inside(AbsRotation(Angle(GetX(pTarget1),GetY(pTarget1),GetX(pTarget2),GetY(pTarget2)))%180,45,135)) {
  if(!Inside((AbsRotation(Angle(nil,nil,iX,iY)) ?? 0)%180,45,135))
    // Wäre hierfür nicht die andere Aussage geeigneter?
    if(!((iX ?? 0) < 26 && (Distance(iX,iY) ?? 0) < 30)) {
      // Das ist zu steil, dafür ist der Fahrstuhl geeignet!
      Sound("Error",nil,pKit);
      Message("$MsgTooDeep$",pKit);
      return(nil);
    }
  // Liegen sie nicht zu nah beieinander?
  if((iX ?? 0) < 26) {
    // So nah beieinander bringt die Seilbahn auch wiederum nichts!
    Sound("Error",nil,pKit);
    Message("$MsgTooNear$",pKit);
    return(nil);
  }
  // Wurde keine der beiden Bahnen von der jeweiligen Seite bereits irgendwie angeschlossen?
  if(LocalN("pLeadTo",pLeft) || FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pRight))) {
    // Soetwas führt zu Unbequemlichkeiten und Fehlern wodurch dies nicht gestattet wird
    Sound("Error",nil,pKit);
    Message("$MsgAlreadyConnected$",pKit);
    return(nil);
  }
  // Alle Vorgaben wurden eingehalten: Seilbahnen miteinander verbinden
  pLeft->~ConnectWith(pRight,iSide);
  Message("$MsgConnect$",Contained(pKit),GetName(pLine),GetName(pTo));
  Sound("Connect",nil,Contained(pKit));
  RemoveObject(pLine);
  RemoveObject(pKit);
  return(true);
}