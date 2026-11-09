/*-- Hilfsanzeige --*/

#strict

local sMessage;
local showTo;
local iFade;

func Initialize()
{
 SetPosition(120,120,this());
 Local()=0;
 Local(1)=0;
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
 for(var cnt=0;cnt<GetLength(sMessage);cnt++)
  if(GetChar(sMessage,cnt) == 124)iCount++;
 return(iCount+1);
}

public func ShowMessage(sMsg,iPlr)
{
// showTo=iPlr;
 sMsg=GetStringTableString(sMsg);
// if(!sMessage) sMessage = sMsg;
// else sMessage = Format("%s||%s", sMsg, sMessage);
// var msg=Format("{{%i}}<c %x>%s</c>",GetID(),RGB(255,50,50),sMsg);
 
 CustomMessage(sMsg,0,iPlr,25,100,RGB(255,50,50),MDMT,"MAHP",0);//MSG_NoLinebreak
 
 Sound("SpellSysCombo1",0,0,150,iPlr);
// iFade = 0;
 return(RemoveObject());
// Initialized();
}

func DoFade()
{
 iFade+=3;
 if(iFade > 180) return(CustomMessage(0, 0, showTo), RemoveObject());
 CustomMessage(sMessage,0,showTo,25,100,RGBa(255,50,50,0),MDMT,"MAHP",0);//MSG_NoLinebreak
}

public func GetStringTableString(sMsg)
{
 if(sMsg == "OnChurchEnter") return("$OnChurchEnter$");
 if(sMsg == "OnHoligonIngotCollect") return("$OnHoligonIngotCollect$");
 if(sMsg == "OnHavingSpellDelay") return("$OnHavingSpellDelay$");
 if(sMsg == "OnStartPraying") return("$OnStartPraying$");
 if(sMsg == "OnStaffCollect") return("$OnStaffCollect$");
 if(sMsg == "OnEnemySainthood") return("$OnEnemySainthood$");
 if(sMsg == "OnFriendlySainthood") return("$OnFriendlySainthood$");
 if(sMsg == "OnCultsiteMenu") return("$OnCultsiteMenu$");
 if(sMsg == "OnMagicRoomEnter") return("$OnMagicRoomEnter$");
 if(sMsg == "OnChurchEnter") return("$OnChurchEnter$");
 return(sMsg);
}

global func GetHelpMessageBit(sMsg)
{
 if(sMsg == "OnChurchEnter") return(Pow(2,0));
 if(sMsg == "OnHoligonIngotCollect") return(Pow(2,1));
 if(sMsg == "OnHavingSpellDelay") return(Pow(2,2));
 if(sMsg == "OnStartPraying") return(Pow(2,3));
 if(sMsg == "OnStaffCollect") return(Pow(2,4));
 if(sMsg == "OnEnemySainthood") return(Pow(2,5));
 if(sMsg == "OnFriendlySainthood") return(Pow(2,6));
 if(sMsg == "OnCultsiteMenu") return(Pow(2,7));
 if(sMsg == "OnMagicRoomEnter") return(Pow(2,8));
 if(sMsg == "OnStaffCollect") return(Pow(2,9));
}

global func MMShowHelpMessage(string sMsg, int iPlr)
{
 var bit=GetHelpMessageBit(sMsg);
 var bitset=GetPlrExtraData(iPlr,"MMHelpMessages");
 if(GetPlayerType(iPlr)!=C4PT_User) return();
 if(bitset & bit)return();
// var obj = FindObject2(Find_ID(MAHP), Find_Owner(iPlr));
// if(!obj) obj=CreateObject(MAHP,0,0,iPlr);
 MAHP->ShowMessage(sMsg,iPlr);
 
 SetPlrExtraData(iPlr,"MMHelpMessages",bitset | bit);
}
