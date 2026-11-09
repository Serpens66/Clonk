/*-- Neues Objekt --*/

#strict
#appendto SEL1

local selection, party;
local px, py;

private func Select_CoAl(){
var player = GetOwner();
AddSomeKnowledges(player,ST_4/*MTS Banhof*/,ST_3/*MTS Sphäre*/);
party = 1;
return(_inherited(Par(),Par(1),Par(2),Par(3),Par(4)));
}


private func Select_SLC(){
var player = GetOwner();
AddSomeKnowledges(player,ST_4/*MTS Banhof*/,ST_3/*MTS Sphäre*/);
party = 2;
return(_inherited(Par(),Par(1),Par(2),Par(3),Par(4)));
}
