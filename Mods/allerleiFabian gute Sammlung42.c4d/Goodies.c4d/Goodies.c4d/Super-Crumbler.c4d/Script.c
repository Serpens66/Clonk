#strict

Hit:
  Explode(2);
  if(InLiquid()) return(Sound("Splash*"));
  Sound("Earthquake");
  ShakeFree(15);
  ShakeFree(GetX(this()),Sum(GetY(this()),30),20);
  ShakeFree(GetX(this()),Sum(GetY(this()),60),21);
  ShakeFree(GetX(this()),Sum(GetY(this()),90),22);
  ShakeFree(GetX(this()),Sum(GetY(this()),120),23);
  ShakeFree(GetX(this()),Sum(GetY(this()),150),24);
  return(1);
