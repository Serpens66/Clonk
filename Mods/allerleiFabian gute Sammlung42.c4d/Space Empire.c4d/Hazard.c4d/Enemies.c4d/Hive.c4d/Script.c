/* Nest-Steuerung */

#strict 3

public func NoWarp() { return(true); }

// IDs
static const BubbleID = HBBL;	// Blasen-ID
// Eier
static const EggMCount = 10;   // Maximale Anzahl Eier

// Ausbreitung
local HiveMSize;	// Maximale Nestausbreitung (Pixel)
local BubbMSize;	// Maximale Nestausbreitung (Blasen)
local BubbMHeight;	// Maximale Höhe einer Blase über dem Boden

local seekR;		// interne Variable für die Bubble-Platzierung

local Eggs;			// Meine Eier!
local Aliens;		// Die Aliens die von uns kommen!
local AlienChance;	// Die Chancen der einzelnen Aliens, dass sie erstellt werden
local prodAliens;	// Herstellbare Aliens
local AlienMax;		// Max Aliens

local bubbleDelay;

// Setzbare Variablen
local NestSize;		// Die Größe des Nests
local SpreadRate;	// Ausbreitungsgeschwindigkeit der Bubbles
local Fertility;	// Fruchtbarkeit: wie schnell Eier schlüpfen (0 bis 100)
local BubbDist;		// Minimale Distanz zwischen zwei Bubbles

local lastcount;

/* Interface */

public func Init(int size, int spread, int fertility, int bubbdist)
{
	size ??= 0;
	spread ??= 0;
	fertility ??= 0;
	bubbdist ??= 0;
	SetSpreadRate(spread);
	SetFertility(fertility);
	SetSize(size);
	SetMinBubbleDistance(bubbdist);
}

public func SetSize(int size)
{

	size ??= 0;
	NestSize = size;
	HiveMSize = (200*(NestSize ?? 0))/100;  // Maximale Nestausbreitung (Pixel)
	BubbMSize = (20*(NestSize ?? 0)*(NestSize ?? 0))/8000;   // Maximale Nestausbreitung (Blasen)
	BubbMHeight = (80*(NestSize ?? 0))/100; // Maximale Höhe einer Blase über dem Boden
}

public func SetSpreadRate(int spreadrate)	// 0 bis 100
{
	spreadrate ??= 0;
	SpreadRate = BoundBy(spreadrate,nil,100);
}

public func SetFertility(int fertility)	// 0 bis 100
{
	fertility ??= 0;
	Fertility = BoundBy(fertility,nil,100);
}

public func SetMinBubbleDistance(int dist) {
	dist ??= 0;
	BubbDist = dist;
}

public func GetSize()				{ return(NestSize); }
public func GetSpreadRate()			{ return(SpreadRate); }
public func GetFertility()			{ return(Fertility); }
public func GetMinBubbleDistance()	{ return(BubbDist); }

/* Initialisierung */

protected func Initialize()
{
	// $Depp hat uns falsch platziert! *sterb*
	if(GBackSolid()) return(RemoveObject());

	// auf den Boden der Tatsachen bringen
	var i;
	while(!GBackSolid(nil,i))
		S42MigrationStep(i, 1, true);
  
	SetPosition(GetX(),(GetY() ?? 0)+(i ?? 0)-20);

	// Objekt sets
	Aliens = CreateObject(L_OS,nil,nil,-1); // unsere Aliens
	Eggs = CreateObject(L_OS,nil,nil,-1); // unsere Eier
	
	// Standardparameter
	Init(80+(RandomX(25) ?? 0), 90, 30, 30);
	DefaultTactic();
	
	// Debugzeugs
	DebugLog("Nest-Info:","Hive",);
	DebugLog("  Position: %d, %d","Hive", GetX(), GetY());
	DebugLog("  Größe: %d","Hive", NestSize);
	DebugLog("  Aliens:","Hive", CountHiveAliens());

	// Ich bin das Nest, dein Gott!
	PlaceBubble();
}


/* Wachstum */

