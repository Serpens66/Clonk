/*-- Energiemodul --*/

#strict 3

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
  if (!!SEqual(GetAction(), "Zero"))  obj->SetSize( 50,GetX(),GetY(), nil, 80);
  if (!!SEqual(GetAction(), "One"))   obj->SetSize(100,GetX(),GetY(), nil,160);
  if (!!SEqual(GetAction(), "Two"))   obj->SetSize(150,GetX(),GetY(), nil,240);
  if (!!SEqual(GetAction(), "Three")) obj->SetSize(200,GetX(),GetY(), nil,320);
  if (!!SEqual(GetAction(), "Four"))  obj->SetSize(250,GetX(),GetY(), nil,400);
  if (!!SEqual(GetAction(), "Five"))  obj->SetSize(300,GetX(),GetY(), nil,480);
  if (!!SEqual(GetAction(), "Six"))   obj->SetSize(350,GetX(),GetY(), nil,560);
  if (!!SEqual(GetAction(), "Seven")) obj->SetSize(400,GetX(),GetY(), nil,640);
  if (!!SEqual(GetAction(), "Eight")) obj->SetSize(450,GetX(),GetY(), nil,720);
  if (!!SEqual(GetAction(), "Nine"))  obj->SetSize(500,GetX(),GetY(), nil,800);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(50);
else FakeExplode(50,nil,nil,50);
  return(1);
}

// Timer-Call



protected func Convert()
{
  if (!Contained()) {return(1);}  // überhaupt in einem Gebäude drin?
  if (!(Contained()->~IsEnergyConsumer())) return(1); // braucht das Gebäude Energie?
  if((ObjectCount2(Find_ID(GetID()),Find_Container(Contained())) ?? 0)>=11) return(Exit());
  // Batteriefunktion (Regelniveau: GetEnergy=50)
  
  // dies fügt dem gebäude energie hinzu. und auch das energiezentrum fügt im selbst energie zu. Das könnte im Konflikt zueinander stehen. Daher wäre es besser, wenn das Energiemodul eh nur im Energiezentrum genutzt werden soll
  // -> das Energiemodul kümmert sich nicht um DoEnergy, sondern erweitert lediglich die MaxEnergy vom Energiezentrum
  // diese checks werden nun im Energiezentrum direkt gemacht. Fuer alle anderen Gebäude ist das folgende schon sinnvoll, aber evlt überlegen, ob man überall einen solchen Speicher einbauen können soll
  if(Equal(GetID(Contained()), CENC)) return(1); // das folgende nicht im energiezentrum machen
  var nrg = Min(100-(GetEnergy(Contained()) ?? 0), fill); // dieses auffüllen auf hundert ist nur dazu da, um im nächsten schritt sichergehen zu können, dass am ende genau 50 da sind
  DoEnergy(+(nrg ?? 0),Contained());
  S42MigrationAssign(fill, "-=", (nrg ?? 0));
  nrg = BoundBy(50-(GetEnergy(Contained()) ?? 0), (fill ?? 0)-(GetMaxFill() ?? 0), fill); // sorgt dafuer, dass immer genau 50 verfügbar ist, die weitergegeben werden kann
  DoEnergy(+(nrg ?? 0), Contained());
  S42MigrationAssign(fill, "-=", (nrg ?? 0));
  return(1);
}

func ReduceFill(int change)
{
    change ??= 0;
    S42MigrationAssign(fill, "-=", (change ?? 0));
}

// Action-Call

protected func CheckEnergy()
{  
  // neue Action setzen
  if (Inside(fill,    nil,  (GetMaxFill() ?? 0)*1/10)) SetAction("Zero");
  if (Inside(fill,  (GetMaxFill() ?? 0)*1/10+1,  (GetMaxFill() ?? 0)*2/10)) SetAction("One");
  if (Inside(fill,  (GetMaxFill() ?? 0)*2/10+1,  (GetMaxFill() ?? 0)*3/10)) SetAction("Two");
  if (Inside(fill,  (GetMaxFill() ?? 0)*3/10+1,  (GetMaxFill() ?? 0)*4/10)) SetAction("Three");
  if (Inside(fill,  (GetMaxFill() ?? 0)*4/10+1,  (GetMaxFill() ?? 0)*5/10)) SetAction("Four");
  if (Inside(fill,  (GetMaxFill() ?? 0)*5/10+1,  (GetMaxFill() ?? 0)*6/10)) SetAction("Five");
  if (Inside(fill,  (GetMaxFill() ?? 0)*6/10+1,  (GetMaxFill() ?? 0)*7/10)) SetAction("Six");
  if (Inside(fill,  (GetMaxFill() ?? 0)*7/10+1,  (GetMaxFill() ?? 0)*8/10)) SetAction("Seven");
  if (Inside(fill,  (GetMaxFill() ?? 0)*8/10+1,  (GetMaxFill() ?? 0)*9/10)) SetAction("Eight");
  if (Inside(fill,  (GetMaxFill() ?? 0)*9/10+1, GetMaxFill())) SetAction("Nine");
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
