/*-- Elektrowaffe --*/

#strict

local target, ammocount, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);
  // noch genug Energie?
  if (!ammocount)
    return(LoadAmmo());
  // Ziel suchen
  target = FindTarget();
  // kein Ziel?
  if (!target)
  {
    Sound("ElNoTarget");
    return(0);
  }
  // Sound & Aktion
  Sound("Electric");
  SetAction("Firing");
  // gleich mal 'ne Ladung loslassen
  restshots = 8;
  --ammocount;
}

private func DrawLightning(x0, y0, x1, y1)
{
  var lgh = CreateObject(SLGH,0,0,-1);
  SetCon(Distance(x0,y0,x1,y1)/2, lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  SetPosition((x0+x1)/2, (y0+y1)/2, lgh);
}

private func Shoot()
{
  // Ziel noch da und in Reichweite?
  if (!target || Contained(target))  return(0);
  if (ObjectDistance(target) > 100)  return(0);
  // Blitz erzeugen
  if (GetDir(Contained()))  DrawLightning(GetX()+7, GetY(), GetX(target), GetY(target));
                      else  DrawLightning(GetX()-6, GetY(), GetX(target), GetY(target));
  // Lebewesen oder nicht?
  if (GetOCF(target) & OCF_Alive())
    DoEnergy(-1-Random(4)/3, target);                            // Schaden: 1.25 HP
  else
    CreateObject(SEMP,0,0,-1)->SetTarget(target, 4+Random(5));   // EMP der Stärke 6.0
  DoDamage(1-Random(5)/4, target);                               // Schaden: 0.8 HP
}


/* Zielsuche */

private func FindTarget()
{
  // Sprengsätze, Minen
  var obj;
  while (1)
  {
    obj = FindObject(0, 0,0,-1,-1, 0,"ActiveDetonation",0,NoContainer(), obj);
    if (!obj) break;
    if (GetDir(Contained()) == (GetX(obj) < GetX())) continue;
    if (ObjectDistance(obj) > 100) break;
    if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
      return(obj);
  }
  // Lebewesen
  obj = 0;
  while (1)
  {
    obj = FindObject(0, 0,0,-1,-1, OCF_Alive(),0,0,NoContainer(), obj);
    if (!obj) break;
    if (GetDir(Contained()) == (GetX(obj) < GetX())) continue;
    if (ObjectDistance(obj) > 100) break;
    if ((GetID(obj) == SSPR) || (GetID(obj) == SNSS))
      if (GetAction(obj) == "Count")
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);       // Sprengsätze
    if (GetOCF(obj) & OCF_CrewMember())
    {
      if (Hostile(GetOwner(Contained()),GetOwner(obj)))
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);     // verfeindetes Crewmitglied
    }
    else if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
           return(obj);    // andere Lebewesen
  }
  // gegnerische Fahrzeuge
  obj = 0;
  while (1)
  {
    obj = FindObject(0, 0,0,-1,-1, 0,0,0,NoContainer(), obj);
    if (!obj) break;
    if (GetDir(Contained()) == (GetX(obj) < GetX())) continue;
    if (ObjectDistance(obj) > 100) break;
    if (GetCategory(obj) & C4D_Vehicle())
      if (GetOCF(obj) & OCF_Entrance())
        if (Hostile(GetOwner(Contained()),GetOwner(obj)))
          if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
            return(obj);
  }
  // gegnerische Gebäude
  obj = 0;
  while (1)
  {
    obj = FindObject(0, 0,0,-1,-1, OCF_Exclusive(),0,0,NoContainer(), obj);
    if (!obj) break;
    if (GetDir(Contained()) == (GetX(obj) < GetX())) continue;
    if (ObjectDistance(obj) > 100) break;
    if (GetCategory(obj) & C4D_Structure())
      if (Hostile(GetOwner(Contained()),GetOwner(obj)))
        if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
          return(obj);
  }
  return(0);
}


/* Überladung */

protected func Activate()
{
  [Waffe überladen]
  if (GetAction() ne "Countdown")
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
  ShiftContents(Contained(),0,GetID());
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
  var counter = 5 - GetActTime()/30;
  // Zeitanzeige
  Message(Format("Waffe überladen|... %d ...", counter), this());
  // Zeit vorbei?
  if (counter <= 0)
  {
    // Fragmente
    var i, obj;
    for (i = 1; i <= 3; i++)
    {
      obj = CreateObject(SFRX, Random(50)-25, Random(50)-25, GetOwner());
      obj->Settings(i, 25);
    }
    // Druckwelle & EMP
    obj = CreateObject(SWV2);
    obj->SetSize(200,GetX(),GetY(), 0, 500+ammocount*80+Random(200));
    // Explosion
    CreateParticle("Blast", 0,0,0,0, 800,RGB(128,0,255));
    Explode(20);
  }
}


/* Objekt wurde ausgewählt */

public func Selection()
{
  Sound("Bip2");
}


/* Infos */

public GetAmmoPercent: return(ammocount*10 + restshots*10/8);


/* Eigenschaften */

public IsWeapon: return(GetAction() ne "Countdown");   // Waffe bei Überladung werfen
public IsWeaponProduct: return(1);