protected func Activitiy()
{

  var count = CountHiveParts(BubbleID);
  // Oh mein Gott! Sie haben das Nest getötet! Ihr Wipfe!
  if(!count) return(Die());

  // Wuuu!<
  //if(!Random(100))
  //  AlienFog(RandomX(20,200), RandomX(-100,100), RandomX(-10,10));

  // Schleim
  if(Chance(GetSpreadRate()))
	if((count ?? 0) > (BubbMSize ?? 0)/4)
		if((CountHiveVeins() ?? 0) <= (BubbMSize ?? 0)/4)
			PlaceVein();

  // Eier
  if(Chance(GetFertility()))
	  if((count ?? 0) > (BubbMSize ?? 0)/4)
	  {
	    if((Eggs->GetItemCount() ?? 0) <= (EggMCount ?? 0))
	      PlaceEgg();
	  }

  // Wir suchen nicht alles auf einmal ab, zu viel Rechenleistung!

  // jede Sekunde runterzählen
  if(bubbleDelay) S42MigrationStep(bubbleDelay, -1, true);
  
  // Blasen zählen, schauen ob genug da
  if(And((count ?? 0) <= (BubbMSize ?? 0), (bubbleDelay ?? 0) <= 0))
	{

	// etwas wurde weggeschossen... wieder zurückziehen
	if((lastcount ?? 0) > (count ?? 0)) {
		seekR = nil;
	}

	// Erstmal den Suchradius hochzählen
	S42MigrationAssign(seekR, "+=", (RandomX(6,10) ?? 0));
	// Zu groß?
	if((seekR ?? 0) > (HiveMSize ?? 0)) seekR = nil;
	// Einmal rundherum suchen
	for(var i = Random(360),x,y, j = (i ?? 0) + 360 ; (i ?? 0) < (j ?? 0) ; S42MigrationAssign(i, "+=", (RandomX(10,20) ?? 0)))
	{
		// Ohne meine Koordinaten sage ich nichts!
		x = Sin(i, seekR);
		y = Cos(i, seekR);
		// Hui, keine da! Wie nachlässig. Na ja. Erstmal schauen, ob der Ort auch in Ordnung ist D:!
		if(!GoodPlace2Bubble(x,y))
			continue;
		// Scheint alles gut zu sein. Warum warten wir noch?
		PlaceBubble(x,y);
		// Ok. Hier brechen wir ab, damit das Nest nicht alles auf einmal da ist :o!
		S42MigrationAssign(seekR, "-=", 10);
		// und ein bischen Delay
		var plusDelay = (100-(GetSpreadRate() ?? 0));
		bubbleDelay = (RandomX(1,4) ?? 0)+(plusDelay ?? 0)*(plusDelay ?? 0)/100;
		break;
		
		// Zur Übersicht wie groß das Delay für neue Bubbles ist bei bestimmter SpreadRate ist... (etwa)
		//100 - 5s
		//80  - 9s
		//60  - 21s
		//40	- 41s
		//20	- 69s
		//10	- 86s
		//0	- 1m 45s
	}
	}
	  
	lastcount = count;
}

/* Tod */

public func Die() // Das Nest stirbt :[
{
  RemoveObject();
}

/* Hilfsfunktionen */

private func AddAlien(object alien)
{
  Aliens->AddItem(alien);
}

// Es begab sich aber zu der Zeit, dass ein Gebot von dem Steuerungsobjekt ausging, dass alles Nest geschätzt würde
private func CountHiveParts(id idC)
{
  return(ObjectCount2(Find_ID(idC), Find_Distance(HiveMSize), Find_Owner(GetOwner())));
}

private func CountHiveVeins() {
  return(ObjectCount2(Find_ID(HBBL), Find_Distance(HiveMSize), Find_Owner(GetOwner()), Find_Func("HasVein")));
}

private func CountHiveAliens(id idAlien) {
  return(Aliens->GetItemCount(idAlien));
}

// Heute ist ein guter Tag zum Sterben..äh..dies ist ein guter Ort für eine Blase!
private func GoodPlace2Bubble(int iX, int iY)
{
  // Komisch in der Wand drin?
  iX ??= 0;
  iY ??= 0;
  if(GBackSolid(iX, iY)) return(nil);
  // Viel zu hoch?
  if(!GroundOK(iX, iY)) return(nil);
  // Schauen, ob Blase vorhanden
  if(FindObject2(Find_Distance(GetMinBubbleDistance(),iX, iY), Find_ID(BubbleID))) return(nil);
  // Wah?!
  if(!PathFree(GetX(),GetY(), (GetX() ?? 0)+(iX ?? 0),(GetY() ?? 0)+(iY ?? 0))) return(nil);
  // Noch mehr checken?
  return(true); // Nö!
}

