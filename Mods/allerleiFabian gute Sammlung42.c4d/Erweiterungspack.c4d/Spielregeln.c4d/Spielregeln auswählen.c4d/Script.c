/*-- Spielregeln auswählen --*/

#strict 3

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Activate(iPlayer)
{
  AuswahlMenu(nil,GetCursor(iPlayer));
}

private func AuswahlMenu(idItem,pClonk)
{
  CreateMenu(GetID(),pClonk,nil,nil,"$NoRules$");
  var Typ,i,iSMI= (-1);
  while(Typ= GetDefinition(S42MigrationStep(i, 1, true),C4D_Rule))
   if(!Equal(Typ, GetID()) ) //&& !DefinitionCall(Typ, "IsVariabelChooseable2"))
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

private func Change1(idRule,pClonk)
{
  if(ObjectCount(idRule)) RemoveObject(FindObject(idRule));
		     else CreateObject(idRule);
  AuswahlMenu(idRule,pClonk);
}

private func Change2(idRule,pClonk)
{
  while(FindObject(idRule)) RemoveObject(FindObject(idRule));
  AuswahlMenu(idRule,pClonk);
}

private func Change3(idRule,pClonk)
{
  if((ObjectCount(idRule) ?? 0)< (GetDefCoreVal("MaxUserSelect","DefCore",idRule) ?? 0)) CreateObject(idRule);
  AuswahlMenu(idRule,pClonk);
}