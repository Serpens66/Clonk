/*-- Fisch --*/

#strict 3
#include ANIM

local Bait; // Verfolgter Köder

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

private func Activity() 
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  if (!!SEqual(GetAction(), "Walk")) WalkDir();
  
  // Schwimmt gerade zu einem Köder
  if (Bait)
  {
    // Köder taugt nix mehr...
    if (!SEqual(GetAction(Bait), "Bait"))
      Bait = nil;
    // ...ansonsten weiter verfolgen und nicht ablenken lassen
    else
      return(1);
  }
    
  var pBait;
  // Nur wenn der Fisch ausgewachsen ist
  if (Equal(GetCon(), 100))
   // Soll nicht zu einfach sein...
   if (!Random(5))
     // Ein Köder in der Nähe?
     if (pBait = FindObject( nil, -250,-250,500,500, OCF_InLiquid, "Bait"))
       // Je nach Köderqualität...
      if ((Random(100) ?? 0) < (pBait->~IsBait() ?? 0))
      {
         // ...hinschwimmen
         SetCommand(this(), "Follow", pBait);
         // Und Köder merken
         Bait = pBait;
      }
  
  // Fortpflanzung (sollte eigentlich laichen...)
  if (!Random(ReproductionRate())) 
    Reproduction();
  
  // Vor Bedrohungen flüchten
  var found_threat;
  if (And(!Contained(), InLiquid()))
  {
    // Keine Regelobjekte, Luftblasen etc.
    for (var threat in FindObjects(Find_Distance(100),
      Find_Category((C4D_Living ?? 0) | (C4D_Object ?? 0) | (C4D_Vehicle ?? 0) | (C4D_Structure ?? 0)),
      Find_Not(Find_Category((C4D_Background ?? 0) | (C4D_Parallax ?? 0))), Find_Not(Find_ID(FXU1)),
      // Keine toten Clonks
      Find_Or(Find_Not(Find_Func("IsClonk")), Find_OCF(OCF_Alive)),
      Sort_Distance()))
    {
      // Bedrohung bewegt sich nicht?
      if (And(Inside(GetXDir(threat), -2, +2), Inside(GetYDir(threat), -3, +3))) continue;
      // Kontakt zum Boden?
      if (GetContact(threat, -1, 8)) continue;
      // Bedrohung nicht im Wasser oder in einem Container -> Nächstes Objekt
      if (Or(!InLiquid(threat), Contained(threat))) continue;
      // Keine unsichtbaren Objekte
      if (GetVisibility(threat)) continue;
      // Sind ziemlich naiv und schwimmen nicht vor anderen Tieren weg
      if (Or(!GetAlive(threat), threat->~IsClonk())) {
        found_threat = true;
        var xdist = (GetX(threat) ?? 0) - (GetX() ?? 0);
        var ydist = (GetY(threat) ?? 0) - (GetY() ?? 0);
        var axdist = Abs(xdist);
        var aydist = Abs(ydist);
        var xsign = (xdist ?? 0) / (axdist ?? 0);
        var ysign = (ydist ?? 0) / (aydist ?? 0);
        var fleex = (GetX() ?? 0) - (xsign ?? 0) * (1500 / (BoundBy(axdist, 20, 80) ?? 0)); // 20..80 -> 70..15
        var fleey = (GetY() ?? 0) - (ysign ?? 0) * (1000 / (BoundBy(aydist, 20, 80) ?? 0)); // 20..80 -> 50..10
        SetCommand(this(), "MoveTo", nil, fleex, fleey, nil, true);
      }
    }
  }
  
  // Bewegung zum Fischturm (hat hoehere Prioritaet als Gefahren) (Tiefsee)
  var fish_tower = FindObject(FSTW, -250, -250, 500, 500, OCF_Fullcon );
  // nicht das U-Boot steuern wenn darin befindlich :D
  if(!Contained()) 
    // Fischturm gefunden
    if(fish_tower)
    {
      SetCommand(this(), "MoveTo", nil, (GetX(fish_tower) ?? 0) - 150 + (Random(300) ?? 0), (GetY(fish_tower) ?? 0) - 150 + (Random(300) ?? 0) );
      found_threat = true;
    }

  // Wenn keine Bedrohung mehr oder nicht im Wasser aufhören zu fliehen
  if (Or(Or(!InLiquid(), Contained()), (And(!found_threat, !!SEqual(GetCommand(), "MoveTo")))))
    SetCommand(this(), "None");
  
  // Schwimmverhalten
  if (And(!GBackLiquid(nil, -8), !SEqual(GetAction(), "Walk"))) return(SetComDir(COMD_Down));
  if (Random(2)) return(1);
  if (!SEqual(GetAction(), "Swim")) return(1);
  if (!Random(10)) return(SetComDir(COMD_Up));
  if (!Random(10)) return(SetComDir(COMD_Down));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func WalkDir()
{
  SetComDir(COMD_Left);
  if (Random(2)) SetComDir(COMD_Right);
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

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  SetComDir(COMD_Down);
  return(1);
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  if (!SEqual(GetAction(), "Walk")) SetComDir(COMD_Up);
  if (Random(10)) SetComDir(COMD_Right);
  if (Random(10)) SetComDir(COMD_Left);
  return(1);
}

