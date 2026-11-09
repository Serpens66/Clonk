/*-- Maschinengewehr --*/

#strict 3

local packID, value, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(nil);
  // keine Munition
  if (And(!Contents(), !packID))
    return(Sound("NoAmmo2"));
  // Munition aus dem geladenen Pack entfernen
  var ammopack = FindContents(packID);
  var ammo = FindContents(GetAmmoID(packID),ammopack);
  if (!ammo) return(Sound("NoAmmo2"));
  RemoveObject(ammo);
  // leere Packs ebenfalls löschen
  if (!FindContents(GetAmmoID(packID),ammopack))
  {
    RemoveObject(ammopack);
    // anderes Pack der gleichen Sorte finden
    ammopack = FindContents(packID,Contained());
    if (ammopack)
      Enter(this(),ammopack);
  }
  // Schussmodus je nach Munitionstyp wählen
  restshots = 7;
  if (Or(Equal(packID, SPC1), Equal(packID, SPC2)))
  {
    Sound("AutoGun*");
    SetAction("ShootAmmo");
    return(1);
  }
  Sound("AutoOnce");
  if (Equal(packID, SPC3)) return(SpecialShot(SMG2));
  if (Equal(packID, SPC4)) return(SpecialShot(SMG3));
  return(1);
}

private func FireAmmo()
{
  // Richtung des Trägers abfragen
  var dir = +1;
  if (Equal(GetDir(Contained()), DIR_Left)) dir = -1;
  // MG-Kugel abfeuern
  var ammo = CreateContents(SMG1);
  Exit(ammo, (dir ?? 0)*15,(Random(5) ?? 0)-2,nil, nil,nil,nil);
  if (Equal(packID, SPC1))  ammo->Launch(GetDir(Contained()));
  if (Equal(packID, SPC2))  ammo->Launch(GetDir(Contained()),(Random(31) ?? 0)-15);
  // Mündungsfeuer
  if (!GBackLiquid())
    CreateObject(SMGF)->Config(Contained(), (dir ?? 0)*8,-1, (dir ?? 0)*90);
  // Ammo verbrauchen
  S42MigrationStep(restshots, -1);
  // Ammo-Anzeige des Clonks
  Contained()->~RefreshAmmoInfo();
  // genug verschossen?
  if (restshots) return(1);
  SetAction("Reload1");
  return(1);
}

private func SpecialShot()
{
  // Richtung des Trägers abfragen
  var dir = +1;
  if (Equal(GetDir(Contained()), DIR_Left)) dir = -1;
  // MG-Kugel abfeuern
  var ammo = CreateContents(Par(nil));
  Exit(ammo, (dir ?? 0)*15,(Random(3) ?? 0)-1,nil, nil,nil,nil);
  ammo->Launch(GetDir(Contained()));
  // Mündungsfeuer
  if (!GBackLiquid())
    CreateObject(SMGF)->Config(Contained(), (dir ?? 0)*8,-1, (dir ?? 0)*90);
  // Nachladen
  SetAction("Reload2");
  return(1);
}


/* Aktivierung & Auswahlmenü */

public func Activate()
{
  // noch beim Nachladen
  if (!ActIdle()) return(nil);
  // Menü erzeugen und füllen
  CreateMenu(SMGN,Contained(),this(),nil,"");
  Add2Menu(SPC1); Add2Menu(SPC2);
  Add2Menu(SPC3); Add2Menu(SPC4);
  AddMenuItem("Keine Munition", "SelectAmmo", SYCC,Contained());
  return(1);                               
}

private func Add2Menu(packID)
{
  // alle getragenen Munitionstypen hinzufügen
  var num = ContentsCount(GetAmmoID(packID), FindContents(packID));
  var obj = nil;
  while (obj = FindObject(packID,nil,nil,nil,nil,nil,"",nil,Contained(),obj))
    S42MigrationAssign(num, "+=", (ContentsCount(GetAmmoID(packID),obj) ?? 0));
  if (num) AddMenuItem("%s", "SelectAmmo", packID,Contained());
}

public func SelectAmmo(ID)
{
  // Alte Munition raus...
  Enter(Contained(),Contents());
  // AmmoPackID = SYCC heisst: alles raus, nix rein
  if (Equal(ID, SYCC)) return(packID = nil);
  // ...und die neue rein
  Enter(this(),FindContents(ID,Contained()));
  // PackID anpassen
  packID = ID;
  // MG wieder auswählen
  ShiftContents(Contained(),nil,GetID());
  // Ammo-Anzeige updaten
  Contained()->~CheckArmed();
}


/* Suchfunktion */

private func GetAmmoID(packID)
{
  if (Equal(packID, SPC1)) return(SAM1);
  if (Equal(packID, SPC2)) return(SAM2);
  if (Equal(packID, SPC3)) return(SAM3);
  if (Equal(packID, SPC4)) return(SAM4);
  return(nil);
}

private func AmmoContentsCount()
{
  if (!packID) return(nil);
  var ammopack = FindContents(packID);
  if (!ammopack) return(nil);
  return(ContentsCount(GetAmmoID(packID), ammopack));
}


/* Objekt wurde ausgewählt */

public func Selection()
{
  Sound("Charge");
}

  
/* Zusatzinfos */

public func GetAmmoPercent(...) { return((AmmoContentsCount() ?? 0)*10 + (restshots ?? 0)*10/7);
}


/* Eigenschaften */

public func IsWeapon(...) { return(1);
}
public func IsWeaponProduct(...) { return(1);
}

