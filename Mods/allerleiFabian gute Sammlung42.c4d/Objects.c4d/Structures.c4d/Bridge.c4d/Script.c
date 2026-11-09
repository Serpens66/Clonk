/*-- Brückensegment --*/

#strict 3

local is_locked;	// Ist im Boden verankert?

protected func Initialize() {
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    Lock(1, 1);
  return(1);
}

protected func Entrance() {
  // Im Behälter: lösen
  Release(1);
  return(1);
}

public func ControlDigDouble(caller) {
  [$TxtLock$]
  // Wenn verankert lösen
  if (is_locked) Release();
  // sonst verankern
  else Lock();
  return(1);
}

private func Lock(quiet, dont_descend) {
  SetAction("Locked");
  SetSolidMask(nil, 11, 72, 3);
  is_locked = 1;

  if (!quiet)
    Sound("Connect");

  // Verankern
  if (!dont_descend)
    {
    var x = GetX(this());
    var y = GetY(this());
    
    if(!PathFree(x,y,x,(y ?? 0)+10)) // Solidmask im Weg?
      return 1;
    if (!GBackSolid(nil, 10))
        SetPosition(GetX(), (GetY() ?? 0)+10);
    }
  return(1);
}

private func Release(quiet, dont_ascend) {
  SetAction("Idle");
  SetSolidMask();
  is_locked = nil;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde lösen
  if (!dont_ascend) {
    var ascend_pixels = nil;
    while (And(Stuck(), (S42MigrationStep(ascend_pixels, 1) < 12)))
      SetPosition(GetX(), (GetY() ?? 0)-1);
  }

  return(1);
}

public func IsBridge() { return(1); }

public func IsLocked() { return(is_locked); }

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}
