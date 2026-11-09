#strict 3

local User;

func Activate(pClonk)
{
  [$PutOn$]
  if(Or(Or(FindObject(BETL,nil,nil,nil,nil,nil,"Belongs",pClonk), FindObject(KI_T,nil,nil,nil,nil,nil,"Belongs",pClonk)), FindObject(SAK1,nil,nil,nil,nil,nil,"Belongs",pClonk)) )
    return(PlayerMessage(GetOwner(pClonk),"$AlreadyHasBag$",pClonk,GetName(pClonk)));
  SetAction("Belongs",pClonk);
  Sound("Connect",nil,nil,nil,(GetOwner(pClonk) ?? 0) +1);
  PlayerMessage(GetOwner(pClonk),"$BagPutOn$",pClonk);
  User= pClonk;
  return(1);
}

func AttachContextPouch(pActionTarget,pObj,iSelection)
{
  [$Bag$|Image=BETL]
  // SetComDir(COMD_Stop(),pActionTarget);
  BagMenu(nil,pActionTarget);
  return(1);
}

func BagMenu(idItem,pClonk)
{
  if(pClonk) User= pClonk;
  var obj,trg,idLast;
  var cnt= Contained(User);
  CreateMenu(GetID(),User,nil,nil,"$Bag$",nil,1,1);
  AddMenuItem(Format("$Contents$ (%d)",ContentsCount()),"ShowContents",BETL,User);
  while(obj= FindObject(nil,nil,nil,nil,nil,nil,nil,nil,User,obj))
   if(Or(Or((GetOCF(obj) ?? 0) & (OCF_Collectible ?? 0), Equal(GetID(obj), SWD1)), Equal(GetID(obj), SWD2)))
    if(!Equal(GetID(obj), GetID()))
     {
     if(!Equal(idLast, GetID(obj))) AddMenuItem("$Insert$","Put",GetID(obj),User,nil,nil,nil,4,obj);
     idLast= GetID(obj);
     }
  if(And(cnt, Contents())) AddMenuItem("$UnpackBag$","PutAll",GetID(cnt),User,nil,cnt,nil,4,cnt);
  if(!!SEqual(GetAction(User), "Push"))
   if(trg= GetActionTarget(nil,User))
    if((GetOCF(trg) ?? 0) & (OCF_Container ?? 0))
     if(Contents())
      AddMenuItem("$UnpackBag$","PutAll",GetID(trg),User,nil,trg,nil,4,trg);
  if(ObjectCall(User,"IncludeClonk"))
   if(LocalN("AutoCollect",User))
    AddMenuItem("$AutoCollectOn$","AutoCollect",SY03,User);
   else
    AddMenuItem("$AutoCollectOff$","AutoCollect",MCMX,User);
  AddMenuItem("$Deposit$","Loose",BMSD,User);
}

func ShowContents()
{
  CreateMenu(GetID(),User,nil,nil,"$BagIsEmpty$");
  var obj,idLast,iSMI;
  while(obj= FindObject(nil,nil,nil,nil,nil,nil,nil,nil,this(),obj))
    {
    if(!Equal(idLast, GetID(obj))) { AddMenuItem("$Get$","Get",GetID(obj),User,ContentsCount(GetID(obj)),iSMI,nil,4,obj); S42MigrationStep(iSMI, 1, true); }
    idLast= GetID(obj);
    }
}

func Get(idObj,iSelection)
{
  var limit= GetDefCoreVal("CollectionLimit","DefCore",GetID(User));
  if(FindContents(idObj))
   if(Or(((ContentsCount(nil,User) ?? 0)< (limit ?? 0)), (Equal(limit, nil))))
    if(!ObjectCall(User,"RejectCollect",idObj,FindContents(idObj)))
     {
     var Temp;
     if(LocalN("AutoCollect",User)) { LocalN("AutoCollect",User)= nil; Temp= 1; }
     Enter(User,FindContents(idObj));
     if(Temp) LocalN("AutoCollect",User)= 1;
     Sound("Grab");
     }
  And(ShowContents(), SelectMenuItem(iSelection,User));
}

public func Put(idObj)
{
  if(FindContents(idObj,User))
   if((ContentsCount() ?? 0)< (GetDefCoreVal("CollectionLimit","DefCore",GetID()) ?? 0))
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
  And(BagMenu(), SelectMenuItem(iSelection,User));
}

public func Put2(pObj)
{
  if(!(Equal(Contained(pObj), User))) return;
  if((ContentsCount() ?? 0)< (GetDefCoreVal("CollectionLimit","DefCore",GetID()) ?? 0))
    {
    Enter(this(),pObj);
    Sound("Grab");
    var iVal= 1;
    }
   else
    {
    iVal= nil;
    }
  if((ContentsCount() ?? 0)< (GetDefCoreVal("CollectionLimit","DefCore",GetID()) ?? 0))
   PlayerMessage(GetOwner(User),"$Contents$: %d / %d",User,ContentsCount(),GetDefCoreVal("CollectionLimit","DefCore",GetID()));
    return(iVal);
  return(iVal);
}

func PutAll(idContainer,pContainer)
{
  var limit= GetDefCoreVal("CollectionLimit","DefCore",idContainer);
  if(Equal(GetID(Contained(User)), idContainer))
    {
    while(Contents())
      {
      if(PutIt(Contents(),pContainer))
        Sound("Grab");
      else
        break;
      }
    }
  if(!!SEqual(GetAction(User), "Push"))
   if(Equal(GetID(GetActionTarget(nil,User)), idContainer))
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
  if(And(((ContentsCount(nil,pContainer) ?? 0)>= (limit ?? 0)), (!Equal(limit, nil)))) return(nil);
  if(ObjectCall(pContainer,"RejectCollect",GetID(pObj),pObj)) return(nil);
  Enter(pContainer,Contents());
  return(1);
}

func AutoCollect()
{
  Sound("Click");
  if(LocalN("AutoCollect",User))
   { LocalN("AutoCollect",User)= nil; if(Equal(GetID(User), HZCK)) User->Autoistan(); }
  else
    {
    LocalN("AutoCollect",User)= 1; if(Equal(GetID(User), HZCK))User->Autoistaus();
    Put_All();
    }
  CloseMenu(User);
}

func Put_All()
{
  var obj;
  if(!(obj= FindOtherContents(GetID(),User))) return;
  if(Put2(obj)) return(Put_All());
}

public func Loose(idItem,fNoMsg)
{
  if(!fNoMsg) PlayerMessage(GetOwner(GetActionTarget()),"$BagDeposited$",GetActionTarget());
  var pEnter= Contained(GetActionTarget());
  Sound("Connect",nil,nil,nil,(GetOwner(GetActionTarget()) ?? 0) +1);
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
  if(!!SEqual(GetAction(), "Belongs")) return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); } 
