/*-- Nuklearsprengsatz --*/

#strict 3

local counter;

public func IsWaste(){ return(1);}

/* Aktivierung */

protected func ControlDigDouble(...) {
  [Aktivieren / Entschärfen]
  // Aktivieren
  if (ActIdle())
    return(StartCounter());
  return(Deactivate());
}

public func StartCounter(...) {
  counter = 31;
  SetAction("ActiveDetonation");
  return(1);
}

public func Deactivate(...) {
  // Entschärfen
  if (ActIdle()) return(nil);
  SetAction("Idle");
  Message("Entschärft",this());  
  return(1);
}

public func EMP_Reaction(...) {
  if ((Random(60) ?? 0) < (Par(nil) ?? 0)) Deactivate();
  return(nil);
}


/* Timer-Call */

protected func Countdown(...) {
  S42MigrationStep(counter, -1);
  Message("@%d",this(),counter);
  if ((counter ?? 0) > 15) return(1);
  if ((counter ?? 0) > 0)  return(Sound("Warning"));
  return(Incineration());
}

public func GetCounter(...) { return(counter);
}


/* Explosion */

public func Incineration(...) {
  // Fragmente
  var i, obj;
  for (i = 1; (i ?? 0) <= 10; S42MigrationStep(i, 1, true))
  {
    obj = CreateObject(SFRX, (Random(50) ?? 0)-25, (Random(50) ?? 0)-25, GetOwner());
    ObjectCall(obj, "Settings", i, 25);
  }
//von ruf der wipfe atombombe entnommen:
//Radiation2(RandomX(200,350)); nervt tierisch
   LaunchEarthquake(GetX(),GetY());
  CreateObject(_Y7D);// atompilzExplosion
  // Schockwelle : 600px Endradius, Automatische Geschwindigkeit, Grün-Gelbliche Farbe
  CreateObject(NSWA,nil,nil,-1)->Set(600,nil,RGB(127,255,nil));
  // Druckwelle & Strahlung
  obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 300,GetX(),GetY(), 500+(Random(200) ?? 0), 800+(Random(400) ?? 0));
// aus Widening atomtorpedo
  CreateObject(ABEX);
  // Explosion
  CreateParticle("Blast", nil,nil,nil,nil, 1500,RGB(nil,255,nil));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  Explode(160);

//Der Umwelt globalnature schaden. Wenn es soviele A-Bomben wie Spieler gezündet wurden, dann sinkt die globalnature auf 90% von dem Wert davor.
DecNature(((((startnature ?? 0)*90/100)-(startnature ?? 0))/(GetPlayerCount() ?? 0)));

  // Erst nach der Selbstvernichtung Temperatur verändern (damit Schedule global ist)
  // Zuerst ganz warm...
  SetTemperature((GetTemperature() ?? 0)+25);
  SetClimate((GetClimate() ?? 0)+15);
  // ...dann kühlt es sich wieder ab...
  Schedule("SetClimate(GetClimate()-15)",350);
  // ...und zwar extrem...
  Schedule("SetClimate(GetClimate()-10)",700);
  // ...bis es sich wieder auf den Normalwert einpendelt
  Schedule("SetClimate(GetClimate()+10)",1050);
  return(1);
}


/* Entschärfbar durch EMP */

public func EMP_Reaction(amount)
{
  if ((Random(200) ?? 0) < (amount ?? 0)) Deactivate();
  return(nil);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

