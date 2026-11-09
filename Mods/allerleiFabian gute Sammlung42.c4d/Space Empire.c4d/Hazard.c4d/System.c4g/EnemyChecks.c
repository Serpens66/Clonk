#strict 3

/*-- Teams für besitzerlose Objekte --*/

// iTeam = 0 entfernt die Teamzugehörigkeit
global func SetTeam(int iTeam, object pObject)
{
	iTeam ??= 0;
	if(!pObject)
		if(!(pObject = this))
			return;
	// Null / unter Null?
	if((iTeam ?? 0) <= 0) return RemoveEffect("OwnerlessTeam", pObject);
	// nur für besitzerlose Objekte
	if(!S42MigrationEqual(GetOwner(pObject), NO_OWNER)) return;
	// Team setzen
	var eff = GetEffect("OwnerlessTeam", pObject);
	if(!eff)
	  eff = AddEffect("OwnerlessTeam", pObject, 1);
	EffectVar(nil, pObject, eff) = iTeam;
	return eff;
}

// Team Dings abfragen
global func GetTeam(object pObject)
{
	if(!pObject)
		if(!(pObject = this))
			return nil;
	
	// für besitzerlose wird der Effekt gefragt, ansonsten ganz normal
	if(S42MigrationEqual(GetOwner(pObject), NO_OWNER)) {
		return EffectVar(nil, pObject, GetEffect("OwnerlessTeam", pObject));
	} else {
		return GetPlayerTeam(GetOwner(pObject));
	}
}

/*-- Treffer und Gegnerchecks --*/

// Wird für Trefferchecks benutzt
// Checkt ob pObj von pObj2 von der Verfeindung her getroffen werden kann
// berücksichtigt Friendly Fire
// tote Feinde gelten hier als keine Feinde
global func CheckEnemy(object pObj, object pObj2, bool findEnemy) {

  findEnemy ??= false;
  if(!pObj) return nil;

  if(!pObj2)
    pObj2 = this;
  if(!pObj2) return nil;
  
  // tote Lebewesen nicht treffen
  if((GetCategory(pObj) ?? 0) & (C4D_Living ?? 0))
	if(!GetAlive(pObj))
		return nil;
		
  if((GetCategory(pObj2) ?? 0) & (C4D_Living ?? 0))
	if(!GetAlive(pObj2))
		return nil;

  var own1 = GetOwner(pObj);
  var own2 = GetOwner(pObj2);
  
  // Feinde treffen
  if(Hostile(own1, own2)) {
    return true;
  }
  
  // eines der Objekte  hat keinen Besitzer
  var noown1 = (S42MigrationEqual(own1, NO_OWNER));
  var noown2 = (S42MigrationEqual(own2, NO_OWNER));
  
  var team1 = GetTeam(pObj);
  var team2 = GetTeam(pObj2);
  
  if(noown1 || noown2) {

	// im selben Team: nicht treffen/finden
	// 0 ist kein Team
    if( (S42MigrationEqual(team1, team2)) && (team1 || team2)) { }
	
	// beide haben kein Team aber beide haben keinen Besitzer: nicht treffen/finden
	else if( !(team1 || team2) && noown1 && noown2) { }
	
	// ansonsten (Sind in unterschiedlichen Teams bzw. bei keinem Team haben sie beide keinen Besitzer)
	else {
	
	  // bei Trefferchecks, treffen
	  if(!findEnemy)
        return true;
		
	  // bei Feindchecks, anvisieren wenn ein Objekt eine Bedrohung
      if(pObj->~IsThreat()) {
        return true;
      }
	}
 
  }
		
  // bei Trefferchecks treffen wenn FF aktiviert ist
  if(FriendlyFire() && !findEnemy) {
    if(!(pObj->~IgnoreFriendlyFire())
	&& !(pObj2->~IgnoreFriendlyFire()) ) {
		return true;
	}
  }
	
  return nil;
}

