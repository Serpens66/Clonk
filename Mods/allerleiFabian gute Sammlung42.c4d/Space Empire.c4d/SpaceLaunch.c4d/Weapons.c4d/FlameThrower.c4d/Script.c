/*-- Flammenwerfer --*/

#strict

local ammocount, restshots;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);
  // noch Munition vorhanden?
  if (!ammocount)
    // versuchen, nachzuladen
    return(LoadAmmo());
  // Sound
  Sound("FtFire*");
  // los geht's
  restshots = GetShotsPerAmmo();
  SetAction("Fire");
  --ammocount;
}

public func Firing()
{
  var ammo, dirX, dirY;
  var i = GetFlamesPerShot();
  while (--i)
  {
    // Ammo erzeugen
    ammo = CreateContents(SFLA);
    SetOwner(GetOwner(Contained()),ammo);
    // Richtung festlegen
    if (GetDir(Contained())) dirX = +50+Random(10);
                        else dirX = -50-Random(10);
    dirY = Random(10) - 5;
    // Ammo abfeuern
    Exit(ammo, (GetXDir()+dirX)/4, -1);
    ammo->Launch(GetXDir()/2+dirX, GetYDir()/2+dirY);
  }
  // Ammo verbrauchen
  --restshots;
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
  if (ammocount) return(0);
  // Waffe wird nicht getragen
  if (!Contained()) return(0);
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

public GetAmmoPercent: return((ammocount*100 + restshots*100/GetShotsPerAmmo()) / GetAmmoPerGasCan());


/* Eigenschaften */

private GetFlamesPerShot: return(4);      // SLC: 4;   CoAl: 5
private GetShotsPerAmmo:  return(30);     // SLC: 30;  CoAl: 20
private GetAmmoPerGasCan: return(8);      // SLC: 8;   CoAl: 10

public IsWeapon: return(1);
public IsWeaponProduct: return(1);

