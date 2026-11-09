/*-- Sprengsatz --*/

#strict

local counter;


/* Aktivierung */

protected Activate:
  [Aktivieren / Entschärfen]
  if (ActIdle())
    return(StartCounter());
  return(Deactivate());

public StartCounter:
  counter = 16;
  SetAction("ActiveDetonation");
  return(1);

public Deactivate:
  // Entschärfen
  if (ActIdle()) return(0);
  SetAction("Idle");
  Message("Entschärft",this());
  return(1);

public EMP_Reaction:
  if (Random(40) < Par(0)) Deactivate();
  return(0);


/* Timer-Call */

protected Countdown:
  --counter;
  Message("@%d",this(),counter);
  if (counter > 10) return(1);
  if (counter > 0)  return(Sound("Warning"));
  return(Incineration());

public GetCounter: return(counter);


/* Explosion mit ein paar Fragmenten */

public Incineration:
  // Fragmente
  var i, obj;
  for (i = 1; i <= 6; i++)
  {
    obj = CreateObject(SFRX, Random(25)-12, Random(25)-12, GetOwner());
    ObjectCall(obj, "Settings", i, 25);
  }
  // Explosionen
  var i, obj;
  for (i = 1; i <= 2; i++)
  {
    obj = CreateObject(ROCK, Random(15)-7, Random(15)-7, GetOwner());
    if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(16+Random(10), obj);
    else FakeExplode(16+Random(10),obj,0,16+Random(10));
  }
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(40);
  else FakeExplode(40,0,0,40);
  return(1);


/* Entschärfbar durch EMP */

public func EMP_Reaction(amount)
{
  if (Random(100) < amount) Deactivate();
  return(0);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

