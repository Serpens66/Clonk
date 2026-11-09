/*-- Forsthaus --*/

#strict 3

#include DOOR
#include BAS4

local idTree,iRadius,iSeed,iSeedgras,
      Mark_LO,Mark_LU,Mark_RO,Mark_RU;

protected func Initialize()
{
  idTree= RMMG;
  iRadius= 250;
  iSeed= 5;
  iSeedgras= 5;
  RemoveMarks();
}

private func Markierungen()
{
  // Linksoben,Linksunten,Rechtsoben,Rechtsunten eine Markierung erstellen
  Mark_LO= CreateObject(FM9I,-(iRadius ?? 0),-(iRadius ?? 0));
  Mark_LU= CreateObject(FM9I,-(iRadius ?? 0),+(iRadius ?? 0));
  Mark_RO= CreateObject(FM9I,+(iRadius ?? 0),-(iRadius ?? 0));
  Mark_RU= CreateObject(FM9I,+(iRadius ?? 0),+(iRadius ?? 0));
  Mark_LO-> Init(this(),1);
  Mark_LU-> Init(this(),2);
  Mark_RO-> Init(this(),3);
  Mark_RU-> Init(this(),4);
}

protected func ContainedUp(pClonk)
{
  CreateMenu(GetID(),pClonk,nil,C4MN_Extra_Value);
  AddMenuItem("$PlantTrees$","Plant",LMMS,pClonk,nil,pClonk,"$Desc1$",128,nil,nil);
  AddMenuItem("$SortOfTree$","SetTreeSortMenu",idTree,pClonk,nil,pClonk,nil,128,nil,nil);
  AddMenuItem("$ForestRadius$","RadiusMenu",WOOD,pClonk,nil,pClonk,"$Desc2$",128,nil,nil);
  AddMenuItem("$BuySeeds$","BuySeed",SA9I,pClonk,nil,pClonk,"$Desc3$",128,nil,20);
  AddMenuItem("$GetSeed$","TakeSeed",SA9I,pClonk,iSeed,pClonk,"$Desc4$",nil,nil);
  AddMenuItem("$BuyGrasSeeds$","BuyGrasSeed",SL9I,pClonk,nil,pClonk,"Kaufe ein paar Grassamen(5 Gold fuer 5 Samen). Können nur manuell gepflanzt werden. Gras vermehrt sich sehr schnell von alleine!",128,nil,10);
  AddMenuItem("$GetGrasSeed$","TakeGrasSeed",SL9I,pClonk,iSeedgras,pClonk,"Ein paar Grassamen. Können nur manuell gepflanzt werden",2,1); 
  AddMenuItem("Pflanzen in Topf kaufen","BuyPot",EH8Z,pClonk,nil,pClonk,"Du bekommst vier Pflanzen im Topf für 10 Clunker",128,nil,20);
  AddMenuItem("Frische Erde kaufen","BuyEarth",ERTH,pClonk,nil,pClonk,"Du bekommst 5 Erdklumpen für 5 Clunker",128,nil,5);
}



protected func BuyEarth(idItem,pClonk)
{
  var iPlr= GetOwner(pClonk);
  if((GetWealth(iPlr) ?? 0)< 5) return(S42MigrationFirst(nil,Message("$NotEnoughMoney$",this()),Sound("Error")));
  SetWealth(iPlr,(GetWealth(iPlr) ?? 0)- 5);
  Sound("Cash");
  CreateContents(ERTH,this(),5);
  if(idItem) And(ContainedUp(pClonk), SelectMenuItem(8,pClonk));
  return(1);
}

protected func BuyPot(idItem,pClonk)
{
  var iPlr= GetOwner(pClonk);
  if((GetWealth(iPlr) ?? 0)< 10) return(S42MigrationFirst(nil,Message("$NotEnoughMoney$",this()),Sound("Error")));
  SetWealth(iPlr,(GetWealth(iPlr) ?? 0)- 10);
  Sound("Cash");
  CreateContents(EH8Z,this(),4);
  if(idItem) And(ContainedUp(pClonk), SelectMenuItem(7,pClonk));
  return(1);
}

