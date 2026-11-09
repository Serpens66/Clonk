/*-- Außenposten --*/

local caller, own, number, seg, index;

#strict

//Eingangssteuerung:

ActivateEntrance:
  if(ActIdle()) SetAction("Opening");
  return(1);

SoundOpen: return(Sound("SteelGate2"));
SoundClose: return(Sound("SteelGate2"));
Offen: return(SetEntrance(1));
Close: return(SetEntrance(0));

//Loren leeren:
Check:
  var obj;
  while(obj=FindObject(LORY,-10,-10,21,21,0,0,0,this(),obj))
    GrabContents(obj);
  return(1);

//Die Station des gewählten Spieler finden:
HasStation:
  var obj;
  while(obj = FindObject(HSSN,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Par(0))
      if(GetCon(obj)==100)
        return(obj);
  while(obj = FindObject(AEHN,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Par(0))
      if(GetCon(obj)==100)
        return(obj);
  return(0);

Effekt2:
  var obj;
  ObjectCall(obj=CreateObject(EFKT,0,0),"SetTarget",Par(0),Par(2),Par(3),Par(4),Par(5));
  if(Par(1))
    Enter(obj,Par(1));
  return(1);

Construction:
  own=GetOwner();
  seg = CreateObject(BAS3,0,8);
  var seg1 = RandomX(1,6), seg2;
  seg2="A";
  if(seg1==2) seg2="B";
  if(seg1==3) seg2="C";
  if(seg1==4) seg2="D";
  if(seg1==5) seg2="E";
  if(seg1==6) seg2="F";
  index=Format("%s - %d",seg2,RandomX(0,999));
  return(1);

ANPNIndex: return(index);

//Indexwert ansehen:
ContainedDig: return(Message("Der Index dieser Station ist: %s",this(),index));

//Menüsystem:
ContainedUp:
  Sound("Click");
  caller=Par(0);
  CreateMenu(ANPN,caller,0,0,0,0,0,0);
  AddMenuItem("Transfer","Transfer",OTTR,caller);
  AddMenuItem(Format("%s in Handelsstation transferieren.",GetName(caller)),"CTransfer",TRAN,caller);
  AddMenuItem("Handelsstationsmenü aufrufen","HSSNMenu",GetID(HasStation(own)),caller);
  return(1);

/* - - - - - - - Transfer: - - - - - - - */
Transfer:
  Sound("Click");
  CreateMenu(ANPN,caller,0,0,0,0,0,0);
  AddMenuItem("Holen","Get",_GET,caller);
  AddMenuItem("Senden","Put",SEND,caller);
  return(1);

/* - - - - - Holen: - - - - - */

Get:
  var obj, station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(0);
}
  GetMenu(0);
  return(1);

GetMenu:
  var station=HasStation(own);
  Sound("Click");
  number=0;
  CreateMenu(_GET,caller,0,0,"Keine Objekte in Handelsstation gefunden!",0,0,0);
  var obj, num;
  while(obj = GetDefinition(num,C4D_All()))
{
    AddNewItemGet(obj,station);
    num++;
}
  SelectMenuItem(Par(0),caller);
  return(1);

AddNewItemGet:
//Nicht enthalten:
  if(!FindContents(Par(0),Par(1)))
    return(0);
//Lebensform:
  if(GetCategory(0,Par(0)) & C4D_Living())
    return(0);

//Auf liste setzen:
  AddMenuItem(Format("%s aus Handelsstation holen",GetName(0,Par(0))),"GetNow",Par(0),caller,ContentsCount(Par(0),Par(1)),number);
  number++;
  return(1);

GetNow:
  var station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(0);
}
  var obj=FindContents(Par(0),station);
  if(!obj)
{
    Sound("Error");
    Message("Das Objekt ist nicht mehr enthalten!",this());
    return(0);
}

  ObjectCall(station,"Effekt");
  ObjectCall(station,"Blitz",this());
  Exit(obj,0,0);
  ObjectCall(station,"Effekt2",this(),obj,1);
  Message("Transfer erfolgreich!",this());
  GetMenu(Par(1));
  return(1);

/* - - - - - Senden: - - - - - */

Put:
  var obj, station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(0);
}
  PutMenu(0);
  return(1);

PutMenu:
  Sound("Click");
  number=0;
  CreateMenu(SEND,caller,0,0,"Keine Objekte in dieser Station gefunden!",0,0,0);
  var obj, num;
  while(obj = GetDefinition(num,C4D_All()))
{
    AddNewItemPut(obj);
    num++;
}
  SelectMenuItem(Par(0),caller);
  return(1);

AddNewItemPut:
//Nicht enthalten:
  if(!FindContents(Par(0)))
    return(0);
//Lebensform:
  if(GetCategory(0,Par(0)) & C4D_Living())
    return(0);

//Auf liste setzen:
  AddMenuItem(Format("%s zur Handelsstation senden",GetName(0,Par(0))),"SendNow",Par(0),caller,ContentsCount(Par(0)),number);
  number++;
  return(1);

SendNow:
  var station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(0);
}
  var obj=FindContents(Par(0));
  if(!obj)
{
    Sound("Error");
    Message("Das Objekt ist nicht mehr enthalten!",this());
    return(0);
}

  ObjectCall(station,"Blitz",this());
  Exit(obj,0,0);
  Effekt2(station,obj,1);
  Message("Transfer erfolgreich!",this());
  PutMenu(Par(1));
  return(1);

/* - - - - - - - Clonktransfer: - - - - - - - */

CTransfer:
  Effekt2(HasStation(own),caller,1);
  ObjectCall(HasStation(own),"Blitz",this());
  Sound("Magic1");
  return(1);

/* - - - - - - - Handelsstationsmenü aufrufen: - - - - - - - */
HSSNMenu:
  var obj=HasStation(own);
  if(!obj)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(0);
}

  ObjectCall(obj,"ContainedUp",caller);
  return(1);