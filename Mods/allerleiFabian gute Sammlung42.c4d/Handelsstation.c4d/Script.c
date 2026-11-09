/*-- Handelsstation --*/

local caller, own, value, Player, Objekt, oldValue, basement;

#strict 3


/* - - - - - - - - Algemeines: - - - - - - - */

//Überprüfen, ob der gewählte Spieler eine Handelsstation hat:
func HasStation(...) {
  var obj;
  while(obj = FindObject(HSSN,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Par(nil)))
      if(Equal(GetCon(obj), 100))
        return(obj);
  while(obj = FindObject(AEHN,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Par(nil)))
      if(Equal(GetCon(obj), 100))
        return(obj);
  return(nil);
}

func IsSpam(...) {
  var obj;
  while(obj = FindObject(SMSR,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), own))
      if(Equal(ObjectCall(obj,"Object"), Par(nil)))
        return(obj);
  return(nil);
}

func HasSpam(...) {
  var obj;
  while(obj = FindObject(SMSR,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Par(nil)))
      if(Equal(ObjectCall(obj,"Object"), own))
        return(1);
  return(nil);
}

//Der Kleine Effekt:
func Effekt(...) {
  CreateObject(LIGT,8,-20);
  return(1);
}

//Blitz:
//  LaunchLightning(GetX(this()),GetY(this()) ,GetX(Par(0))-GetX(this()), 0 //,GetY(Par(0))-GetY(this()), 0);
//  return(1);

func Effekt2(...) {
var obj;
ObjectCall(obj=CreateObject(EFKT,nil,nil),"SetTarget",Par(nil),Par(2),Par(3),Par(4),Par(5));
if(Par(1))
  Enter(obj,Par(1));
return(1);
}

//Hat der Spieler bereits eine Handelsstation?
func Construction(...) {
  own = GetOwner();
  var obj;
  if(!Equal(HasStation(own), nil))
{
    Sound("Error");
    Message("Du hast bereits eine Handelsstation!",this());
    RemoveObject();
    return(1);
}
  basement = CreateObject(BS13,nil,+8,GetOwner());
  return(1);
}

//Eingangssteuerung:

func ActivateEntrance(...) {
  if(ActIdle()) SetAction("Opening");
  return(1);
}

func SoundOpen(...) { return(Sound("SteelGate2"));
}
func SoundClose(...) { return(Sound("SteelGate2"));
}
func Offen(...) { return(SetEntrance(1));
}
func Close(...) { return(SetEntrance(nil));
}

//Loren werden geleert:
func Check(...) {
  var obj,Sender;
  while(obj=FindObject(LORY,-10,-10,21,21,nil,nil,nil,this(),obj))
    GrabContents(obj);

//Von einer Station zur anderen Beamen:
  while(obj=FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this(),obj))
    if(Sender=FindContents(SNDR,obj))
      if(!Equal(GetOwner(obj), own))
{
        Exit(obj,nil,nil);
        var Station=HasStation(GetOwner(obj));
        Effekt();
        Effekt2(Station,obj,1);
      //  Blitz(Station);
        Sound("Magic1",1);
        RemoveObject(Sender);
}
//Chips entfernen:
  while(obj=FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this(),obj))
    if(Equal(GetOwner(obj), own))
      if(Sender=FindContents(SNDR,obj))
{
        Effekt();
        RemoveObject(Sender);
}
  return(1);
}

protected func Destruction()
{
  if (basement) RemoveObject(basement); 
  return(_inherited());
}



/* - - - - - - - Das Hauptmenü: - - - - - - - */