protected func BuyGrasSeed(idItem,pClonk)
{
  if(And(!idItem, FindContents(GA9I)))
    {
    RemoveObject(FindContents(GA9I));
    S42MigrationStep(iSeedgras, 1, true);
    return(1);    
    }
  var iPlr= GetOwner(pClonk);
  if((GetWealth(iPlr) ?? 0)< 5) return(S42MigrationFirst(nil,Message("$NotEnoughMoney$",this()),Sound("Error")));
  SetWealth(iPlr,(GetWealth(iPlr) ?? 0)- 5);
  Sound("Cash");
  S42MigrationAssign(iSeedgras, "+=", 5);
  if(idItem) And(ContainedUp(pClonk), SelectMenuItem(5,pClonk));
  return(1);
}

protected func BuySeed(idItem,pClonk)
{
  if(And(!idItem, FindContents(SA9I)))
    {
    RemoveObject(FindContents(SA9I));
    S42MigrationStep(iSeed, 1, true);
    return(1);    
    }
  var iPlr= GetOwner(pClonk);
  if((GetWealth(iPlr) ?? 0)< 20) return(S42MigrationFirst(nil,Message("$NotEnoughMoney$",this()),Sound("Error")));
  SetWealth(iPlr,(GetWealth(iPlr) ?? 0)- 20);
  Sound("Cash");
  S42MigrationAssign(iSeed, "+=", 10);
  if(idItem) And(ContainedUp(pClonk), SelectMenuItem(3,pClonk));
  return(1);
}

private func TakeSeed(idItem,pClonk)
{
  if(!iSeed) return(S42MigrationFirst(ContainedUp(pClonk),SelectMenuItem(4,pClonk)));
  var Limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(pClonk));
  if(And((ContentsCount(nil,pClonk) ?? 0)>= (Limit ?? 0), !Equal(Limit, nil)))
   return(S42MigrationFirst(ContainedUp(pClonk),SelectMenuItem(4,pClonk)));
  Sound("Grab");
  LocalN("idTree",CreateContents(SA9I,pClonk))= idTree;
  S42MigrationStep(iSeed, -1, true);
  And(ContainedUp(pClonk), SelectMenuItem(4,pClonk));
}

private func TakeGrasSeed(idItem,pClonk)
{
  if(!iSeedgras) return(S42MigrationFirst(ContainedUp(pClonk),SelectMenuItem(4,pClonk)));
  var Limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(pClonk));
  if(And((ContentsCount(nil,pClonk) ?? 0)>= (Limit ?? 0), !Equal(Limit, nil)))
   return(S42MigrationFirst(ContainedUp(pClonk),SelectMenuItem(6,pClonk)));
  Sound("Grab");
  LocalN("idTree",CreateContents(GA9I,pClonk))= idTree;
  S42MigrationStep(iSeedgras, -1, true);
  And(ContainedUp(pClonk), SelectMenuItem(6,pClonk));
}

private func Plant(idItem,pClonk)
{
  if(!iSeed)
   if(!BuySeed())
    return(SetCommand(pClonk,"None"));
  // Wird nur als Wegpunkt erstellt
  var Ziel= PlaceVegetation(TRE1,-(iRadius ?? 0),-(iRadius ?? 0),2* (iRadius ?? 0),2* (iRadius ?? 0),1);
  if(!Ziel) return(Message("$NoSuitablePlace$",this()));
  S42MigrationStep(iSeed, -1, true);
  // 7. Vorgang wiederholen!
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),nil,nil,nil,"Loop");
  // 6. Warten!
  AddCommand(pClonk,"Wait",nil,nil,nil,nil,50);
  // 5. Forsthaus betreten!
  AddCommand(pClonk,"Enter",this());
  // 4. Warten!
  AddCommand(pClonk,"Wait",nil,nil,nil,nil,50);
  // 3. Baum pflanzen!
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),ObjectNumber(Ziel),nil,nil,"PlantTree");
  // 2. Warten!
  AddCommand(pClonk,"Wait",nil,nil,nil,nil,100);
  // 1. Zum Zielpunkt bewegen!
  AddCommand(pClonk,"MoveTo",nil,GetX(Ziel),GetY(Ziel));
  RemoveObject(Ziel);
}

private func Loop(Par,iObj)
{
  var pObj= Object(iObj);
  SetCommand(pObj,"None");
  Plant(nil,pObj);
}

