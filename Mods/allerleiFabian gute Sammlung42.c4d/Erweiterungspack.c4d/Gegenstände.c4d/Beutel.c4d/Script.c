#strict

local User;

func Activate(pClonk)
{
  [$PutOn$]
  if(FindObject(BETL,0,0,0,0,0,"Belongs",pClonk) || FindObject(KI_T,0,0,0,0,0,"Belongs",pClonk) || FindObject(SAK1,0,0,0,0,0,"Belongs",pClonk) ) 
    return(PlayerMessage(GetOwner(pClonk),"$AlreadyHasBag$",pClonk,GetName(pClonk)));
  SetAction("Belongs",pClonk);
  Sound("Connect",0,0,0,GetOwner(pClonk) +1);
  PlayerMessage(GetOwner(pClonk),"$BagPutOn$",pClonk);
  User= pClonk;
  return(1);
}

func AttachContextPouch(pActionTarget,pObj,iSelection)
{
  [$Bag$|Image=BETL]
  // SetComDir(COMD_Stop(),pActionTarget);
  BagMenu(0,pActionTarget);
  return(1);
}

func BagMenu(idItem,pClonk)
{
  if(pClonk) User= pClonk;
  var obj,trg,idLast;
  var cnt= Contained(User);
  CreateMenu(GetID(),User,0,0,"$Bag$",0,1,1);
  AddMenuItem(Format("$Contents$ (%d)",ContentsCount()),"ShowContents",BETL,User);
  while(obj= FindObject(0,0,0,0,0,0,0,0,User,obj))  
   if(GetOCF(obj) & OCF_Collectible() || GetID(obj)== SWD1 || GetID(obj)== SWD2)
    if(GetID(obj)!= GetID())
     {
     if(idLast!= GetID(obj)) AddMenuItem("$Insert$","Put",GetID(obj),User,0,0,0,4,obj);
     idLast= GetID(obj);
     }
  if(cnt && Contents()) AddMenuItem("$UnpackBag$","PutAll",GetID(cnt),User,0,cnt,0,4,cnt);
  if(GetAction(User) eq "Push")
   if(trg= GetActionTarget(0,User)) 
    if(GetOCF(trg) & OCF_Container())
     if(Contents())
      AddMenuItem("$UnpackBag$","PutAll",GetID(trg),User,0,trg,0,4,trg);
  if(ObjectCall(User,"IncludeClonk"))
   if(LocalN("AutoCollect",User))
    AddMenuItem("$AutoCollectOn$","AutoCollect",SY03,User);
   else
    AddMenuItem("$AutoCollectOff$","AutoCollect",MCMX,User);
  AddMenuItem("$Deposit$","Loose",BMSD,User);
}

func ShowContents()
{
  CreateMenu(GetID(),User,0,0,"$BagIsEmpty$");
  var obj,idLast,iSMI;
  while(obj= FindObject(0,0,0,0,0,0,0,0,this(),obj))
    {
    if(idLast!= GetID(obj)) { AddMenuItem("$Get$","Get",GetID(obj),User,ContentsCount(GetID(obj)),iSMI,0,4,obj); iSMI++; }
    idLast= GetID(obj);
    }
}

func Get(idObj,iSelection)
{
  var limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(User));
  if(FindContents(idObj))
   if((ContentsCount(0,User)< limit) || (limit== 0))
    if(!ObjectCall(User,"RejectCollect",idObj,FindContents(idObj)))
     {
     var Temp;
     if(LocalN("AutoCollect",User)) { LocalN("AutoCollect",User)= 0; Temp= 1; }
     Enter(User,FindContents(idObj));
     if(Temp) LocalN("AutoCollect",User)= 1;
     Sound("Grab");
     }
  ShowContents() && SelectMenuItem(iSelection,User);
}

public func Put(idObj)
{
  if(FindContents(idObj,User))
   if(ContentsCount()< GetDefCoreVal("CollectionLimit","DefCore",GetID()))
    {
    Enter(this(),FindContents(idObj,User));
    Sound("Grab");
    }
   else
    {
    PlayerMessage(GetOwner(User),"$BagIsFull$",User);
    }
  // Menü aktualisieren
  var iSelection;
  if(FindOtherContents(GetID(),User)) iSelection= GetMenuSelection(User);
  BagMenu() && SelectMenuItem(iSelection,User);
}

public func Put2(pObj)
{
  if(!(Contained(pObj)== User)) return();
  if(ContentsCount()< GetDefCoreVal("CollectionLimit","DefCore",GetID()))
    {
    Enter(this(),pObj);
    Sound("Grab");
    var iVal= 1;
    }
   else
    {
    iVal= 0;
    }
  if(ContentsCount()< GetDefCoreVal("CollectionLimit","DefCore",GetID()))
   PlayerMessage(GetOwner(User),"$Contents$: %d / %d",User,ContentsCount(),GetDefCoreVal("CollectionLimit","DefCore",GetID()));
    return(iVal);
  return(iVal);
}

func PutAll(idContainer,pContainer)
{
  var limit= GetDefCoreVal("CollectionLimit","DefCore",idContainer);
  if(GetID(Contained(User))== idContainer)
    {
    while(Contents())
      {
      if(PutIt(Contents(),pContainer))
        Sound("Grab");
      else
        break;
      }
    }
  if(GetAction(User) eq "Push")
   if(GetID(GetActionTarget(0,User))== idContainer)
    {
    while(Contents())
      {
      if(PutIt(Contents(),pContainer))
        Sound("Grab");
      else
        break;
      }
    }
  BagMenu();
}

func PutIt(pObj,pContainer)
{
  var limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(pContainer));
  if((ContentsCount(0,pContainer)>= limit) && (limit!= 0)) return(0);
  if(ObjectCall(pContainer,"RejectCollect",GetID(pObj),pObj)) return(0);
  Enter(pContainer,Contents());
  return(1);
}

func AutoCollect()
{
  Sound("Click");
  if(LocalN("AutoCollect",User))
   { LocalN("AutoCollect",User)= 0; if(GetID(User)== HZCK) User->Autoistan(); }
  else
    {
    LocalN("AutoCollect",User)= 1; if(GetID(User)== HZCK)User->Autoistaus();
    Put_All();
    }
  CloseMenu(User);
}

func Put_All()
{
  var obj;
  if(!(obj= FindOtherContents(GetID(),User))) return();
  if(Put2(obj)) return(Put_All());
}

public func Loose(idItem,fNoMsg)
{
  if(!fNoMsg) PlayerMessage(GetOwner(GetActionTarget()),"$BagDeposited$",GetActionTarget());
  var pEnter= Contained(GetActionTarget());
  Sound("Connect",0,0,0,GetOwner(GetActionTarget()) +1);
  SetAction("Idle");
  SetActionTargets();
  if(pEnter) Enter(pEnter);
  CloseMenu(User); 
}

func Hit()
{
  Sound("WoodHit*");
}

func RejectEntrance(pContainer)
{
  if(GetAction() eq "Belongs") return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); } 
