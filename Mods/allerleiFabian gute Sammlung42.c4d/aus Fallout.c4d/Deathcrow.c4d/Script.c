/*-- Vogel --*/

#strict
/*-- Vogel --*/

#strict

#include ANIM

local Bait; // Verfolgter Köder
local pNest;

private func ReproductionRate()     { return(2000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet


public func IsPossessible() { return(0); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  if (Random(2)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));
}



// Nur im Fliegen
private func SpecialReprodCond()
{	
  if(Contained()) return();
  if(GetCommand()) return();
  return(GetAction()S="Fly");
}

private func SpecialRepr()
{
//  if(FindContents(BIRD)) return(1);
	
  // Wenn das Nest nicht mehr am Baum hängt wird es verworfen
  if(pNest) if(GetAction(pNest) ne "Be") pNest = 0;
  // oder wenn das Nest geflutet wurde
  if(pNest) if(GBackSemiSolid(GetX(pNest)-GetX(), GetY(pNest)-GetY())) pNest = 0;

  // Kein Nest da? Dann schnell eines bauenn
  if(!pNest)
  {
    var pTree = FindTree();	  
    if(pTree)
    {
      var id = GetID(pTree);	    
      SetCommand(this(), "Call", this(), pTree, 0, 0, "BuildNest");
      // Schönes Plätzchen in der Baumkrone suchen
      AddCommand(this(), "MoveTo", 0, GetX(pTree)+GetDefOffset(id)+Random(GetDefWidth(id)-20)+10, GetY(pTree)+GetDefOffset(id, 1)+Random(GetDefHeight(id)-GetDefFireTop(id)-20)+10);
    }
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    return(1);
  }
  
  // Manchmal im Nest ein Ei legen
  if(pNest && !FindObject(BEGG, 0, 0, 0, 0, 0, 0, pNest))
  {
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    FlyToNest();	  
  }
  return(1);
}

private func FindTree()
{
   var obj, pTree;
   // Suche einen Baum, der nicht zu weit weg ist, nicht brennt und nicht gefällt
   for(obj in FindObjects(Find_Func("IsTree"), Find_Distance(500), 
              Find_Not(Find_OCF(OCF_OnFire)), Find_Func("IsStanding")))
   {
      // Außerdem sollte er ausgewachesen sein
      if(GetCon(obj)<100) continue;
      // nicht unter Wasser
      if(GBackSemiSolid(GetX(obj)-GetX(), GetY(obj)-GetY())) continue;
      // und noch kein Nest haben
      if(FindObject2(Find_ID(NEST), Find_ActionTarget(obj))) continue;
      return(obj);
   }
}

public func BuildNest(pThis, pTree)
{
  if(!pTree) return(0);
  if(GetX(pTree)<GetX()) SetDir(0);
  else SetDir(1);
  pNest = CreateConstruction(NEST, -12+24*GetDir(), +5, -1, 50);
  LocalN("pTree", pNest) = pTree;
  ObjectSetAction(pNest, "Be", pTree);
  SetAction("BuildNest");
  SetComDir(COMD_Stop);
  SetXDir(0);
  SetYDir(0);
}

protected func Build()
{
  // Nest bauen	
  if(!pNest || OnFire(pNest)) return(SetAction("Fly"), Activity());	
  DoCon(10, pNest);
  if(GetCon(pNest)>=100) { SetAction("Fly"); FlyToNest(); }
}

private func FlyToNest()
{
  // Ins Nest fliegen und dort das Brüten anfangen	
  SetCommand(this(), "Call", this(), 0, 0, 0, "StartBreeding");
//  AddCommand(this(), "Enter", pNest);
  AddCommand(this(), "MoveTo", pNest); 
}

protected func StartBreeding()
{
  ForceEnter(pNest);
  // Nest benachrichtigen	
  ObjectSetAction(pNest, "Bird");	
}

protected func LayEgg()
{
  // Ein Ei legen
  var pEgg = CreateObject(BEGG);
  pEgg->SetAction("Nest", pNest);
  SetPosition(GetX(pNest), GetY(pNest)-1, pEgg);
  if(FindContents(BIRD))
    Enter(pEgg, FindContents(BIRD));
  Exit();
  SetAction("Fly");
  Activity();
}

/* Geburt */

public func Birth()
{
  SetAction("Fly");
  if (Random(2)) SetComDir(COMD_Left());
  else SetComDir(COMD_Right());
  return(1);
}

protected func HitCheck()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Einen Ballon oder ein Luftschiff zum attackieren suchen
  return(Attack());
}

