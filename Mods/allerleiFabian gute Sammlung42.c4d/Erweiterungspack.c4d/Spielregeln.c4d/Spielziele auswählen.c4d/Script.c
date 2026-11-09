/*-- Spielziele auswählen --*/

#strict 3

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Activate(iPlayer)
{
  AuswahlMenu(nil,GetCursor(iPlayer));
}

private func AuswahlMenu(idItem,pClonk)
{
  CreateMenu(GetID(),pClonk,nil,nil,"$NoGoals$");
  var Typ,i,iSMI= (-1);
  while(Typ= GetDefinition(S42MigrationStep(i, 1, true),C4D_Goal))
   if(!Equal(Typ, GetID()))
    {
    S42MigrationStep(iSMI, 1, true);
    if(Equal(GetDefCoreVal("MaxUserSelect","DefCore",Typ), 1))
      {
      if(ObjectCount(Typ)) AddMenuItem("$DisposeOf$","Change1",Typ,pClonk,ObjectCount(Typ),pClonk);
		      else AddMenuItem("$Add$","Change1",Typ,pClonk,ObjectCount(Typ),pClonk);
      }
    else
      {
      S42MigrationStep(iSMI, 1, true);
      AddMenuItem("$DisposeOf$","Change2",Typ,pClonk,ObjectCount(Typ),pClonk);
      AddMenuItem("$Add$","Change3",Typ,pClonk,ObjectCount(Typ),pClonk);
      }
    if(Equal(idItem, Typ)) SelectMenuItem(iSMI,pClonk);
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
  if((ObjectCount(idGoal) ?? 0)< (GetDefCoreVal("MaxUserSelect","DefCore",idGoal) ?? 0)) CreateObject(idGoal);
  AuswahlMenu(idGoal,pClonk);
}