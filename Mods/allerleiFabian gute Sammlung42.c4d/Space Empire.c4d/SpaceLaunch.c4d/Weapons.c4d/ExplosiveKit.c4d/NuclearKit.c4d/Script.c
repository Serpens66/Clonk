/*-- Nuklearsprengsatz --*/

#strict

local counter;

public func IsWaste(){ return(1);}

/* Aktivierung */

protected ControlDigDouble:
  [Aktivieren / Entschärfen]
  // Aktivieren
  if (ActIdle())
    return(StartCounter());
  return(Deactivate());

public StartCounter:
  counter = 31;
  SetAction("ActiveDetonation");
  return(1);

public Deactivate:
  // Entschärfen
  if (ActIdle()) return(0);
  SetAction("Idle");
  Message("Entschärft",this());  
  return(1);

public EMP_Reaction:
  if (Random(60) < Par(0)) Deactivate();
  return(0);


/* Timer-Call */

protected Countdown:
  --counter;
  Message("@%d",this(),counter);
  if (counter > 15) return(1);
  if (counter > 0)  return(Sound("Warning"));
  return(Incineration());

public GetCounter: return(counter);


/* Explosion */

public Incineration:
  // Fragmente
  var i, obj;
  for (i = 1; i <= 10; i++)
  {
    obj = CreateObject(SFRX, Random(50)-25, Random(50)-25, GetOwner());
    ObjectCall(obj, "Settings", i, 25);
  }
//von ruf der wipfe atombombe entnommen:
//Radiation2(RandomX(200,350)); nervt tierisch
   LaunchEarthquake(GetX(),GetY());
  CreateObject(_Y7D);// atompilzExplosion
  // Schockwelle : 600px Endradius, Automatische Geschwindigkeit, Grün-Gelbliche Farbe
  CreateObject(NSWA,0,0,-1)->Set(600,0,RGB(127,255,0));
  // Druckwelle & Strahlung
  obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 300,GetX(),GetY(), 500+Random(200), 800+Random(400));
// aus Widening atomtorpedo
  CreateObject(ABEX);
  // Explosion
  CreateParticle("Blast", 0,0,0,0, 1500,RGB(0,255,0));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  Explode(160);

//Der Umwelt globalnature schaden. Wenn es soviele A-Bomben wie Spieler gezündet wurden, dann sinkt die globalnature auf 90% von dem Wert davor.
DecNature((((startnature*90/100)-startnature)/GetPlayerCount()));

  // Erst nach der Selbstvernichtung Temperatur verändern (damit Schedule global ist)
  // Zuerst ganz warm...
  SetTemperature(GetTemperature()+25);
  SetClimate(GetClimate()+15);
  // ...dann kühlt es sich wieder ab...
  Schedule("SetClimate(GetClimate()-15)",350);
  // ...und zwar extrem...
  Schedule("SetClimate(GetClimate()-10)",700);
  // ...bis es sich wieder auf den Normalwert einpendelt
  Schedule("SetClimate(GetClimate()+10)",1050);
  return(1);


/* Entschärfbar durch EMP */

public func EMP_Reaction(amount)
{
  if (Random(200) < amount) Deactivate();
  return(0);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

