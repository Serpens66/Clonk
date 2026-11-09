/*-- Vogel --*/

#strict 3
/*-- Vogel --*/

#strict 3

#include ANIM

local Bait; // Verfolgter Köder
local pNest;

private func ReproductionRate()     { return(2000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet


public func IsPossessible() { return(nil); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  if (Random(2)) return(SetComDir(COMD_Right));
  return(SetComDir(COMD_Left));
}



// Nur im Fliegen
private func SpecialReprodCond()
{	
  if(Contained()) return;
  if(GetCommand()) return;
  return(!!SEqual(GetAction(), "Fly"));
}

private func SpecialRepr()
{
//  if(FindContents(BIRD)) return(1);
	
  // Wenn das Nest nicht mehr am Baum hängt wird es verworfen
  if(pNest) if(!SEqual(GetAction(pNest), "Be")) pNest = nil;
  // oder wenn das Nest geflutet wurde
  if(pNest) if(GBackSemiSolid((GetX(pNest) ?? 0)-(GetX() ?? 0), (GetY(pNest) ?? 0)-(GetY() ?? 0))) pNest = nil;

  // Kein Nest da? Dann schnell eines bauenn
  if(!pNest)
  {
    var pTree = FindTree();	  
    if(pTree)
    {
      var id = GetID(pTree);	    
      SetCommand(this(), "Call", this(), pTree, nil, nil, "BuildNest");
      // Schönes Plätzchen in der Baumkrone suchen
      AddCommand(this(), "MoveTo", nil, (GetX(pTree) ?? 0)+(GetDefOffset(id) ?? 0)+(Random((GetDefWidth(id) ?? 0)-20) ?? 0)+10, (GetY(pTree) ?? 0)+(GetDefOffset(id, 1) ?? 0)+(Random((GetDefHeight(id) ?? 0)-(GetDefFireTop(id) ?? 0)-20) ?? 0)+10);
    }
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    return(1);
  }
  
  // Manchmal im Nest ein Ei legen
  if(And(pNest, !FindObject(BEGG, nil, nil, nil, nil, nil, nil, pNest)))
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
      if((GetCon(obj) ?? 0)<100) continue;
      // nicht unter Wasser
      if(GBackSemiSolid((GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0))) continue;
      // und noch kein Nest haben
      if(FindObject2(Find_ID(NEST), Find_ActionTarget(obj))) continue;
      return(obj);
   }
}

public func BuildNest(pThis, pTree)
{
  if(!pTree) return(nil);
  if((GetX(pTree) ?? 0)<(GetX() ?? 0)) SetDir(nil);
  else SetDir(1);
  pNest = CreateConstruction(NEST, -12+24*(GetDir() ?? 0), +5, -1, 50);
  LocalN("pTree", pNest) = pTree;
  ObjectSetAction(pNest, "Be", pTree);
  SetAction("BuildNest");
  SetComDir(COMD_Stop);
  SetXDir(nil);
  SetYDir(nil);
}

protected func Build()
{
  // Nest bauen	
  if(Or(!pNest, OnFire(pNest))) return(S42MigrationFirst(SetAction("Fly"), Activity()));
  DoCon(10, pNest);
  if((GetCon(pNest) ?? 0)>=100) { SetAction("Fly"); FlyToNest(); }
}

private func FlyToNest()
{
  // Ins Nest fliegen und dort das Brüten anfangen	
  SetCommand(this(), "Call", this(), nil, nil, nil, "StartBreeding");
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
  SetPosition(GetX(pNest), (GetY(pNest) ?? 0)-1, pEgg);
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
  if (Random(2)) SetComDir(COMD_Left);
  else SetComDir(COMD_Right);
  return(1);
}

protected func HitCheck()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  // Einen Ballon oder ein Luftschiff zum attackieren suchen
  return(Attack());
}

public func Attack(object pAttack)
{
  // Nur während des Fliegens kann gebissen werden
  if (!SEqual(GetAction(), "Fly")) return;

  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,nil,40,30, nil, nil,nil, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,nil,60,40, nil, nil,nil, NoContainer());
  if (!pAttack) return;

  // Reinbeißen!
  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
}

