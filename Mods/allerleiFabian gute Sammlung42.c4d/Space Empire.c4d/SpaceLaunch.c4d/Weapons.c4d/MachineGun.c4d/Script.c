/*-- Maschinengewehr --*/

#strict

local packID, value, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);
  // keine Munition
  if (!Contents() && !packID)
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
  if (packID==SPC1 || packID==SPC2)
  {
    Sound("AutoGun*");
    SetAction("ShootAmmo");
    return(1);
  }
  Sound("AutoOnce");
  if (packID==SPC3) return(SpecialShot(SMG2));
  if (packID==SPC4) return(SpecialShot(SMG3));
  return(1);
}

private func FireAmmo()
{
  // Richtung des Trägers abfragen
  var dir = +1;
  if (GetDir(Contained())==DIR_Left()) dir = -1;
  // MG-Kugel abfeuern
  var ammo = CreateContents(SMG1);
  Exit(ammo, dir*15,Random(5)-2,0, 0,0,0);
  if (packID==SPC1)  ammo->Launch(GetDir(Contained()));
  if (packID==SPC2)  ammo->Launch(GetDir(Contained()),Random(31)-15);
  // Mündungsfeuer
  if (!GBackLiquid())
    CreateObject(SMGF)->Config(Contained(), dir*8,-1, dir*90);
  // Ammo verbrauchen
  --restshots;
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
  if (GetDir(Contained())==DIR_Left()) dir = -1;
  // MG-Kugel abfeuern
  var ammo = CreateContents(Par(0));
  Exit(ammo, dir*15,Random(3)-1,0, 0,0,0);
  ammo->Launch(GetDir(Contained()));
  // Mündungsfeuer
  if (!GBackLiquid())
    CreateObject(SMGF)->Config(Contained(), dir*8,-1, dir*90);
  // Nachladen
  SetAction("Reload2");
  return(1);
}


/* Aktivierung & Auswahlmenü */

public func Activate()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);
  // Menü erzeugen und füllen
  CreateMenu(SMGN,Contained(),this(),0,"");
  Add2Menu(SPC1); Add2Menu(SPC2);
  Add2Menu(SPC3); Add2Menu(SPC4);
  AddMenuItem("Keine Munition", "SelectAmmo", SYCC,Contained());
  return(1);                               
}

private func Add2Menu(packID)
{
  // alle getragenen Munitionstypen hinzufügen
  var num = ContentsCount(GetAmmoID(packID), FindContents(packID));
  var obj = 0;
  while (obj = FindObject(packID,0,0,0,0,0,"",0,Contained(),obj))
    num += ContentsCount(GetAmmoID(packID),obj);
  if (num) AddMenuItem("%s", "SelectAmmo", packID,Contained());
}

public func SelectAmmo(ID)
{
  // Alte Munition raus...
  Enter(Contained(),Contents());
  // AmmoPackID = SYCC heisst: alles raus, nix rein
  if (ID == SYCC) return(packID = 0);
  // ...und die neue rein
  Enter(this(),FindContents(ID,Contained()));
  // PackID anpassen
  packID = ID;
  // MG wieder auswählen
  ShiftContents(Contained(),0,GetID());
  // Ammo-Anzeige updaten
  Contained()->~CheckArmed();
}


/* Suchfunktion */

private func GetAmmoID(packID)
{
  if (packID == SPC1) return(SAM1);
  if (packID == SPC2) return(SAM2);
  if (packID == SPC3) return(SAM3);
  if (packID == SPC4) return(SAM4);
  return(0);
}

private func AmmoContentsCount()
{
  if (!packID) return(0);
  var ammopack = FindContents(packID);
  if (!ammopack) return(0);
  return(ContentsCount(GetAmmoID(packID), ammopack));
}


/* Objekt wurde ausgewählt */

public func Selection()
{
  Sound("Charge");
}

  
/* Zusatzinfos */

public GetAmmoPercent: return(AmmoContentsCount()*10 + restshots*10/7);


/* Eigenschaften */

public IsWeapon: return(1);
public IsWeaponProduct: return(1);

