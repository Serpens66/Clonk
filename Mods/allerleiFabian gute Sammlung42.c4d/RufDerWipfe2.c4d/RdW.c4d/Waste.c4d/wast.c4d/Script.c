/*-- Materialhelfer --*/

#strict

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

  RdW_WASTE_Amount+=(GetMaterialVal("Dig2ObjectRatio","Material",Material("Waste"))-1);

  var waste;
  for(var i = 0; i < GetLength(RdW_WASTE_Types); i++)
  {
    if(!RdW_WASTE_Count[i])
    {
      DeleteArrayItem(i,RdW_WASTE_Count);
      DeleteArrayItem(i,RdW_WASTE_Types);
      i--; continue;
    }
    if(Random(RdW_WASTE_Count[i]+1))
    {
      waste=CreateObject(RdW_WASTE_Types[i],0,0,-1)->~IsWaste();
      RdW_WASTE_Amount-=waste;
      RdW_WASTE_Count[i]--;
    }
  }
  
  if(RdW_WASTE_Amount>0)
  {
    waste=Random(RdW_WASTE_Amount);
    CastPXS("Waste",waste,10,0,0);
    RdW_WASTE_Amount-=waste;
  }
  
  RemoveObject();
  return(1);
}
