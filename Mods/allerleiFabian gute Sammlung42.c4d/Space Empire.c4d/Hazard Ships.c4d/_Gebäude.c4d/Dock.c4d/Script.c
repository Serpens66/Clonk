/*-- Dock --*/

#strict
local order, timer, light, height, product, brain, plr;

func Initialize()
{

}

func Check()
{
  if(product && ObjectDistance(this(),product)<80 || order)  return(0);
  return(1);
}

func Order(_order,_timer,_height,brained,player)
{
  brain=brained;
  plr=player;
  if(order)
  {
  Message("Produktion läuft!", this());
  return(0);
  }
  if(product && ObjectDistance(this(),product)<80)
  {
  Message("Dock noch belegt!", this());
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
  product=CreateObject(order,0,height/2);
  order=0;
  SetAction("Raise");
  }
  // Sound
  if(!Random(5))
  {
  Sound("PRReparatur*",0, this());
  }
  //Lichter
  light++;
  if(light>4) light=0;
  CreateParticle ("NoGravSpark", -27+light*13, -27, 0, 0, 20, RGBa(250,170,100,0)); 
  return(1);
}

func Raise()
{
  //Schon Oben?
  height--;
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
  SetPosition (GetX(), GetY(product)-1, product); 
  return(1);
}