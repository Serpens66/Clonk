/*-- Fernrohr-Sichtfenster --*/

#strict 3

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
  if(InLiquid()) S42MigrationAssign(iDistance, "-=", 4);
   else S42MigrationStep(iDistance, -1);
  if((iDistance ?? 0)<1) RemoveObject();
  return(1);
}
  
protected func Hit()
{
   RemoveObject(); 
   return(1);
}