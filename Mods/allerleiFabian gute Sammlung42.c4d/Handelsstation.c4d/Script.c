/*-- Handelsstation --*/

local caller, own, value, Player, Objekt, oldValue, basement;

#strict


/* - - - - - - - - Algemeines: - - - - - - - */

//Überprüfen, ob der gewählte Spieler eine Handelsstation hat:
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

IsSpam:
  var obj;
  while(obj = FindObject(SMSR,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==own)
      if(ObjectCall(obj,"Object")==Par(0))
        return(obj);
  return(0);

HasSpam:
  var obj;
  while(obj = FindObject(SMSR,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Par(0))
      if(ObjectCall(obj,"Object")==own)
        return(1);
  return(0);

//Der Kleine Effekt:
Effekt:
  CreateObject(LIGT,8,-20);
  return(1);

//Blitz:
//  LaunchLightning(GetX(this()),GetY(this()) ,GetX(Par(0))-GetX(this()), 0 //,GetY(Par(0))-GetY(this()), 0);
//  return(1);

Effekt2:
var obj;
ObjectCall(obj=CreateObject(EFKT,0,0),"SetTarget",Par(0),Par(2),Par(3),Par(4),Par(5));
if(Par(1))
  Enter(obj,Par(1));
return(1);

//Hat der Spieler bereits eine Handelsstation?
Construction:
  own = GetOwner();
  var obj;
  if(HasStation(own)!=0)
{
    Sound("Error");
    Message("Du hast bereits eine Handelsstation!",this());
    RemoveObject();
    return(1);
}
  basement = CreateObject(BS13,0,+8,GetOwner());
  return(1);

//Eingangssteuerung:

ActivateEntrance:
  if(ActIdle()) SetAction("Opening");
  return(1);

SoundOpen: return(Sound("SteelGate2"));
SoundClose: return(Sound("SteelGate2"));
Offen: return(SetEntrance(1));
Close: return(SetEntrance(0));

//Loren werden geleert:
Check:
  var obj,Sender;
  while(obj=FindObject(LORY,-10,-10,21,21,0,0,0,this(),obj))
    GrabContents(obj);

//Von einer Station zur anderen Beamen:
  while(obj=FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this(),obj))
    if(Sender=FindContents(SNDR,obj))
      if(GetOwner(obj)!=own)
{
        Exit(obj,0,0);
        var Station=HasStation(GetOwner(obj));
        Effekt();
        Effekt2(Station,obj,1);
      //  Blitz(Station);
        Sound("Magic1",1);
        RemoveObject(Sender);
}
//Chips entfernen:
  while(obj=FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this(),obj))
    if(GetOwner(obj)==own)
      if(Sender=FindContents(SNDR,obj))
{
        Effekt();
        RemoveObject(Sender);
}
  return(1);

protected func Destruction()
{
  if (basement) RemoveObject(basement); 
  return(_inherited());
}



/* - - - - - - - Das Hauptmenü: - - - - - - - */

ContainedUp:
  if(GetOwner(Par(0))!=own)
{
    Sound("Error");
    Message("Das ist kein Clonk von %s!",this(),GetPlayerName(own));
    return(0);
}

  Sound("Click");
  caller=Par(0);
  CreateMenu(HSSN,caller,0,0,0,0,0,0);
  AddMenuItem("Verkaufsliste erstellen","SellList",LIST,caller);
  AddMenuItem("Kaufen","Buy",_BUY,caller);
  AddMenuItem("Geld geben","Money",MONY,caller);
  AddMenuItem("Meldung an einen Spieler geben","Meldung",MELD,caller);
  AddMenuItem("Spam - Menü","Spams",SPAM,caller);
  AddMenuItem("Clonk transferieren","Transfer",TRAN,caller);
  return(1);

/* - - - - - - - Geld an einen anderen Spieler geben: - - - - - - - */

Money:
  Sound("Click");
  CreateMenu(MONY,caller,0,0,"Keine weiteren Spieler mit Handelsstation gefunden!",0,0,0);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; ++i) 
  {
    if (i != own)
      if(HasStation(i)!=0)
        AddMenuItem(Format("Geld an %s geben",GetPlayerName(i)) , "SetMoneyAmount",STMY,caller,0,i);
    --Players;
  }
  return(1);

SetMoneyAmount:
  Sound("Click");
  value=0;
  ShowM();
  Player=Par(1);
  CreateMenu(STMY,caller,0,0,0,0,0,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,0,0);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,0,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,0,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,0,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,0,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,0,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,0,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,0,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,0,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,0,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Geldbetrag geben","GiveMoney",OKAY,caller);
  return(1);