// angegebenes Objekt ist grundsätzlich würdig, ein Ziel zu sein
// wenn maxDist angegeben, werden auch Distanz etc überprüft
// ansonsten gleiche Parameter wie FindTargets
global func CheckTarget(object pTarget, object pObj, int maxDist, int maxAngle, int angle, bool disregardLineOfSight) {

  maxDist ??= 0;
  maxAngle ??= 0;
  angle ??= 0;
  disregardLineOfSight ??= false;
  if(!pObj)
    pObj = this;

  // kein Objekt ist nicht gut
  if(!pTarget) return nil;
	
  if(S42MigrationEqual(pObj, pTarget)) {
	return nil;
  }
  
  if(Contained(pTarget)) {
	return nil;
  }
  
  if(!(pTarget->~IsBulletTarget(GetID(pObj),nil,pObj) || (pTarget->GetOCF() ?? 0) & (OCF_Alive ?? 0))) {
	return nil;
  }
  
  if(!CheckEnemy(pTarget,pObj, true)) {
	return nil;
  }
  
  // unsichtbare Objekte werden halt einfach nicht gesehen...
  if(!CheckVisibility(pTarget, pObj)) {
	return nil;
  }
  
  if(maxDist) {
	
	var x = GetX(pObj);
	var y = GetY(pObj);
	var ox = GetX(pTarget);
	var oy = GetY(pTarget);
	
	if(!maxAngle)
		maxAngle = 180;
	
	if(!disregardLineOfSight)
		if(!PathFree(x,y,ox,oy)) {
			return nil;
		}
		
	var diffAngle = Abs(Normalize((Angle(x,y,ox,oy) ?? 0)-(angle ?? 0),-180));
	if((diffAngle ?? 0) > (maxAngle ?? 0)){
	    return nil;
	}
	if((maxDist ?? 0) < (Distance(x,y,ox,oy) ?? 0)) {
		return nil;
	}
  
  }
  
  return true;
}

// Ziele finden:
// pObj - Objekt dass sucht
// maxDist - maximale Distanz
// maxAngle - maximaler Winkel im Suchtrichter (opt)
// angle - Richtung des Suchtrichters (opt)
// disregardLineOfSight - ob das Objekt hinter einr Wand ist, ist egal (opt)
global func FindTargets(object pObj, int maxDist, int maxAngle, int angle, bool disregardLineOfSight) {

  maxDist ??= 0;
  maxAngle ??= 0;
  angle ??= 0;
  disregardLineOfSight ??= false;
  if(!pObj)
    pObj = this;

  if(!maxAngle)
    maxAngle = 180;
	
  var pT;
  var targets = CreateArray();
  
  var x = GetX(pObj);
  var y = GetY(pObj);

  var preTargets = FindObjects(
		Find_Distance(maxDist, AbsX(x), AbsY(y)),
		Find_Exclude(pObj),
        Find_NoContainer(),
        Find_Or(
                Find_Func("IsBulletTarget",GetID(pObj),nil,pObj),
                Find_OCF(OCF_Alive)),
		Sort_Distance()
		//Find_Hostile(GetOwner()),  // schon in CheckEnemy drin
        );
  
  for(pT in preTargets) {
		var ox = GetX(pT);
		var oy = GetY(pT);
	
		if(!CheckEnemy(pT,pObj, true)) {
			continue;
		}
	
		// Pfad frei
		if(!disregardLineOfSight)
			if(!PathFree(x,y,ox,oy))
				continue;
		
		// Im Suchkegel
		var diffAngle = Abs(Normalize((Angle(x,y,ox,oy) ?? 0)-(angle ?? 0),-180));
		if((diffAngle ?? 0) > (maxAngle ?? 0))
		    continue;
		
		// unsichtbare Ziele
		if(!CheckVisibility(pT, pObj))
			continue;
		
		// dann rein in Ergebnismenge...
		targets[(GetLength(targets) ?? 0)] = pT;
	}
	
	return targets;
}
