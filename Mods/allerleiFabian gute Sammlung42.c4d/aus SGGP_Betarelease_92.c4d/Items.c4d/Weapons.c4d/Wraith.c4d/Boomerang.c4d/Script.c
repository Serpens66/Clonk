#strict

local iXDir,iYDir,thrower,disable;

/* --- Boomerang --- */

protected func Departure(pObj) 
{
  thrower = pObj;
  disable = 5;
  if(GetXDir())
    if(GetOCF(pObj)&OCF_Living())
      SetAction("Depart");
}

protected func Check()
{
  if(Contained())
  {
   thrower = 0;
  }

  if(disable != 0)
  {
   disable --;
  }
  
  if(thrower)
  {
   if(ObjectDistance(thrower) <= 10) 
   {
    if(disable == 0)
    {
     var help;
     if(GetX(thrower)-GetX() > 0)  { help = 0;}
     if(GetX(thrower)-GetX() < 0)  { help = 1;}
    
     if(GetDir(thrower) != help)
     {
      return(1);
     }
     Enter(thrower);
     thrower ->SetAction("Scale");
     return(1);
    } 
   }
  }
  return(1);
}

protected func Departure2() 
{
  if(Contained()) return(SetAction("Idle"));
  Sound("Catapult");
  SetXDir(GetXDir()*5);
  SetYDir(-50);
  SetRDir(GetXDir()*1);
  iXDir=(GetXDir()>0)*2-1;
  iYDir=20;
}

protected func Hit() 
{
  Spark();
  thrower = 0;
  Sound("BmHit");
  if(!Random(5)) return(1);
  if(Random(3)) SetRDir(0-GetRDir());
  if(!GBackSolid(0,-10)) SetYDir(-8);
  if(!ActIdle()) NormalFlight();
  if(GBackSolid(-10,0)) return(SetXDir(+15));
  if(GBackSolid(10,0))  return(SetXDir(-15));
  SetXDir(GetXDir()/2);
}

protected func FlyCheck() {
  if(--iYDir < 1) return(NormalFlight());
  SetXDir(GetXDir()-iXDir*10);
  if(iYDir>18) SetYDir(GetYDir()+12);
  if(iYDir<16) SetYDir(GetYDir()-10);
}

func NormalFlight() {
  SetRDir(GetRDir()/4);
  SetYDir(GetYDir()/1);
  SetAction("Idle");
  iXDir=0;
  iYDir=1;
}

func Spark()
{
  CreateParticle("SparkB",0,0,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  return(1);
}