AddNumber:
  if(value >= 1000)
    return(1);

  Sound("Click");
  var ZSave = value;
  value=value*10+Par(1);
  ShowM();
  return(1);

ShowM:
  Message("Geldbetrag: %d",this(),value);
  return(1);

Delete:
  value=value / 10;
  ShowM();
  Sound("Click");
  return(1);

GiveMoney:
  if(GetWealth(own)<value)
{
    Sound("Error");
    Message("Du hast nicht genug Geld dafür!",this());
    return(1);
}

  Sound("Uncash");
  CloseMenu(caller);
  var Station=HasStation(Player);
  Effekt();
//  Blitz(Station);
  Effekt2(Station,0,2,value,Player);
  SetWealth(own,GetWealth(own)-value);
  Message("%s hat %d Geldpunkte an %s gegeben.",0,GetPlayerName(own),value,GetPlayerName(Player));
  return(1);

/* - - - - - - - Angeboterstellung: - - - - - - - */

SellList:
  Sound("Click");
  CreateMenu(LIST,caller,0,0,0,0,0,0);
  AddMenuItem("Kleine Objekte","Objects",KEOE,caller);
  AddMenuItem("Fahrzeuge","Vehicles",FRZE,caller);
  AddMenuItem("Liste angucken","LookOnList",LEGN,caller);
  return(1);

