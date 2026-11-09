#strict 3

func Hit(...) {
  Explode(1);
  if(InLiquid()) return(Sound("Splash*"));
  Sound("Earthquake");
  ShakeFree(15);
  ShakeFree(GetX(this()),Sum(GetY(this()),15),15);
  ShakeFree(GetX(this()),Sum(GetY(this()),30),17);
  ShakeFree(GetX(this()),Sum(GetY(this()),45),19);
  ShakeFree(GetX(this()),Sum(GetY(this()),60),21);
  ShakeFree(GetX(this()),Sum(GetY(this()),75),23);
  ShakeFree(GetX(this()),Sum(GetY(this()),90),25);
  return(1);
}
