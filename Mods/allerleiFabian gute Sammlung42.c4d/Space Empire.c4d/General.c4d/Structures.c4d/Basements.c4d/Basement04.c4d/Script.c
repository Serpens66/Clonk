/*-- Fundament Größe 20 (Grundfundament) --*/

#strict 3

/* Das Objekt BS04 stellt alle nötigen Funktionen zur Verfügung */

private func BasementID(...) { return(BS04);
}
private func BasementWidth(...) { return(20);
}
func IsBasement(){return(1);}  

/* Fertigstellung */

protected func Initialize(...) {
  if (Not(Equal(GetID(),BasementID()))) return(nil);
  SetAction("Consolidate");
  MoveOutClonks();
  return(1);
}

private func Consolidated(...) {
  if (Not(Equal(GetID(),BasementID()))) return(nil);
  // Das Fundament ist fertig und abgesunken: in den Hintergrund (fixieren)
  SetCategory(1);
  return(1);  
}

/* Schaden */

protected func Damage(...) {
  if (!Equal(GetID(), BasementID())) return(nil);
  // Schaden auf benutzendes Gebäude leiten
  if ((Par(nil) ?? 0) > 0) DoDamage(-(Par(nil) ?? 0));
  return(1);
}


/* Feststeckende Clonks rauswerfen */

private func MoveOutClonks(...) {
  if (Not(Equal(GetID(),BasementID()))) return(nil);
  while(SetVar(nil,FindObject(nil,Div(BasementWidth(),-2),-15,BasementWidth(),20,OCF_CrewMember,nil,nil,NoContainer(),Var(nil))))
    MoveOutClonk(Var(nil));
  return(1);
}

private func MoveOutClonk(...) { // C4Object *pClonk
  if (Not(Equal(GetID(),BasementID()))) return(nil);
  while(And( Stuck(Par(nil)), Inside(Sub(GetY(Par(nil)),GetY()),-13,+5) ))
    ForcePosition(Par(nil),GetX(Par(nil)),Sum(GetY(Par(nil)),-2));
  return(1);
}






