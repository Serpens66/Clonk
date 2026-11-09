/*-- Leitungen --*/

#strict
#appendto DPIP
#appendto PWRL
#appendto SPIP

private func BreakMessage(a,b,c)
{
  var trg1= GetActionTarget(0);
  var trg2= GetActionTarget(1);
  if(GetID(trg1)!= LNKT)
   if(GetID(trg2)!= LNKT)
    if(GetCategory(trg1) & C4D_Structure()) 
     if(GetCategory(trg2) & C4D_Structure()) 
      trg1-> CreateObject(LNKT);
  _inherited(a,b,c);
}