func ContainedUp(...) {
  if(!Equal(GetOwner(Par(nil)), own))
{
    Sound("Error");
    Message("Das ist kein Clonk von %s!",this(),GetPlayerName(own));
    return(nil);
}

  Sound("Click");
  caller=Par(nil);
  CreateMenu(HSSN,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Verkaufsliste erstellen","SellList",LIST,caller);
  AddMenuItem("Kaufen","Buy",_BUY,caller);
  AddMenuItem("Geld geben","Money",MONY,caller);
  AddMenuItem("Meldung an einen Spieler geben","Meldung",MELD,caller);
  AddMenuItem("Spam - Menü","Spams",SPAM,caller);
  AddMenuItem("Clonk transferieren","Transfer",TRAN,caller);
  return(1);
}

/* - - - - - - - Geld an einen anderen Spieler geben: - - - - - - - */

func Money(...) {
  Sound("Click");
  CreateMenu(MONY,caller,nil,nil,"Keine weiteren Spieler mit Handelsstation gefunden!",nil,nil,nil);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; S42MigrationStep(i, 1))
  {
    if (!Equal(i, own))
      if(!Equal(HasStation(i), nil))
        AddMenuItem(Format("Geld an %s geben",GetPlayerName(i)) , "SetMoneyAmount",STMY,caller,nil,i);
    S42MigrationStep(Players, -1);
  }
  return(1);
}

func SetMoneyAmount(...) {
  Sound("Click");
  value=nil;
  ShowM();
  Player=Par(1);
  CreateMenu(STMY,caller,nil,nil,nil,nil,nil,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,nil,nil);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,nil,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,nil,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,nil,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,nil,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,nil,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,nil,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,nil,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,nil,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,nil,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Geldbetrag geben","GiveMoney",OKAY,caller);
  return(1);
}

func AddNumber(...) {
  if((value ?? 0) >= 1000)
    return(1);

  Sound("Click");
  var ZSave = value;
  value=(value ?? 0)*10+(Par(1) ?? 0);
  ShowM();
  return(1);
}

func ShowM(...) {
  Message("Geldbetrag: %d",this(),value);
  return(1);
}

func Delete(...) {
  value=(value ?? 0) / 10;
  ShowM();
  Sound("Click");
  return(1);
}

func GiveMoney(...) {
  if((GetWealth(own) ?? 0)<(value ?? 0))
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
  Effekt2(Station,nil,2,value,Player);
  SetWealth(own,(GetWealth(own) ?? 0)-(value ?? 0));
  Message("%s hat %d Geldpunkte an %s gegeben.",nil,GetPlayerName(own),value,GetPlayerName(Player));
  return(1);
}

/* - - - - - - - Angeboterstellung: - - - - - - - */

func SellList(...) {
  Sound("Click");
  CreateMenu(LIST,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Kleine Objekte","Objects",KEOE,caller);
  AddMenuItem("Fahrzeuge","Vehicles",FRZE,caller);
  AddMenuItem("Liste angucken","LookOnList",LEGN,caller);
  return(1);
}

//Liste angucken:
func LookOnList(...) {
  Sound("Click");
  CreateMenu(LEGN,caller,nil,nil,"Verkaufsliste ist Leer",nil,nil,nil);
  var obj;
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), own))
      AddMenuItem(Format("%s (%d)",GetName(nil,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"ChangeIt",ObjectCall(obj,"Object"),caller,ContentsCount(ObjectCall(obj,"Object"),this()),obj);
  return(1);
}

func ChangeIt(...) {
  Sound("Click");
  value=ObjectCall(Par(1),"Preis");
  Objekt=ObjectCall(Par(1),"Object");
  ShowM();
  CreateMenu(LIST,caller,nil,nil,nil,nil,nil,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,nil,nil);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,nil,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,nil,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,nil,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,nil,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,nil,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,nil,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,nil,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,nil,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,nil,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Eintrag löschen","DeleteThis",DELE,caller,nil,Par(1));
  AddMenuItem("Eintrag Ändern","Change",OKAY,caller,nil,Par(1));
  AddMenuItem("Originalwert ansehen","Normal",NO_M,caller);
  return(1);
}

func Normal(...) {
  Sound("Click");
  Message("Der Wert von %s ist: %d",this(),GetName(nil,Objekt),GetValue(nil,Objekt));
  return(1);
}

func DeleteThis(...) {
  Sound("Click");
  CloseMenu(caller);
  Message("%s wurde aus der Verkaufsliste entfernt.",this(),GetName(nil,ObjectCall(Par(1),"Object")));
  RemoveObject(Par(1));
  return(1);
}

func Change(...) {
  Sound("Click");
  var obj = Par(1);
  CloseMenu(caller);
  ObjectCall(obj,"SetSaver",Objekt,value);
  SetOwner(own,obj);
  Message("%s kostet bei dir nun %d",this(),GetName(nil,Objekt),value);
  return(1);
}

func Objects()
{
  Sound("Click");
  CreateMenu(KEOE,caller,nil,nil,"Keine Objekte gefunden!",nil,nil,nil);
  var obj, num;
  while(obj = (GetDefinition(num,C4D_Object)))
{if ((GetCategory(nil,obj) ?? 0)&2048)
{
    AddMenuItem(Format("%s (%d) auf Verkaufsliste setzten",GetName(nil,obj),GetValue(nil,obj)),"AddOnList",obj,caller);
}
    S42MigrationStep(num, 1, true);
}
  return(1);
}

func Vehicles(...) {
  Sound("Click");
  CreateMenu(FRZE,caller,nil,nil,"Keine Fahrzeuge gefunden!",nil,nil,nil);
  var obj, num;
  while(obj = GetDefinition(num,C4D_Vehicle))
{if ((GetCategory(nil,obj) ?? 0)&2048)
{
    AddMenuItem(Format("%s (%d) auf Verkaufsliste setzten",GetName(nil,obj),GetValue(nil,obj)),"AddOnList",obj,caller);
}
    S42MigrationStep(num, 1, true);
}
  return(1);
}

func AddOnList(...) {
  Sound("Click");
  var obj;
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), own))
      if(Equal(ObjectCall(obj,"Object"), Par(nil)))
{
        Message("Dieser Eintrag ist bereits vorhanden, und kann bearbeitet werden!",this());
        ChangeIt(nil,obj);
        return(1);
}
  Objekt=Par(nil);
  value=nil;
  Preis();
  return(1);
}

