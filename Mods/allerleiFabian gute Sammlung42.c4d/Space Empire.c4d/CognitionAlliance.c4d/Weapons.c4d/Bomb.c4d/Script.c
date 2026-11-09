/*-- Bombe --*/

#strict 3

local timer;

protected func Initialize()
{
  timer=10;
  return(1);
}

protected func Activate(caller)
{
  if (Hostile(GetOwner(caller),GetOwner())) return(1);
  if (!ActIdle()) { 
    SetGraphics("Graphics");
    Message("Bombe entschärft!",this()); 
    SetAction("Idle"); 
    timer = 10; 
  }
  else { 
    SetGraphics("Armed"); 
    Message("Bombe aktiviert!",this()); 
    SetAction("ActiveDetonation"); 
  } 
  return(1);
}

protected func Hit()
{
  Sound("RockHit1");
  return(1);
}

protected func Damage()
{
  Sound("Fusing");
  if (!ActIdle()) return(1);
  SetGraphics("Armed");
  SetAction("ActiveDetonation"); 
  Message("Bombe aktiviert!",this());
  return(1);
}

protected func Countdown()
{
  S42MigrationStep(timer, -1, true);
  if ((timer ?? 0) <= 5) Sound("Bip");
  if (!timer) Detonation();
  Message("Explosion in: %d",this(),timer);
  return(1);
}

protected func Detonation()
{
  // Fragmente
  var i, obj;
  for (i = 1; (i ?? 0) <= 6; S42MigrationStep(i, 1, true))
  {
    obj = CreateObject(SFRX, (Random(25) ?? 0)-12, (Random(25) ?? 0)-12, GetOwner());
    obj->Settings(i, 25);
  }
  // Explosionen
  for (i = 1; (i ?? 0) <= 3; S42MigrationStep(i, 1, true))
  {
    obj = CreateObject(ROCK, (Random(25) ?? 0)-14, (Random(25) ?? 0)-14, GetOwner());
    if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(16+(Random(10) ?? 0), obj);
    else FakeExplode(16+(Random(10) ?? 0),obj,nil,16+(Random(10) ?? 0));
  }
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(50);
  else FakeExplode(50,nil,nil,50);
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
