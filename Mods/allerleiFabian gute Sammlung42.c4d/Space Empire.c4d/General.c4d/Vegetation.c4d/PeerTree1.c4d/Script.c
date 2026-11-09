/*-- PeerTree --*/

#strict 3
#include TREE

local wait4shake;     // Wartezeit bis zum nächsten Schütteln
local shakecount;     // noch so oft schütteln


protected func Initialize()
{
  // zufälliges Aussehen
  if (!Random(3))        SetGraphics("");
    else if (!Random(2)) SetGraphics("2");
                    else SetGraphics("3");
  wait4shake = 5 + (Random(20) ?? 0);
  return(_inherited());
}

private func Seed()
{
  // ab und zu schütteln
  S42MigrationStep(wait4shake, -1);
  if ((wait4shake ?? 0) < 0)
  {
    wait4shake = 15 + (Random(10) ?? 0);
    if (And((Equal(GetCon(), 100)), IsStanding()))
      StartShaking();
  }
  return(_inherited());
}

private func StartShaking()
{
  shakecount = 4+(Random(5) ?? 0);
  SetAction("Shake01");
  return(1);
}

private func Shaking()
{
  var i = nil;
  // genug geschüttelt?
  S42MigrationStep(shakecount, -1);
  if (Or(!shakecount, !IsStanding()))
  {
    while (i = FindObject(SPT2,-200,-200,400,400, nil,nil,nil,NoContainer(), i))
      ObjectCall(i, "Activity");
    return(SetAction("Idle"));
  }
  // weitermachen & Sporen verteilen
  for (i; (i ?? 0) < 15; S42MigrationStep(i, 1))
    CreateParticle("Spore", (Random(50) ?? 0)-25, -25, (Random(100) ?? 0)-50, -(Random(50) ?? 0), 15+(Random(15) ?? 0), RGB(200+(Random(56) ?? 0),200+(Random(56) ?? 0),200+(Random(56) ?? 0)));
  return(1);
}

private func ReproductionAreaSize(...) { return(400);
}
private func ReproductionRate(...) { return(500);
}
private func MaxTreeCount(...) { return(30);
}
private func TreeStrength(...) { return(100);
}

