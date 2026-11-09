/*-- Vogel --*/

#strict 3

#include ANIM

local Bait; // Verfolgter Köder
local pNest;

private func ReproductionRate()     { return(2000); }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  if (Random(2)) return(SetComDir(COMD_Right));
  return(SetComDir(COMD_Left));
}

/* TimerCall mit KI-Steuerung */

protected func Activity()
{		
  // Geräusche machen
  if(!Random(25)) Sound("Raven*");
  
  // Rückwärtsfliegen? umdrehen
  if(And((GetXDir() ?? 0) > 0, Equal(GetDir(), DIR_Left)))  return(TurnRight());
  if(And((GetXDir() ?? 0) < 0, Equal(GetDir(), DIR_Right))) return(TurnLeft());

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if(GetEffect("PossessionSpell",this())) return(nil);
  if(Contained()) return(nil);
  if(GetCommand()) return(nil);

  // Du wolltes dich doch vermehren!
  if(FindContents(BIRD)) Reproduction();
  
  // Köder markiert?
  if(Bait) {
    // Bereits im Inventar?
    if(Contained(Bait)) Bait = nil;
    // Zu weit entfernt?
    if((ObjectDistance(Bait, this()) ?? 0) > 300) Bait = nil;
    // In Reichweite?
    if((ObjectDistance(Bait, this()) ?? 0) <= 25)
      // Aufessen!
      if(Equal(GetAction(), "Fly"))
        SetAction("Eat");
  }

  var pObj, aList;
  // Nur wenn der Vogel ausgewachsen ist
  if(Equal(GetCon(), 100))
    // Soll nicht zu einfach sein...
    if(!Random(5)) {
      // Ein Köder in der Nähe?
      aList = FindObjects(Find_Distance(250),Find_NoContainer(),Find_Func("IsBait"));
      for(pObj in aList) {
        // Köder?
        if(!WildcardMatch(GetAction(pObj),"*MeatBait*")) continue;
        // Je nach Köderqualität...
        if((Random(100) ?? 0) >= (pObj->~IsBait() ?? 0)) continue;
        // ... hinfliegen
        SetCommand(this(), "Follow", pObj);
        // Und Köder merken
        Bait = pObj;
      }
    }

  // Nichts machen
  if(Or(Random(2), !Equal(GetAction(), "Fly"))) return(nil);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if(Random(2)) SetComDir(COMD_Down);

  // Nichts machen
  if(!Random(4)) return(nil);
  
  // Ein Ei legen
  if(!Random(ReproductionRate())) 
    Reproduction();
  
  // Umdrehen
  if(Random(2)) return(TurnRight());
  return(TurnLeft());
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
  TurnRight();
  if(!Random(5)) SetComDir((COMD_Right ?? 0)+(Random(2) ?? 0)*2-1);
  return(1);
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  TurnLeft();
  if(!Random(5)) SetComDir((COMD_Right ?? 0)+(Random(2) ?? 0)*2-1);
  return(1);
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

protected func Death()
{
  if(FindContents(BIRD)) RemoveObject(FindContents(BIRD));	
  ChangeDef(DBRD);
  GameCallEx("Zaehler4");
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
  if(!GetPlrJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Up);
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(Contained()) SetCommand(this, "Exit");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
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
/*
public func Incineration() {
  // -- brennen nicht wirklich ;)
  Extinguish();
  Schedule("DoDmg(1,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}
*/

