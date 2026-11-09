#strict 3

func Initialize() {
Incinerate();
SetRDir(RandomX(-5,5));
SetAction("Destroyed");
return(1);
}

func Effects() {
  if (Not(OnFire())) return(1);
  Smoke(Sum(-20,Random(40)),Sum(-20,Random(10)),Sum(10,Random(20)));
  if (GreaterThan(GetActTime(),900)) Extinguish();
  return(1);
}