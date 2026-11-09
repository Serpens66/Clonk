/*-- Spawnpoint --*/

#strict 3

local initialized, angle, player, nocollect, spawntimer;

global func PlaceSpawnpoint(id idObj, int iX, int iY, int timer)
{
  iX ??= 0;
  iY ??= 0;
  timer ??= 0;
  var spwn = CreateObject(SPNP, iX, iY, -1);
  spwn->CreateContents(idObj);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

protected func Initialize()
{
  // Standardtimer = 1000 Frames
  spawntimer = 1000;
  SetClrModulation(RGBa(nil,nil,nil,255));
  // Arrays anlegen
  nocollect = CreateArray();
  player = CreateArray();
}

protected func Timer()
{
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Inhaltsobjekt?
    if(Contents())
      // Initialisieren
      return(Initialized());
  // Kein Inhalt?
  if(!Contents())
    {
    // Nicht mehr initialisiert
    initialized = nil;
    return;
    }
  // Tolle Effekt starten
  S42MigrationAssign(angle, "+=", 10);
  if((angle ?? 0) >= 360) angle = nil;
  SetObjDrawTransform(1000, nil,nil,nil, 1000, (Sin(angle, 8) ?? 0)*1000 - 2000,nil, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), nil, -5, 25, RGBa(210, 210, 255, 100));
  // Alle Timer runterzählen
  DecreaseTimer();
}

private func Initialized()
{
  // Unsichtbar werden
  SetAction("Invis");
  // Objekt als Layer auflegen
  SetGraphics(nil, nil, GetID(Contents()), 1, 1);
  // Transformation
  SetObjDrawTransform(1000, nil,nil,nil, 1000, -5000,nil, 1);
  // Sammeleffekt starten
//  AddEffect("Collect", this(), 1, 3, this());
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    for(var i=nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
      if(GetPlayerType(GetPlayerByIndex(i)))
        if(!((Local() ?? 0) & 1<<(GetPlayerByIndex(i) ?? 0)))
          S42MigrationAssign(Local(), "+=", 1<<(GetPlayerByIndex(i) ?? 0));
    SetVisibility((VIS_Local ?? 0) | (VIS_God ?? 0));
    }
  // Initialisiert
  initialized = true;
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    for(var i=nil, iPlr ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
      // Spielertimer runterzählen
      {
      iPlr = GetPlayerByIndex(i);
      if(Equal(GetPlayerType(iPlr), C4PT_Script)) continue;

      if(player[(iPlr ?? 0)])
        S42MigrationAssign(player[(iPlr ?? 0)], "-=", 5);
      if(And((player[(iPlr ?? 0)] ?? 0) <= 0, nocollect[(iPlr ?? 0)]))
        {
        // Spieler kann das Objekt wieder einsammeln
        player[(iPlr ?? 0)] = nil;
        nocollect[(iPlr ?? 0)] = nil;
        S42MigrationAssign(Local(), "+=", 1<<(GetPlayerByIndex(i) ?? 0));
        }
      }
    }
  else
    {
    // Timer runterzählen
    S42MigrationAssign(player[0], "-=", 5);
    if((player[0] ?? 0) <= 0)
      {
      // Objekt ist wieder da
      player[0] = nil;
      nocollect[0] = nil;
      SetVisibility(VIS_All);
      }
    }
}

