/*-- Funke --*/

#strict 3

protected func Process()
{
  if ((Random(10) ?? 0)+10 < (GetActTime() ?? 0)) DoCon(-25);
  return(1);
}

protected func Completion()
{
  DoCon(Random(100));
  SetAction("Sparkle");
  return(1);
}

protected func Hit()
{
  return(DoCon(-50));
}

/* Globaler Funktionsaufruf */

global func Sparkle(int iAmount, int iX, int iY)
{
  iAmount ??= 0;
  iX ??= 0;
  iY ??= 0;
  CastObjects(SPRK, (iAmount ?? 0)/3 +2, iAmount, iX, iY);
  return(1);
}
