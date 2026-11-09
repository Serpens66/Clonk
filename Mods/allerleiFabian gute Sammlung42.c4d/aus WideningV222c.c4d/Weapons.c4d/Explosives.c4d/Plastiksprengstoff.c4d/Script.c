/*-- Plastiksprengstoff --*/

#strict 3

local fuse; //ID des Zünders
local caller;
local mass;

local active; //Alle: Angeschaltet?

local time; //Zeitzünder: Zeit bis zur Detonation.

local range; //Bewegungszünder: radius in Pixel


/*---------------------- Triviales -----------------------------*/

func Initialize()
{
  mass=100;
  return(1);
}

public func CalcValue()
{
 var basic = GetValue(nil,GetID());
 var fuseprice = nil;
 if(fuse) fuseprice = GetValue(nil,fuse);
 return((basic ?? 0)*(mass ?? 0)/100+(fuseprice ?? 0));
}


/*---------------------- Explosion und Timer Call -----------------------------*/

func Check()
{
 if(!active) return(1);

 if(Equal(fuse, SFUS)) //Zeitzünder
 {
  if(time) S42MigrationStep(time, -1, true);
  else Boom(50);
  Message("%d",this(),time);
 }

 if(Equal(fuse, MFUS)) //Bewegungszünder
 {
  var count, obj, call;
  for(count=ObjectCount(nil,-(range ?? 0),-(range ?? 0),(range ?? 0)*2,(range ?? 0)*2,OCF_Alive);(count ?? 0)>0;S42MigrationStep(count, -1, true))
  {
   obj = FindObject (nil,nil,nil,nil,nil,OCF_Alive,nil,nil,Contained(),obj);
   if(Hostile(GetOwner(),GetOwner(obj))) Boom(50);
  }
 }

}

func Boom(value) //Besondere Explosion, value in der regel 50
{
 var range = (value ?? 0) * (mass ?? 0)/100; // 12 bis 150 bei value 50
 var count = (2 * (mass ?? 0))/50; // 1 bsi 12 blastobjects mit range/3 schaden

 while(count)
 {
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) BlastObjects((GetX() ?? 0)+(RandomX(-(range ?? 0)/2,(+(range ?? 0) ?? 0)/2) ?? 0),(GetY() ?? 0)+(RandomX(-(range ?? 0)/2,(+(range ?? 0) ?? 0)/2) ?? 0),(range ?? 0)/3);   //  mit mehrfahcem explode laggt es zu sehr //Explode(range/3,CreateObject(ROCK,RandomX(-range,+range),RandomX(-range,+range),-1));
  else FakeExplode((range ?? 0)/3,CreateObject(ROCK,RandomX(-(range ?? 0)/2,(+(range ?? 0) ?? 0)/2),RandomX(-(range ?? 0)/2,(+(range ?? 0) ?? 0)/2),-1),nil,(range ?? 0)/3);  // kommt eig aufsselbe raus... weil fakeexplode auch blastobjects macht
  S42MigrationStep(count, -1, true);
 }
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(range); else FakeExplode(range,nil,nil,range);
}


/*---------------------- Menü -----------------------------*/

func Activate(user)
{
 //Vorbereitungen
 caller = user;
 if(!caller) return(nil);

 // Info für den Spieler
 var fusetxt;
 if(!fuse) fusetxt = "kein Zünder";
 if(Equal(fuse, SFUS)) fusetxt = "Elektrischer Zeitzünder";
 if(Equal(fuse, MFUS)) fusetxt = "Elektrischer Bewegungszünder";

 if(active) var additional = "AKTIVER ";

 Message("C4, Masse %d Gramm, %s%s", caller, mass, additional, fusetxt);

 // Menü erstellen
 CreateMenu(GetID(),caller,this());

 //Zerteilen
 if(And((mass ?? 0) > 25, !fuse)) AddMenuItem("Zerteilen","SplitMelt",GetID(),caller,nil,1,"Teile den Plastiksprengstoff in 2 Teile");

 // Zusammenfügen
 var other = FindObject (GetID(),nil,nil,nil,nil,nil,nil,nil,caller);
 if(And(And((mass ?? 0) < 300, other), !fuse)) AddMenuItem("Zusammenfügen","SplitMelt",GetID(),caller,nil,2,"Füge mehrere Plastiksprengstoffe zusammen, um einen noch größeren Wums zu erzeugen.");

 if(!fuse ) AddMenuItem("Zünder einbauen","FuseInOut",GetID(),caller,nil,1,"Baue einen Zündmechanismus ein. Dieser muss seperat hergestellt werden. Zeitzünder oder Bewegungszünder.");
 if(And(fuse, !active)) AddMenuItem("Zünder ausbauen","FuseInOut",fuse,caller,nil,2,"Zünder ausbauen");

 if(And(Equal(fuse, SFUS), !active)) AddMenuItem("Zeitzünder aktivieren","TimeFuse",fuse,caller,nil,1);
 if(And(Equal(fuse, SFUS), active)) AddMenuItem("Zeitzünder deaktivieren","TimeFuse",fuse,caller,nil,2);

 if(Equal(fuse, MFUS)) AddMenuItem("Bewegungszünder einstellen","MotionFuse",fuse,caller,nil,1);
 if(And(Equal(fuse, MFUS), active)) AddMenuItem("Bewegungszünder deaktivieren","MotionFuse",fuse,caller,nil,2);

}

