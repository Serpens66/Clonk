/*-- Energiemodul --*/

#strict

local fill;

protected func Initialize()
{
  SetAction("Zero");
  return(1);
}

protected func Hit()
{
  Sound("RockHit2");
  return(1);
}

protected func Damage()
{
  // EMP generieren und Größe an Ladung anpassen
  var obj = CreateObject(SWV2);
  if (GetAction() eq "Zero")  obj->SetSize( 50,GetX(),GetY(), 0, 80);
  if (GetAction() eq "One")   obj->SetSize(100,GetX(),GetY(), 0,160);
  if (GetAction() eq "Two")   obj->SetSize(150,GetX(),GetY(), 0,240);
  if (GetAction() eq "Three") obj->SetSize(200,GetX(),GetY(), 0,320);
  if (GetAction() eq "Four")  obj->SetSize(250,GetX(),GetY(), 0,400);
  if (GetAction() eq "Five")  obj->SetSize(300,GetX(),GetY(), 0,480);
  if (GetAction() eq "Six")   obj->SetSize(350,GetX(),GetY(), 0,560);
  if (GetAction() eq "Seven") obj->SetSize(400,GetX(),GetY(), 0,640);
  if (GetAction() eq "Eight") obj->SetSize(450,GetX(),GetY(), 0,720);
  if (GetAction() eq "Nine")  obj->SetSize(500,GetX(),GetY(), 0,800);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(50);
else FakeExplode(50,0,0,50);
  return(1);
}

// Timer-Call



protected func Convert()
{
  if (!Contained()) {return(1);}  // überhaupt in einem Gebäude drin?
  if (!(Contained()->~IsEnergyConsumer())) return(1); // braucht das Gebäude Energie?
  if(ObjectCount2(Find_ID(GetID()),Find_Container(Contained()))>=11) return(Exit());
  // Batteriefunktion (Regelniveau: GetEnergy=50)
  
  // dies fügt dem gebäude energie hinzu. und auch das energiezentrum fügt im selbst energie zu. Das könnte im Konflikt zueinander stehen. Daher wäre es besser, wenn das Energiemodul eh nur im Energiezentrum genutzt werden soll
  // -> das Energiemodul kümmert sich nicht um DoEnergy, sondern erweitert lediglich die MaxEnergy vom Energiezentrum
  // diese checks werden nun im Energiezentrum direkt gemacht. Fuer alle anderen Gebäude ist das folgende schon sinnvoll, aber evlt überlegen, ob man überall einen solchen Speicher einbauen können soll
  if(GetID(Contained())==CENC) return(1); // das folgende nicht im energiezentrum machen
  var nrg = Min(100-GetEnergy(Contained()), fill); // dieses auffüllen auf hundert ist nur dazu da, um im nächsten schritt sichergehen zu können, dass am ende genau 50 da sind
  DoEnergy(+nrg,Contained());
  fill -= nrg;
  nrg = BoundBy(50-GetEnergy(Contained()), fill-GetMaxFill(), fill); // sorgt dafuer, dass immer genau 50 verfügbar ist, die weitergegeben werden kann
  DoEnergy(+nrg, Contained());
  fill -= nrg;
  return(1);
}

func ReduceFill(int change)
{
    fill-=change;
}

// Action-Call

protected func CheckEnergy()
{  
  // neue Action setzen
  if (Inside(fill,    0,  GetMaxFill()*1/10)) SetAction("Zero");
  if (Inside(fill,  GetMaxFill()*1/10+1,  GetMaxFill()*2/10)) SetAction("One");
  if (Inside(fill,  GetMaxFill()*2/10+1,  GetMaxFill()*3/10)) SetAction("Two");
  if (Inside(fill,  GetMaxFill()*3/10+1,  GetMaxFill()*4/10)) SetAction("Three");
  if (Inside(fill,  GetMaxFill()*4/10+1,  GetMaxFill()*5/10)) SetAction("Four");
  if (Inside(fill,  GetMaxFill()*5/10+1,  GetMaxFill()*6/10)) SetAction("Five");
  if (Inside(fill,  GetMaxFill()*6/10+1,  GetMaxFill()*7/10)) SetAction("Six");
  if (Inside(fill,  GetMaxFill()*7/10+1,  GetMaxFill()*8/10)) SetAction("Seven");
  if (Inside(fill,  GetMaxFill()*8/10+1,  GetMaxFill()*9/10)) SetAction("Eight");
  if (Inside(fill,  GetMaxFill()*9/10+1, GetMaxFill())) SetAction("Nine");
  return(1);
}

// Statusabfrage von außen

protected func ControlThrow()
{
  [Statusabfrage]
  Message("Füllstand: %d/%d",this(), GetStatus(),GetMaxFill());
  return(1);
}

// Eigenschaften

public func GetMaxFill() { return(5000); } // ist echte GetEnergy Energie
public func GetStatus() { return(fill); } // Füllstand 
public func IsRefineryProduct() { return(1); }
