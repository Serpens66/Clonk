/*-- T-Flint --*/

#strict 3

public func ExplodeSize() {
  return(20);
}

protected func Hit() {
  Sound("RockHit*");
  if(!ActIdle()) return(1);
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(nil, 12 + 64, 64, 64);
  return(1);
}

public func Activate()
{
 [$TxtFuse$]
 if(!ActIdle()) return(1);
 Sound("Fuse");
 SetAction("Activated");
 // Picture anpassen
 SetPicture(nil, 12 + 64, 64, 64);
 return(1);
}

private func BlowUp() {
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))
  Explode(ExplodeSize());
 else FakeExplode(ExplodeSize(),nil,nil,ExplodeSize());
  return(1);
}                   
                    
public func MagicExtinguish() {
  // kann durch Zauber "Löschen" gelöscht werden
  if(!SEqual(GetAction(), "Activated")) return(nil);
  DoExtinguish();
  return(1);
}

private func DoExtinguish() {
  Sound("Pshshsh");
  SetAction("Idle");  
  // Picture zurücksetzen
  SetPicture(nil, 12, 64, 64);
}

/* Produkteigenschaften */

public func IsChemicalProduct() {
  return(1);
}

/* Forschung */

public func GetResearchBase() {
  return(FLNT);
}
