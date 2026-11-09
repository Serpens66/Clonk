/*-- Wipf --*/

#strict 3
#include ANIM

local Bait; // Verfolgter Köder

local lorry_wipf; // Speichert das zugehörige Lorenwipf-Objekt

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Geräusche machen
  if(!Random(15)) Sound("Snuff*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if(GetEffect("PossessionSpell", this())) return(nil);

  // Köder markiert?
  if(Bait) {
    // Bereits im Inventar?
    if(Contained(Bait)) Bait = nil;
    // Zu weit entfernt?
    if((ObjectDistance(Bait, this()) ?? 0) > 300) Bait = nil;
    // In Reichweite?
    if((ObjectDistance(Bait, this()) ?? 0) <= 15)
      // Aufessen!
      if(Equal(GetAction(), "Walk")) {
        SetAction("Eat");
        SetXDir();
      }
  }

  var pObj, aList;
  // Nur wenn der Wipf ausgewachsen ist
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
        // ... hinrennen
        SetCommand(this(), "Follow", pObj);
        // Und Köder merken
        Bait = pObj;
      }
    }

  // Graben, aber keine Richtung? Dann wieder laufen
  if(And(!!SEqual(GetAction(), "Dig"), Equal(GetComDir(), COMD_None)))
    SetAction("Walk");

  // Die folgenden Aktionen nur im Freien
  if(Contained()) return(nil);

  // Die folgenden Aktionen nur im Gehen oder Schwimmen
  if(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))) return(nil);

  // Auftauchen, wenn der Atem ausgeht
  if(And(And(InLiquid(), (GetBreath() ?? 0) <= 25), !Equal(GetComDir(), COMD_Up)))
    SetComDir(COMD_Up);

  // Brennenden Objekten ausweichen
  if(pObj = FindObject(nil,-(!GetDir() ?? 0)*80,-30,80,35,OCF_OnFire)) {
    if(Or(!Random(5), !SpaceToFlee(pObj))) {
      if(GetDir()) SetComDir(COMD_Right);
      else SetComDir(COMD_Left);
      Jump();
    }
    else {
      if(Equal(GetComDir(), COMD_Left)) SetComDir(COMD_Right);
      else SetComDir(COMD_Left);
    }
  }

  // Nichts machen
  if(Random(2)) return(nil);

  // Freigraben, falls er feststeckt
  if(Stuck()) return(DigFree());

  // Fortpflanzung
  if(!Random(ReproductionRate())) 
    Reproduction();

  if(!!SEqual(GetAction(), "Walk")) {
    // Springen
    if(!Random(3)) return(Jump());
    // Hinsetzen
    if(!Random(3)) return(SetAction("Sit"));
    // Graben
    if(!Random(25))
      // "In der Prärie wird nicht gegraben" - nicht buddeln bei aktiviertem Westernpack (arg!)
      if(!GetName(nil, COWB))
        return(StartDig());
  }

  // Umdrehen
  if(Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func SpaceToFlee(pFromObj)
{
 var x=-100;
 if ((GetX() ?? 0) > (GetX(pFromObj) ?? 0)) x=100;

 // Am Rand der Landschaft eingesperrt
 if (!Inside((x ?? 0)+(GetX() ?? 0), nil, LandscapeWidth())) return;

 // Am Rand einer Höhle eingesperrt
 if (!PathFree(GetX(),GetY(),(GetX() ?? 0)+(x ?? 0),GetY())) return;

 return(1);
}

private func StartDig()
{
  SetAction("Dig");
  SetComDir(2 + (Random(7) ?? 0));
  if (Equal(GetComDir(), COMD_Down)) SetComDir(COMD_Stop);
  return(1);
}

private func DigSound()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;

  if (!Random(10)) StartDig();
  return(1);
}