protected func Survive()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (Or(Or(Or(InLiquid(), GBackLiquid((GetXDir() ?? 0)*2, (GetYDir() ?? 0)*2)), GBackLiquid((GetXDir() ?? 0)*3, (GetYDir() ?? 0)*3)), FindObject(nil, -20 + (GetXDir() ?? 0) * 2, -20 + (GetYDir() ?? 0) * 2, 40, 40, OCF_OnFire)))
   SetComDir(COMD_Up);
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

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  return(SetComDir(COMD_Down));
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  return(SetComDir(COMD_Up));
}

/* Aktionen */

public func TurnRight()
{
  if (Or(Stuck(), !SEqual(GetAction(), "Fly"))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

public func TurnLeft()
{
  if (Or(Stuck(), !SEqual(GetAction(), "Fly"))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
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
 if (!!SEqual(szCommand, "MoveTo"))
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(nil);
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
    SetComDir(COMD_Up);
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(Contained()) SetCommand(this, "Exit");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down);
  return(1);
}

/* JumpAndRun-Steuerung */

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

protected func ContainedUpdate(object controller, int comdir)
{
  comdir ??= 0;
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(Or(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_Down)), Equal(comdir, COMD_Stop)))
    ScheduleCall(this(), "ClearDir", 1, ((Abs(GetXDir()) ?? 0)+1)/2, true);
  if(Or(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right)), Equal(comdir, COMD_Stop)))
    ScheduleCall(this(), "ClearDir", 1, ((Abs(GetYDir()) ?? 0)+1)/2, nil);

  if(Or(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_Right)), Equal(comdir, COMD_DownRight)))
    SetDir(DIR_Right);
  if(Or(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_Left)), Equal(comdir, COMD_DownLeft)))
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
  if(!Bait) return;
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
  if(And(!pTarget, !GetCommand()))
    pTarget = FindObject(nil, -(SearchRange() ?? 0), -(SearchRange() ?? 0), (SearchRange() ?? 0)*2, (SearchRange() ?? 0)*2, OCF_CrewMember, nil, nil, NoContainer());
  
  // Beute am Maul beißen
  if (Attack()) return;
  
  // Angreifen
  if(pTarget)
  {
    // Wenn Gegner im Haus, tot oder zu weit weg abbrechen	  
    if(Contained(pTarget)) return(S42MigrationFirst(pTarget=nil, SetCommand(this(), "None")));
    if(!GetAlive(pTarget)) return(S42MigrationFirst(pTarget=nil, SetCommand(this(), "None")));
    if((ObjectDistance(pTarget) ?? 0)>(SearchRange() ?? 0)) return(S42MigrationFirst(pTarget=nil, SetCommand(this(), "None")));
    // sonst zum Gegner fliegen
    var iFromSide = +1; if ((GetX(pTarget) ?? 0)>(GetX() ?? 0)) iFromSide = -1;
    SetCommand(this(), "MoveTo", nil, (GetX(pTarget) ?? 0) + 13 * (iFromSide ?? 0), GetY(pTarget));
    if(And((iFromSide ?? 0)>0, GetDir())) TurnLeft();
    if(And((iFromSide ?? 0)<0, !GetDir())) TurnRight();
    return;
  }
  
  // Nichts machen
  if (Or(Random(2), !SEqual(GetAction(), "Fly"))) return(1);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if (Random(2)) SetComDir(COMD_Down);

  // Nicht zu hoch fliegen
  if((GetY() ?? 0)<0) SetComDir(COMD_Down);

  // Nichts machen
  if (!Random(4)) return(1);

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func Attack(object pAttack)
{
  // Während des Fliegens kann alle 2 Sekunden einmal gebissen werden
  if (Or(GetEffect("IntAttackDelay", this()), !SEqual(GetAction(), "Fly"))) return;

  // Beute suchen
  pAttack = FindObject(nil, -14 + 28 * (GetDir() ?? 0), nil, nil, nil, OCF_Prey, nil, nil, NoContainer());
  
  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,nil,40,30, nil, nil,nil, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,nil,60,40, nil, nil,nil, NoContainer());
  if (!pAttack) return;

  if(GetAlive(pAttack))
  {
    // Schaden machen und schleudern	  
    Punch(pAttack);
    if(pAttack) // nach dem punch noch da
    if((GetX(pAttack) ?? 0) < (GetX(this()) ?? 0))
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
