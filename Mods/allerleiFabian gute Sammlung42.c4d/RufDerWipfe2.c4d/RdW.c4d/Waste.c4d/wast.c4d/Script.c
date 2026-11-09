/*-- Materialhelfer --*/

#strict 3

static RdW_WASTE_Amount;  // Nicht wieder ausgeworfenes Mat
static RdW_WASTE_Types;   // IDs der weggeworfenen Müllarten
static RdW_WASTE_Count;   // Anzahl der wegeworfenen Dinge

//I ---------------------------- I//
///  Spukt das Zeug was          ///
///  weggeworfen wird wieder aus ///
//I ---------------------------- I//

func Initialize()
{
  if(!GetLength(RdW_WASTE_Types))
  {
    RdW_WASTE_Types=[HMWS];
    RdW_WASTE_Count=[1];
  }

  S42MigrationAssign(RdW_WASTE_Amount, "+=", ((GetMaterialVal("Dig2ObjectRatio","Material",Material("Waste")) ?? 0)-1));

  var waste;
  for(var i = nil; (i ?? 0) < (GetLength(RdW_WASTE_Types) ?? 0); S42MigrationStep(i, 1, true))
  {
    if(!RdW_WASTE_Count[(i ?? 0)])
    {
      DeleteArrayItem(i,RdW_WASTE_Count);
      DeleteArrayItem(i,RdW_WASTE_Types);
      S42MigrationStep(i, -1, true); continue;
    }
    if(Random((RdW_WASTE_Count[(i ?? 0)] ?? 0)+1))
    {
      waste=CreateObject(RdW_WASTE_Types[(i ?? 0)],nil,nil,-1)->~IsWaste();
      S42MigrationAssign(RdW_WASTE_Amount, "-=", (waste ?? 0));
      S42MigrationStep(RdW_WASTE_Count[(i ?? 0)], -1, true);
    }
  }
  
  if((RdW_WASTE_Amount ?? 0)>0)
  {
    waste=Random(RdW_WASTE_Amount);
    CastPXS("Waste",waste,10,nil,nil);
    S42MigrationAssign(RdW_WASTE_Amount, "-=", (waste ?? 0));
  }
  
  RemoveObject();
  return(1);
}
