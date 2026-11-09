#strict 3

func Initialize() {
SetRDir(RandomX(-2,2));
SetAction("Destroyed");
return(1);
}

func Effects() {
  Sparks(10,RGB(nil,100,255),RandomX(-40,40),RandomX(-15,15));
  if (GreaterThan(GetActTime(),300)) SetAction("Over");
  return(1);
}