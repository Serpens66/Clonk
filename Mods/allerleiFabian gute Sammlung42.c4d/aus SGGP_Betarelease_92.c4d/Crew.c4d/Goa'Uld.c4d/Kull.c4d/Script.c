/*---- Kull ----*/

#strict 3
#include HZCK

static const HZCK_LADDERSPEED = 7;

func Initialize()
{
  if(!!SEqual(GetName(), "Nr."))
  {
   SetName("Nr. 1");
  }
  CreateContents(GOKW);
  CreateContents(GOKR);
  return(_inherited());
}

func RejectCollect(pObj)
{
  if(!Equal(pObj, GOKR))
  {
   if(!Equal(pObj, GOKW))
   {
    if(!Equal(pObj, ENAP))
    {
     return(1);
    }
   }
  }
  return;
}

func Zated()
{
  if(HasGear())
  {
   return(1);
  }
  if(!!SEqual(GetAction(), "Lie"))
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
  if(Equal(RandomX(nil,2000), 1))
  {
   DoEnergy(-1);
  }
  return(_inherited());
}