/*-- Fabrik-Objekt --*/

#strict 3
#include FCOB

public func IsWaste(){ return(10);}

/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (Equal(symbol, SYF3));
  CreateMenu(symbol,caller, this(),1,"Keine Baupläne vorhanden");
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),nil,S42MigrationStep(num, 1),C4D_All))
    if (DefinitionCall(plan,"IsWeaponProduct"))
        
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

private func ProductionEndCall(productID)
{
  // überladen, wenn bei Fertigstellung was anderes passieren soll
  var obj = CreateContents(productID, Factory);
if(!Random(10)) CreateContents(CHMU,Factory);
if(!Random(5)) CreateContents(MTWS,Factory);
if(!Random(3)) CreateContents(HMWS,Factory);
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
  if (!Random(10)) Smoke(-40,-26, 5+(Random(6) ?? 0));
  if (!Random(11)) Smoke(-38,-26, 5+(Random(6) ?? 0));
  if (!Random(12)) Smoke(-28,-26, 5+(Random(6) ?? 0));
  if (!Random(13)) Smoke(-23,-26, 5+(Random(6) ?? 0));
  // if (!Random(10))
    Sound("PrWeapon"); 
  return(1);
}

