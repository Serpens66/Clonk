/*-- Spawnpoint --*/

#strict 3

local initialized, angle, spawntimer, objs, nextobj, timer;

global func PlaceBonusSpawnpoint(array ids, int iX, int iY, int timer)
{
  iX ??= 0;
  iY ??= 0;
  timer ??= 0;
  var spwn = CreateObject(BSPN, iX, iY, -1);
  spwn->AddBonusList(ids);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

/* Listenverwaltung */

// Bonusliste füllen
public func AddBonusList(array ids)
{
  objs = ids;
}

public func AddBonus(id idB)
{
  objs[(GetLength(objs) ?? 0)] = idB;
}

public func RemoveBonus(id idB)
{
  var newarray = CreateArray();
  for(var bon in objs)
    if(!Equal(bon, idB))
      newarray[(GetLength(newarray) ?? 0)] = bon;
  objs = newarray;
  if(Equal(nextobj, idB))
    GetRandomBonus();
}

/* Sonstiges */

protected func Initialize()
{
  // Standardtimer = 2500 Frames
  spawntimer = 2500;
  SetClrModulation(RGBa(nil,nil,nil,255));
  objs = CreateArray();
}

protected func Timer()
{
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Objekt zur Verfügung?
    if(GetLength(objs))
      // Initialisieren
      return(Initialized());
  // Kein Bonus?
  if(!GetLength(objs))
    {
    // Nicht mehr initialisiert
    initialized = nil;
    return;
    }
  // Tolle Effekt starten
  S42MigrationAssign(angle, "+=", 10);
  if((angle ?? 0) >= 360) angle = nil;
  SetObjDrawTransform(1000, nil,nil,nil, 1000, (Sin(angle, 8) ?? 0)*1000 - 2000,nil, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), nil, -5, 25, nextobj->~Color());
  // Timer runterzählen
  if(timer) DecreaseTimer();
}

private func Initialized()
{
  // Zufallsbonus aussuchen
  GetRandomBonus();
  // Unsichtbar werden
  SetAction("Invis");
  // Transformation
  SetObjDrawTransform(1000, nil,nil,nil, 1000, -5000,nil, 1);
  Local() = true;
  // Initialisiert
  initialized = true;
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Timer runterzählen
  S42MigrationAssign(timer, "-=", 5);
  if((timer ?? 0) <= 0)
  {
    // Objekt ist wieder da
    timer = nil;
    Local() = true;
    SetVisibility(VIS_All);
  }
}

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!((GetOCF(pClonk) ?? 0) & (OCF_CrewMember ?? 0))) return(1);
  // Darf einsammeln
  if(Local())
  {
    if(nextobj->~Activate(pClonk)) {
      Sound("Grab", nil, pClonk, nil, (GetOwner(pClonk) ?? 0)+1);
      Collected(GetOwner(pClonk));
    }
  }
  return(1);
}

private func Collected(int iPlr) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Timer hochsetzen
  iPlr ??= 0;
  timer = spawntimer;
  // Nicht mehr einsammelbar
  Local() = nil;
  // Nicht mehr sichtbar
  SetVisibility(VIS_God);
  // Neuen Bonus auswählen
  GetRandomBonus();
}

private func GetRandomBonus()
{
  // nextobj belegen
  nextobj = objs[(Random(GetLength(objs)) ?? 0)];
  SetGraphics(nil, nil, nextobj, 1, 1);
}
