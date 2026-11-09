#strict 3

local iXDir,iYDir,thrower,disable;

/* --- Boomerang --- */

protected func Departure(pObj) 
{
  thrower = pObj;
  disable = 5;
  if(GetXDir())
    if((GetOCF(pObj) ?? 0)&(OCF_Living ?? 0))
      SetAction("Depart");
}

protected func Check()
{
  if(Contained())
  {
   thrower = nil;
  }

  if(!Equal(disable, nil))
  {
   S42MigrationStep(disable , -1, true);
  }
  
  if(thrower)
  {
   if((ObjectDistance(thrower) ?? 0) <= 10)
   {
    if(Equal(disable, nil))
    {
     var help;
     if((GetX(thrower) ?? 0)-(GetX() ?? 0) > 0)  { help = nil;}
     if((GetX(thrower) ?? 0)-(GetX() ?? 0) < 0)  { help = 1;}
    
     if(!Equal(GetDir(thrower), help))
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
  SetXDir((GetXDir() ?? 0)*5);
  SetYDir(-50);
  SetRDir((GetXDir() ?? 0)*1);
  iXDir=(((GetXDir() ?? 0)>0) ?? 0)*2-1;
  iYDir=20;
}

protected func Hit() 
{
  Spark();
  thrower = nil;
  Sound("BmHit");
  if(!Random(5)) return(1);
  if(Random(3)) SetRDir(0-(GetRDir() ?? 0));
  if(!GBackSolid(nil,-10)) SetYDir(-8);
  if(!ActIdle()) NormalFlight();
  if(GBackSolid(-10,nil)) return(SetXDir(+15));
  if(GBackSolid(10,nil))  return(SetXDir(-15));
  SetXDir((GetXDir() ?? 0)/2);
}

protected func FlyCheck() {
  if(S42MigrationStep(iYDir, -1) < 1) return(NormalFlight());
  SetXDir((GetXDir() ?? 0)-(iXDir ?? 0)*10);
  if((iYDir ?? 0)>18) SetYDir((GetYDir() ?? 0)+12);
  if((iYDir ?? 0)<16) SetYDir((GetYDir() ?? 0)-10);
}

func NormalFlight() {
  SetRDir((GetRDir() ?? 0)/4);
  SetYDir((GetYDir() ?? 0)/1);
  SetAction("Idle");
  iXDir=nil;
  iYDir=1;
}

func Spark()
{
  CreateParticle("SparkB",nil,nil,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  return(1);
}