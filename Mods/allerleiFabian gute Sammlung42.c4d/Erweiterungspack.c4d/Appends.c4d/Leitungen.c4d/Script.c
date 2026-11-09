/*-- Leitungen --*/

#strict 3
#appendto DPIP
#appendto PWRL
#appendto SPIP

private func BreakMessage(a,b,c)
{
  var trg1= GetActionTarget(nil);
  var trg2= GetActionTarget(1);
  if(!Equal(GetID(trg1), LNKT))
   if(!Equal(GetID(trg2), LNKT))
    if((GetCategory(trg1) ?? 0) & (C4D_Structure ?? 0))
     if((GetCategory(trg2) ?? 0) & (C4D_Structure ?? 0))
      trg1-> CreateObject(LNKT);
  _inherited(a,b,c);
}