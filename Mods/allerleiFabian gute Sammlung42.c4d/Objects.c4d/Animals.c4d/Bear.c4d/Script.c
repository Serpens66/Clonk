/*-- B‰r --*/

#strict 3
#include ANIM

local iRun;
local iAggressiv;

public func IsPossessible() { return(1); }
public func IsThreat() {return(true);}

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity()
{
  var pEnemy;
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  // Nichts machen
  if (Or(Random(2), (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Run")), !SEqual(GetAction(), "Swim"))))) return(1);

  // Ein Ei legen
//  if (!Random(ReproductionRate())) 
//   Reproduction();	

  // Feind in Sicht: abhauen
  if(pEnemy=EnemyNearby())
  {
//    if(GetEnergy()>60)  return(RunAway(pEnemy));
//    else   return(RunAttack(pEnemy));
  }
  
  // Springen
  if (Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Run")))
   if (!Random(3)) return(DoJump());

  if (!Random(3)) if(!!SEqual(GetAction(), "Walk")) return(SetAction("Run")); else return(S42MigrationFirst(iRun=nil, SetAction("Walk")));

  if (!Random(3)) return(SetAction("Attack"));

  // Umsehen
//  if (GetAction() eq "Walk")
//   if (!Random(8)) return(SetAction("HeadDown"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func EnemyNearby() {
  var pEnemy=FindObject(nil,nil,nil,-1,-1, OCF_CrewMember);
  var iDist=ObjectDistance(pEnemy);
  var pContainer=Contained(pEnemy);
  if(pContainer) if(Equal(GetID(pContainer), GetID())) return;
  if(pEnemy)
    if((iDist ?? 0)<350)
      if(Or(!Contained(pEnemy), (iDist ?? 0)<150))
        if(Or(PathFree(GetX(),GetY(),GetX(pEnemy),GetY(pEnemy)), (iDist ?? 0)<100) )
          return(pEnemy);
}

/* Verhaltenssteuerung */
  
private func RunAway(pEnemy) { // C4Object *pEnemy
  // Nahrungssuche einstellen
  if (!!SEqual(GetCommand(), "Follow")) SetCommand(this(),"None");
  // In die entgegengesetzte Richtung
  if((GetX(pEnemy) ?? 0)>(GetX() ?? 0)) TurnLeft();
  if((GetX(pEnemy) ?? 0)<(GetX() ?? 0)) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  AddCommand(this(),"MoveTo",nil,(GetX() ?? 0)+100*((GetDir() ?? 0)*2-1),GetY(),nil,20);
  // Rennen
//  SetAction("Run");
}

private func RunAttack(pPrey) { // C4Object *pEnemy
  if(Contained(pPrey)) return;
  // Nahrungssuche einstellen
  if (!!SEqual(GetCommand(), "Follow")) SetCommand(this(),"None");
  // In die passende Richtung
  if((GetX(pPrey) ?? 0)<(GetX() ?? 0)) TurnLeft();
  if((GetX(pPrey) ?? 0)>(GetX() ?? 0)) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  // Immer von hinten mit einem Abstand angreifen
  var iFromSide = +1; if ((GetX(pPrey) ?? 0)>(GetX() ?? 0)) iFromSide = -1;
  SetCommand(this(), "MoveTo", nil, (GetX(pPrey) ?? 0) + 21 * (iFromSide ?? 0), GetY(pPrey));
  // Rennen
  if(!GetEffect("Attack", this())) AddEffect("Attack", this(), 1, 10, this());
  iRun=1;  
}

func FxAttackTimer()
{
  if(!Random(10)) return(S42MigrationFirst(-1, SetCommand(this(), "None")));
  var pEnemy = EnemyNearby();
  if(Attack()) return;
  // Feind in Sicht: abhauen
  if(!Contained(pEnemy)) return(RunAttack(pEnemy));
  else return(S42MigrationFirst(-1, SetCommand(this(), "None")));
}

private func Attack()
{
  // W‰hrend des Schwimmens kann alle 2 Sekunden einmal gebissen werden
  if (Or(GetEffect("IntAttackDelay", this()), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Run"))))) return;
  
  // Beute suchen
  var pPrey = FindObject(nil, -20 + 40 * (GetDir() ?? 0), nil, -1, -1, OCF_Prey, nil, nil, NoContainer());
  if((Abs((GetX() ?? 0)-20 + 40 * (GetDir() ?? 0)-(GetX(pPrey) ?? 0)) ?? 0)>10) return;
  if((Abs((GetY() ?? 0)-(GetY(pPrey) ?? 0)) ?? 0)>10) return;
  if (Or(!pPrey, !GetAlive(pPrey))) return;
  
  // Beiﬂen :D
  Punch(pPrey, 8);
  Fling(pPrey, -1 + 2*(GetDir() ?? 0), -2);
  SetAction("Attack");
  if (!Random(5)) Sound("BearAttack");
  Sound("Munch1");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return(1);
}

private func Walking()
{
  // Vorherige Gangart wieder aufnehmen (nach Sprung oder Wendung)
  if (iRun) 
    { SetAction("Run"); return; }
  // Geschwindigkeit und Ger‰usch
  SetPhysical("Walk",55000,2);
  // Beute am Maul beiﬂen
  if (Attack()) return;
  return(1);
}
  
private func Running()
{
  iRun=1;
  // Geschwindigkeit und Ger‰usch
  SetPhysical("Walk", 90000, 2);
  // Beute am Maul beiﬂen
  if (Attack()) return;
  return(1);     
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  return(TurnRight());
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  return(TurnLeft());
}

/* Aktionen */

public func TurnRight()
{
  if (Or(Stuck(), (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Run")), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

public func TurnLeft()
{
  if (Or(Stuck(), (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Run")), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

public func DoJump()
{
  if (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Run"))) return;
  if (Random(2)) Sound("Growl*");
  Jump();
}

/* Einwirkungen */

protected func Death()
{
  Sound("DeathGrowl");
//  SetDir(DIR_Left());
  ChangeDef(DBER);
  SetAction("Dead");
   GameCallEx("Zaehler15");
  return(1);
}

/* Vermehrung */
/*
private func ReproductionRate() { return(2000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()   { return(4); }    // Maximale Tieranzahl im Umkreis


private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(GetID(), ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()) + ObjectCount(MEGG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , 0));
}*/

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (!!SEqual(szCommand, "MoveTo"))
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(nil);
}

protected func ContainedLeft()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 TurnLeft();
 return(1);
}

protected func ContainedRight()
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 TurnRight();
 return(1);
}

protected func ContainedUp()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (!!SEqual(GetAction(), "Swim"))
  return(SetComDir(COMD_Up));

 DoJump();
 return(1);
}

protected func ContainedDown()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (!!SEqual(GetAction(), "Swim"))
  return(SetComDir(COMD_Down));

 if (!!SEqual(GetAction(), "Walk"))
  SetAction("HeadDown");
  
 return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}
