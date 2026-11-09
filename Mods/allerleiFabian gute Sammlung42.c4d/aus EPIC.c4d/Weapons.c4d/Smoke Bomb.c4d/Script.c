/*-- T-Flint --*/

#strict 3

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
 if(!ActIdle()) return(1);
 Sound("Fuse");
 SetAction("Activated");
 // Picture anpassen
 SetPicture(nil, 12 + 64, 64, 64);
 return(1);
}

private func RemoveBomb() {
  SetPicture(nil, 12, 64, 64);
  ChangeDef(COAL);
  return(1);
}                  

protected func SmokeAss() {
  Smoke(nil,nil,RandomX(40,200));
  Smoke(nil,nil,RandomX(80,300));
  if((GetActTime() ?? 0) > 1000) RemoveBomb();
}
                    
public func MagicExtinguish() {
  // kann durch Zauber "Löschen" gelöscht werden
  if(!SEqual(GetAction(), "Activated")) return(nil);
  DoExtinguish();
  return(1);
}

private func DoExtinguish() {
  Sound("Pshshsh");
  RemoveBomb();  
}

/* Produkteigenschaften */

public func IsChemicalProduct() {
  return(1);
}

/* Forschung */

public func GetResearchBase() {
  return(TFLN);
}