func Preis(...) {
  Sound("Click");
  value=nil;
  ShowM();
  Player=Par(1);
  CreateMenu(LIST,caller,nil,nil,nil,nil,nil,1);
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,nil,nil);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,nil,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,nil,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,nil,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,nil,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,nil,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,nil,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,nil,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,nil,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,nil,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("In Verkaufsliste setzen","SaveIt",OKAY,caller);
  AddMenuItem("Originalwert ansehen","Normal",NO_M,caller);
  return(1);
}

func SaveIt(...) {
  CloseMenu(caller);
  Sound("Click");
  Sound("Ding",1);

  var obj;
  ObjectCall(obj=CreateObject(SAVE,nil,nil),"SetSaver",Objekt,value);
  SetOwner(own,obj);
  Message("%s wurde mit einem Kostenbetrag von %d in die Verkaufsliste eingetragen!",this(),GetName(nil,Objekt),value);
  Message("%s hat ein neues Verkaufsangebot: %s (%d)",nil,GetPlayerName(own),GetName(nil,Objekt),value);
  return(1);
}

/* - - - - - - - Objekte Kaufen: - - - - - - - */

func Buy(...) {
  Sound("Click");
  CreateMenu(_BUY,caller,nil,nil,"Keine weiteren Spieler mit Handelsstation gefunden!",nil,nil,nil);
  var i;
  var Players = GetPlayerCount();
  value=nil;
  for (i; Players; S42MigrationStep(i, 1))
  {
    if (!Equal(i, own))
      if(!Equal(HasStation(i), nil))
        AddMenuItem(Format("Von %s Kaufen",GetPlayerName(i)) , "BuyFrom",KAUF,caller,nil,i);
    S42MigrationStep(Players, -1);
  }
  return(1);
}

func BuyFrom(...) {
  Sound("Click");
  Player = Par(1);
  BuyMenu();
  return(1);
}

func BuyMenu(...) {
  CreateMenu(_BUY,caller,nil,nil,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),nil,nil,1);
  var obj;
  var Station=HasStation(Player);
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Player))
      AddMenuItem(Format("%s (%d) kaufen",GetName(nil,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"BuyIt",ObjectCall(obj,"Object"),caller,ContentsCount(ObjectCall(obj,"Object"),Station),obj);
  return(1);
}

func GetIndexOf(...) { //C4Object Objekt
  var obj, num;
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Player))
{
      if(Equal(obj, Par(nil)))
        return(num);
      S42MigrationStep(num, 1, true);
}
  return(nil);
}