private func Sitting()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  // Weiterlaufen
  if (!Random(10)) SetAction("Walk");
  return(1);
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
  if (Or(Stuck(), (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim")), !SEqual(GetAction(), "Sit"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  if (!!SEqual(GetAction(), "Sit")) SetAction("Walk");
  return(1);
}

public func TurnLeft()
{
  if (Or(Stuck(), (And(And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim")), !SEqual(GetAction(), "Sit"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  if (!!SEqual(GetAction(), "Sit")) SetAction("Walk");
  return(1);
}

private func DigFree()
{
  SetAction("DigFree");
  SetComDir(3+(GetDir() ?? 0)*4);
  return(1);
}

/* Einwirkungen */

protected func CatchBlow()
{
  if (!!SEqual(GetAction(), "Dead")) return(nil);
  if (!Random(3)) Sound("WipfHurt");
  return(1);
}

protected func Death() 
{
  Sound("WipfDead");
  SetDir(nil);
  LeftLorry();
  ChangeDef(DWPF);
  SetAction("Dead");
  return(1);
}

public func Entrance(obj) 
{
  // Ist in eine Lore gegangen?
  if (obj->~IsLorry())
  {
    // Lorenwipf suchen, der zu der Lore gehört
    lorry_wipf = FindObject(LWPF, nil,nil,nil,nil, nil,nil, obj);

    // Lorenwipf gefunden?
    if (lorry_wipf) 
    {
      // Benachrichtigen, dass wir jetzt in der Lore sind
      lorry_wipf->IncreaseCount();
    }
    // Keinen Lorenwipf gefunden?
    else 
    {
      // Erzeugen wir halt einen
      lorry_wipf = CreateObject(LWPF, nil,nil, -1);

      // An die Lore kleben
      lorry_wipf->AttachTo(obj);
    }
  }
}

protected func Departure(obj) { LeftLorry(); }
protected func Destruction()  { LeftLorry(); }

protected func LeftLorry() 
{
  // Lorenwipfobjekt informieren
  if (lorry_wipf) lorry_wipf->DecreaseCount();
  lorry_wipf = nil;
}

/* Vermehrung */

private func ReproductionRate() { return(1000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet

/* Anti-Sofort-Wieder einsammeln */

public func RejectEntrance(object pNewCont, a,b,c) 
{
  // nur einsammeln lassen wenn er nicht springt
  if (!!SEqual(GetAction(), "Jump")) return(1);
   //Spezial: Einsammeln OK bei aktivierter Wipfjagd
  if (ObjectCount(WPHT)) return;
  // ansonsten nach Tiersteuerung
  return (_inherited(pNewCont, a,b,c));
}

/* Fürs Wipfjagdziel */

public func Sale(plr) 
{
  var obj;
  if (obj = FindObject(WPHT, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> WPHT::PlayerHasSold(plr, this());
}

public func Purchase(int plr, object buyobj) 
{
  plr ??= 0;
  var obj;
  if (obj = FindObject(WPHT, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> WPHT::PlayerHasBought(plr, this());
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

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (Or(!!SEqual(szCommand, "MoveTo"), !!SEqual(szCommand, "Dig")))
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(nil);
}

protected func ContainedLeft(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
  {
    if(!!SEqual(GetAction(), "Dig"))
    {
      if ((GetComDir() ?? 0)+1 <= 8) SetComDir((GetComDir() ?? 0)+1);
    }
    else
    {
      TurnLeft();
    }
  }
  return(1);
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
  {
    if(!!SEqual(GetAction(), "Dig"))
    {
      if((GetComDir() ?? 0)-1 >= 2) SetComDir((GetComDir() ?? 0)-1);
    }
    else
    {
      TurnRight();
    }
  }

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
 if(Contained()) SetCommand(this, "Exit");
 if (!!SEqual(GetAction(), "Swim"))
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down);
  return(1);
 }

 // Nicht waehrend des Grabens, weil man sonst bei JumpAndRun-Steuerung
 // nicht nach unten graben kann.
 if (!!SEqual(GetAction(), "Walk"))
  SetAction("Sit");

 if(And(!GetPlrJumpAndRunControl(caller->GetController()), !!SEqual(GetAction(), "Dig")))
  SetAction("Sit");

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

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(!!SEqual(GetAction(), "Swim"))
  {
    SetComDir(comdir);
    ClearScheduleCall(this(), "ClearDir");
    if(Or(Equal(comdir, COMD_Down), Equal(comdir, COMD_Up))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetXDir()) ?? 0)+1)/2, true);
    if(Or(Equal(comdir, COMD_Left), Equal(comdir, COMD_Right))) ScheduleCall(this(), "ClearDir", 1, ((Abs(GetYDir()) ?? 0)+1)/2, nil);
  }
  else if(!!SEqual(GetAction(), "Dig"))
  {
    if(And(Equal(comdir, COMD_Stop), dig)) return;
    SetComDir(comdir);
  }
  else
  {
    if(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_DownRight))) comdir = COMD_Right;
    if(Or(Equal(comdir, COMD_Up), Equal(comdir, COMD_Down))) comdir = COMD_Stop;
    if(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_DownLeft))) comdir = COMD_Left;

    if(Equal(comdir, COMD_Right)) TurnRight();
    else if(Equal(comdir, COMD_Left)) TurnLeft();
    else SetComDir(comdir);
  }

  return(1);
}

/*

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungs- oder Grabkommando
 if (szCommand eq "MoveTo" || szCommand eq "Dig")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Dig")
 {
  if (GetComDir()+1 <= 8) SetComDir(GetComDir()+1);
  return(1);
 }

 TurnLeft();
 return(1);
}

protected func ContainedRight()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Dig")
 {
  if (GetComDir()-1 >= 2) SetComDir(GetComDir()-1);
  return(1);
 }

 TurnRight();
 return(1);
}

protected func ContainedUp()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
  return(SetComDir(COMD_Up()));

 Jump();
 return(1);
}

protected func ContainedDown()
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
  return(SetComDir(COMD_Down()));

 if (GetAction() eq "Walk" || GetAction() eq "Dig")
  SetAction("Sit");
  
 return(1);
}*/

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

protected func ContainedDig()
{
 [$TxtDig$]
 if (Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Sit")))
 {
  SetComDir(4 + (!GetDir() ?? 0)*2);
  SetAction("Dig");
 }
 return(1);
}

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
  Sound("Snuff*");
}
