/*--  GubblBlubbl  --*/

#strict 3

local pd;
local idAlien;
local hive;
local myalien;
local li, a;
local vein, hasvein;

public func MaxDamage()		{ return(25); }

public func IsAlien()			{ return(true); }
public func IsBulletTarget()	{ return(true); }
public func NoWarp()			{ return(true); }

public func GetAlienID()	{ return(idAlien); }
public func GetMyHive()		{ return(hive); }
public func HasVein()		{ return(hasvein); }
public func GetVeinTarget()	{ return(vein); }

public func Hatch()
{
	if(!idAlien) return;
	
	var alien = CreateObject(idAlien, nil, nil, GetOwner());
	if(hive) hive->AddAlien(alien);

	// Aufbrechen
	Sound("AlienEggBreak*");
	DoDamage(100);
}

// Was werd ich wohl mal werden, wenn ich groß bin?
public func Set(id idA, object by)
{
	idAlien = idA;
	hive = by;
	myalien = nil;
	
	// machen wir mal ein kleines Ding da rein
	SetGraphics(nil,nil,HBBL,1,GFXOV_MODE_Action,"Alien");
	SetClrModulation(RGBa(255,255,255,200),nil,1);
	SetObjDrawTransform(myalien,nil,nil,nil,myalien,nil,nil,1);
}

public func SetVeinTo(object to) {
	
  // wir ham schon...
  if(vein) return;
  if(Equal(to->GetVeinTarget(), this())) return;
  
  var distance = ObjectDistance(to);
  
  // zu nah
  if((distance ?? 0) < 50) return;

  vein = to;
  hasvein = true;
  
  SetGraphics(nil,nil,HBBL,2,GFXOV_MODE_Action,"Vein");
  SetClrModulation(RGBa(li,li,li,30),nil,2);
  
  ShowVein();
}

public func RemoveVein() {
	vein = nil;
	hasvein = nil;
	SetGraphics(nil,nil,nil,2);
}

private func ShowVein(int widthMod) {

  widthMod ??= 0;
  if(!vein) return;
  if(!widthMod) widthMod = 1000;

  var angle = (Angle(GetX(),GetY(),GetX(vein),GetY(vein)) ?? 0)+90;
  var distance = ObjectDistance(vein);
  
  var w = 1000*(distance ?? 0)/140;
  var l = widthMod;
  
  var fsin = -(Sin(angle, 1000) ?? 0), fcos = Cos(angle, 1000);

  var xoff = -(distance ?? 0)/2;
  var yoff = nil;

  var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(l ?? 0)/1000;
  var xskew = (+(fsin ?? 0) ?? 0)*(l ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;

  var xadjust = (+(fcos ?? 0) ?? 0)*(xoff ?? 0) + (fsin ?? 0)*(yoff ?? 0);
  var yadjust = -(fsin ?? 0)*(xoff ?? 0) + (fcos ?? 0)*(yoff ?? 0);
  
  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust,
	nil, 2
  );
}

protected func Initialize()
{
	pd = Random(90);
	DoCon(RandomX(nil,25));
	
	// Wir sehen klasse aus!
	SetAction("Bubble");
	SetPhase(Random(7));
	
	// vorne oder hinten
	li = 200;
	var alpha = nil;
	var size = RandomX(50,80);
	var front = nil;
	
	if(Chance(60)) {
		SetCategory((GetCategory() ?? 0) | (C4D_Foreground ?? 0));
		li = 255;
		S42MigrationAssign(alpha, "+=", 60);
		front = true;
		
	}
	
	// ein bischen Farbmodulation
	a = 40+(alpha ?? 0)/2;
	
	SetClrModulation(RGBa(li,li,li,a));
	
	// Aleinnest-hintergrund
	var color = RGBa(li,li,li,alpha);
	AlienHive(size, RandomX(-5,5), RandomX(-5,5), color,this(), front);

}

protected func BubbleBobble() {

	// Effekte
	
	var t = (GetActTime() ?? 0)*20;
	
	var pulse = Abs(Pulse(t));
	
	var wdt = 1000+(pulse ?? 0)/7;
	var hgt = 1000+(Abs(Pulse((t ?? 0)+(pd ?? 0))) ?? 0)/7;

	SetObjDrawTransform(wdt,nil,nil,nil,hgt,nil,this(),nil);
	
	var lo = BoundBy((li ?? 0)-(myalien ?? 0)/8,50,255);
	
	SetClrModulation(RGBa(lo,lo,lo,a));
	
	// Eier Bayer
	if(idAlien) {
		S42MigrationAssign(myalien, "+=", (RandomX(1,3) ?? 0));
			
		if((myalien ?? 0) >= 1200) {
			Hatch();
		}
		SetObjDrawTransform((myalien ?? 0)*(wdt ?? 0)/1000,nil,nil,nil,(myalien ?? 0)*(wdt ?? 0)/1000,nil,nil,1);
		SetClrModulation(RGBa(255,255,255,200-(myalien ?? 0)/8),nil,1);
	}
	
	// Ader Makaber!
	if(hasvein) {
		if(!vein) RemoveVein();
		else {
			ShowVein(800+(Pulse(3*(t ?? 0)/2) ?? 0)/20);
		}
	}
}

global func Pulse(int pos) {
	pos ??= 0;
	var pulse = Sin(3*(pos ?? 0)/5,1000);
	var fire = 1000-(Abs(Sin((pos ?? 0)/7+90,1000)) ?? 0);
	return( ((pulse ?? 0) * (fire ?? 0) ) / 1000);
}

// Aua!
public func Damage() {
  if((GetDamage() ?? 0) > (MaxDamage() ?? 0)) {
    // Effekt
        AlienSplatter(60*(GetCon() ?? 0)/100);
	AlienBlood(60);
    RemoveObject();
  }
}

public func OnDmg(int iDmg, int iType)
{
  //Melee
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Melee))
    return(50);
  if(Equal(iType, DMG_Bio)) //Bioresistent.
    return(90);
  if(Equal(iType, DMG_Energy)) //fast Energieresistent
    return(50);
  if(Equal(iType, DMG_Fire))
    return(-50);
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }