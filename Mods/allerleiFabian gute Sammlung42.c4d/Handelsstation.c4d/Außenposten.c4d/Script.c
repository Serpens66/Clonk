/*-- Außenposten --*/

local caller, own, number, seg, index;

#strict 3

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

//Loren leeren:
func Check(...) {
  var obj;
  while(obj=FindObject(LORY,-10,-10,21,21,nil,nil,nil,this(),obj))
    GrabContents(obj);
  return(1);
}

//Die Station des gewählten Spieler finden:
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

func Effekt2(...) {
  var obj;
  ObjectCall(obj=CreateObject(EFKT,nil,nil),"SetTarget",Par(nil),Par(2),Par(3),Par(4),Par(5));
  if(Par(1))
    Enter(obj,Par(1));
  return(1);
}

func Construction(...) {
  own=GetOwner();
  seg = CreateObject(BAS3,nil,8);
  var seg1 = RandomX(1,6), seg2;
  seg2="A";
  if(Equal(seg1, 2)) seg2="B";
  if(Equal(seg1, 3)) seg2="C";
  if(Equal(seg1, 4)) seg2="D";
  if(Equal(seg1, 5)) seg2="E";
  if(Equal(seg1, 6)) seg2="F";
  index=Format("%s - %d",seg2,RandomX(nil,999));
  return(1);
}

func ANPNIndex(...) { return(index);
}

//Indexwert ansehen:
func ContainedDig(...) { return(Message("Der Index dieser Station ist: %s",this(),index));
}

//Menüsystem:
func ContainedUp(...) {
  Sound("Click");
  caller=Par(nil);
  CreateMenu(ANPN,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Transfer","Transfer",OTTR,caller);
  AddMenuItem(Format("%s in Handelsstation transferieren.",GetName(caller)),"CTransfer",TRAN,caller);
  AddMenuItem("Handelsstationsmenü aufrufen","HSSNMenu",GetID(HasStation(own)),caller);
  return(1);
}

/* - - - - - - - Transfer: - - - - - - - */
func Transfer(...) {
  Sound("Click");
  CreateMenu(ANPN,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Holen","Get",_GET,caller);
  AddMenuItem("Senden","Put",SEND,caller);
  return(1);
}

/* - - - - - Holen: - - - - - */

func Get(...) {
  var obj, station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(nil);
}
  GetMenu(nil);
  return(1);
}

func GetMenu(...) {
  var station=HasStation(own);
  Sound("Click");
  number=nil;
  CreateMenu(_GET,caller,nil,nil,"Keine Objekte in Handelsstation gefunden!",nil,nil,nil);
  var obj, num;
  while(obj = GetDefinition(num,C4D_All))
{
    AddNewItemGet(obj,station);
    S42MigrationStep(num, 1, true);
}
  SelectMenuItem(Par(nil),caller);
  return(1);
}

func AddNewItemGet(...) {
//Nicht enthalten:
  if(!FindContents(Par(nil),Par(1)))
    return(nil);
//Lebensform:
  if((GetCategory(nil,Par(nil)) ?? 0) & (C4D_Living ?? 0))
    return(nil);

//Auf liste setzen:
  AddMenuItem(Format("%s aus Handelsstation holen",GetName(nil,Par(nil))),"GetNow",Par(nil),caller,ContentsCount(Par(nil),Par(1)),number);
  S42MigrationStep(number, 1, true);
  return(1);
}

func GetNow(...) {
  var station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(nil);
}
  var obj=FindContents(Par(nil),station);
  if(!obj)
{
    Sound("Error");
    Message("Das Objekt ist nicht mehr enthalten!",this());
    return(nil);
}

  ObjectCall(station,"Effekt");
  ObjectCall(station,"Blitz",this());
  Exit(obj,nil,nil);
  ObjectCall(station,"Effekt2",this(),obj,1);
  Message("Transfer erfolgreich!",this());
  GetMenu(Par(1));
  return(1);
}

/* - - - - - Senden: - - - - - */

func Put(...) {
  var obj, station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(nil);
}
  PutMenu(nil);
  return(1);
}

func PutMenu(...) {
  Sound("Click");
  number=nil;
  CreateMenu(SEND,caller,nil,nil,"Keine Objekte in dieser Station gefunden!",nil,nil,nil);
  var obj, num;
  while(obj = GetDefinition(num,C4D_All))
{
    AddNewItemPut(obj);
    S42MigrationStep(num, 1, true);
}
  SelectMenuItem(Par(nil),caller);
  return(1);
}

func AddNewItemPut(...) {
//Nicht enthalten:
  if(!FindContents(Par(nil)))
    return(nil);
//Lebensform:
  if((GetCategory(nil,Par(nil)) ?? 0) & (C4D_Living ?? 0))
    return(nil);

//Auf liste setzen:
  AddMenuItem(Format("%s zur Handelsstation senden",GetName(nil,Par(nil))),"SendNow",Par(nil),caller,ContentsCount(Par(nil)),number);
  S42MigrationStep(number, 1, true);
  return(1);
}

func SendNow(...) {
  var station=HasStation(own);
  if(!station)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(nil);
}
  var obj=FindContents(Par(nil));
  if(!obj)
{
    Sound("Error");
    Message("Das Objekt ist nicht mehr enthalten!",this());
    return(nil);
}

  ObjectCall(station,"Blitz",this());
  Exit(obj,nil,nil);
  Effekt2(station,obj,1);
  Message("Transfer erfolgreich!",this());
  PutMenu(Par(1));
  return(1);
}

/* - - - - - - - Clonktransfer: - - - - - - - */

func CTransfer(...) {
  Effekt2(HasStation(own),caller,1);
  ObjectCall(HasStation(own),"Blitz",this());
  Sound("Magic1");
  return(1);
}

/* - - - - - - - Handelsstationsmenü aufrufen: - - - - - - - */
func HSSNMenu(...) {
  var obj=HasStation(own);
  if(!obj)
{
    Sound("Error");
    Message("Du hast keine Handelsstation!",this());
    return(nil);
}

  ObjectCall(obj,"ContainedUp",caller);
  return(1);
}