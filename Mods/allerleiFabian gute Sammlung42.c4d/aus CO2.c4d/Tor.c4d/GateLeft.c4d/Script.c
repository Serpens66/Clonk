/*-- Wachturm-Tor --*/

#strict 3

#include SHTL

/* Initialisierung */

protected func Initialize()
{
  SetComDir(COMD_Up);
  return(1);
}

/* Schaden */

public func MaxDamage() { return 350; }

public func OnDmg(int iDmg, int iType) {

	iDmg ??= 0;
	iType ??= 0;
	if(shieldupgrade) {
		if(Equal(iType, DMG_Energy))		return(60);
		if(Equal(iType, DMG_Explosion))	return(60);
	        if(Equal(iType, DMG_Projectile))	return(20);

	}

	if(Equal(iType, DMG_Melee))		return(50);
	if(Equal(iType, DMG_Bio))		return(100);
	if(Equal(iType, DMG_Fire))		return(80);
	if(Equal(iType, DMG_Explosion))	return(nil);
	if(Equal(iType, DMG_Energy))		return(-20);
        if(Equal(iType, DMG_Projectile))	return(50);

	return(30);
}

public func OnHit(int iDmg, int iType) {

	// shield flare for energy damage
	iDmg ??= 0;
	iType ??= 0;
	if(!Equal(iType, DMG_Energy)) return(nil);
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Damage() {
	if((GetDamage() ?? 0) < (MaxDamage() ?? 0)) return;

	// Exit all clonks, otherwise they would be removed
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// create debris
	for(var i=nil; (i ?? 0)<3; S42MigrationStep(i, 1)) {
	
// boom
	Explode(10);
	}
	
}





/* Außer Reichweite */

private func CheckRange()
{
  if (!GetActionTarget()) return;
	var iY = GetActionTarget()->GetY();
	if ((GetY() ?? 0) < (iY ?? 0) - 10) SetComDir(COMD_Down);
	if ((GetY() ?? 0) > (iY ?? 0) + 30) SetComDir(COMD_Up);
}

/* Burgteilfunktion */

private func DestructCheck()
{
  if (!GetActionTarget()) 
		return(RemoveObject());
	if (!Equal(GetActionTarget()->GetID(), BG5C))
		return(RemoveObject());
}

public func CastleChange() 
{ 
	return(DestructCheck()); 
}

/* Steuerung */

public func Activate()
{
  Sound("Click");
  Sound("SteelGate1");
  if (Equal(GetComDir(), COMD_Up))
  	return(Close());
  else
  	return(Open());
}

private func Close()
{
  if (!GetActionTarget()) return(nil);
	if ((GetY() ?? 0) > (GetActionTarget()->GetY() ?? 0) + 30) return(nil);
  SetComDir(COMD_Down);
  return(1);
}

private func Open()
{
  if (!GetActionTarget()) return(nil);
	if ((GetY() ?? 0) < (GetActionTarget()->GetY() ?? 0) - 10) return(nil);
  SetComDir(COMD_Up);
  return(1);
}
