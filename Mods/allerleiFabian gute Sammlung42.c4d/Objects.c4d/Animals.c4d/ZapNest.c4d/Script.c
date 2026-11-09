/*-- Zapnest --*/

#strict 3

/* Aufschlag */

protected func Hit()
  {
  return(BreakUp(28));
  }

public func BreakUp(int num, bool alchemy)
{
  num ??= 0;
  alchemy ??= false;
  if (alchemy)
  {
  var offsetx=(2*(GetDir(Contained(this)) ?? 0)-1)*15;
  var offsety= 8;
  }
  var x,y;
  if(!num) num=28;
  for (var i=nil;(i ?? 0)<(num ?? 0);S42MigrationStep(i, 1))
  {
    x=(offsetx ?? 0)+(Random(11) ?? 0)-5;
    y=(offsety ?? 0)+(Random(11) ?? 0)-5;
    if (!GBackSolid(x,y))
      CreateObject(ZAP1,x,y,-1);
  }
  RemoveObject();
}

public func IsAlchemContainer() { return(true); }
public func AlchemProcessTime() { return(100); }

public func Exhaustion() { BreakUp(14,1); return(1); }