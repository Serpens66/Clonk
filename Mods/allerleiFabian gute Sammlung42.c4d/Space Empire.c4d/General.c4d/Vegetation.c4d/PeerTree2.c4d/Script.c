/*-- PeerTree --*/

#strict
#include TREE

local zaps;   // Zahl der Zaps im Inneren


protected func Initialize()
{
  // zufälliges Aussehen
  if (Random(2)) SetGraphics("");
            else SetGraphics("2");
  zaps = Random(13);
  return(_inherited());
}

public func Activity()
{
  // Zaps erzeugen
  if (!IsStanding() || (GetCon() < 100))
    return(0);
  zaps = Min(zaps+6, 12);
  var i = 0;
  for (i; i < 4; ++i)
    CreateObject(ZAP1, Random(50)-25, Random(10)-30, -1);
  return(1);
}

protected func Damage(value)
{
  // bei Schaden Zaps freisetzen
  if (!zaps || (value <= 0))
    return(_inherited());
  var z = Max(zaps, 3);
  zaps -= z;
  while (--z)
    CreateObject(ZAP1, Random(50)-25, Random(10)-30, -1);
  return(_inherited());
}

private ReproductionAreaSize: return(300);
private ReproductionRate: return(500);
private MaxTreeCount: return(20);
private TreeStrength: return(125);

