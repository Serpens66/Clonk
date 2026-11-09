#strict

local Sell_A,Sell_L,Sell_O,Sell_W;

protected func Initialize()
{
  Sell_A= "$On$";
  Sell_L= "$On$";
  Sell_O= "$On$";
  Sell_W= "$On$";
}

public func BarrelSellControl(pClonk)
{
  CreateMenu(SY02,pClonk,0,0,"$SaleOf$",0,1);
  AddMenuItem(Format("$Water$: %s",Sell_W),"ChangeSellControl",WBRL,pClonk,0,pClonk);
  AddMenuItem(Format("$Oil$: %s",Sell_O),"ChangeSellControl",OBRL,pClonk,0,pClonk);
  AddMenuItem(Format("$Lava$: %s",Sell_L),"ChangeSellControl",LBRL,pClonk,0,pClonk);
  AddMenuItem(Format("$Acid$: %s",Sell_A),"ChangeSellControl",ABRL,pClonk,0,pClonk);
  return(1);
}

private func ChangeSellControl(idItem,pClonk)
{
  Sound("Click");
  if(idItem== WBRL)
    {
    if(Sell_W eq "$On$") Sell_W= "$Off$";
		    else Sell_W= "$On$";
    return(BarrelSellControl(pClonk),SelectMenuItem(0,pClonk));
    }
  if(idItem== OBRL)
    {
    if(Sell_O eq "$On$") Sell_O= "$Off$";
		    else Sell_O= "$On$";
    return(BarrelSellControl(pClonk),SelectMenuItem(1,pClonk));
    }
  if(idItem== LBRL)
    {
    if(Sell_L eq "$On$") Sell_L= "$Off$";
		    else Sell_L= "$On$";
    return(BarrelSellControl(pClonk),SelectMenuItem(2,pClonk));
    }
  if(idItem== ABRL)
    {
    if(Sell_A eq "$On$") Sell_A= "$Off$";
		    else Sell_A= "$On$";
    return(BarrelSellControl(pClonk),SelectMenuItem(3,pClonk));
    }
}


public func ContainerCheck()
{
  var target= GetActionTarget();
  if(!target) return(RemoveObject());
  var iPlr= GetOwner(target);
  var obj;
  while(obj= FindObject(0,0,0,0,0,0,0,0,target,obj))
   if(IsBarrel(GetID(obj)))
    if(LocalN("iFillLevel",obj)>= (obj-> BarrelMaxFill()))
      {
      if(GetID(obj)== OBRL)
       if(Sell_O eq "$On$")
         {
         Sell(iPlr,obj);
         }
      if(GetID(obj)== ABRL)
       if(Sell_A eq "$On$")
         {
         Sell(iPlr,obj);
         }
      if(GetID(obj)== LBRL)
       if(Sell_L eq "$On$")
         {
         Sell(iPlr,obj);
         }
      if(GetID(obj)== WBRL)
       if(Sell_W eq "$On$")
         {
         Sell(iPlr,obj);
         }
      if(GetID(obj)== WDWB) // holzwasserfass
       if(Sell_W eq "$On$")
         {
         Sell(iPlr,obj);
         }
      }
  if(GetID(obj)!= WDWB)  if(!FindContents(EmptyBarrelID(),target)) Buy(EmptyBarrelID(),iPlr,iPlr,target,1);
  else if(!FindContents(WDBR,target)) Buy(WDBR,iPlr,iPlr,target,1);  // bei holzfass
}

public func IsBarrel(idObj)
{
  if(idObj== ABRL) return(1);
  if(idObj== LBRL) return(1);
  if(idObj== OBRL) return(1);
  if(idObj== WBRL) return(1);
  if(idObj== WDWB) return(1); // holzwasserfass
  return(0);
}

/*----------------------------------------- Damit auch Holzfässer mit Wasser gefüllt werden können ----------------------------------------*/

/*-- Noch nicht fertig --*/

/*
// Auffüllbares Fass im Objekt suchen
global func FindFillBarrel(pInObj,idType)
{
  // Alle Inhaltsobjekte durchlaufen
  var pObj;
  for(var i = 0; pObj = Contents(i, pInObj); i++)
    // ID stimmt?
    if(pObj->GetID() == idType)
      // Fass nicht voll?
      if (!pObj->~BarrelIsFull())
        // Nehmen wir doch das
        return(pObj);

  // Notfalls Holzfass füllen
  if(FindDefinition(WDWB))
   if(idType== WBRL)
    {
    // Alle Inhaltsobjekte durchlaufen
    var pObj;
    for(var i = 0; pObj = Contents(i, pInObj); i++)
      // ID stimmt?
      if(pObj->GetID() == WDWB)
        // Fass nicht voll?
        if (!pObj->~BarrelIsFull())
          // Nehmen wir doch das
          return(pObj);
    }

  // Nix? Dann halt ein leeres Fass suchen und füllen
  if(pObj= FindContents(EmptyBarrelID(),pInObj))
    {
    ChangeDef(idType,pObj);
    return(pObj);
    }

  // Immer noch Nix? Dann halt ein leeres Holzfass suchen
  if(FindDefinition(WDBR))
   if(idType== WBRL)
    if(pObj= FindContents(WDBR,pInObj))
     {
     ChangeDef(WDWB,pObj);
     return(pObj);
     }

  return(0);
}

*/