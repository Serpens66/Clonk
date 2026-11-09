/*---- Wraith ----*/

#strict
#include CLNK
#include HZCK

local SDestroy;

public func Zated()
{
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

protected func Initialize()
{
  SDestroy=1;
  return(inherited());
}

/* Kontextmenü */

public func FindKnockClonk() 
{
  if(FindObject(0,-15,-15,30,30,OCF_Alive(),"Lie",0,NoContainer()))
  {
   return(1); 
  }
  return();
}

//Die Funktion zum Aussaugen des Feindes

protected func ContextSuck() 
{
  [$CtxSuck$|Image=MEPU:4|Condition=FindKnockClonk]
  // Lebensenergie aussaugen

  if(GetAction(this()) eq "Suck")
  {
   SetAction("Walk");
   return(1);
  }

  if(!FindObject(0,-10,-10,20,20,OCF_Alive()))
  {
   Message("Hier ist keine Nahrung!",this());
   return(1);
  }

  SetAction("Suck");

  Sound("Confirm*");
}

protected func Suck()
{
  if(FindObject(0,-10,-10,20,20,OCF_Alive()))
  {
   FindObject(0,-10,-10,20,20,OCF_Alive())->SetAction("Lie");
   DoEnergy(-1,FindObject(0,-10,-10,20,20,OCF_Alive()));
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
  if(GetAction(this()) eq "Lie")
  {
   return();
  }

  if(SDestroy == 1)
  {
   return(1);
  }
  return();
}


private func Control2Contents (string command, par1, par2, par3, par4)
{
  if(GetAction() S= "Push")
    return(0);
  if(ObjectCall(Contents(), command, this(), par1, par2, par3, par4))
    return(1);
  return(0);
}