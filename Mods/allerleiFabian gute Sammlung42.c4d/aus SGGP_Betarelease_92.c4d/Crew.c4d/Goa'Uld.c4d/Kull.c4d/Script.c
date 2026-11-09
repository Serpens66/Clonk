/*---- Kull ----*/

#strict
#include HZCK

static const HZCK_LADDERSPEED = 7;

func Initialize()
{
  if(GetName() eq "Nr.")
  {
   SetName("Nr. 1");
  }
  CreateContents(GOKW);
  CreateContents(GOKR);
  return(_inherited());
}

func RejectCollect(pObj)
{
  if(pObj != GOKR)
  {
   if(pObj != GOKW)
   {
    if(pObj != ENAP)
    {
     return(1);
    }
   }
  }
  return();
}

func Zated()
{
  if(HasGear())
  {
   return(1);
  }
  if(GetAction() eq "Lie")
  {
   SetAction("Lie");
   EMPShockEffect(50);
   return(1);
  }
  SetAction("Zatfall");
  EMPShockEffect(50);
  return(1);
}

func UpdateCharge()
{
  if(RandomX(0,2000) == 1)
  {
   DoEnergy(-1);
  }
  return(_inherited());
}