func BuyIt(...) {
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
    Message("Der Vorrat von %s an %s ist erschöpft!",this(),GetPlayerName(Player),GetName(nil,Objekt));
    return(1);
}

  if((GetWealth(own) ?? 0)<(value ?? 0))
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
    Message("Der Vorrat von %s an %s ist erschöpft!",this(),GetPlayerName(Player),GetName(nil,Objekt));
    return(1);
}
  Effekt();
  Exit(obj,nil,nil);
ObjectCall(station,"Effekt2",this(),obj,1);
//  Blitz(station);
  SetWealth(own,(GetWealth(own) ?? 0)-(value ?? 0));
  SetWealth(Player,(GetWealth(Player) ?? 0)+(value ?? 0));
  Sound("Uncash");
  Message("%s hat von %s %s für %d gekauft.",nil,GetPlayerName(own),GetPlayerName(Player),GetName(nil,Objekt),value);
  BuyMenu();
  SelectMenuItem(GetIndexOf(Par(1)),caller);
  return(1);
}

/* - - - - - - - Eine Meldung an einen Spieler senden: - - - - - - - */

//Spieler auswählen:
func Meldung(...) {
  Sound("Click");
  CreateMenu(MELD,caller,nil,nil,"Keine Weiteren Spieler mit Handelsstation gefunden!",nil,nil,nil);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; S42MigrationStep(i, 1))
  {
    if (!GetPlayerName(i)) continue;
    if (!Equal(i, own))
      if(!Equal(HasStation(i), nil))
        AddMenuItem(Format("%s eine Meldung überbringen",GetPlayerName(i)) , "MeldHim",MELD,caller,nil,i);
    S42MigrationStep(Players, -1);
  }
  return(1);
}

//Meldungstext auswählen:
func MeldHim(...) {
  Sound("Click");
  Player=Par(1);
  CreateMenu(MELD,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Meldung: Kosten zu hoch","MTooMuch",MKZH,caller);
  AddMenuItem("Meldung: Verkaufsanfrage","MNochNicht",MFDV,caller);
  AddMenuItem("Meldung: Benötige Geld","MKohleHer",MBGD,caller);
  AddMenuItem("Meldung: Vorrat erschöpft","MNetMerDa",MNMD,caller);
  return(1);
}

/* - - - - - Meldung: Kosten zu hoch - - - - - */

func MTooMuch(...) {
  Sound("Click");
  Message("Wähle das Verkaufsangebot des Spielers aus, das deiner Meinung nach zu teuer ist:",this());
  CreateMenu(MELD,caller,nil,nil,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),nil,nil,nil);
  var obj;
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Player))
      AddMenuItem(Format("%s (%d) als zu teuer markieren",GetName(nil,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"CostTooMuch",ObjectCall(obj,"Object"),caller,nil,obj);
  return(1);
}

func CostTooMuch(...) {
  Sound("Click");
  Message("Gib den Betrag ein, der deiner Meinung nach angemessen ist:",this());
  Objekt=ObjectCall(Par(1),"Object");
  oldValue=ObjectCall(Par(1),"Preis");
  CreateMenu(MELD,caller,nil,nil,nil,nil,nil,1);
  value=nil;
  ShowM();
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,nil,nil);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,nil,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,nil,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,nil,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,nil,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,nil,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,nil,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,nil,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,nil,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,nil,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Geldbetrag vorschlagen","Suggest",OKAY,caller,nil,Par(1));
  return(1);
}

func Suggest(...) {
  CloseMenu(caller);
  PlrMessage("%s findet, das dieses Angebot zu teuer ist: %s (%d).|%s schlägt vor, dass %d ein angemessener Preis sei!",Player,GetPlayerName(own),GetName(nil,Objekt),oldValue,GetPlayerName(own),value);
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);
}

/* - - - - - Meldung: Verkaufsanfrage - - - - - */

