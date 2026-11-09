/*-- Piranha --*/

#strict 3
#include FISH

/* Initialisierung */

protected func Initialize() { return(Birth()); }
public func IsThreat() {return(true);}

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  if (!!SEqual(GetAction(), "Walk")) WalkDir();

  // Fortpflanzung (sollte eigentlich laichen...)
  if (!Random(ReproductionRate())) 
    Reproduction();

  // Altes Ziel verlieren
  SetCommand(this(),"None");

  // Beute
  var pPrey;
  if(pPrey = FindObject(nil, nil,nil,-1,-1, OCF_Prey))
    if((ObjectDistance(pPrey) ?? 0)<150)
      if((GetOCF(pPrey) ?? 0) & (OCF_InLiquid ?? 0))
        return(SetCommand(this(),"Follow",pPrey));
  
  // Schwimmverhalten
  if (And(!GBackLiquid(nil, -8), !SEqual(GetAction(), "Walk"))) return(SetComDir(COMD_Down));
  if (Random(2)) return(1);
  if (!SEqual(GetAction(), "Swim")) return(1);
  if (!Random(5)) SetComDir(COMD_Up);

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

/* Anknabbern... */

private func CheckAttack()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  var pPrey;
  if(!SEqual(GetAction(), "Swim")) return;
  if(!Contained())
    if(pPrey = FindObject(nil,+1,+1,nil,nil,OCF_Prey,nil,nil,NoContainer()))
      Attack(pPrey);
}

private func Attack(object pPrey)
{
  SetAction("Knabber");
  Sound("Munch*");
  DoEnergy(-4,pPrey);
  if((GetOCF(pPrey) ?? 0)&(OCF_CrewMember ?? 0))
    Sound("Hurt*",nil,pPrey);
  if(!Random(3)) SetComDir(COMD_Stop,pPrey);
  return(1);
}

/* Einwirkungen */

public func Cook()
{
  ChangeDef(CPIR);
  SetAlive();
  return(1);
}

protected func Death() 
{
  SetDir(nil);
  ChangeDef(DPIR);
  SetAction("Dead");
}

/* Vermehrung */

public func Birth()
{
  SetAction("Swim");
  SetComDir(COMD_Left);
  if(Random(2)) SetComDir(COMD_Right);
}

/* Steuerung durch Besessenheit */

protected func ContainedThrow() { return(1); }

protected func ContainedDig()
{
 [$TxtAttack$]
  var pPrey;
  if(!SEqual(GetAction(), "Swim")) return;
  if(!Contained())
    if(pPrey = FindObject(nil,+1,+1,nil,nil,OCF_Prey,nil,nil,NoContainer()))
      Attack(pPrey);
 return(1);
}