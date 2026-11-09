/*-- T-Flint --*/

#strict

protected func Hit() {
  Sound("RockHit*");
  if(!ActIdle()) return(1);
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return(1);
}

public func Activate()
{
 if(!ActIdle()) return(1);
 Sound("Fuse");
 SetAction("Activated");
 // Picture anpassen
 SetPicture(0, 12 + 64, 64, 64);
 return(1);
}

private func RemoveBomb() {
  SetPicture(0, 12, 64, 64);
  ChangeDef(COAL);
  return(1);
}                  

protected func SmokeAss() {
  Smoke(0,0,RandomX(40,200));
  Smoke(0,0,RandomX(80,300));
  if(GetActTime() > 1000) RemoveBomb();
}
                    
public func MagicExtinguish() {
  // kann durch Zauber "Löschen" gelöscht werden
  if(GetAction() ne "Activated") return(0);
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