func MNochNicht(...) {
  Sound("Click");
  Message("Wähle das Objekt, das man bei %s kaufen können soll:",this(),GetPlayerName(Player));
  CreateMenu(MELD,caller,nil,nil,nil,nil,nil,nil);
//Kleine Objekte:
  var obj, num;
  while(obj = GetDefinition(num,C4D_Object))
{
    AddMenuItem(Format("%s (%d) vorschlagen",GetName(nil,obj),GetValue(nil,obj)),"NewSuggest",obj,caller);
    S42MigrationStep(num, 1, true);
}
//Fahrzeuge:
  while(obj = GetDefinition(num,C4D_Vehicle))
{
    AddMenuItem(Format("%s (%d) vorschlagen",GetName(nil,obj),GetValue(nil,obj)),"NewSuggest",obj,caller);
    S42MigrationStep(num, 1, true);
}
  return(1);
}

func NewSuggest(...) {
  CloseMenu(caller);
  PlrMessage("%s hat einen neuen Verkaufsvorschlag gemacht: %s",Player,GetPlayerName(own),GetName(nil,Par(nil)));
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);
}

/* - - - - - Meldung: Benötige Geld - - - - - */

func MKohleHer(...) {
  Sound("Click");
  Message("Gib ein, wieviel Geld du benötigst:",this());
  CreateMenu(MELD,caller,nil,nil,nil,nil,nil,1);
  value=nil;
  ShowM();
  AddMenuItem("0 Hinzufügen","AddNumber",ZERO,caller,nil,nil);
  AddMenuItem("1 Hinzufügen","AddNumber",_ONE,caller,nil,1);
  AddMenuItem("2 Hinzufügen","AddNumber",_TWO,caller,nil,2);
  AddMenuItem("3 Hinzufügen","AddNumber",THRE,caller,nil,3);
  AddMenuItem("4 Hinzufügen","AddNumber",FOUR,caller,nil,4);
  AddMenuItem("5 Hinzufügen","AddNumber",FIVE,caller,nil,5);
  AddMenuItem("6 Hinzufügen","AddNumber",_SIX,caller,nil,6);
  AddMenuItem("7 Hinzufügen","AddNumber",SEVN,caller,nil,7);
  AddMenuItem("8 Hinzufügen","AddNumber",EIGT,caller,nil,8);
  AddMenuItem("9 Hinzufügen","AddNumber",NINE,caller,nil,9);
  AddMenuItem("Letzte Stelle löschen","Delete",BACK,caller);
  AddMenuItem("Nachfragen","Ask",OKAY,caller,nil,Par(1));
  return(1);
}

func Ask(...) {
  CloseMenu(caller);
  PlrMessage("%s bittet um eine Spende von %d",Player,GetPlayerName(own),value);
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);
}

/* - - - - - Meldung: Vorrat erschöpft - - - - - */

func MNetMerDa(...) {
  Sound("Click");
  Message("Wähle das Objekt, das bei %s nicht mehr da ist:",this(),GetPlayerName(Player));
  CreateMenu(MELD,caller,nil,nil,Format("Spieler %s hat keine Verkaufsangebote!",GetPlayerName(Player)),nil,nil,nil);
  var obj;
  while(obj = FindObject(SAVE,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), Player))
      AddMenuItem(Format("%s (%d) als nicht mehr da markieren",GetName(nil,ObjectCall(obj,"Object")),ObjectCall(obj,"Preis")),"NMD",ObjectCall(obj,"Object"),caller,nil,obj);
  return(1);
}

func NMD(...) {
  CloseMenu(caller);
  PlrMessage("%s sagt, dass dein Vorrat an %s erschöpft ist",Player,GetPlayerName(own),GetName(nil,Par(nil)));
  Message("Meldung an %s weitergeleitet.",this(),GetPlayerName(Player));
  Sound("Meldung",1);
  return(1);
}

/* - - - - - - - Spam - Menü: - - - - - - - */

