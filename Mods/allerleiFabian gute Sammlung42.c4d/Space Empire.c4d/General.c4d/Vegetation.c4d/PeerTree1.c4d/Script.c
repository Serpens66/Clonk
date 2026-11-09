/*-- PeerTree --*/

#strict
#include TREE

local wait4shake;     // Wartezeit bis zum nächsten Schütteln
local shakecount;     // noch so oft schütteln


protected func Initialize()
{
  // zufälliges Aussehen
  if (!Random(3))        SetGraphics("");
    else if (!Random(2)) SetGraphics("2");
                    else SetGraphics("3");
  wait4shake = 5 + Random(20);
  return(_inherited());
}

private func Seed()
{
  // ab und zu schütteln
  --wait4shake;
  if (wait4shake < 0)
  {
    wait4shake = 15 + Random(10);
    if ((GetCon() == 100) && IsStanding())
      StartShaking();
  }
  return(_inherited());
}

private func StartShaking()
{
  shakecount = 4+Random(5);
  SetAction("Shake01");
  return(1);
}

private func Shaking()
{
  var i = 0;
  // genug geschüttelt?
  --shakecount;
  if (!shakecount || !IsStanding())
  {
    while (i = FindObject(SPT2,-200,-200,400,400, 0,0,0,NoContainer(), i))
      ObjectCall(i, "Activity");
    return(SetAction("Idle"));
  }
  // weitermachen & Sporen verteilen
  for (i; i < 15; ++i)
    CreateParticle("Spore", Random(50)-25, -25, Random(100)-50, -Random(50), 15+Random(15), RGB(200+Random(56),200+Random(56),200+Random(56)));
  return(1);
}

private ReproductionAreaSize: return(400);
private ReproductionRate: return(500);
private MaxTreeCount: return(30);
private TreeStrength: return(100);

