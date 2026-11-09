/*-- Kiste mit Brennstäben --*/

#strict

/* Aufprall: Sound ausgeben */

protected Hit:
  Sound("RockHit*");
if(!Random(10)) Incinerate();
  return(1);

/* Wehe, wenn das Ding mal explodiert... */

protected Incineration:
  // Explosion mit Verstrahlung
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), Random(100)+50);
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(15); else FakeExplode(15,0,0,15);
  return(1);

/* Eigenschaften */

public IsChemicalProduct: return(1);
public func GetWasteID(){return YECK;}
