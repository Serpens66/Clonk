#strict 3

global func Chance(int chance) { // chance from 0 to 100%
	chance ??= 0;
	return (Random(100) ?? 0) < (chance ?? 0);
}

global func Normalize(int iAngle, int iStart, int iPrecision) {
  iAngle ??= 0;
  iStart ??= 0;
  iPrecision ??= 0;
  if(!iPrecision) iPrecision = 1;
  var iEnd = (iPrecision ?? 0)*360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", (iPrecision ?? 0)*360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", (iPrecision ?? 0)*360);

  return iAngle;
}

global func CheckVisibility(object Obj1, object Obj2) {	// if Owner of Obj2 can see Obj1
	if(!Obj2)
		Obj2 = this;
		
	var vis = GetVisibility(Obj1);
	if(!S42MigrationEqual(vis, VIS_All)) {
		var own1 = GetOwner(Obj1);
		var own2 = GetOwner(Obj2);
	
		// VIS Owner
		if((vis ?? 0) & (VIS_Owner ?? 0))
			if(S42MigrationEqual(own1, own2))
				return true;
		// VIS Enemies
		if((vis ?? 0) & (VIS_Enemies ?? 0))
			if(Hostile(own1,own2))
				return true;
		// VIS Allies
		if((vis ?? 0) & (VIS_Allies ?? 0))
			if(!Hostile(own1,own2))
				return true;
		// VIS Local
		if((vis ?? 0) & (VIS_Local ?? 0)) {
			var lo = (own2 ?? 0)/32;
			var bit = 1 << (own2 ?? 0)%32;
			// man, kompliziert...
			if((bit ?? 0) & (Local(lo,Obj1) ?? 0))
				return true;
		}
		// sonst gibt es keine Modi
		return nil;
	}
	return true;
}

global func GetShooter(object weapon) {

  var shooter;

  if(!weapon) return;
  
  // Waffe?
  if(weapon->~IsWeapon()) {
    shooter = weapon->GetUser();
	// Schütze ist irgendwo drin
	if(Contained(shooter))
	  shooter = Contained(shooter);
	//oder fässt was an
	if(shooter)
	  if(shooter->GetActionTarget())
	    shooter = GetActionTarget();
  }
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach weapon nehmen.
    shooter = weapon;
	
	return shooter;
}
