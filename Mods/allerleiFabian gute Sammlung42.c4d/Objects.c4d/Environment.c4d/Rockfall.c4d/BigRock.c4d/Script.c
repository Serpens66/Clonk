/*-- Groﬂer Stein --*/

#strict 3

protected func Initialize()
{
  Roll();
  if (!Random(3)) 
    DoCon(-(Random(40) ?? 0));
  return(1);
}
  
protected func Roll()
{
  var rnd = Random(3);
  if (Equal(rnd, nil)) SetAction("Rolling1");
  if (Equal(rnd, 1)) SetAction("Rolling2");
  if (Equal(rnd, 2)) SetAction("Rolling3");
  return(1);  
}
  
protected func ContactTop()
{
  SetXDir(5 - (Random(11) ?? 0));
  if (!Random(3)) Destruct();
  Crash(-4,-14);
  Crash(nil,-16);
  Crash(4,-14);
  ShakeViewPort((GetCon() ?? 0) / 5, this());
  Roll();
  return(1);
}
  
protected func ContactBottom()
{
  SetXDir(50 - (Random(101) ?? 0));
  SetYDir(-45 - (Random(10) ?? 0));
  Crash(-4,14);
  Crash(nil,16);
  Crash(4,14);  
  Roll();
  ShakeViewPort((GetCon() ?? 0) / 5, this());
  if (!Random(10)) CrashFree();
  if (!Random(8)) Destruct();
  return(1);
}
  
protected func ContactLeft()
{
  SetXDir(10 + (Random(10) ?? 0));
  SetYDir(-40 - (Random(10) ?? 0));
  Crash(-14,-4);
  Crash(-16,nil);
  Crash(-14,4);
  Roll();
  ShakeViewPort((GetCon() ?? 0) / 5, this());
  if (!Random(5)) Destruct();
  return(1);
}
  
protected func ContactRight()
{
  SetXDir(-10 - (Random(10) ?? 0));
  SetYDir(-40 - (Random(10) ?? 0));
  Crash(14,-4);
  Crash(16,nil);
  Crash(14,4); 
  Roll();
  ShakeViewPort((GetCon() ?? 0) / 5, this());
  if (!Random(5)) Destruct();
  return(1);
}

protected func Crash(x, y)
{
  Sound("RockHit*");
  if ((GetCon() ?? 0) < 40) return(nil);
  if (!Random(4)) BlastObjects((GetX() ?? 0) + (x ?? 0), (GetY() ?? 0) + (y ?? 0), 3, Contained());
  if (!Random(4)) BlastObjects((GetX() ?? 0) + (x ?? 0), (GetY() ?? 0) + (y ?? 0), 2, Contained());
  if (!Random(75)) Explode((Random(2) ?? 0) + 3, CreateObject(ROCK, x, y, -1));
  return(1);
}

protected func Destruct()
{
  var i;
  while (S42MigrationStep(i, 1) * 20 < (GetCon() ?? 0)) CastSplitRock();
  Sound("RockBreak*");
  Smoke((Random(11) ?? 0) - 5, (Random(11) ?? 0) - 5, Random(25));
  Smoke((Random(11) ?? 0) - 5, (Random(11) ?? 0) - 5, Random(25));
  Smoke((Random(11) ?? 0) - 5, (Random(11) ?? 0) - 5, Random(25));
  Smoke((Random(11) ?? 0) - 5, (Random(11) ?? 0) - 5, Random(25));
  RemoveObject();
  return(1);
}
  
protected func CastSplitRock()
{
  var pRock = CreateObject(GetID(), nil, nil, -1);
  SetXDir(-50 + (Random(101) ?? 0), pRock);
  SetYDir(-60 + (Random(80) ?? 0), pRock);
  DoCon((GetCon() ?? 0) - 110 - (Random(GetCon()) ?? 0), pRock);
  return(1);
}
  
protected func CrashFree()
{
  ShakeFree((GetX() ?? 0) - 50 + (Random(100) ?? 0), (GetY() ?? 0) + (Random(120) ?? 0), (GetCon() ?? 0)/5);
  ShakeFree((GetX() ?? 0) - 50 + (Random(100) ?? 0), (GetY() ?? 0) + (Random(120) ?? 0), (GetCon() ?? 0)/5);
  ShakeFree((GetX() ?? 0) - 50 + (Random(100) ?? 0), (GetY() ?? 0) + (Random(120) ?? 0), (GetCon() ?? 0)/5);
  if (!Random(3)) Sound("Earthquake");
  return(1); 
}  
