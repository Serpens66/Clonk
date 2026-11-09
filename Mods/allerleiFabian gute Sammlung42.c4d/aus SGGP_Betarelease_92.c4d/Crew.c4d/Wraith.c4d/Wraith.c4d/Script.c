/*---- Wraith ----*/

#strict 3
#include CLNK
#include HZCK

local SDestroy;

public func Zated()
{
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

protected func Initialize()
{
  SDestroy=1;
  return(inherited());
}

/* Kontextmenü */

public func FindKnockClonk() 
{
  if(FindObject(nil,-15,-15,30,30,OCF_Alive,"Lie",nil,NoContainer()))
  {
   return(1); 
  }
  return;
}

//Die Funktion zum Aussaugen des Feindes

protected func ContextSuck() 
{
  [$CtxSuck$|Image=MEPU:4|Condition=FindKnockClonk]
  // Lebensenergie aussaugen

  if(!!SEqual(GetAction(this()), "Suck"))
  {
   SetAction("Walk");
   return(1);
  }

  if(!FindObject(nil,-10,-10,20,20,OCF_Alive))
  {
   Message("Hier ist keine Nahrung!",this());
   return(1);
  }

  SetAction("Suck");

  Sound("Confirm*");
}

protected func Suck()
{
  if(FindObject(nil,-10,-10,20,20,OCF_Alive))
  {
   FindObject(nil,-10,-10,20,20,OCF_Alive)->SetAction("Lie");
   DoEnergy(-1,FindObject(nil,-10,-10,20,20,OCF_Alive));
   DoEnergy(1,this());
   SetAction("Suck");
   return(1);
  }
  SetAction("Walk");
  return(1);
}

protected func ContextDestroy()
{
  [$CtxSDestroy$|Image=MEPU:1|Condition=DestroyerCheck]
  Schedule("Explode(60)",100);
  Sound("WRReady");
  return(1);
}


private func DestroyerCheck() 
{
  if(!!SEqual(GetAction(this()), "Lie"))
  {
   return;
  }

  if(Equal(SDestroy, 1))
  {
   return(1);
  }
  return;
}


private func Control2Contents (string command, par1, par2, par3, par4)
{
  if(!!SEqual(GetAction(), "Push"))
    return(nil);
  if(ObjectCall(Contents(), command, this(), par1, par2, par3, par4))
    return(1);
  return(nil);
}