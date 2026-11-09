/*-- Forsthaus --*/

#strict

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
  Mark_LO= CreateObject(FM9I,-iRadius,-iRadius);
  Mark_LU= CreateObject(FM9I,-iRadius,+iRadius);
  Mark_RO= CreateObject(FM9I,+iRadius,-iRadius);
  Mark_RU= CreateObject(FM9I,+iRadius,+iRadius);
  Mark_LO-> Init(this(),1);
  Mark_LU-> Init(this(),2);
  Mark_RO-> Init(this(),3);
  Mark_RU-> Init(this(),4);
}

protected func ContainedUp(pClonk)
{
  CreateMenu(GetID(),pClonk,0,C4MN_Extra_Value);
  AddMenuItem("$PlantTrees$","Plant",LMMS,pClonk,0,pClonk,"$Desc1$",128,0,0); 
  AddMenuItem("$SortOfTree$","SetTreeSortMenu",idTree,pClonk,0,pClonk,0,128,0,0);
  AddMenuItem("$ForestRadius$","RadiusMenu",WOOD,pClonk,0,pClonk,"$Desc2$",128,0,0);
  AddMenuItem("$BuySeeds$","BuySeed",SA9I,pClonk,0,pClonk,"$Desc3$",128,0,20);
  AddMenuItem("$GetSeed$","TakeSeed",SA9I,pClonk,iSeed,pClonk,"$Desc4$",0,0); 
  AddMenuItem("$BuyGrasSeeds$","BuyGrasSeed",SL9I,pClonk,0,pClonk,"Kaufe ein paar Grassamen(5 Gold fuer 5 Samen). Können nur manuell gepflanzt werden. Gras vermehrt sich sehr schnell von alleine!",128,0,10);
  AddMenuItem("$GetGrasSeed$","TakeGrasSeed",SL9I,pClonk,iSeedgras,pClonk,"Ein paar Grassamen. Können nur manuell gepflanzt werden",2,1); 
  AddMenuItem("Pflanzen in Topf kaufen","BuyPot",EH8Z,pClonk,0,pClonk,"Du bekommst vier Pflanzen im Topf für 10 Clunker",128,0,20); 
  AddMenuItem("Frische Erde kaufen","BuyEarth",ERTH,pClonk,0,pClonk,"Du bekommst 5 Erdklumpen für 5 Clunker",128,0,5);  
}



protected func BuyEarth(idItem,pClonk)
{
  var iPlr= GetOwner(pClonk);
  if(GetWealth(iPlr)< 5) return(0,Message("$NotEnoughMoney$",this()),Sound("Error"));
  SetWealth(iPlr,GetWealth(iPlr)- 5);
  Sound("Cash");
  CreateContents(ERTH,this(),5);
  if(idItem) ContainedUp(pClonk) && SelectMenuItem(8,pClonk);
  return(1);
}

protected func BuyPot(idItem,pClonk)
{
  var iPlr= GetOwner(pClonk);
  if(GetWealth(iPlr)< 10) return(0,Message("$NotEnoughMoney$",this()),Sound("Error"));
  SetWealth(iPlr,GetWealth(iPlr)- 10);
  Sound("Cash");
  CreateContents(EH8Z,this(),4);
  if(idItem) ContainedUp(pClonk) && SelectMenuItem(7,pClonk);
  return(1);
}

protected func BuyGrasSeed(idItem,pClonk)
{
  if(!idItem && FindContents(GA9I))
    {
    RemoveObject(FindContents(GA9I));
    iSeedgras++;    
    return(1);    
    }
  var iPlr= GetOwner(pClonk);
  if(GetWealth(iPlr)< 5) return(0,Message("$NotEnoughMoney$",this()),Sound("Error"));
  SetWealth(iPlr,GetWealth(iPlr)- 5);
  Sound("Cash");
  iSeedgras+= 5;
  if(idItem) ContainedUp(pClonk) && SelectMenuItem(5,pClonk);
  return(1);
}

protected func BuySeed(idItem,pClonk)
{
  if(!idItem && FindContents(SA9I))
    {
    RemoveObject(FindContents(SA9I));
    iSeed++;    
    return(1);    
    }
  var iPlr= GetOwner(pClonk);
  if(GetWealth(iPlr)< 20) return(0,Message("$NotEnoughMoney$",this()),Sound("Error"));
  SetWealth(iPlr,GetWealth(iPlr)- 20);
  Sound("Cash");
  iSeed+= 10;
  if(idItem) ContainedUp(pClonk) && SelectMenuItem(3,pClonk);
  return(1);
}

private func TakeSeed(idItem,pClonk)
{
  if(!iSeed) return(ContainedUp(pClonk),SelectMenuItem(4,pClonk));
  var Limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(pClonk));
  if(ContentsCount(0,pClonk)>= Limit && Limit!= 0)
   return(ContainedUp(pClonk),SelectMenuItem(4,pClonk));
  Sound("Grab");
  LocalN("idTree",CreateContents(SA9I,pClonk))= idTree;
  iSeed--;
  ContainedUp(pClonk) && SelectMenuItem(4,pClonk);
}

