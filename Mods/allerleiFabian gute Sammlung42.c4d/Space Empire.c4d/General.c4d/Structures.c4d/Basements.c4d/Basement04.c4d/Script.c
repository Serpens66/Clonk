/*-- Fundament Größe 20 (Grundfundament) --*/

#strict

/* Das Objekt BS04 stellt alle nötigen Funktionen zur Verfügung */

private BasementID: return(BS04);
private BasementWidth: return(20);
func IsBasement(){return(1);}  

/* Fertigstellung */

protected Initialize:
  if (Not(Equal(GetID(),BasementID()))) return(0);
  SetAction("Consolidate");
  MoveOutClonks();
  return(1);

private Consolidated:
  if (Not(Equal(GetID(),BasementID()))) return(0);
  // Das Fundament ist fertig und abgesunken: in den Hintergrund (fixieren)
  SetCategory(1);
  return(1);  

/* Schaden */

protected Damage:
  if (GetID() != BasementID()) return(0);
  // Schaden auf benutzendes Gebäude leiten
  if (Par(0) > 0) DoDamage(-Par(0));
  return(1);


/* Feststeckende Clonks rauswerfen */

private MoveOutClonks:
  if (Not(Equal(GetID(),BasementID()))) return(0);
  while(SetVar(0,FindObject(0,Div(BasementWidth(),-2),-15,BasementWidth(),20,OCF_CrewMember(),0,0,NoContainer(),Var(0))))
    MoveOutClonk(Var(0));
  return(1);

private MoveOutClonk: // C4Object *pClonk
  if (Not(Equal(GetID(),BasementID()))) return(0);
  while(And( Stuck(Par(0)), Inside(Sub(GetY(Par(0)),GetY()),-13,+5) ))
    ForcePosition(Par(0),GetX(Par(0)),Sum(GetY(Par(0)),-2));  
  return(1);






