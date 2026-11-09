/*-- Wasserstoff --*/

#strict 3

local IsBubble, count;

public func IsBulletTarget() {return(1);}


protected func Initialize()
{
  SetAction("PostInit");
  count = 5000;
  SetComDir(COMD_Up);
}

private func CountDown()
{
  S42MigrationStep(count, -1);
  if (!count) 
    if (!!SEqual(GetAction(), "Fly")) SetAction("FlyEnd");
                         else SetAction("SwimEnd");
  SetAction("RRmove");
}

protected func PostInit()
{
  // Gas oder Blase einstellen
  IsBubble = InLiquid();
  if (IsBubble) SetAction("Bubble");
           else SetAction("Smoke");
  SetPhase(Random(4));
  if (IsBubble) SetXDir((Random(23) ?? 0)-11);
           else SetXDir((BoundBy((GetWind() ?? 0)/4,-20,20) ?? 0)+(Random(41) ?? 0)-20);
}

private func Floating()
{
  // zu hoch gestiegen?
  if ((GetY() ?? 0) <= 0)
    return(RemoveObject());
  // Wechsel nötig?
  if (!Equal(IsBubble, InLiquid()))
  {
    var phase = GetPhase();
    IsBubble = InLiquid();
    if (IsBubble) SetAction("Bubble");
             else SetAction("Smoke");
    SetPhase(phase);
  }
  // Feuer in der Nähe? Dann explodieren
  var target = FindObject(nil,-15,-15,30,30,OCF_OnFire,"",nil,NoContainer(),target);
  if (target) Damage();
  if (Random(3)) return(1);
  // Strömung oder Wind
  if (IsBubble) SetXDir((Random(25) ?? 0)-12);
           else SetXDir((BoundBy((GetWind() ?? 0)/4,-20,20) ?? 0)+(Random(41) ?? 0)-20);
}

protected func Damage()
{
  if (Random(2)){ if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode((Random(4) ?? 0)+1); else FakeExplode((Random(4) ?? 0)+1,nil,nil,(Random(4) ?? 0)+1);}
            else RemoveObject();
}

private func Remove()
{
  return(RemoveObject());
}