//Liste angucken:
LookOnList:
  Sound("Click");
  CreateMenu(LEGN,caller,0,0,"Verkaufsliste ist Leer",0,0,0);
  var obj;
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==own)
      AddMenuItem(Format("%s (%d)",GetName(0,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"ChangeIt",ObjectCall(obj,"Object"),caller,ContentsCount(ObjectCall(obj,"Object"),this()),obj);
  return(1);

ChangeIt:
  Sound("Click");
  value=ObjectCall(Par(1),"Preis");
  Objekt=ObjectCall(Par(1),"Object");
  ShowM();
  CreateMenu(LIST,caller,0,0,0,0,0,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,0,0);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,0,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,0,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,0,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,0,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,0,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,0,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,0,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,0,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,0,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Eintrag löschen","DeleteThis",DELE,caller,0,Par(1));
  AddMenuItem("Eintrag Ändern","Change",OKAY,caller,0,Par(1));
  AddMenuItem("Originalwert ansehen","Normal",NO_M,caller);
  return(1);

Normal:
  Sound("Click");
  Message("Der Wert von %s ist: %d",this(),GetName(0,Objekt),GetValue(0,Objekt));
  return(1);

DeleteThis:
  Sound("Click");
  CloseMenu(caller);
  Message("%s wurde aus der Verkaufsliste entfernt.",this(),GetName(0,ObjectCall(Par(1),"Object")));
  RemoveObject(Par(1));
  return(1);

Change:
  Sound("Click");
  var obj = Par(1);
  CloseMenu(caller);
  ObjectCall(obj,"SetSaver",Objekt,value);
  SetOwner(own,obj);
  Message("%s kostet bei dir nun %d",this(),GetName(0,Objekt),value);
  return(1);

func Objects()
{
  Sound("Click");
  CreateMenu(KEOE,caller,0,0,"Keine Objekte gefunden!",0,0,0);
  var obj, num;
  while(obj = (GetDefinition(num,C4D_Object())))
{if (GetCategory(0,obj)&2048)
{
    AddMenuItem(Format("%s (%d) auf Verkaufsliste setzten",GetName(0,obj),GetValue(0,obj)),"AddOnList",obj,caller);
}
    num++;
}
  return(1);
}

Vehicles:
  Sound("Click");
  CreateMenu(FRZE,caller,0,0,"Keine Fahrzeuge gefunden!",0,0,0);
  var obj, num;
  while(obj = GetDefinition(num,C4D_Vehicle()))
{if (GetCategory(0,obj)&2048)
{
    AddMenuItem(Format("%s (%d) auf Verkaufsliste setzten",GetName(0,obj),GetValue(0,obj)),"AddOnList",obj,caller);
}
    num++;
}
  return(1);

AddOnList:
  Sound("Click");
  var obj;
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==own)
      if(ObjectCall(obj,"Object")==Par(0))
{
        Message("Dieser Eintrag ist bereits vorhanden, und kann bearbeitet werden!",this());
        ChangeIt(0,obj);
        return(1);
}
  Objekt=Par(0);
  value=0;
  Preis();
  return(1);

Preis:
  Sound("Click");
  value=0;
  ShowM();
  Player=Par(1);
  CreateMenu(LIST,caller,0,0,0,0,0,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,0,0);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,0,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,0,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,0,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,0,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,0,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,0,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,0,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,0,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,0,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("In Verkaufsliste setzen","SaveIt",OKAY,caller);
  AddMenuItem("Originalwert ansehen","Normal",NO_M,caller);
  return(1);

SaveIt:
  CloseMenu(caller);
  Sound("Click");
  Sound("Ding",1);

  var obj;
  ObjectCall(obj=CreateObject(SAVE,0,0),"SetSaver",Objekt,value);
  SetOwner(own,obj);
  Message("%s wurde mit einem Kostenbetrag von %d in die Verkaufsliste eingetragen!",this(),GetName(0,Objekt),value);
  Message("%s hat ein neues Verkaufsangebot: %s (%d)",0,GetPlayerName(own),GetName(0,Objekt),value);
  return(1);

/* - - - - - - - Objekte Kaufen: - - - - - - - */

Buy:
  Sound("Click");
  CreateMenu(_BUY,caller,0,0,"Keine weiteren Spieler mit Handelsstation gefunden!",0,0,0);
  var i;
  var Players = GetPlayerCount();
  value=0;
  for (i; Players; ++i) 
  {
    if (i != own)
      if(HasStation(i)!=0)
        AddMenuItem(Format("Von %s Kaufen",GetPlayerName(i)) , "BuyFrom",KAUF,caller,0,i);
    --Players;
  }
  return(1);

BuyFrom:
  Sound("Click");
  Player = Par(1);
  BuyMenu();
  return(1);

BuyMenu:
  CreateMenu(_BUY,caller,0,0,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),0,0,1);
  var obj;
  var Station=HasStation(Player);
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Player)
      AddMenuItem(Format("%s (%d) kaufen",GetName(0,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"BuyIt",ObjectCall(obj,"Object"),caller,ContentsCount(ObjectCall(obj,"Object"),Station),obj);
  return(1);

GetIndexOf: //C4Object Objekt
  var obj, num;
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Player)
{
      if(obj==Par(0))
        return(num);
      num++;
}
  return(0);

BuyIt:
  if(HasSpam(Player))
{
    Sound("Error");
    Message("%s hat dich als Spam eingerichtet!",this(),GetPlayerName(Player));
    return(1);
}

  var station = HasStation(Player);
//Werte holen:
  Objekt=ObjectCall(Par(1),"Object");
  value=ObjectCall(Par(1),"Preis");

//Gucken ob überhaupt gekauft werden kann:
  if(!FindContents(Objekt,station))
{
    Sound("Error");
    Message("Der Vorrat von %s an %s ist erschöpft!",this(),GetPlayerName(Player),GetName(0,Objekt));
    return(1);
}

  if(GetWealth(own)<value)
{
    Sound("Error");
    Message("Du hast nicht genug Geld! Benötigt wird %d.",this(),value);
    return(1);
}

//Kaufen:
  var obj = FindContents(Objekt,station);
  if(!obj)
{
    Sound("Error");
    Message("Der Vorrat von %s an %s ist erschöpft!",this(),GetPlayerName(Player),GetName(0,Objekt));
    return(1);
}
  Effekt();
  Exit(obj,0,0);
ObjectCall(station,"Effekt2",this(),obj,1);
//  Blitz(station);
  SetWealth(own,GetWealth(own)-value);
  SetWealth(Player,GetWealth(Player)+value);
  Sound("Uncash");
  Message("%s hat von %s %s für %d gekauft.",0,GetPlayerName(own),GetPlayerName(Player),GetName(0,Objekt),value);
  BuyMenu();
  SelectMenuItem(GetIndexOf(Par(1)),caller);
  return(1);

/* - - - - - - - Eine Meldung an einen Spieler senden: - - - - - - - */

//Spieler auswählen:
Meldung:
  Sound("Click");
  CreateMenu(MELD,caller,0,0,"Keine Weiteren Spieler mit Handelsstation gefunden!",0,0,0);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; ++i) 
  {
    if (!GetPlayerName(i)) continue;
    if (i != own)
      if(HasStation(i)!=0)
        AddMenuItem(Format("%s eine Meldung überbringen",GetPlayerName(i)) , "MeldHim",MELD,caller,0,i);
    --Players;
  }
  return(1);