public func Attack(object pAttack)
{
  // Nur während des Fliegens kann gebissen werden
  if (GetAction() ne "Fly") return();

  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
  if (!pAttack) return();

  // Reinbeißen!
  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
}

protected func Survive()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (InLiquid() || GBackLiquid(GetXDir()*2, GetYDir()*2) || GBackLiquid(GetXDir()*3, GetYDir()*3) || FindObject(0, -20 + GetXDir() * 2, -20 + GetYDir() * 2, 40, 40, OCF_OnFire()))
   SetComDir(COMD_Up());
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(TurnRight());
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(TurnLeft());
}

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(SetComDir(COMD_Down()));
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  return(SetComDir(COMD_Up()));
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || GetAction() ne "Fly") return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

/* Einwirkungen */

protected func CatchBlow(iLevel, pObject)
{
  if (pObject->~IsArrow()) SetAction("Tumble");
  if (!Random(3)) Sound("RavenHurt");
  return(1);
}


/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnLeft();
  return(1);
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController())) 
    TurnRight();
  return(1);
}

protected func ContainedUp(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Up());
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(Contained()) SetCommand(this, "Exit");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down());
  return(1);
}

/* JumpAndRun-Steuerung */

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

protected func ContainedUpdate(object controller, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(comdir == COMD_Up || comdir == COMD_Down || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);

  if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    SetDir(DIR_Right);
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    SetDir(DIR_Left);

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

/* Köder fressen */

protected func Eating()
{
  // Köder weg
  if(!Bait) return();
  // Köder fressen
  RemoveObject(Bait);
  // Schmeckt gut
  Sound("Raven*");
}



public func SearchRange() { return(300+5*5); }

local pTarget;

/* TimerCall mit KI-Steuerung */

protected func Activity()
{

  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Gegner suchen
  if(!pTarget && !GetCommand())
    pTarget = FindObject(0, -SearchRange(), -SearchRange(), SearchRange()*2, SearchRange()*2, OCF_CrewMember, 0, 0, NoContainer());
  
  // Beute am Maul beißen
  if (Attack()) return();
  
  // Angreifen
  if(pTarget)
  {
    // Wenn Gegner im Haus, tot oder zu weit weg abbrechen	  
    if(Contained(pTarget)) return(pTarget=0, SetCommand(this(), "None"));	 
    if(!GetAlive(pTarget)) return(pTarget=0, SetCommand(this(), "None"));
    if(ObjectDistance(pTarget)>SearchRange()) return(pTarget=0, SetCommand(this(), "None"));
    // sonst zum Gegner fliegen
    var iFromSide = +1; if (GetX(pTarget)>GetX()) iFromSide = -1;
    SetCommand(this(), "MoveTo", 0, GetX(pTarget) + 13 * iFromSide, GetY(pTarget));
    if(iFromSide>0 && GetDir()) TurnLeft();
    if(iFromSide<0 && !GetDir()) TurnRight();
    return();
  }
  
  // Nichts machen
  if (Random(2) || GetAction() ne "Fly") return(1);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up());
  if (Random(2)) SetComDir(COMD_Down());

  // Nicht zu hoch fliegen
  if(GetY()<0) SetComDir(COMD_Down());

  // Nichts machen
  if (!Random(4)) return(1);

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func Attack(object pAttack)
{
  // Während des Fliegens kann alle 2 Sekunden einmal gebissen werden
  if (GetEffect("IntAttackDelay", this()) || GetAction() ne "Fly") return();

  // Beute suchen
  pAttack = FindObject(0, -14 + 28 * GetDir(), 0, 0, 0, OCF_Prey(), 0, 0, NoContainer());
  
  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
  if (!pAttack) return();

  if(GetAlive(pAttack))
  {
    // Schaden machen und schleudern	  
    Punch(pAttack);
    if(pAttack) // nach dem punch noch da
    if(GetX(pAttack) < GetX(this()))
      Fling(pAttack, -1, -1);
    else
      Fling(pAttack, 1, -1);
  }
  
  // Reinbeißen!
  if(pAttack) pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
  AddEffect("IntAttackDelay", this(), 1, 70);
  return(1);
}

protected func Death()
{  
  GameCallEx("Zaehler11");
  Sound("RavenHurt");
  ChangeDef(DBBR);
  return(1);
}
