/*-- Schmonk --*/

#strict
#include ANIM

public func IsThreat() {return(true);}

/* Initialisierung */

func IsBulletTarget() { return(1); }

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  // Auftauchen, wenn der Atem ausgeht
  if(InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
    SetComDir(COMD_Up());

  // Nichts machen
  if (Random(2) || (GetAction() ne "Walk" && GetAction() ne "Swim")) return(1);

  // Ein Ei legen
  if (!Random(ReproductionRate())) 
   Reproduction();	
  for (var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_NoContainer(),Find_Not(Find_Owner(-1)),Find_OCF(OCF_Alive),Find_Distance(200)))
   if (GetAction() ne "Attack") 
    if (!GetCommand()) SetCommand(this(),"Follow",obj);
     else if (ObjectDistance(this(),obj)<=10)
     SetAction("Attack");
  // Umdrehen
  if (!Random(10) && !GetActionTarget()) return(TurnRight());
  return(1);
}

/* Kontakte */

protected func ContactLeft()
{
  return(TurnRight());
}

protected func ContactRight()
{
  return(TurnLeft());
}

/* Aktionen */

public func TurnRight()
{
  if (!Random(6)) return(TurnLeft());
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

private func Attack()
{
  for (var obj in FindObjects(Find_Distance(10),Find_OCF(OCF_CrewMember),Find_NoContainer(),Find_OCF(OCF_Alive)))
   if(!Random(5)) Punch(obj, RandomX(3,20));
   else DoDmg(RandomX(1,10), DMG_Melee, obj);
  return(1);
}

/* Einwirkungen */

protected func Death()
{
  Sound("DeathGrowl");
  SetAction("Death");
  GameCallEx("Zaehler19");
  return(1);
}

/* Vermehrung */

private func ReproductionRate() { return(1500); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()   { return(7); }    // Maximale Tieranzahl im Umkreis

private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(SHMK, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()) + ObjectCount(SEGG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , 0));
}

private func SpecialRepr()
{
  Exit(CreateContents(SEGG), 0, 10);
  return(1);
}
