/*-- Waffenpläne --*/

#strict

local idPlan_1,idPlan_2,idPlan_3;

public func Init(id_1,id_2,id_3)
{
  idPlan_1= id_1;
  idPlan_2= id_2;
  idPlan_3= id_3;
}

protected func Activate(pClonk)
{
  Sound("PlanUnfold");
  if(idPlan_1) SetPlrKnowledge(GetOwner(pClonk),idPlan_1);
  if(idPlan_2) SetPlrKnowledge(GetOwner(pClonk),idPlan_2);
  if(idPlan_3) SetPlrKnowledge(GetOwner(pClonk),idPlan_3);

  var text= "";
  if(idPlan_1) text= Format("%s%s",text,Format("Bauplan von %s.|",GetName(0,idPlan_1)));
  if(idPlan_2) text= Format("%s%s",text,Format("Bauplan von %s.|",GetName(0,idPlan_2)));
  if(idPlan_3) text= Format("%s%s",text,Format("Bauplan von %s.|",GetName(0,idPlan_3)));
  PlayerMessage(GetOwner(pClonk),text,pClonk);
  RemoveObject();
}