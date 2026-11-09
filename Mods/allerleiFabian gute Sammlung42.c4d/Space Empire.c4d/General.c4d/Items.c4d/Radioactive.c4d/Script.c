/*-- Kiste mit Brennstäben --*/

#strict 3

/* Aufprall: Sound ausgeben */

protected func Hit(...) {
  Sound("RockHit*");
if(!Random(10)) Incinerate();
  return(1);
}

/* Wehe, wenn das Ding mal explodiert... */

protected func Incineration(...) {
  // Explosion mit Verstrahlung
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(100) ?? 0)+50);
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(15); else FakeExplode(15,nil,nil,15);
  return(1);
}

/* Eigenschaften */

public func IsChemicalProduct(...) { return(1);
}
public func GetWasteID(){return YECK;}
