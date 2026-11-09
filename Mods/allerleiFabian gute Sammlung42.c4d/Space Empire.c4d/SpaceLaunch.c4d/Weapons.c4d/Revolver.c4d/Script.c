/*-- Revolver --*/

#strict

local mode;


/* Feuern */

public func Fire()
{
  // noch beim Nachladen
  if (!ActIdle()) return(0);
  // Ammo erzeugen
  var ammo = CreateContents(SRES);
  // Richtung des Tr‰gers abfragen
  var direction = GetDir(Contained());
  // Ammo abfeuern
  Exit(ammo, direction*(24+mode*10) - (12+mode*5));
  ammo->Launch(direction,mode);
  // Sound & Nachladen
  if (mode)
  {
    Sound("LaserShot2");
    SetAction("Reload2");
  }
  else
  {
    Sound("LaserShot1");
    SetAction("Reload1");
  }
}


/* Modus umschalten */

public func Activate()
{
  mode = !mode;
  if (mode) Message("groﬂe Ladung", this());
       else Message("schnelle Feuerrate", this());
  return(1);
}


/* Objekt wurde ausgew‰hlt */

public func Selection()
{
  Sound("Bip2");
}


/* Eigenschaften */

public IsWeapon: return(1);
public IsWeaponProduct: return(1);

