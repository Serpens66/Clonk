/*-- Dock --*/

#strict
local order, timer, light, height, product;

func Initialize()
{

}

func Check()
{
  if(product && ObjectDistance(this(),product)<80 || order)  return(0);
  return(1);
}

func Order(_order,_timer,_height)
{
  if(order)
  {
  return(0);
  }
  if(product && ObjectDistance(this(),product)<80)
  {
  return(0);
  }
  order=_order;
  timer=_timer;
  height=_height;
  light=0;
  SetAction("Construct");
  return(1);
}

func Construct()
{
  //Bauzeit
  timer-=2;
  //Fertig?
  if(timer<0)
  {
  product=CreateObject(order,0,80+GetY()-LandscapeHeight());
  order=0;
  if(GetID(product==MAM_)) ObjectSetAction(product,"Fly");
  product->SetComDir(COMD_Down());
  SetAction("Raise");
  }
  //Lichter
  light++;
  if(light>6) light=0;
  CreateParticle ("NoGravSpark", -95+light*13, 0, 0, 0, 20, RGBa(250,170,100,0)); 
  CreateParticle ("NoGravSpark", +85-light*13, 0, 0, 0, 20, RGBa(250,170,100,0)); 
  return(1);
}

func Raise()
{
  light++;
  if(light>6) light=0;
  CreateParticle ("NoGravSpark", -95+light*13, 0, 0, 0, 20, RGBa(250,170,100,0)); 
  CreateParticle ("NoGravSpark", +85-light*13, 0, 0, 0, 20, RGBa(250,170,100,0)); 
  //Schon Oben?
  if(ObjectDistance(product,this())<30 || GetX()!=GetX(product))
  {
  product->SetComDir(COMD_Stop()); 
  SetAction("Wait");
  }
  if(GetID(product==MAM_)&&SEqual(GetAction(product),"Hover"))
  {
  ObjectSetAction(product,"Landed");
  SetAction("Wait");
  }
  return(1);
}