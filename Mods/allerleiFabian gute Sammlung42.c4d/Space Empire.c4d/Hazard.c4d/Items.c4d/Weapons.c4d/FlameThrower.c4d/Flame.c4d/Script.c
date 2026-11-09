/*-- Anzünderobjekt --*/
 
#strict 3

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

  direction = 2 * (GetDir(shooter) ?? 0) - 1;
  
  maxlifetime = 40;
  hits = nil;
}


public func MaxLifeTime() { return maxlifetime; }

public func SetMaxLifetime(int max) {
	max ??= 0;
	maxlifetime = max;
}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  xdir ??= 0;
  ydir ??= 0;
  slime ??= false;
  iDamagepar ??= 0;
  slimy = slime;
  iDamage = iDamagepar;
  if(!iDamage)
    iDamage = 1;

  SetSpeed(xdir,ydir);
  SetAction("Fly");
  AddEffect("HitCheck", this(), 1,1, nil,GetID(this),shooter);
//  if(slimy) { TurnLightOnN();}
//  else { TurnLightOn();}
}


protected func Flying()
{

  if(InLiquid()) {
    // Blubberblubber
    for(var i=nil; (i ?? 0)<5; S42MigrationStep(i, 1)) Bubble(RandomX(-5,+5),RandomX(-5,5));
    RemoveObject();
	return;
  }

  if((GetActTime() ?? 0) > (MaxLifeTime() ?? 0)-(Random(5) ?? 0)) {
    RemoveObject();
    return;
  }
  
  if(!Random(4) && (GetActTime() ?? 0) > (MaxLifeTime() ?? 0)/4)
    SetYDir((GetYDir() ?? 0)+(RandomX(-4,2) ?? 0));

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
  var schwaecher = Max(100 - (hits ?? 0)*20,nil);
  //Message("@%d%",this,schwaecher);
  // Energie abziehen und langsamer machen
  if(!slimy) 
  {
    color = FIRE_Red;
    DoDmg((iDamage ?? 0)*(schwaecher ?? 0)/100, DMG_Fire, pObj,1);
  }
  else {
    if ((GetOCF(pObj) ?? 0) & (OCF_Chop ?? 0) || pObj->~IsBulletGrasTarget() )
    {
// sorgt dafür, dass getroffene Pflanzen "krank" werden. Dies wird allerdings schnell wieder durch das Umweltschutz-objekt rückgängig gemacht, sofern die Umwelt unverschmutzt ist.
var graphics;
graphics = "Krank";
if(!pObj->~ChangeGraphic(graphics))
        FadeGraphics(graphics,pObj);
    }
    else { DoDmg((iDamage ?? 0)*(schwaecher ?? 0)/100, DMG_Bio, pObj, 1);}
       }

  S42MigrationStep(hits, 1, true);
  
  //falls feuer anzündbares anzünden, aber nicht lebewesen.
if(!slimy)
 {
  if(pObj) //existiert es überhaupt noch?
    if((pObj->GetOCF() ?? 0) & (OCF_Inflammable ?? 0)) {
      if(!((pObj->GetOCF() ?? 0) & (OCF_Living ?? 0)))
        Incinerate(pObj);
	  else
	    AddFireEffect(pObj,50,color,1);
	}
 }
}

protected func CastFireEffects(int time)
{
  time ??= 0;
  var maxtime = MaxLifeTime();
  var dir = (((GetXDir() ?? 0)>0) ?? 0)*2-1;
  var angle = Angle(nil,nil,GetXDir(),GetYDir());
  
  var ntime = 40*(time ?? 0)/(maxtime ?? 0);

  for (var i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
  {

    var xoff = (RandomX(-2,5) ?? 0)+(ntime ?? 0)/2;
	var yoff = RandomX(-(ntime ?? 0) , ntime);
  
    var xdir = (+(Cos(angle,yoff) ?? 0) ?? 0)-(Sin(angle,xoff) ?? 0);
    var ydir = (+(Sin(angle,yoff) ?? 0) ?? 0)+(Cos(angle,xoff) ?? 0);
	
    var size = 20 + (ntime ?? 0)*6;

	var x = Sin(angle,((+10 ?? 0)-2*(i ?? 0)));
    var y = (RandomX(-(size ?? 0),size) ?? 0)/20;

	var alpha = BoundBy(180 + ((time ?? 0)-(maxtime ?? 0)*3/4)*2,nil,255);
    var color = RGBa(255, 255, 255, alpha);

    CreateParticle("Fire2", x, y, xdir, ydir, size, color);
  }  
  
}

protected func CastSlimeEffects(int time)
{
  time ??= 0;
  var maxtime = MaxLifeTime();
  var dir = (((GetXDir() ?? 0)>0) ?? 0)*2-1;
  var angle = Angle(nil,nil,GetXDir(),GetYDir());
  
  var ntime = 40*(time ?? 0)/(maxtime ?? 0);
  
  for (var i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
  {

    var xoff = RandomX(-2,5);
	var yoff = (RandomX(-(ntime ?? 0) , ntime) ?? 0)/2;
  
    var xdir = (+(Cos(angle,yoff) ?? 0) ?? 0)-(Sin(angle,xoff) ?? 0);
    var ydir = (+(Sin(angle,yoff) ?? 0) ?? 0)+(Cos(angle,xoff) ?? 0);
	
    var size = 20 + (ntime ?? 0)*8;

	var x = ((+10 ?? 0)-2*(i ?? 0))*(dir ?? 0);
    var y = (RandomX(-(size ?? 0),size) ?? 0)/20;

	var alpha = BoundBy(ntime ,nil,255);
    var color = RGBa(100, 255, nil, alpha);

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
  effect ??= 0;
  temp ??= 0;
  if(temp) return;
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = nil;
}

private func FxHitCheckTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  var obj;
  // Oh man. :O
  var x = GetX(target);
  var y = GetY(target);
  var angle = Angle(nil,nil,GetXDir(target),GetYDir(target));
  var maxlife = target->~MaxLifeTime();
  var sq = 30*(time ?? 0)/(maxlife ?? 0);
  var wdt = 20 + (Abs(Cos(angle, sq)) ?? 0);
  var hgt = 20 + (Abs(Sin(angle, sq)) ?? 0);
  
  //target->SetObjDrawTransform(wdt*1000/4,0,0,0,hgt*1000/4);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)

  for(obj in FindObjects(Find_InRect((x ?? 0)-(wdt ?? 0)/2,(y ?? 0)-(hgt ?? 0)/2,wdt,hgt),Find_NoContainer()))
  {
		// Excludes
		if(S42MigrationEqual(obj, target)) continue;
		if(S42MigrationEqual(obj, exclude)) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)) || (GetOCF(obj) ?? 0) & (OCF_Alive ?? 0)) {
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
	      if(S42MigrationEqual(foo, EffectVar(2, target, effect)))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = nil;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

protected func Hit()
{
return(RemoveObject());
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