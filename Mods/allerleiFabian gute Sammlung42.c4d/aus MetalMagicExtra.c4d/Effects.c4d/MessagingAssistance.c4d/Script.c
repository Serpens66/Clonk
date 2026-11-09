/*-- Hilfsanzeige --*/

#strict 3

local sMessage;
local showTo;
local iFade;

func Initialize()
{
 SetPosition(120,120,this());
 Local()=nil;
 Local(1)=nil;
}

func Initialized() {
/* var obj;
 var cnt=0;
 while(FindObject2(Find_Distance(10),Find_ID(GetID(this())),Find_Exclude(this())))
  for(obj in FindObjects(Find_Distance(10),Find_ID(GetID(this()),Find_Exclude(this()))))
  {
   if(cnt++ > 100)break;
   if(LocalN("showTo",obj) != showTo)continue;
   SetPosition(GetX(),GetY()+(obj->GetLineCount())*20,this());
   break;
  }*/
  return(1);
}

func GetLineCount()
{
 var iCount;
 for(var cnt=nil;(cnt ?? 0)<(GetLength(sMessage) ?? 0);S42MigrationStep(cnt, 1, true))
  if(Equal(GetChar(sMessage,cnt), 124))S42MigrationStep(iCount, 1, true);
 return((iCount ?? 0)+1);
}

public func ShowMessage(sMsg,iPlr)
{
// showTo=iPlr;
 sMsg=GetStringTableString(sMsg);
// if(!sMessage) sMessage = sMsg;
// else sMessage = Format("%s||%s", sMsg, sMessage);
// var msg=Format("{{%i}}<c %x>%s</c>",GetID(),RGB(255,50,50),sMsg);
 
 CustomMessage(sMsg,nil,iPlr,25,100,RGB(255,50,50),MDMT,"MAHP",nil);//MSG_NoLinebreak
 
 Sound("SpellSysCombo1",nil,nil,150,iPlr);
// iFade = 0;
 return(RemoveObject());
// Initialized();
}

func DoFade()
{
 S42MigrationAssign(iFade, "+=", 3);
 if((iFade ?? 0) > 180) return(S42MigrationFirst(CustomMessage(nil, nil, showTo), RemoveObject()));
 CustomMessage(sMessage,nil,showTo,25,100,RGBa(255,50,50,nil),MDMT,"MAHP",nil);//MSG_NoLinebreak
}

public func GetStringTableString(sMsg)
{
 if(Equal(sMsg, "OnChurchEnter")) return("$OnChurchEnter$");
 if(Equal(sMsg, "OnHoligonIngotCollect")) return("$OnHoligonIngotCollect$");
 if(Equal(sMsg, "OnHavingSpellDelay")) return("$OnHavingSpellDelay$");
 if(Equal(sMsg, "OnStartPraying")) return("$OnStartPraying$");
 if(Equal(sMsg, "OnStaffCollect")) return("$OnStaffCollect$");
 if(Equal(sMsg, "OnEnemySainthood")) return("$OnEnemySainthood$");
 if(Equal(sMsg, "OnFriendlySainthood")) return("$OnFriendlySainthood$");
 if(Equal(sMsg, "OnCultsiteMenu")) return("$OnCultsiteMenu$");
 if(Equal(sMsg, "OnMagicRoomEnter")) return("$OnMagicRoomEnter$");
 if(Equal(sMsg, "OnChurchEnter")) return("$OnChurchEnter$");
 return(sMsg);
}

global func GetHelpMessageBit(sMsg)
{
 if(Equal(sMsg, "OnChurchEnter")) return(Pow(2,nil));
 if(Equal(sMsg, "OnHoligonIngotCollect")) return(Pow(2,1));
 if(Equal(sMsg, "OnHavingSpellDelay")) return(Pow(2,2));
 if(Equal(sMsg, "OnStartPraying")) return(Pow(2,3));
 if(Equal(sMsg, "OnStaffCollect")) return(Pow(2,4));
 if(Equal(sMsg, "OnEnemySainthood")) return(Pow(2,5));
 if(Equal(sMsg, "OnFriendlySainthood")) return(Pow(2,6));
 if(Equal(sMsg, "OnCultsiteMenu")) return(Pow(2,7));
 if(Equal(sMsg, "OnMagicRoomEnter")) return(Pow(2,8));
 if(Equal(sMsg, "OnStaffCollect")) return(Pow(2,9));
}

global func MMShowHelpMessage(string sMsg, int iPlr)
{
 iPlr ??= 0;
 var bit=GetHelpMessageBit(sMsg);
 var bitset=GetPlrExtraData(iPlr,"MMHelpMessages");
 if(!Equal(GetPlayerType(iPlr), C4PT_User)) return;
 if((bitset ?? 0) & (bit ?? 0))return;
// var obj = FindObject2(Find_ID(MAHP), Find_Owner(iPlr));
// if(!obj) obj=CreateObject(MAHP,0,0,iPlr);
 MAHP->ShowMessage(sMsg,iPlr);
 
 SetPlrExtraData(iPlr,"MMHelpMessages",(bitset ?? 0) | (bit ?? 0));
}
