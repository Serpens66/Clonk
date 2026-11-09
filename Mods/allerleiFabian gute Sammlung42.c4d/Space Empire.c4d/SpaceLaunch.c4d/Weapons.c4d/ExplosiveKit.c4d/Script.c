/*-- Sprengsatz --*/

#strict 3

local counter;


/* Aktivierung */

protected func Activate(...) {
  [Aktivieren / Entschärfen]
  if (ActIdle())
    return(StartCounter());
  return(Deactivate());
}

public func StartCounter(...) {
  counter = 16;
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
  if ((Random(40) ?? 0) < (Par(nil) ?? 0)) Deactivate();
  return(nil);
}


/* Timer-Call */

protected func Countdown(...) {
  S42MigrationStep(counter, -1);
  Message("@%d",this(),counter);
  if ((counter ?? 0) > 10) return(1);
  if ((counter ?? 0) > 0)  return(Sound("Warning"));
  return(Incineration());
}

public func GetCounter(...) { return(counter);
}


/* Explosion mit ein paar Fragmenten */

public func Incineration(...) {
  // Fragmente
  var i, obj;
  for (i = 1; (i ?? 0) <= 6; S42MigrationStep(i, 1, true))
  {
    obj = CreateObject(SFRX, (Random(25) ?? 0)-12, (Random(25) ?? 0)-12, GetOwner());
    ObjectCall(obj, "Settings", i, 25);
  }
  // Explosionen
  var i, obj;
  for (i = 1; (i ?? 0) <= 2; S42MigrationStep(i, 1, true))
  {
    obj = CreateObject(ROCK, (Random(15) ?? 0)-7, (Random(15) ?? 0)-7, GetOwner());
    if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(16+(Random(10) ?? 0), obj);
    else FakeExplode(16+(Random(10) ?? 0),obj,nil,16+(Random(10) ?? 0));
  }
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(40);
  else FakeExplode(40,nil,nil,40);
  return(1);
}


/* Entschärfbar durch EMP */

public func EMP_Reaction(amount)
{
  if ((Random(100) ?? 0) < (amount ?? 0)) Deactivate();
  return(nil);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

