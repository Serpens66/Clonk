/*-- Schlange --*/

#strict 3
#include ANIM

local Bait; // Verfolgter Köder
public func IsThreat() {return(true);}

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

protected func Activity() 
{
  // Tote Schlangen machen nichts
  if (!GetAlive()) return;

  // Geräusche machen
  if (!Random(15)) Sound("Snake*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  // Nichts machen
  if (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))) return;

  // In einem Haus, nur selten was machen
  if (Contained()) if (Random(3)) return(1);

  // Beißen, wenn geht
  Attack();

  // Köder fressen
  if(Bait)
    if((ObjectDistance(Bait, this()) ?? 0) <= 25)
      if(!!SEqual(GetAction(), "Walk"))
        SetAction("Eat");

  var pBait;
  // Nur wenn die Schlange ausgewachsen ist
  if (Equal(GetCon(), 100))
   // Soll nicht zu einfach sein...
   if (!Random(5))
     // Ein Köder in der Nähe?
     while (pBait = FindObject( nil, -250,-250,500,500, nil,nil,nil,NoContainer(), pBait))
      // Köder?
      if(WildcardMatch(GetAction(pBait), "*MeatBait*"))
       // Je nach Köderqualität...
       if ((Random(100) ?? 0) < (pBait->~IsBait() ?? 0))
        {
         // ...hinschlängeln
         SetCommand(this(), "Follow", pBait);
         // Und Köder merken
         Bait = pBait;
        }

  // Fortpflanzung
  if (!Random(ReproductionRate())) 
   Reproduction();

  // Nichts machen
  if (Random(2)) return(1);

  // Springen
  if (!!SEqual(GetAction(), "Walk"))
   if (!Random(3))
    return(Jump());

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}



/* Kontakt */

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
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

public func TurnLeft() 
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

public func Attack()
{
  // Nur während des Laufens oder Schwimmens kann alle 2 Sekunden einmal gebissen werden
  if (Or(GetEffect("IntAttackDelay", this()), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  
  // Beute suchen
  var pPrey = Contained();
  if (Or(!pPrey, !GetAlive(pPrey))) pPrey = FindObject(nil, -13 + 26 * (GetDir() ?? 0), nil, nil, nil, OCF_Prey, nil, nil, NoContainer());
  if (Or(!pPrey, !GetAlive(pPrey))) return;
  
  // Beißen :D
  Punch(pPrey, 8);
  Sound("Snake*");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return(1);
}

/* Einwirkungen */

public func Cook()
{ 
  SetAlive(nil);
  return(ChangeDef(CSNK)); 
}

/* Vermehrung */

private func ReproductionRate() { return(10000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet


/* Einsammeln */

local fForceEnter;

// Einsammeln erzwingen
public func ForceEnter(object pContainer)
{
  fForceEnter = 1;
  Enter(pContainer);
  fForceEnter = nil;
  return(1);
}

public func RejectEntrance(object pContainer, a,b,c)
{
  // Scriptforciertes Einsammeln
  if(fForceEnter) return(nil);
  // Tote Schlangen sind generell OK
  if(!GetAlive()) return(nil);
  // Durch Regel verboten?
  if (_inherited(pContainer, a,b,c)) return(true);
  // Nur einsammeln lassen, wenn sie nicht springt
  // Aber nur von Clonks - Kochtöpfe dürfen beispielsweise immer
  if(Or(!!SEqual(GetAction(), "Jump"), !!SEqual(GetAction(), "Tumble")))
   if ((GetCategory(pContainer) ?? 0) & (C4D_Living ?? 0))
    return(true);
  // Ansonsten ist einsammeln OK
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (!!SEqual(szCommand, "MoveTo"))
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(nil);
}

protected func ContainedLeft(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnLeft();
 return(1);
}

protected func ContainedRight(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrJumpAndRunControl(caller->GetController()))
  TurnRight();
 return(1);
}

protected func ContainedUp(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (!!SEqual(GetAction(), "Swim"))
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Up);

  return(1);
 }

 Jump();
 return(1);
}

protected func ContainedDown(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if(Contained()) return SetCommand(this, "Exit");
 if (!!SEqual(GetAction(), "Swim"))
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down);
  return(1);
 }

 return(1);
}

/* JumpAndRun Steuerung */

private func ClearDir(bool fX)
{
  fX ??= false;
  if(And(fX, GetXDir()))
  {
    if((GetXDir() ?? 0) > 0) SetXDir(Max((GetXDir() ?? 0) - 2, nil));
    else SetXDir(Min((GetXDir() ?? 0) + 2, nil));
  }
  if(And(!fX, GetYDir()))
  {
    if((GetYDir() ?? 0) > 0) SetYDir(Max((GetYDir() ?? 0) - 2, nil));
    else SetYDir(Min((GetYDir() ?? 0) + 2, nil));
  }
}

public func ContainedUpdate(object self, int comdir)
{
  comdir ??= 0;
  if(!!SEqual(GetAction(), "Swim"))
  {
    SetComDir(comdir);
    ClearScheduleCall(this(), "ClearDir");
    if(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_Up))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetXDir()) ?? 0)+1)/2, true);
    if(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetYDir()) ?? 0)+1)/2, nil);
  }
  else
  {
    if(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_DownRight))) comdir = COMD_Right;
    if(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_Down))) comdir = COMD_Stop;
    if(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_DownLeft))) comdir = COMD_Left;
    SetComDir(comdir);
  }

  return(1);
}

protected func ContainedDig()
{
 [$TxtAttack$]
 Attack();
 return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}

/* Zerlegen */

public func Activate(pClonk) {
  [$TxtEmbowel$|Image=KNFE]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pClonk) {
  // In Bestandteile zerfallen
  if (pClonk) pClonk->Split2Components(this()); else Split2Components(this());
  return(1);
}

/* Köder fressen */

protected func Eating()
{
  // Köder weg
  if(!Bait) return;
  // Köder fressen
  RemoveObject(Bait);
  // Schmeckt gut
  Sound("Snake*");
}


protected func Death()
{
  Sound("Snake1");
  SetAction("Dead");
  GameCallEx("Zaehler3");
  return(1);
}

/*
public func Incineration() {
  // -- brennen nicht wirklich ;)
  Extinguish();
  Schedule("DoDmg(1,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}
*/