//Meldungstext auswählen:
MeldHim:
  Sound("Click");
  Player=Par(1);
  CreateMenu(MELD,caller,0,0,0,0,0,0);
  AddMenuItem("Meldung: Kosten zu hoch","MTooMuch",MKZH,caller);
  AddMenuItem("Meldung: Verkaufsanfrage","MNochNicht",MFDV,caller);
  AddMenuItem("Meldung: Benötige Geld","MKohleHer",MBGD,caller);
  AddMenuItem("Meldung: Vorrat erschöpft","MNetMerDa",MNMD,caller);
  return(1);

/* - - - - - Meldung: Kosten zu hoch - - - - - */

MTooMuch:
  Sound("Click");
  Message("Wähle das Verkaufsangebot des Spielers aus, das deiner Meinung nach zu teuer ist:",this());
  CreateMenu(MELD,caller,0,0,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),0,0,0);
  var obj;
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Player)
      AddMenuItem(Format("%s (%d) als zu teuer markieren",GetName(0,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"CostTooMuch",ObjectCall(obj,"Object"),caller,0,obj);
  return(1);

CostTooMuch:
  Sound("Click");
  Message("Gib den Betrag ein, der deiner Meinung nach angemessen ist:",this());
  Objekt=ObjectCall(Par(1),"Object");
  oldValue=ObjectCall(Par(1),"Preis");
  CreateMenu(MELD,caller,0,0,0,0,0,1);
  value=0;
  ShowM();
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,0,0);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,0,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,0,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,0,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,0,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,0,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,0,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,0,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,0,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,0,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Geldbetrag vorschlagen","Suggest",OKAY,caller,0,Par(1));
  return(1);

Suggest:
  CloseMenu(caller);
  PlrMessage("%s findet, das dieses Angebot zu teuer ist: %s (%d).|%s schlägt vor, dass %d ein angemessener Preis sei!",Player,GetPlayerName(own),GetName(0,Objekt),oldValue,GetPlayerName(own),value);
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);

/* - - - - - Meldung: Verkaufsanfrage - - - - - */

MNochNicht:
  Sound("Click");
  Message("Wähle das Objekt, das man bei %s kaufen können soll:",this(),GetPlayerName(Player));
  CreateMenu(MELD,caller,0,0,0,0,0,0);
//Kleine Objekte:
  var obj, num;
  while(obj = GetDefinition(num,C4D_Object()))
{
    AddMenuItem(Format("%s (%d) vorschlagen",GetName(0,obj),GetValue(0,obj)),"NewSuggest",obj,caller);
    num++;
}
//Fahrzeuge:
  while(obj = GetDefinition(num,C4D_Vehicle()))
{
    AddMenuItem(Format("%s (%d) vorschlagen",GetName(0,obj),GetValue(0,obj)),"NewSuggest",obj,caller);
    num++;
}
  return(1);

NewSuggest:
  CloseMenu(caller);
  PlrMessage("%s hat einen neuen Verkaufsvorschlag gemacht: %s",Player,GetPlayerName(own),GetName(0,Par(0)));
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);

/* - - - - - Meldung: Benötige Geld - - - - - */

MKohleHer:
  Sound("Click");
  Message("Gib ein, wieviel Geld du benötigst:",this());
  CreateMenu(MELD,caller,0,0,0,0,0,1);
  value=0;
  ShowM();
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,0,0);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,0,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,0,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,0,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,0,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,0,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,0,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,0,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,0,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,0,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Nachfragen","Ask",OKAY,caller,0,Par(1));
  return(1);

Ask:
  CloseMenu(caller);
  PlrMessage("%s bittet um eine Spende von %d",Player,GetPlayerName(own),value);
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);

/* - - - - - Meldung: Vorrat erschöpft - - - - - */

