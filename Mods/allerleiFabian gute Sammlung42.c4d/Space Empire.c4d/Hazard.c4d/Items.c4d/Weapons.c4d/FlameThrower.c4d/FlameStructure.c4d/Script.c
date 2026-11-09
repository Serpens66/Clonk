/*-- Anzünderobjekt --*/
 
#strict 2

local slimy, iDamage, iLight;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.
local hits;
local maxlifetime;
local mlight;
local direction;


func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
  
  direction = 2 * GetDir(shooter) - 1;

  maxlifetime = 40;
  hits = 0;
}


public func MaxLifeTime() { return maxlifetime; }

public func SetMaxLifetime(int max) {
	maxlifetime = max;
}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  slimy = slime;
  iDamage = iDamagepar;
  if(!iDamage)
    iDamage = 1;

  SetSpeed(xdir,ydir);
  SetAction("Fly");
  AddEffect("HitCheck", this(), 1,1, 0,GetID(this),shooter);
//  if(slimy) { TurnLightOnN();}
//  else { TurnLightOn();}
}


protected func Flying()
{

  if(InLiquid()) {
    // Blubberblubber
    for(var i=0; i<5; ++i) Bubble(RandomX(-5,+5),RandomX(-5,5));
    RemoveObject();
	return;
  }

  if(GetActTime() > MaxLifeTime()-Random(5)) {
    RemoveObject();
    return;
  }
  
  if(!Random(4) && GetActTime() > MaxLifeTime()/4)
    SetYDir(GetYDir()+RandomX(-4,2));

  if(slimy) {
    CastSlimeEffects(GetActTime());
  } else {
    CastFireEffects(GetActTime());
  }
}

public func HitObject(object pObj)
{
  var color = FIRE_Green;
  // 100..70
  var schwaecher = Max(100 - hits*20,0);
  //Message("@%d%",this,schwaecher);
  // Energie abziehen und langsamer machen
  if(!slimy) 
  {
    color = FIRE_Red;
    DoDmg(iDamage*schwaecher/100, DMG_Fire, pObj,1);
  }
  else {
    if (GetOCF(pObj) & OCF_Chop || pObj->~IsBulletGrasTarget() ) 
    {
// sorgt dafür, dass getroffene Pflanzen "krank" werden. Dies wird allerdings schnell wieder durch das Umweltschutz-objekt rückgängig gemacht, sofern die Umwelt unverschmutzt ist.
var graphics;
graphics = "Krank";
if(!pObj->~ChangeGraphic(graphics))
        FadeGraphics(graphics,pObj);
    }
    else { DoDmg(iDamage*schwaecher/100, DMG_Bio, pObj, 1);}
       }

  hits++;
  
  //falls feuer anzündbares anzünden, aber nicht lebewesen.
if(!slimy)
 {
  if(pObj) //existiert es überhaupt noch?
    if(pObj->GetOCF() & OCF_Inflammable) {
      if(!(pObj->GetOCF() & OCF_Living))
        Incinerate(pObj);
	  else
	    AddFireEffect(pObj,50,color,1);
	}
 }
}

protected func CastFireEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  
  var ntime = 40*time/maxtime;

  for (var i = 0; i < 3; i++) 
  {

    var xoff = RandomX(-2,5)+ntime/2;
	var yoff = RandomX(-ntime , ntime);
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);
	
    var size = 20 + ntime*6;

	var x = Sin(angle,(+10-2*i));
    var y = RandomX(-size,size)/20;

	var alpha = BoundBy(180 + (time-maxtime*3/4)*2,0,255);
    var color = RGBa(255, 255, 255, alpha);

    CreateParticle("Fire2", x, y, xdir, ydir, size, color);
  }  
  
}

protected func CastSlimeEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  
  var ntime = 40*time/maxtime;
  
  for (var i = 0; i < 3; i++) 
  {

    var xoff = RandomX(-2,5);
	var yoff = RandomX(-ntime , ntime)/2;
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);
	
    var size = 20 + ntime*8;

	var x = (+10-2*i)*dir;
    var y = RandomX(-size,size)/20;

	var alpha = BoundBy(ntime ,0,255);
    var color = RGBa(100, 255, 0, alpha);

    CreateParticle("AlienPlasma", x, y, xdir, ydir, size, color);
  }  
}

/* Effekt für Trefferüberprüfung */

// weitgehend kopiert aus Schuss :(

// EffectVars:
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 

private func FxHitCheckStart(object target, int effect, int temp, object byObj)
{
  if(temp) return;
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
}

private func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var x = GetX(target);
  var y = GetY(target);
  var angle = Angle(0,0,GetXDir(target),GetYDir(target));
  var maxlife = target->~MaxLifeTime();
  var sq = 30*time/maxlife;
  var wdt = 20 + Abs(Cos(angle, sq));
  var hgt = 20 + Abs(Sin(angle, sq));
  
  //target->SetObjDrawTransform(wdt*1000/4,0,0,0,hgt*1000/4);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)

  for(obj in FindObjects(Find_InRect(x-wdt/2,y-hgt/2,wdt,hgt),Find_NoContainer()))
  {
		// Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletExtraTarget(GetID(target),target,EffectVar(2, target, effect))|| GetOCF(obj) & OCF_Chop || obj->~IsBulletGrasTarget(GetID(target),target,EffectVar(2, target, effect))) {
			return target->~HitObject(obj);
	  }
  }
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(x,y),Find_ID(EffectVar(3, target, effect))))
	    	// und schauen, ob es der Schütze ist.
	      if(foo == EffectVar(2, target, effect))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = false;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

func MeltIce(iX,iY)
{
  DigFree(iX, iY, 12);
  CastPXS("Water", 5, 10, iX, iY+RandomX(-6, 6));
}

// aus eke reloaded
protected func Hit()
{

  if(slimy) return(RemoveObject());

  var m = GetMaterial(direction);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}
  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*2);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*3);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction,1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction,-1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*2,1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*2,-1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*3,1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // noch nichts?
  if (!offset) 
 {
  var m = GetMaterial(direction*3,-1);
  var offset;  
  if (m == Material("Snow"))
  {
    offset = direction * 10;
  }
  if (m == Material("Ice"))
  { 
    offset = direction * 20;
  }
 }
if(offset)
{
ScheduleCall(0,"MeltIce",12,0,GetX(),GetY());
}

  // andere Materialien
  if (!offset) return(RemoveObject());
  return(1);
}

func NoWarp() { return true; }



private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,85);
  mlight = AddLight(1000, iColor, this());
  return(1);
}


private func TurnLightOnN()
{
  var iColor;
  iColor = RGBa(150,235,10,85);
  mlight = AddLight(1000, iColor, this());
  return(1);
}