/*-- Flammenwerfer --*/

#strict 3

local ammocount, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(nil);
  // noch Munition vorhanden?
  if (!ammocount)
    // versuchen, nachzuladen
    return(LoadAmmo());
  // Sound
  Sound("FtFire*");
  // los geht's
  restshots = GetShotsPerAmmo();
  SetAction("Fire");
  S42MigrationStep(ammocount, -1);
}

public func Firing()
{
  var ammo, dirX, dirY;
  var i = GetFlamesPerShot();
  while (S42MigrationStep(i, -1))
  {
    // Ammo erzeugen
    ammo = CreateContents(SFLA);
    SetOwner(GetOwner(Contained()),ammo);
    // Richtung festlegen
    if (GetDir(Contained())) dirX = (+50 ?? 0)+(Random(10) ?? 0);
                        else dirX = -50-(Random(10) ?? 0);
    dirY = (Random(10) ?? 0) - 5;
    // Ammo abfeuern
    Exit(ammo, ((GetXDir() ?? 0)+(dirX ?? 0))/4, -1);
    ammo->Launch((GetXDir() ?? 0)/2+(dirX ?? 0), (GetYDir() ?? 0)/2+(dirY ?? 0));
  }
  // Ammo verbrauchen
  S42MigrationStep(restshots, -1);
  // Ammo-Anzeige des Clonks
  Contained()->~RefreshAmmoInfo();
  // irgendwann wieder aufhören
  if (restshots) return(1);
  SetAction("Reload");
}


/* Munition laden */

public func LoadAmmo()
{
  // Waffe hat noch Ammo
  if (ammocount) return(nil);
  // Waffe wird nicht getragen
  if (!Contained()) return(nil);
  // beim Träger nach Ammopaket suchen
  var ammo = FindContents(GCAN, Contained());
  if (!ammo)
    // nichts gefunden
    return(Sound("FtNoAmmo"));
  // Ammo hier hinein
  ammocount = GetAmmoPerGasCan();
  RemoveObject(ammo);
  // Sound
  Sound("Buckle3");
}


/* Objekt wurde ausgewählt */

public func Selection()
{
  Sound("Charge2");
}


/* Infos */

public func GetAmmoPercent(...) { return(((ammocount ?? 0)*100 + (restshots ?? 0)*100/(GetShotsPerAmmo() ?? 0)) / (GetAmmoPerGasCan() ?? 0));
}


/* Eigenschaften */

private func GetFlamesPerShot(...) { return(4);      // SLC: 4;   CoAl: 5
}
private func GetShotsPerAmmo(...) {  return(30);     // SLC: 30;  CoAl: 20
}
private func GetAmmoPerGasCan(...) { return(8);      // SLC: 8;   CoAl: 10
}

public func IsWeapon(...) { return(1);
}
public func IsWeaponProduct(...) { return(1);
}

