/*-- Fernrohr-Sichtfenster --*/

#strict

local iDistance;
local iYDir;

protected func Initialize()
{
   iDistance=100;
   SetAction("Fly");
   return(1);
}

protected func Fly()
{
  SetYDir(iYDir,this());
  if(InLiquid()) iDistance-=4;
   else --iDistance;
  if(iDistance<1) RemoveObject();
  return(1);
}
  
protected func Hit()
{
   RemoveObject(); 
   return(1);
}