/*-- Spielregeln auswählen --*/

#strict

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Activate(iPlayer)
{
  AuswahlMenu(0,GetCursor(iPlayer));
}

private func AuswahlMenu(idItem,pClonk)
{
  CreateMenu(GetID(),pClonk,0,0,"$NoEnvironment$");
  var Typ,i,iSMI= (-1);
  while(Typ= GetDefinition(i++,C4D_StaticBack()))
   if(GetCategory(0,Typ)== 65 || GetCategory(0,Typ)== 64 || GetCategory(0,Typ)== 1048641 )
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
  if(ObjectCount(idRule)< GetDefCoreVal("MaxUserSelect","DefCore",idRule)) CreateObject(idRule);
  AuswahlMenu(idRule,pClonk);
}