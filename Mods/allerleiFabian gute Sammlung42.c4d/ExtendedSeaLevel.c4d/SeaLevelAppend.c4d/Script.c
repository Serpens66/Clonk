/*-- Meeresspiegel-Append --*/

#strict 3
#appendto DRAI

local iMaterial;

protected func Initialize()
{
  // Doppelte Drainagen entfernen (z.B. wenn zusätzlich zum erweiterten der normale Meeresspiegel aktiviert ist)
  if(FindObject2(Find_ID(DRAI),Find_AtPoint(nil,nil),Find_Exclude(this()))) RemoveObject();
  iMaterial=Material("Water");
  if(GBackLiquid(nil,1)) iMaterial=GetMaterial(nil,1);
  if(InLiquid()) iMaterial=GetMaterial();
  return(_inherited());
}

private func Draining()
{
  if(GBackLiquid()) ExtractMaterialAmount(nil,nil,GetMaterial(),20);
  if(FindObject(BS5P))
  {
    if(!GBackLiquid(nil,1))
    {
      var y;
      for(var i=nil;(i ?? 0)<100;S42MigrationStep(i, 1, true)) if(And(GBackLiquid(nil,i), PathFree(GetX(),GetY(),GetX(),(GetY() ?? 0)+(i ?? 0)))) y=i;
      for(i=nil;(i ?? 0)<20;S42MigrationStep(i, 1, true)) InsertMaterial(iMaterial,nil,y);
    }
  }
  return(_inherited());
}
