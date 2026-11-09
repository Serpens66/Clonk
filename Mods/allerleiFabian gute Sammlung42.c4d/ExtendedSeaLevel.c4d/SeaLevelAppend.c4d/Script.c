/*-- Meeresspiegel-Append --*/

#strict
#appendto DRAI

local iMaterial;

protected func Initialize()
{
  // Doppelte Drainagen entfernen (z.B. wenn zusätzlich zum erweiterten der normale Meeresspiegel aktiviert ist)
  if(FindObject2(Find_ID(DRAI),Find_AtPoint(0,0),Find_Exclude(this()))) RemoveObject();
  iMaterial=Material("Water");
  if(GBackLiquid(0,1)) iMaterial=GetMaterial(0,1);
  if(InLiquid()) iMaterial=GetMaterial();
  return(_inherited());
}

private func Draining()
{
  if(GBackLiquid()) ExtractMaterialAmount(0,0,GetMaterial(),20);
  if(FindObject(BS5P))
  {
    if(!GBackLiquid(0,1))
    {
      var y;
      for(var i=0;i<100;i++) if(GBackLiquid(0,i) && PathFree(GetX(),GetY(),GetX(),GetY()+i)) y=i;
      for(i=0;i<20;i++) InsertMaterial(iMaterial,0,y);
    }
  }
  return(_inherited());
}
