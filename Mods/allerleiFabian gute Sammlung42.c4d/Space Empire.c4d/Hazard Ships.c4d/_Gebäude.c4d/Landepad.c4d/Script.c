/*-- Dock --*/

#strict 3
local order, timer, light, height, product;

func Initialize()
{

}

func Check()
{
  if(Or(And(product, (ObjectDistance(this(),product) ?? 0)<80), order))  return(nil);
  return(1);
}

func Order(_order,_timer,_height)
{
  if(order)
  {
  return(nil);
  }
  if(And(product, (ObjectDistance(this(),product) ?? 0)<80))
  {
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
  product=CreateObject(order,nil,80+(GetY() ?? 0)-(LandscapeHeight() ?? 0));
  order=nil;
  if(GetID(Equal(product, MAM_))) ObjectSetAction(product,"Fly");
  product->SetComDir(COMD_Down);
  SetAction("Raise");
  }
  //Lichter
  S42MigrationStep(light, 1, true);
  if((light ?? 0)>6) light=nil;
  CreateParticle ("NoGravSpark", -95+(light ?? 0)*13, nil, nil, nil, 20, RGBa(250,170,100,nil));
  CreateParticle ("NoGravSpark", (+85 ?? 0)-(light ?? 0)*13, nil, nil, nil, 20, RGBa(250,170,100,nil));
  return(1);
}

func Raise()
{
  S42MigrationStep(light, 1, true);
  if((light ?? 0)>6) light=nil;
  CreateParticle ("NoGravSpark", -95+(light ?? 0)*13, nil, nil, nil, 20, RGBa(250,170,100,nil));
  CreateParticle ("NoGravSpark", (+85 ?? 0)-(light ?? 0)*13, nil, nil, nil, 20, RGBa(250,170,100,nil));
  //Schon Oben?
  if(Or((ObjectDistance(product,this()) ?? 0)<30, !Equal(GetX(), GetX(product))))
  {
  product->SetComDir(COMD_Stop);
  SetAction("Wait");
  }
  if(And(GetID(Equal(product, MAM_)), SEqual(GetAction(product),"Hover")))
  {
  ObjectSetAction(product,"Landed");
  SetAction("Wait");
  }
  return(1);
}