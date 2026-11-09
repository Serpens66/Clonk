/*-- Sprengpfeil --*/

#strict 3

#include ARRW

local Armed;

/* Abfeuern */

public func Launch(object pByObject)
{
  // Der Sprengpfeil wird nur durch korrektes Abfeuern scharf gemacht
  Armed = 1;    
  return(_inherited() );
}

/* Pfeileigenschaften */
public func PackTo() {  return(XARP); }
public func ArrowStrength() { return(2+(Random(2) ?? 0)); }
public func ArrowTumbleRandom() {return(2);}
public func ArrowStopsOnHit() { return(nil); }

/* Aufschlag */

protected func Hit()
{
  if (Armed)
   if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))
    Explode((Random(5) ?? 0) + 10);
   else FakeExplode((Random(5) ?? 0) + 10,nil,nil,(Random(5) ?? 0) + 10);
}

/* Treffer */

private func HitTarget(pTarget)
{
 if(pTarget->~IsArrowTarget(this())) return(Hit());
 
 return(_inherited(pTarget));
}
