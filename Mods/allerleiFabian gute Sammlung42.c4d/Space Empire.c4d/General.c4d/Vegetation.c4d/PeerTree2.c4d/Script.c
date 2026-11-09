/*-- PeerTree --*/

#strict 3
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
  if (Or(!IsStanding(), ((GetCon() ?? 0) < 100)))
    return(nil);
  zaps = Min((zaps ?? 0)+6, 12);
  var i = nil;
  for (i; (i ?? 0) < 4; S42MigrationStep(i, 1))
    CreateObject(ZAP1, (Random(50) ?? 0)-25, (Random(10) ?? 0)-30, -1);
  return(1);
}

protected func Damage(value)
{
  // bei Schaden Zaps freisetzen
  if (Or(!zaps, ((value ?? 0) <= 0)))
    return(_inherited());
  var z = Max(zaps, 3);
  S42MigrationAssign(zaps, "-=", (z ?? 0));
  while (S42MigrationStep(z, -1))
    CreateObject(ZAP1, (Random(50) ?? 0)-25, (Random(10) ?? 0)-30, -1);
  return(_inherited());
}

private func ReproductionAreaSize(...) { return(300);
}
private func ReproductionRate(...) { return(500);
}
private func MaxTreeCount(...) { return(20);
}
private func TreeStrength(...) { return(125);
}

