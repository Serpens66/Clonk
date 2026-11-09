/*-- Elektrowaffe --*/

#strict 3

local target, ammocount, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(nil);
  // noch genug Energie?
  if (!ammocount)
    return(LoadAmmo());
  // Ziel suchen
  target = FindTarget();
  // kein Ziel?
  if (!target)
  {
    Sound("ElNoTarget");
    return(nil);
  }
  // Sound & Aktion
  Sound("Electric");
  SetAction("Firing");
  // gleich mal 'ne Ladung loslassen
  restshots = 8;
  S42MigrationStep(ammocount, -1);
}

private func DrawLightning(x0, y0, x1, y1)
{
  var lgh = CreateObject(SLGH,nil,nil,-1);
  SetCon((Distance(x0,y0,x1,y1) ?? 0)/2, lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  SetPosition(((x0 ?? 0)+(x1 ?? 0))/2, ((y0 ?? 0)+(y1 ?? 0))/2, lgh);
}

private func Shoot()
{
  // Ziel noch da und in Reichweite?
  if (Or(!target, Contained(target)))  return(nil);
  if ((ObjectDistance(target) ?? 0) > 100)  return(nil);
  // Blitz erzeugen
  if (GetDir(Contained()))  DrawLightning((GetX() ?? 0)+7, GetY(), GetX(target), GetY(target));
                      else  DrawLightning((GetX() ?? 0)-6, GetY(), GetX(target), GetY(target));
  // Lebewesen oder nicht?
  if ((GetOCF(target) ?? 0) & (OCF_Alive ?? 0))
    DoEnergy(-1-(Random(4) ?? 0)/3, target);                            // Schaden: 1.25 HP
  else
    CreateObject(SEMP,nil,nil,-1)->SetTarget(target, 4+(Random(5) ?? 0));   // EMP der Stärke 6.0
  DoDamage(1-(Random(5) ?? 0)/4, target);                               // Schaden: 0.8 HP
}


/* Zielsuche */

private func FindTarget()
{
  // Sprengsätze, Minen
  var obj;
  while (1)
  {
    obj = FindObject(nil, nil,nil,-1,-1, nil,"ActiveDetonation",nil,NoContainer(), obj);
    if (!obj) break;
    if (Equal(GetDir(Contained()), ((GetX(obj) ?? 0) < (GetX() ?? 0)))) continue;
    if ((ObjectDistance(obj) ?? 0) > 100) break;
    if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
      return(obj);
  }
  // Lebewesen
  obj = nil;
  while (1)
  {
    obj = FindObject(nil, nil,nil,-1,-1, OCF_Alive,nil,nil,NoContainer(), obj);
    if (!obj) break;
    if (Equal(GetDir(Contained()), ((GetX(obj) ?? 0) < (GetX() ?? 0)))) continue;
    if ((ObjectDistance(obj) ?? 0) > 100) break;
    if (Or((Equal(GetID(obj), SSPR)), (Equal(GetID(obj), SNSS))))
      if (Equal(GetAction(obj), "Count"))
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);       // Sprengsätze
    if ((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0))
    {
      if (Hostile(GetOwner(Contained()),GetOwner(obj)))
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);     // verfeindetes Crewmitglied
    }
    else if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
           return(obj);    // andere Lebewesen
  }
  // gegnerische Fahrzeuge
  obj = nil;
  while (1)
  {
    obj = FindObject(nil, nil,nil,-1,-1, nil,nil,nil,NoContainer(), obj);
    if (!obj) break;
    if (Equal(GetDir(Contained()), ((GetX(obj) ?? 0) < (GetX() ?? 0)))) continue;
    if ((ObjectDistance(obj) ?? 0) > 100) break;
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      if ((GetOCF(obj) ?? 0) & (OCF_Entrance ?? 0))
        if (Hostile(GetOwner(Contained()),GetOwner(obj)))
          if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
            return(obj);
  }
  // gegnerische Gebäude
  obj = nil;
  while (1)
  {
    obj = FindObject(nil, nil,nil,-1,-1, OCF_Exclusive,nil,nil,NoContainer(), obj);
    if (!obj) break;
    if (Equal(GetDir(Contained()), ((GetX(obj) ?? 0) < (GetX() ?? 0)))) continue;
    if ((ObjectDistance(obj) ?? 0) > 100) break;
    if ((GetCategory(obj) ?? 0) & (C4D_Structure ?? 0))
      if (Hostile(GetOwner(Contained()),GetOwner(obj)))
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);
  }
  return(nil);
}


/* Überladung */

protected func Activate()
{
  [Waffe überladen]
  if (!SEqual(GetAction(), "Countdown"))
  {
    SetAction("Countdown");
    Sound("Overload");
  }
  else
  {
    SetAction("Idle");
    Message("Überladung|deaktiviert", this());
    Sound("Overload2");
  }
  return(1);
}


/* Munition laden */

public func LoadAmmo()
{
  // nicht nötig?
  if (ammocount)    return(1);
  if (!Contained()) return(1);
  // nachladen
  var accu = FindContents(SACU, Contained());
  if (!accu)
    return(Sound("NoAmmo"));
  // Akku entleeren
  ChangeDef(SACE, accu);
  ammocount = 10;
  // Sound
  Sound("Buckle3");
  // Waffe wieder auswählen
  ShiftContents(Contained(),nil,GetID());
}


/* Action-Calls */

protected func Firing()
{
  // einmal schiessen
  Shoot();
//  --restshots;
//  // Ammo-Anzeige des Clonks
//  if (Contained())
//    Contained()->RefreshAmmoInfo();
//  // schon fertig?
//  if (restshots) return(1);
//  SetAction("Reload");
}

protected func Countdown()
{
  var counter = 5 - (GetActTime() ?? 0)/30;
  // Zeitanzeige
  Message(Format("Waffe überladen|... %d ...", counter), this());
  // Zeit vorbei?
  if ((counter ?? 0) <= 0)
  {
    // Fragmente
    var i, obj;
    for (i = 1; (i ?? 0) <= 3; S42MigrationStep(i, 1, true))
    {
      obj = CreateObject(SFRX, (Random(50) ?? 0)-25, (Random(50) ?? 0)-25, GetOwner());
      obj->Settings(i, 25);
    }
    // Druckwelle & EMP
    obj = CreateObject(SWV2);
    obj->SetSize(200,GetX(),GetY(), nil, 500+(ammocount ?? 0)*80+(Random(200) ?? 0));
    // Explosion
    CreateParticle("Blast", nil,nil,nil,nil, 800,RGB(128,nil,255));
    Explode(20);
  }
}


/* Objekt wurde ausgewählt */

public func Selection()
{
  Sound("Bip2");
}


/* Infos */

public func GetAmmoPercent(...) { return((ammocount ?? 0)*10 + (restshots ?? 0)*10/8);
}


/* Eigenschaften */

public func IsWeapon(...) { return(!SEqual(GetAction(), "Countdown"));   // Waffe bei Überladung werfen
}
public func IsWeaponProduct(...) { return(1);
}