func Spams(...) {
  Sound("Click");
  CreateMenu(SPAM,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Vorhandenen Spam löschen","DeleteSpam",DESM,caller);
  AddMenuItem("Neuen Spam einrichten","AddSpam",ADDS,caller);
  return(1);
}

/* - - - - - Spieler aus Spamliste entfernen: - - - - - */

func DeleteSpam(...) {
  Sound("Click");
  CreateMenu(DESM,caller,nil,nil,"Keine Spams eingerichtet!",nil,nil,nil);
  var obj;
  while(obj = FindObject(SMSR,nil,nil,nil,nil,nil,nil,nil,nil,obj))
    if(Equal(GetOwner(obj), own))
      AddMenuItem(Format("%s nicht mehr als Spam behandeln",GetPlayerName(ObjectCall(obj,"Object"))),"DeleteThisSpam",DESM,caller,nil,obj);
  return(1);
}

func DeleteThisSpam(...) {
  Sound("Click");
  Message("%s wurde aus Spamliste entfernt",this(),GetPlayerName(ObjectCall(Par(1),"Object")));
  RemoveObject(Par(1));
  return(1);
}

/* - - - - - Neuen Spam einrichten - - - - - */

func AddSpam(...) {
  Sound("Click");
  CreateMenu(ADDS,caller,nil,nil,"Keine weiteren Spieler mit Handelsstation gefunden!",nil,nil,nil);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; S42MigrationStep(i, 1))
  {
    if (!Equal(i, own))
      if(!Equal(HasStation(i), nil))
        if(Equal(IsSpam(i), nil))
          AddMenuItem(Format("%s als Spam behandeln",GetPlayerName(i)) , "NewSpam",ADDS,caller,nil,i);
    S42MigrationStep(Players, -1);
  }
  return(1);
}

func NewSpam(...) {
  var obj;
  ObjectCall(obj = CreateObject(SMSR,nil,nil),"SetSaver",Par(1));
  SetOwner(own,obj);
  Message("%s wird ab sofort als Spam behandelt!",this(),GetPlayerName(Par(1)));
  Sound("Ding",1);
  return(1);
}

/* - - - - - - - Clonk transferieren: - - - - - - - */

func Transfer(...) {
  Sound("Click");
  CreateMenu(TRAN,caller,nil,nil,"Keine Clonks in der Handelsstation",nil,nil,nil);
  var obj;
  while(obj = FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this(),obj))
    AddMenuItem(Format("%s transferieren",GetName(obj)),"Target",GetID(obj),caller,nil,obj);
  return(1);
}

func Target(...) {
  Sound("Click");
  Objekt=Par(1);
  CreateMenu(TRAN,caller,nil,nil,"Keine weiteren Spieler mit Handelsstation gefunden!",nil,nil,nil);
  var i;
  var Players = GetPlayerCount();
  for (i; Players; S42MigrationStep(i, 1))
  {
    if (!Equal(i, own))
      if(!Equal(HasStation(i), nil))
        if(Equal(IsSpam(i), nil))
 AddMenuItem(Format("%s zur Handelsstation von %s transferieren",GetName(Objekt),GetPlayerName(i)) , "Trans",TRAN,caller,nil,i);
    S42MigrationStep(Players, -1);
  }
// außenposten keine alte grafik, und für zukunft gibts das uplinksystem
//  AddMenuItem("Clonk in Außenposten transferieren","CTransfer",TRN2,caller);
  return(1);
}

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
  Exit(Objekt,nil,nil);
  Effekt2(obj,Objekt,nil);
//  Blitz(obj);
  Sound("Magic1",1);
  SetOwner(GetOwner(caller),obj = CreateContents(SNDR,caller));	
  PlrMessage("Transfer von %s zur Handelsstation von %s!",own,GetName(Objekt),GetPlayerName(Par(1)));
  return(1);
}

/* - - - - - - - Clonk in Außenposten transferieren: - - - - - - - */

func CTransfer(...) {
  CreateMenu(TRN2,caller,nil,nil,"Du hast keine Außenposten!",nil,nil,nil);
  var obj;
  while(obj=FindObject(ANPN,nil,nil,nil,nil,nil,nil,nil,NoContainer(),obj))
    if(Equal(GetOwner(obj), own))
      AddMenuItem(Format("%s",ObjectCall(obj,"ANPNIndex")),"CTrans",ANPN,caller,nil,obj);
  return(1);
}

func CTrans(...) {
  CloseMenu(caller);
  Sound("Magic1");
  Effekt();
 // Blitz(Par(1));
  Exit(caller,nil,nil);
  Effekt2(Par(1),caller,1);
  return(1);
}