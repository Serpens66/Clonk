/*-- Wachturm-Tor --*/

#strict

#include SHTL

/* Initialisierung */

protected func Initialize()
{
  SetComDir(COMD_Up());
  return(1);
}

/* Schaden */

public func MaxDamage() { return 350; }

public func OnDmg(int iDmg, int iType) {

	if(shieldupgrade) {
		if(iType == DMG_Energy)		return(60);
		if(iType == DMG_Explosion)	return(60);
	        if(iType == DMG_Projectile)	return(20);

	}

	if(iType == DMG_Melee)		return(50);
	if(iType == DMG_Bio)		return(100);
	if(iType == DMG_Fire)		return(80);
	if(iType == DMG_Explosion)	return(0);
	if(iType == DMG_Energy)		return(-20);
        if(iType == DMG_Projectile)	return(50);

	return(30);
}

public func OnHit(int iDmg, int iType) {

	// shield flare for energy damage
	if(iType != DMG_Energy) return(0);
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Damage() {
	if(GetDamage() < MaxDamage()) return;

	// Exit all clonks, otherwise they would be removed
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// create debris
	for(var i=0; i<3; ++i) {
	
// boom
	Explode(10);
	}
	
}





/* Außer Reichweite */

private func CheckRange()
{
  if (!GetActionTarget()) return();
	var iY = GetActionTarget()->GetY();
	if (GetY() < iY - 10) SetComDir(COMD_Down());
	if (GetY() > iY + 30) SetComDir(COMD_Up());
}

/* Burgteilfunktion */

private func DestructCheck()
{
  if (!GetActionTarget()) 
		return(RemoveObject());
	if (GetActionTarget()->GetID() != BG5C)
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
  if (GetComDir() == COMD_Up())
  	return(Close());
  else
  	return(Open());
}

private func Close()
{
  if (!GetActionTarget()) return(0);
	if (GetY() > GetActionTarget()->GetY() + 30) return(0);
  SetComDir(COMD_Down());
  return(1);
}

private func Open()
{
  if (!GetActionTarget()) return(0);
	if (GetY() < GetActionTarget()->GetY() - 10) return(0);
  SetComDir(COMD_Up());
  return(1);
}