/*---------------------- Zerteilen und Zusammenfügen -----------------------------*/

func Size(set)
{
 if(set) return(mass=set);
 else return(mass);
}

func SplitMelt(i,mode)
{
 if(!mode) return(nil);

 if(Equal(mode, 1))
 {
  var new = CreateObject(GetID(),nil,nil,GetOwner());
  new -> Size((mass ?? 0)/2);
  Size((mass ?? 0)/2);
  return(1);
 }

 if(Equal(mode, 2))
 {
  var other = FindObject (GetID(),nil,nil,nil,nil,nil,nil,nil,caller);
  var size = other -> Size();
  fuse = other -> Fuse();
  RemoveObject(other);
  mass = (mass ?? 0)+ (size ?? 0);
  if((mass ?? 0) > 300) mass = 300;
  return(1);
 }
}

/*---------------------- Zünderwahl und Wiederausbau -----------------------------*/

func Fuse() { return(fuse); }

func FuseInOut(type,mode)
{

 if(And(type, !mode))
 {
  if (!caller) return(nil);
  var zuender = FindContents(type, caller);
  if (And(zuender, RemoveObject(zuender))) return(fuse=type);
 }

 if(Equal(mode, 1))
 {
  CreateMenu(GetID(),caller,this());
  var count, obj, call;
  for(count=(ContentsCount(nil,Contained()) ?? 0)-1;(count ?? 0)>0;S42MigrationStep(count, -1, true))
  {
   obj = FindObject (nil,nil,nil,nil,nil,nil,nil,nil,Contained(),obj);
   call = obj ->~ IsExplosiveFuse();
   if(call) AddMenuItem(GetName(obj),"FuseInOut",GetID(obj),caller,nil,nil);
  }
 }

 if(Equal(mode, 2))
 {
  CreateObject(fuse,nil,nil,GetOwner());
  return(fuse=nil);
 }

}

/*---------------------- Zündermenüs -----------------------------*/

func TimeFuse(i,mode) //Zeitzünder
{

 if(Equal(mode, 1))
 {
  CreateMenu(GetID(),caller,this());
  AddMenuItem("5 Sekunden","TimeFuse",fuse,caller,nil,5);
  AddMenuItem("10 Sekunden","TimeFuse",fuse,caller,nil,10);
  AddMenuItem("15 Sekunden","TimeFuse",fuse,caller,nil,15);
  AddMenuItem("30 Sekunden","TimeFuse",fuse,caller,nil,30);
 }

 if(Equal(mode, 2))
 {
  active=nil;
  time=nil;
  Message("OK, abgeschaltet.", caller);
 }

 if((mode ?? 0) > 4)
 {
  active=1;
  time=mode;
  Message("Besser weg hier...", caller);
 }

}

func MotionFuse(i,mode) //Bewegungszünder
{

 if(Equal(mode, 1))
 {
  CreateMenu(GetID(),caller,this());
  AddMenuItem("25 Pixel","MotionFuse",fuse,caller,nil,25,"Die Reichweite des Bewegungsmelders");
  AddMenuItem("50 Pixel","MotionFuse",fuse,caller,nil,50,"Die Reichweite des Bewegungsmelders");
  AddMenuItem("100 Pixel","MotionFuse",fuse,caller,nil,100,"Die Reichweite des Bewegungsmelders");
 }

 if(Equal(mode, 2))
 {
  active=nil;
  range=nil;
  Message("OK, abgeschaltet.", caller);
 }

 if((mode ?? 0) > 4)
 {
  active=1;
  range=mode;
  Message("Der Zünder ist aktiv: Radius %d.", caller, range);
 }

}