/* Aktionen */

private func TurnRight()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

public func Entrance(container) 
{
  // Damit der Fisch nicht aus U-Booten flieht und so.
  SetCommand(this(), "None");
}


/* Einwirkungen */

public func Activate(object pClonk)
{
  [$TxtEmbowel$|Image=KNFE]
  if (pClonk)
    {
    // Aquaclonks (und damit Hydroclonks) können Fische direkt essen
    if (pClonk->~IsAquaClonk()) return(Eat(pClonk));
    // Andere versuchen zu zerlegen (lebende Fische im Inventar dürften ohnehin selten sein)
    return(ObjectSetAction(pClonk, "Embowel", this()));
    }
  return(1);
}

public func Eat(object pByObject)
{
  pByObject->~Feed(50);
  RemoveObject();
  return(1);
}

public func Cook()
{
  ChangeDef(CFSH);
  SetAlive(nil);
  return(1);
}

protected func Death()
{
  SetDir(nil);
  ChangeDef(DFSH);
  SetAction("Dead");
  return(1);
}

/* Vermehrung */

private func SpecialReprodCond()
{
  return(!!SEqual(GetAction(), "Swim"));
}

public func Birth()
{
  var pEnv;
  if (pEnv=FindObject(CLFS)) pEnv->CLFS::Colorize(this());
  else SetColorDw(RGB(255,255,255));
  SetAction("Swim");
  SetComDir(COMD_Left);
  if(Random(2)) SetComDir(COMD_Right);
}

public func RejectEntrance(pContainer)
{
  // Aquaclonks/Hydroclonks können ungeachtet der Regel immer einsammeln
  // (In Tiefsee sind Fische die einzige Möglichkeit, Energie aufzuladen!)
  if (pContainer->~IsAquaClonk()) return;
  // Fischtürme sollten auch immer funktionieren
  if (pContainer->~IsFishTower()) return;
  // ANIM-Definition (Einsammelbare Tiere)
  return(_inherited(pContainer, ...));
}

/* Steuerung durch Besessenheit */

protected func RejectCollect(c4ID, pObject)
{
 var iEffectNumber, pSorcerer;
 if (iEffectNumber = GetEffect("PossessionSpell", this()))
  if (pSorcerer = EffectVar(nil, this(), iEffectNumber))
   if (!GetEffect("IntCollectionDelay", this()))
    if (!pSorcerer->Contents())
     if (!((pObject->GetOCF() ?? 0) & (OCF_Living ?? 0)))
      Collect(pObject, pSorcerer);
 return(1);
}

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
  SetCommand(this(),"None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnLeft();
  return(1);
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnRight();
  return(1);
}

protected func ContainedUp(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Up);
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(Contained()) return SetCommand(this, "Exit");
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

public func ContainedUpdate(object self, int comdir)
{
  comdir ??= 0;
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_Up))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetXDir()) ?? 0)+1)/2, true);
  if(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetYDir()) ?? 0)+1)/2, nil);

  return(1);
}

protected func ContainedThrow()
{
 [$TxtDrop$]
 var iEffectNumber, pSorcerer;
 if (iEffectNumber = GetEffect("PossessionSpell", this()))
  if (pSorcerer = EffectVar(nil, this(), iEffectNumber))
  {
   if (pSorcerer->Contents()) pSorcerer->Contents()->Exit(nil,nil,6);
   AddEffect("IntCollectionDelay", this(), 1, 70);
  }
 return(1);
}

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}

/* Aufwertungszauberkombo: Mit Fisch wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) { return(ACLK); }


/* Zerlegen nach Clonktyp */

protected func GetCustomComponents(object pClonk)
  {
  if (pClonk)
    {
    // Jedem seine Extrawürste
    if (pClonk->~IsTrapper())return (West()); 
    if (pClonk->~IsIndian()) return (West());
    if (pClonk->~IsInuk()) return([MEAT, FAT1, FSHB]);
    //if (pClonk->~IsJungleClonk()) return([MEAT, FAT1, FSHB]); // Jungelclonk kann (noch) nicht zerlegen
    }
  }

protected func West()
{
  CastObjects(FSHM,2,3);
  CastObjects(FSHB,1,2);
}

/* Kranke Tiere (Arktis) */

public func Sick() { ChangeDef(QFSH);  Birth(); }
