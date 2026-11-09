/*-- Blaster --*/

#strict 3

local overload, count;

protected func Initialize()
{
  count=5;  
  return(1);
}

public func Fire()
{
  // noch beim Nachladen?
  if (!ActIdle()) return(nil);
  
  // Schuss erzeugen und Feuerroutine aufrufen
  var ammo = CreateObject(C_S2,(GetDir(Contained()) ?? 0)*11-5,nil,GetOwner(Contained()));
  ammo->Launch(GetDir(Contained()));
  
  // nachladen
  SetAction("Reload");
  return(1);
}

// Waffe überladen
public func Activate(caller)
{
  overload = !overload;
  if (!overload) { SetAction("Reload");   Message("Überladung abgebrochen",caller); count = 5; }
  if (overload) {  SetAction("Overload"); Message("Waffe überladen",       caller); }
  return(1);
}

// Action-Call
protected func Over_Load()
{
  S42MigrationStep(count, -1, true);
  Message("Explosion in : %d",this(),count);
  if (!count) Explode(20);  
  return(1);
}

/* Eigenschaften */
public func IsWeapon() { return(!overload); }
public func IsWeaponProduct() { return(1); }
