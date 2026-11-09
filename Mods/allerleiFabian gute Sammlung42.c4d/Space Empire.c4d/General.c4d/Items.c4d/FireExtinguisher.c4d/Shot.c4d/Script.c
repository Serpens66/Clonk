/*-- Löschschuss --*/

#strict


/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  return(1);
}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  SetSpeed(xdir,ydir);
  SetAction("Fly");
//  AddEffect("HitCheck", this(), 1,1, 0,GetID(this),shooter);
}



/* Brand löschen */

public func ExtinguishFire()
{
  var obj = FindObject(0,-10,-10,20,20, OCF_OnFire());
  if (!obj) return(1);
  Extinguish(obj);
  Sound("Pshshsh");
  return(Remove());
}


/* Treffer */

protected func Hit()
{
  var ph = BoundBy(GetPhase(),0,6);
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
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)) || GetOCF(obj) & OCF_Alive) {
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