// Das Spice muss fließen!
private func PlaceBubble(int iX, int iY)
{
  iX ??= 0;
  iY ??= 0;
  CreateObject(BubbleID, iX, (iY ?? 0) + (GetDefHeight(BubbleID) ?? 0)/2, GetOwner());
  // Blasen platzen ab und an auch
  // -> machen die blasen selber
  //if(!Random(3))
  //  ScheduleCall(bubble, "Destruction", RandomX(500,1000));
}

// Plaziert tolles schleimiges Zeugs.
private func PlaceVein()
{
  var bubbles = FindObjects(Find_ID(HBBL), Find_Distance(HiveMSize)); //Blasen!
  var b1 = bubbles[(Random(GetLength(bubbles)) ?? 0)];
  var b2 = bubbles[(Random(GetLength(bubbles)) ?? 0)];
  if(Equal(b1, b2)) return;
  
  //b1->SetVeinTo(b2);
}

// Eier für die Brut!
private func PlaceEgg()
{
  //if(Aliens->GetItemCount() >= 15) return();
  // Herausfinden, ob ein Alientyp gebraucht wird
  var idAlien = GetAlienID();
  if(!idAlien) return;
  // Ein Ei platzieren
  var bubble = FindObject2(Find_ID(BubbleID),Find_Distance(HiveMSize),Sort_Random());
  if(bubble) {
		bubble->Set(idAlien,this());
		Eggs->AddItem(bubble);
  }
}

// Nicht den Boden unter den Füßen verlieren
private func Check2Ground(int iX, int iY)
{
  iX ??= 0;
  iY ??= 0;
  for(var i = nil ; (i ?? 0) - 10 < (BubbMHeight ?? 0) ; S42MigrationAssign(i, "+=", 10))
    if(GBackSolid(iX, (iY ?? 0)+(i ?? 0)))
      break;
  return(i);
}

private func GroundOK(int iX, int iY) {
	iX ??= 0;
	iY ??= 0;
	return( (Check2Ground(iX,iY) ?? 0) <= (BubbMHeight ?? 0)*(100-100*(iX ?? 0)*(iX ?? 0)/(HiveMSize ?? 0)/(HiveMSize ?? 0)) );
}

protected func Destruction() {
	if(Eggs) Eggs->RemoveObject();
	if(Aliens) Aliens->RemoveObject();
}

/** Alien-Produzier-Zeug **/

/* Vermehrung */

// Liefert die ID eines Alientyps, der zZ benötigt wird
public func GetAlienID()
{
	// auswürfeln welches wir haben wollen
	var die = Random(100);
	for(var i = nil; (i ?? 0) < (GetLength(prodAliens) ?? 0); S42MigrationStep(i, 1)) {
		var chance = AlienChance[(i ?? 0)];
		if((die ?? 0) <= (chance ?? 0)) {
			var maxAliens = AlienMax[(i ?? 0)];
			var idAlien = prodAliens[(i ?? 0)];
			var aliens = CountHiveAliens(idAlien);
			if(Or(!maxAliens, (maxAliens ?? 0) >= (aliens ?? 0))) {
				return( idAlien );
			} else {
				return( nil );
			}
		}
	}
}

public func AlienClear() {
	prodAliens = nil;
	AlienChance = nil;
}

public func AlienAdd(id idAlien, int probability, int max) {
	probability ??= 0;
	max ??= 0;
	if(!prodAliens)
		prodAliens = CreateArray();
	if(!AlienChance)
		AlienChance = CreateArray();
	if(!AlienMax)
		AlienMax = CreateArray();
		
	var len = GetLength(AlienChance);
	
	// schon voll...
	if((AlienChance[(len ?? 0)-1] ?? 0) >= 100) return;
	
	prodAliens[(len ?? 0)] = idAlien;
	AlienMax[(len ?? 0)] = max;
	
	// Ein Array was so aussieht...
	// 0 -> 20
	// 1 -> 50
	// 2 -> 100
	// bedeutet dass 0 20% Chance hat, 1 30% und 2 50%...
	var before = nil;
	if((len ?? 0)-1 >= 0)
		before = AlienChance[(len ?? 0)-1];
	AlienChance[(len ?? 0)] = Min((before ?? 0) + (probability ?? 0),100);
}

public func DefaultTactic() {
	AlienAdd(ALN2,60,20);
	AlienAdd(ALN4,25,10);
	AlienAdd(ALN1,15,3);
}