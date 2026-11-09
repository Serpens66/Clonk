/*-- Schmetterling --*/

#strict 3

#include ANIM

// Bevölkerungskontrolle
private func ReproductionAreaSize() { return(800); }   // Größe des Bereichs, in dem neue Tiere dieser Art entstehen können
private func ReproductionRate()     { return(4000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()       { return(10); }    // Maximale Tieranzahl im Umkreis

func IsBulletTarget() {return(1);}

protected func Initialize(...) {
  SetAction("Fly");
  MoveToTarget();
  return(1);
}
  
/* TimerCall */  
 
private func Activity(...) {
  if (!GetAlive()) return;
  // fortpflanzen
  if(!Random(500)) Reproduction();
  // Unterwasser
  if (InLiquid()) return(SetComDir(COMD_Up));
  // Sitzt: warten
  if (SEqual(GetAction(),"Sit")) return(1);
  // Neues Ziel
  if (Or( Not(GetCommand()), Not(Random(5)) )) MoveToTarget();
  return(1);  
}
  

// Tiere Zählen
private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = (ReprodSize ?? 0)  / -2;
  return(ObjectCount(GetID(this()), ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive));
}

/* Vermehrung */

public func Reproduction(bool fRepr)
{
  // Nicht mehr am Leben
  fRepr ??= false;
  if (!GetAlive()) return(nil);
  // Noch nicht ausgewachsen
  if ((GetCon() ?? 0) < 100) return(nil);
  // Schon zu viele Tiere dieser Art
  if(!FindObject(REPR)) { if ((CountMe() ?? 0) >= (MaxAnimalCount() ?? 0)) return(nil); }
  else if((ObjectCount(GetID()) ?? 0)+1 >= (GetComponent(GetID(), nil, FindObject(REPR)) ?? 0)) return(nil);
  // Reproduktion
    // Normale Reproduktion
    var pChild = CreateConstruction(GetID(this()), nil, nil, -1, 40);
    pChild->~Birth();
  // Erfolg
  return(1);
}

/* Bewegung */

private func Flying(...) {
  // Richtung anpassen
  if (GreaterThan(GetXDir())) SetDir(DIR_Right);
  if (LessThan(GetXDir())) SetDir(DIR_Left);
  // Aktion ändern
  if (Not(Random(3))) SetAction("Flutter");
  return(1);
}
  
private func Fluttering(...) {
  // Richtung anpassen
  if (GreaterThan(GetXDir())) SetDir(DIR_Right);
  if (LessThan(GetXDir())) SetDir(DIR_Left);
  // Aktion ändern
  if (Not(Random(7))) SetAction("Fly");
  return(1);
}
  
  
/* Kontakt */  
  
protected func ContactBottom(...) {
  SetCommand(this(),"None");
  SetComDir(COMD_Up);
  return(1);  
}
  
func SitDown(...) {
  SetXDir(nil);
  SetYDir(nil);
  SetComDir(COMD_Stop);
  SetAction("Sit");
  SetCommand(nil,"None");
  return(1);  
}
  
func TakeOff(...) {
  SetComDir(COMD_Up);
  return(1);  
}
  
private func MoveToTarget(...) {
  SetVar(nil,Random(LandscapeWidth()));
  SetVar(1,Sum(Random(Sum(GetHorizonHeight(Var(nil)),-60)),+30));
  SetCommand(this(),"MoveTo",nil,Var(nil),Var(1));
  return(1);
}
  
private func GetHorizonHeight(...) { // int iX
  while (And( LessThan(Var(nil),LandscapeHeight()), Not(GBackSemiSolid(Par(nil),Var(nil))) ))
    SetVar(nil,Sum(Var(nil),+10));
  return(Var(nil));
}
