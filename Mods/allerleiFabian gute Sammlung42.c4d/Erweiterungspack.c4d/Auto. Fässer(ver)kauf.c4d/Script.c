#strict 3

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
  CreateMenu(SY02,pClonk,nil,nil,"$SaleOf$",nil,1);
  AddMenuItem(Format("$Water$: %s",Sell_W),"ChangeSellControl",WBRL,pClonk,nil,pClonk);
  AddMenuItem(Format("$Oil$: %s",Sell_O),"ChangeSellControl",OBRL,pClonk,nil,pClonk);
  AddMenuItem(Format("$Lava$: %s",Sell_L),"ChangeSellControl",LBRL,pClonk,nil,pClonk);
  AddMenuItem(Format("$Acid$: %s",Sell_A),"ChangeSellControl",ABRL,pClonk,nil,pClonk);
  return(1);
}

private func ChangeSellControl(idItem,pClonk)
{
  Sound("Click");
  if(Equal(idItem, WBRL))
    {
    if(!!SEqual(Sell_W, "$On$")) Sell_W= "$Off$";
		    else Sell_W= "$On$";
    return(S42MigrationFirst(BarrelSellControl(pClonk),SelectMenuItem(nil,pClonk)));
    }
  if(Equal(idItem, OBRL))
    {
    if(!!SEqual(Sell_O, "$On$")) Sell_O= "$Off$";
		    else Sell_O= "$On$";
    return(S42MigrationFirst(BarrelSellControl(pClonk),SelectMenuItem(1,pClonk)));
    }
  if(Equal(idItem, LBRL))
    {
    if(!!SEqual(Sell_L, "$On$")) Sell_L= "$Off$";
		    else Sell_L= "$On$";
    return(S42MigrationFirst(BarrelSellControl(pClonk),SelectMenuItem(2,pClonk)));
    }
  if(Equal(idItem, ABRL))
    {
    if(!!SEqual(Sell_A, "$On$")) Sell_A= "$Off$";
		    else Sell_A= "$On$";
    return(S42MigrationFirst(BarrelSellControl(pClonk),SelectMenuItem(3,pClonk)));
    }
}


public func ContainerCheck()
{
  var target= GetActionTarget();
  if(!target) return(RemoveObject());
  var iPlr= GetOwner(target);
  var obj;
  while(obj= FindObject(nil,nil,nil,nil,nil,nil,nil,nil,target,obj))
   if(IsBarrel(GetID(obj)))
    if((LocalN("iFillLevel",obj) ?? 0)>= ((obj-> BarrelMaxFill()) ?? 0))
      {
      if(Equal(GetID(obj), OBRL))
       if(!!SEqual(Sell_O, "$On$"))
         {
         Sell(iPlr,obj);
         }
      if(Equal(GetID(obj), ABRL))
       if(!!SEqual(Sell_A, "$On$"))
         {
         Sell(iPlr,obj);
         }
      if(Equal(GetID(obj), LBRL))
       if(!!SEqual(Sell_L, "$On$"))
         {
         Sell(iPlr,obj);
         }
      if(Equal(GetID(obj), WBRL))
       if(!!SEqual(Sell_W, "$On$"))
         {
         Sell(iPlr,obj);
         }
      if(Equal(GetID(obj), WDWB)) // holzwasserfass
       if(!!SEqual(Sell_W, "$On$"))
         {
         Sell(iPlr,obj);
         }
      }
  if(!Equal(GetID(obj), WDWB))  if(!FindContents(EmptyBarrelID(),target)) Buy(EmptyBarrelID(),iPlr,iPlr,target,1);
  else if(!FindContents(WDBR,target)) Buy(WDBR,iPlr,iPlr,target,1);  // bei holzfass
}

public func IsBarrel(idObj)
{
  if(Equal(idObj, ABRL)) return(1);
  if(Equal(idObj, LBRL)) return(1);
  if(Equal(idObj, OBRL)) return(1);
  if(Equal(idObj, WBRL)) return(1);
  if(Equal(idObj, WDWB)) return(1); // holzwasserfass
  return(nil);
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