private func PlantTree(Par,iObj,iTarget)
{
  var pObj= Object(iObj);
  var pTarget= Object(iTarget);
  var ID= idTree,i= Random(4);
  if(Equal(ID, RMMG))
    {
    if(Equal(i, nil)) ID= TRE1;
    if(Equal(i, 1)) ID= TRE2;
    if(Equal(i, 2)) ID= TRE3;
    if(Equal(i, 3)) ID= TRE4;
    }
  // Ist noch Platz für einen Baum?
  var iSize= DefinitionCall(ID,"ReproductionAreaSize"),iOffset= (iSize ?? 0)/ -2;
  if(!((pObj-> ObjectCount(nil,iOffset,iOffset,iSize,iSize,OCF_Chop) ?? 0)< (DefinitionCall(ID,"MaxTreeCount") ?? 0)))
   return(Message("$NoPlace$",pObj));
  pObj-> CreateConstruction(ID,nil,+15,-1,5);
  ObjectSetAction(pObj,"Bridge");
}

private func SetTreeSortMenu(idItem,pClonk)
{
  var Typ,i= nil;
  CreateMenu(idItem,pClonk,nil,1,"$NoPlants$");
  AddMenuItem("$Random$","SetTreeSort",RMMG,pClonk,nil,nil,"$PlantRandomTrees$");
  while(Typ= GetDefinition(S42MigrationStep(i, 1, true),C4D_StaticBack))
   if(Equal(GetCategory(nil,Typ), 32769))
    if(!Equal(Typ, CACT))
    if(!Equal(Typ, CAC2))
     if(!WildcardMatch(GetName(nil,Typ),"Verbrannt*"))
      if(!WildcardMatch(GetName(nil,Typ),"Burned*"))
       if(!WildcardMatch(GetName(nil,Typ),"Tot*"))
        if(!WildcardMatch(GetName(nil,Typ),"Dead*"))
         AddMenuItem("%s","SetTreeSort",Typ,pClonk);
}

private func SetTreeSort(idItem)
{
  Sound("Click");
  idTree= idItem;
}

private func RadiusMenu(idItem,pClonk,iRepeat)
{
  RemoveMarks();
  Markierungen();
  if(!iRepeat)
    {
    SetOwner(GetOwner(pClonk),Mark_LO); SetVisibility(VIS_Owner,Mark_LO);
    SetOwner(GetOwner(pClonk),Mark_LU); SetVisibility(VIS_Owner,Mark_LU);
    SetOwner(GetOwner(pClonk),Mark_RO); SetVisibility(VIS_Owner,Mark_RO);
    SetOwner(GetOwner(pClonk),Mark_RU); SetVisibility(VIS_Owner,Mark_RU);
    }
  CreateMenu(WOOD,pClonk);
  AddMenuItem("$IncreaseRadius$","SetRadius",LMM3,pClonk,nil,pClonk);
  AddMenuItem("$ReduceRadius$","SetRadius",LMM7,pClonk,nil,pClonk);
}

private func SetRadius(idItem,pClonk)
{  
  var iChange,iSMI;
  if(Equal(idItem, LMM3)) iChange= (+5);
  if(Equal(idItem, LMM7)) { iChange= (-5); iSMI= 1; }
  iRadius= (iRadius ?? 0)+ (iChange ?? 0);
  if(!Inside(iRadius,100,500))
    {
    if((iRadius ?? 0)> 500) iRadius= 500;
    if((iRadius ?? 0)< 100) iRadius= 100;
    }
  else
    {
    Mark_LO-> Move(iChange);
    Mark_LU-> Move(iChange);
    Mark_RO-> Move(iChange);
    Mark_RU-> Move(iChange);
    }
  Message("$ShowRadius$",this(),iRadius);
  And(RadiusMenu(nil,pClonk,1), SelectMenuItem(iSMI,pClonk));
}

protected func MenuQueryCancel()
{
  return(S42MigrationFirst(nil,RemoveMarks()));
}

protected func Destruction(a,b,c)
{
  RemoveMarks();
  return(_inherited(a,b,c)); 
}

private func RemoveMarks()
{
  if(Mark_LO) RemoveObject(Mark_LO);
  if(Mark_LU) RemoveObject(Mark_LU);
  if(Mark_RO) RemoveObject(Mark_RO);
  if(Mark_RU) RemoveObject(Mark_RU);
}