private func TakeGrasSeed(idItem,pClonk)
{
  if(!iSeedgras) return(ContainedUp(pClonk),SelectMenuItem(4,pClonk));
  var Limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(pClonk));
  if(ContentsCount(0,pClonk)>= Limit && Limit!= 0)
   return(ContainedUp(pClonk),SelectMenuItem(6,pClonk));
  Sound("Grab");
  LocalN("idTree",CreateContents(GA9I,pClonk))= idTree;
  iSeedgras--;
  ContainedUp(pClonk) && SelectMenuItem(6,pClonk);
}

private func Plant(idItem,pClonk)
{
  if(!iSeed)
   if(!BuySeed())
    return(SetCommand(pClonk,"None"));
  // Wird nur als Wegpunkt erstellt
  var Ziel= PlaceVegetation(TRE1,-iRadius,-iRadius,2* iRadius,2* iRadius,1);
  if(!Ziel) return(Message("$NoSuitablePlace$",this()));
  iSeed--;
  // 7. Vorgang wiederholen!
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),0,0,0,"Loop");
  // 6. Warten!
  AddCommand(pClonk,"Wait",0,0,0,0,50);
  // 5. Forsthaus betreten!
  AddCommand(pClonk,"Enter",this());
  // 4. Warten!
  AddCommand(pClonk,"Wait",0,0,0,0,50);
  // 3. Baum pflanzen!
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),ObjectNumber(Ziel),0,0,"PlantTree");
  // 2. Warten!
  AddCommand(pClonk,"Wait",0,0,0,0,100);
  // 1. Zum Zielpunkt bewegen!
  AddCommand(pClonk,"MoveTo",0,GetX(Ziel),GetY(Ziel));
  RemoveObject(Ziel);
}

private func Loop(Par,iObj)
{
  var pObj= Object(iObj);
  SetCommand(pObj,"None");
  Plant(0,pObj);
}

private func PlantTree(Par,iObj,iTarget)
{
  var pObj= Object(iObj);
  var pTarget= Object(iTarget);
  var ID= idTree,i= Random(4);
  if(ID== RMMG)
    {
    if(i== 0) ID= TRE1;
    if(i== 1) ID= TRE2;
    if(i== 2) ID= TRE3;
    if(i== 3) ID= TRE4;
    }
  // Ist noch Platz für einen Baum?
  var iSize= DefinitionCall(ID,"ReproductionAreaSize"),iOffset= iSize/ -2;
  if(!(pObj-> ObjectCount(0,iOffset,iOffset,iSize,iSize,OCF_Chop())< DefinitionCall(ID,"MaxTreeCount")))
   return(Message("$NoPlace$",pObj));
  pObj-> CreateConstruction(ID,0,+15,-1,5);
  ObjectSetAction(pObj,"Bridge");
}

private func SetTreeSortMenu(idItem,pClonk)
{
  var Typ,i= 0;
  CreateMenu(idItem,pClonk,0,1,"$NoPlants$");
  AddMenuItem("$Random$","SetTreeSort",RMMG,pClonk,0,0,"$PlantRandomTrees$");
  while(Typ= GetDefinition(i++,C4D_StaticBack()))
   if(GetCategory(0,Typ)== 32769)
    if(Typ!= CACT)
    if(Typ!= CAC2)
     if(!WildcardMatch(GetName(0,Typ),"Verbrannt*"))
      if(!WildcardMatch(GetName(0,Typ),"Burned*"))
       if(!WildcardMatch(GetName(0,Typ),"Tot*"))
        if(!WildcardMatch(GetName(0,Typ),"Dead*"))
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
    SetOwner(GetOwner(pClonk),Mark_LO); SetVisibility(VIS_Owner(),Mark_LO);
    SetOwner(GetOwner(pClonk),Mark_LU); SetVisibility(VIS_Owner(),Mark_LU);
    SetOwner(GetOwner(pClonk),Mark_RO); SetVisibility(VIS_Owner(),Mark_RO);
    SetOwner(GetOwner(pClonk),Mark_RU); SetVisibility(VIS_Owner(),Mark_RU);
    }
  CreateMenu(WOOD,pClonk);
  AddMenuItem("$IncreaseRadius$","SetRadius",LMM3,pClonk,0,pClonk); 
  AddMenuItem("$ReduceRadius$","SetRadius",LMM7,pClonk,0,pClonk);  
}

private func SetRadius(idItem,pClonk)
{  
  var iChange,iSMI;
  if(idItem== LMM3) iChange= (+5);
  if(idItem== LMM7) { iChange= (-5); iSMI= 1; }
  iRadius= iRadius+ iChange;
  if(!Inside(iRadius,100,500))
    {
    if(iRadius> 500) iRadius= 500;
    if(iRadius< 100) iRadius= 100;
    }
  else
    {
    Mark_LO-> Move(iChange);
    Mark_LU-> Move(iChange);
    Mark_RO-> Move(iChange);
    Mark_RU-> Move(iChange);
    }
  Message("$ShowRadius$",this(),iRadius);
  RadiusMenu(0,pClonk,1) && SelectMenuItem(iSMI,pClonk);
}

protected func MenuQueryCancel()
{
  return(0,RemoveMarks());
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