/* Einsammel-Effekte */
/*
public func FxCollectTimer()
{
  // Ist ein Clonk hier? -> Einsammeln lassen
  var pObj;
  for(var pClonk in FindObjects(Find_AtPoint(), Find_OCF(OCF_CrewMember()), Find_OCF(OCF_CrewMember()), Find_NoContainer()))
    if(CheckCollect(GetOwner(pClonk)))
      {
      pObj = CreateContents(GetID(Contents()));
      // Kann der Clonk einsammeln?
      Collect(pObj, pClonk);
      if(Contained(pObj) == this()) RemoveObject(pObj);
      else
        {
        Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
        Collected(GetOwner(pClonk));
        // Munition
        if(pObj->~IsAmmoPacket())
        	pObj->~TransferAmmo(pClonk);
        }
    }
}*/

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(And(!((GetOCF(pClonk) ?? 0) & (OCF_CrewMember ?? 0)), !(pClonk->~CanCollectFromSpawnpoints()))) return(1);
  // Ich hab Contents?
  if(Or(!Contents(), !initialized)) return(1);
  // Darf einsammeln
  if(CheckCollect(GetOwner(pClonk), pClonk))
  {
    var pObj = CreateContents(GetID(Contents()));
    // Kann der Clonk einsammeln?
    if(pObj->~IsAmmoPacket()) {
      if(!(pObj->MayTransfer(pClonk)))
        return(1);
      if(NoAmmo()) return(1);
    }

    Collect(pObj, pClonk);
    if(Equal(Contained(pObj), this())) RemoveObject(pObj);
    else
    {
      Sound("Grab", nil, pClonk, nil, (GetOwner(pClonk) ?? 0)+1);
      Collected(GetOwner(pClonk), pClonk);
      // Munition
      if(pObj->~IsAmmoPacket()) {
      	  pObj->~TransferAmmo(pClonk);
      }
      // Waffe, gleich einsatzbereit!
      else if(pObj->~IsWeapon())
        DoAmmo(pObj->GetFMData(FM_AmmoID),pObj->GetFMData(FM_AmmoLoad),pObj);
    }
  }
  return(1);
}

private func CheckCollect(int iPlr, object pClonk) // Überprüft, ob ein Spieler das Objekt einsammeln darf
{
  iPlr ??= 0;
  if(!initialized) return;
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
  {
    if(Equal(GetPlayerType(iPlr), C4PT_User))
      return(!nocollect[(iPlr ?? 0)]);
    if(!nocollect[(iPlr ?? 0)])
    {
      nocollect[(iPlr ?? 0)] = CreateArray();
      return(true);
    }
    for(var pClnk in nocollect[(iPlr ?? 0)])
      if(Equal(pClnk, pClonk))
        return(nil);
    return(true);
  }
  else
    return(!nocollect[0]);
}

private func Collected(int iPlr, object pClonk) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Waffen-Bleiben-Regel
  iPlr ??= 0;
  if(FindObject(WPST))
    {
    if(Equal(GetPlayerType(iPlr), C4PT_Script))
    {
      if(!nocollect[(iPlr ?? 0)])
        nocollect[(iPlr ?? 0)] = CreateArray();
      for(var i=nil ; (i ?? 0) < (GetLength(nocollect[(iPlr ?? 0)]) ?? 0) ; S42MigrationStep(i, 1, true))
        if(!nocollect[(iPlr ?? 0)][(i ?? 0)])
          break;
      nocollect[(iPlr ?? 0)][(i ?? 0)] = pClonk;
      ScheduleCall(this(), "AICollectTimer", spawntimer, nil, iPlr, pClonk);
      return;
    }
    // Spielertimer hochsetzen
    player[(iPlr ?? 0)] = spawntimer;
    nocollect[(iPlr ?? 0)] = true;
    // Nicht mehr sichtbar für den Spieler
    if((iPlr ?? 0) >= 0)
      if((Local() ?? 0) & 1<<(iPlr ?? 0))
        S42MigrationAssign(Local(), "-=", 1<<(iPlr ?? 0));
    }
  else
    {
    // Timer hochsetzen
    player[0] = spawntimer;
    // Nicht mehr einsammelbar
    nocollect[0] = true;
    // Nicht mehr sichtbar
    SetVisibility(VIS_God);
    }
}

protected func AICollectTimer(int iPlr, object pClonk)
{
  iPlr ??= 0;
  for(var i=nil ; (i ?? 0) < (GetLength(nocollect[(iPlr ?? 0)]) ?? 0) ; S42MigrationStep(i, 1, true))
    if(Equal(nocollect[(iPlr ?? 0)][(i ?? 0)], pClonk))
      nocollect[(iPlr ?? 0)][(i ?? 0)] = nil;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra = Format("PlaceSpawnpoint(%i,%%d+%d,%%d+%d)", Contents(nil)->GetID(), GetX(), GetY());
	return !nil;
}
