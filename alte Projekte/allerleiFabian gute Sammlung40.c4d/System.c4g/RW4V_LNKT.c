/*-- Leitungsbausatz --*/

#strict 2
#appendto LNKT

// Für die Seilbahn

private func ConnectLine(object pLine, object pTo)
{
  var pTarget1 = GetActionTarget(0,pLine);
  var pTarget2 = pTo;
  if(pTarget1 == this()) { pTarget2 = GetActionTarget(1,pLine); pTarget1 = pTo; }
  // Wird gerade eine Seilbahn mit einer Leitung versehen?
  if(GetID(pLine) == RL4V)
    return(RL4V->~Connect(pTarget1,pTarget2,this(),pLine,pTo));
  // Das ist nicht der Fall
  return(_inherited(pLine, pTo));
}