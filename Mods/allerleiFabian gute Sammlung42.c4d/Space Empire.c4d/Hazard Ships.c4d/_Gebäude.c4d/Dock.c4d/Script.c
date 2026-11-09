/*-- Dock --*/

#strict 3
local order, timer, light, height, product, brain, plr;

func Initialize()
{

}

func Check()
{
  if(Or(And(product, (ObjectDistance(this(),product) ?? 0)<80), order))  return(nil);
  return(1);
}

func Order(_order,_timer,_height,brained,player)
{
  brain=brained;
  plr=player;
  if(order)
  {
  Message("Produktion läuft!", this());
  return(nil);
  }
  if(And(product, (ObjectDistance(this(),product) ?? 0)<80))
  {
  Message("Dock noch belegt!", this());
  return(nil);
  }
  order=_order;
  timer=_timer;
  height=_height;
  light=nil;
  SetAction("Construct");
  return(1);
}

func Construct()
{
  //Bauzeit
  S42MigrationAssign(timer, "-=", 2);
  //Fertig?
  if((timer ?? 0)<0)
  {
  product=CreateObject(order,nil,(height ?? 0)/2);
  order=nil;
  SetAction("Raise");
  }
  // Sound
  if(!Random(5))
  {
  Sound("PRReparatur*",nil, this());
  }
  //Lichter
  S42MigrationStep(light, 1, true);
  if((light ?? 0)>4) light=nil;
  CreateParticle ("NoGravSpark", -27+(light ?? 0)*13, -27, nil, nil, 20, RGBa(250,170,100,nil));
  return(1);
}

func Raise()
{
  //Schon Oben?
  S42MigrationStep(height, -1, true);
  if(!height)
  {
  if(brain)
  {
  SetOwner(plr,product);
  CreateContents(BRA_,product);
  }
  SetAction("Wait");
  }
  //Hochlassen
  SetPosition (GetX(), (GetY(product) ?? 0)-1, product);
  return(1);
}