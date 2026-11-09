/*-- Fabrik-Objekt --*/

#strict 3
#include FCOB

public func IsWaste(){ return(10);}


/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen
  Endless = (Equal(symbol, SYF3));
  CreateMenu(symbol,caller, this(),1,"Keine Baupläne vorhanden");
  // mit weiteren Bauplänen des Spielers füllen
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),nil,S42MigrationStep(num, 1),C4D_Object))
    if (DefinitionCall(plan,"IsChemicalProduct"))
      
  if(GetLength(hashSpielRegeln))
        {
            var ProdCostAnzeige = nil;
              ProdCostAnzeige = (GetValue(nil,plan) ?? 0) * (HashGet(hashSpielRegeln, "Produktionskosten") ?? 0)/200;
            if (HashGet(hashSpielRegeln, "Produktionskosten"))
                AddMenuItem(Format("%d C. %s herstellen",ProdCostAnzeige,GetName(nil,plan)), "SelectProduction", plan,caller,nil,caller);
            else
                AddMenuItem("%s herstellen", "SelectProduction", plan,caller,nil,caller);
        }
      else
        AddMenuItem("%s herstellen", "SelectProduction", plan,caller,nil,caller);
    
  return(1);
}

private func SpecialProduction(par0, ID)
{
  SelectProduction(ID, 500);
}

private func ProductionEndCall(productID)
{
  // überladen, wenn bei Fertigstellung was anderes passieren soll
  var obj = CreateContents(productID, Factory);
if(!Random(15)) CreateContents(CHMU,Factory);
if(!Random(25)) CreateContents(MTWS,Factory);
if(!Random(10)) CreateContents(HMWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(nil);
  Sound("Pshshsh");
  return(1);
}


/* Raucheffekt */

private func DoSmoke()
{
  // Fabrikschlote rauchen lassen
  if (!Random(6)) Smoke(-28,-30, 5+(Random(6) ?? 0));
  if (!Random(4)) Smoke(-31,-27, 5+(Random(6) ?? 0));
  // if (!Random(10))
    Sound("PrChemical"); 
  return(1);
}


/* Eigenschaften */

// private NeedEnergy: return(0); // wenn 0, dann keine energie nötig

