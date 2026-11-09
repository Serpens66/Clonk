/*-- Spielziele auswählen --*/

#strict

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Activate(iPlayer)
{
  AuswahlMenu(0,GetCursor(iPlayer));
}

private func AuswahlMenu(idItem,pClonk)
{
  CreateMenu(GetID(),pClonk,0,0,"$NoGoals$");
  var Typ,i,iSMI= (-1);
  while(Typ= GetDefinition(i++,C4D_Goal()))
   if(Typ!= GetID())
    {
    iSMI++;
    if(GetDefCoreVal("MaxUserSelect","DefCore",Typ)== 1)
      {
      if(ObjectCount(Typ)) AddMenuItem("$DisposeOf$","Change1",Typ,pClonk,ObjectCount(Typ),pClonk);
		      else AddMenuItem("$Add$","Change1",Typ,pClonk,ObjectCount(Typ),pClonk);
      }
    else
      {
      iSMI++;
      AddMenuItem("$DisposeOf$","Change2",Typ,pClonk,ObjectCount(Typ),pClonk);
      AddMenuItem("$Add$","Change3",Typ,pClonk,ObjectCount(Typ),pClonk);
      }
    if(idItem== Typ) SelectMenuItem(iSMI,pClonk);
    }
}

private func Change1(idGoal,pClonk)
{
  if(ObjectCount(idGoal)) RemoveObject(FindObject(idGoal));
		     else CreateObject(idGoal);
  AuswahlMenu(idGoal,pClonk);
}

private func Change2(idGoal,pClonk)
{
  while(FindObject(idGoal)) RemoveObject(FindObject(idGoal));
  AuswahlMenu(idGoal,pClonk);
}

private func Change3(idGoal,pClonk)
{
  if(ObjectCount(idGoal)< GetDefCoreVal("MaxUserSelect","DefCore",idGoal)) CreateObject(idGoal);
  AuswahlMenu(idGoal,pClonk);
}