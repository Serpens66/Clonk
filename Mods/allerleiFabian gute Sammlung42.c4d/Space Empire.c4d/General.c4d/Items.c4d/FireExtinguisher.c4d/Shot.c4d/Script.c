/*-- Löschschuss --*/

#strict 3


/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  return(1);
}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  xdir ??= 0;
  ydir ??= 0;
  slime ??= false;
  iDamagepar ??= 0;
  SetSpeed(xdir,ydir);
  SetAction("Fly");
//  AddEffect("HitCheck", this(), 1,1, 0,GetID(this),shooter);
}



/* Brand löschen */

public func ExtinguishFire()
{
  var obj = FindObject(nil,-10,-10,20,20, OCF_OnFire);
  if (!obj) return(1);
  Extinguish(obj);
  Sound("Pshshsh");
  return(Remove());
}


/* Treffer */

protected func Hit()
{
  var ph = BoundBy(GetPhase(),nil,6);
  SetAction("Attach");
  SetPhase(ph);
  return(1);
}


/* Weg damit */

public func Remove()
{
  return(RemoveObject());
}





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
		if(Equal(obj, target)) continue;
		if(Equal(obj, exclude)) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(Or(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)), (GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))) {
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
	      if(Equal(foo, EffectVar(2, target, effect)))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = nil;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