MNetMerDa:
  Sound("Click");
  Message("Wähle das Objekt, das bei %s nicht mehr da ist:",this(),GetPlayerName(Player));
  CreateMenu(MELD,caller,0,0,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),0,0,0);
  var obj;
  while(obj = FindObject(SAVE,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==Player)
      AddMenuItem(Format("%s (%d) als nicht mehr da markieren",GetName(0,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"NMD",ObjectCall(obj,"Object"),caller,0,obj);
  return(1);

NMD:
  CloseMenu(caller);
  PlrMessage("%s sagt, dass dein Vorrat an %s erschöpft ist",Player,GetPlayerName(own),GetName(0,Par(0)));
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);

/* - - - - - - - Spam - Menü: - - - - - - - */

Spams:
  Sound("Click");
  CreateMenu(SPAM,caller,0,0,0,0,0,0);
  AddMenuItem("Vorhandenen Spam löschen","DeleteSpam",DESM,caller);
  AddMenuItem("Neuen Spam einrichten","AddSpam",ADDS,caller);
  return(1);

/* - - - - - Spieler aus Spamliste entfernen: - - - - - */

DeleteSpam:
  Sound("Click");
  CreateMenu(DESM,caller,0,0,"Keine Spams eingerichtet!",0,0,0);
  var obj;
  while(obj = FindObject(SMSR,0,0,0,0,0,0,0,0,obj))
    if(GetOwner(obj)==own)
      AddMenuItem(Format("%s nicht mehr als Spam behandeln",GetPlayerName(ObjectCall(obj,"Object"))),"DeleteThisSpam",DESM,caller,0,obj);
  return(1);

DeleteThisSpam:
  Sound("Click");
  Message("%s wurde aus Spamliste entfernt",this(),GetPlayerName(ObjectCall(Par(1),"Object")));
  RemoveObject(Par(1));
  return(1);

/* - - - - - Neuen Spam einrichten - - - - - */

AddSpam:
  Sound("Click");
  CreateMenu(ADDS,caller,0,0,"Keine weiteren Spieler mit Handelsstation gefunden!",0,0,0);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; ++i) 
  {
    if (i != own)
      if(HasStation(i)!=0)
        if(IsSpam(i)==0)
          AddMenuItem(Format("%s als Spam behandeln",GetPlayerName(i)) , "NewSpam",ADDS,caller,0,i);
    --Players;
  }
  return(1);

NewSpam:
  var obj;
  ObjectCall(obj = CreateObject(SMSR,0,0),"SetSaver",Par(1));
  SetOwner(own,obj);
  Message("%s wird ab sofort als Spam behandelt!",this(),GetPlayerName(Par(1)));
  Sound("Ding",1);
  return(1);

/* - - - - - - - Clonk transferieren: - - - - - - - */

Transfer:
  Sound("Click");
  CreateMenu(TRAN,caller,0,0,"Keine Clonks in der Handelsstation",0,0,0);
  var obj;
  while(obj = FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this(),obj))
    AddMenuItem(Format("%s transferieren",GetName(obj)),"Target",GetID(obj),caller,0,obj);
  return(1);

Target:
  Sound("Click");
  Objekt=Par(1);
  CreateMenu(TRAN,caller,0,0,"Keine weiteren Spieler mit Handelsstation gefunden!",0,0,0);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; ++i) 
  {
    if (i != own)
      if(HasStation(i)!=0)
        if(IsSpam(i)==0)
 AddMenuItem(Format("%s zur Handelsstation von %s transferieren",GetName(Objekt),GetPlayerName(i)) , "Trans",TRAN,caller,0,i);
    --Players;
  }
// außenposten keine alte grafik, und für zukunft gibts das uplinksystem
//  AddMenuItem("Clonk in Außenposten transferieren","CTransfer",TRN2,caller);
  return(1);

func Trans()
{
if(!GetPlrKnowledge(GetOwner(Objekt),HATR)) {PlayerMessage(GetOwner(Objekt),"du musst zuerst 'Transfer' erforschen!",Objekt); Sound("Error"); return(1);};

  var obj=HasStation(Par(1));
  if(HasSpam(Player))
{
    Sound("Error");
    Message("%s hat dich als Spam eingerichtet!",this(),GetPlayerName(Player));
    return(1);
}

  Effekt();
  Exit(Objekt,0,0);
  Effekt2(obj,Objekt,0);
//  Blitz(obj);
  Sound("Magic1",1);
  SetOwner(GetOwner(caller),obj = CreateContents(SNDR,caller));	
  PlrMessage("Transfer von %s zur Handelsstation von %s!",own,GetName(Objekt),GetPlayerName(Par(1)));
  return(1);
}

/* - - - - - - - Clonk in Außenposten transferieren: - - - - - - - */

CTransfer:
  CreateMenu(TRN2,caller,0,0,"Du hast keine Außenposten!",0,0,0);
  var obj;
  while(obj=FindObject(ANPN,0,0,0,0,0,0,0,NoContainer(),obj))
    if(GetOwner(obj)==own)
      AddMenuItem(Format("%s",ObjectCall(obj,"ANPNIndex")),"CTrans",ANPN,caller,0,obj);
  return(1);

CTrans:
  CloseMenu(caller);
  Sound("Magic1");
  Effekt();
 // Blitz(Par(1));
  Exit(caller,0,0);
  Effekt2(Par(1),caller,1);
  return(1);