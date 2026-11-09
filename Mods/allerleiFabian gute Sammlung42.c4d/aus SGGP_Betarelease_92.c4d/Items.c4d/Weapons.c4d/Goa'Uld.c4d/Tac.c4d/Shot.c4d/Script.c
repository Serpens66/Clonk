#strict
#include SHT1


public func BulletStrike(object pObj) 
{
  if(pObj)
  {   
   pObj->DoDamage(2);
   if(pObj)
   {
    pObj->DoEnergy(-2);
   }
  }
  return(1);
}