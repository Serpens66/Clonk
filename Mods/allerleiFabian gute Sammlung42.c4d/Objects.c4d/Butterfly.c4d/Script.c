/*-- Schmetterling --*/

#strict

#include ANIM

// Bevölkerungskontrolle
private func ReproductionAreaSize() { return(800); }   // Größe des Bereichs, in dem neue Tiere dieser Art entstehen können
private func ReproductionRate()     { return(4000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()       { return(10); }    // Maximale Tieranzahl im Umkreis

func IsBulletTarget() {return(1);}

protected Initialize:
  SetAction("Fly");
  MoveToTarget();
  return(1);
  
/* TimerCall */  
 
private Activity:
  if (!GetAlive()) return();
  // fortpflanzen
  if(!Random(500)) Reproduction();
  // Unterwasser
  if (InLiquid()) return(SetComDir(COMD_Up()));
  // Sitzt: warten
  if (SEqual(GetAction(),"Sit")) return(1);
  // Neues Ziel
  if (Or( Not(GetCommand()), Not(Random(5)) )) MoveToTarget();
  return(1);  
  

// Tiere Zählen
private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(GetID(this()), ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()));
}

/* Vermehrung */

public func Reproduction(bool fRepr)
{
  // Nicht mehr am Leben
  if (!GetAlive()) return(0);
  // Noch nicht ausgewachsen
  if (GetCon() < 100) return(0);
  // Schon zu viele Tiere dieser Art
  if(!FindObject(REPR)) { if (CountMe() >= MaxAnimalCount()) return(0); }
  else if(ObjectCount(GetID())+1 >= GetComponent(GetID(), 0, FindObject(REPR))) return(0);
  // Reproduktion
    // Normale Reproduktion
    var pChild = CreateConstruction(GetID(this()), 0, 0, -1, 40);
    pChild->~Birth();
  // Erfolg
  return(1);
}

/* Bewegung */

private Flying:
  // Richtung anpassen
  if (GreaterThan(GetXDir())) SetDir(DIR_Right());
  if (LessThan(GetXDir())) SetDir(DIR_Left());  
  // Aktion ändern
  if (Not(Random(3))) SetAction("Flutter");
  return(1);
  
private Fluttering:
  // Richtung anpassen
  if (GreaterThan(GetXDir())) SetDir(DIR_Right());
  if (LessThan(GetXDir())) SetDir(DIR_Left());  
  // Aktion ändern
  if (Not(Random(7))) SetAction("Fly");
  return(1);
  
  
/* Kontakt */  
  
protected ContactBottom:
  SetCommand(this(),"None");
  SetComDir(COMD_Up());
  return(1);  
  
SitDown:
  SetXDir(0);
  SetYDir(0);
  SetComDir(COMD_Stop());
  SetAction("Sit");
  SetCommand(0,"None");
  return(1);  
  
TakeOff:
  SetComDir(COMD_Up());
  return(1);  
  
private MoveToTarget:
  SetVar(0,Random(LandscapeWidth()));
  SetVar(1,Sum(Random(Sum(GetHorizonHeight(Var(0)),-60)),+30));
  SetCommand(this(),"MoveTo",0,Var(0),Var(1));
  return(1);
  
private GetHorizonHeight: // int iX
  while (And( LessThan(Var(0),LandscapeHeight()), Not(GBackSemiSolid(Par(0),Var(0))) ))
    SetVar(0,Sum(Var(0),+10));
  return(Var(0));  