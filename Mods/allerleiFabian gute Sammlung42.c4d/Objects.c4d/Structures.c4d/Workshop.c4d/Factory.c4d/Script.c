/*-- Werkstatt-Produktionsobjekt --*/

#strict 3
#include FCOB

public func IsWaste(){ return(20);}

/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (Equal(symbol, SYF3));
  CreateMenu(symbol,caller, this(),1,"Keine Baupläne verfügbar");
  // Standard-Zeugs
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),nil,S42MigrationStep(num, 1),C4D_Vehicle))
    if(!(plan->~IsVehicleProduct())) // große oder neue fahrzeuge wie raumschiffe, die nur in raumschifffabrik herstellbar sein sollen
    if(!(plan->~IsRefineryProduct()))
      if(Or(Or(Or(Or(Or(Or(Or(Or(Or(InArray(gForschungsliste(nil), plan), InArray(gForschungsliste(1), plan)), InArray(gForschungsliste(2), plan)), InArray(gForschungsliste(3), plan)), InArray(gForschungsliste(4), plan)), InArray(gForschungsliste(5), plan)), InArray(gForschungsliste(6), plan)), InArray(gForschungsliste(7), plan)), InArray(gForschungsliste(8), plan)), InArray(gForschungsliste(9), plan)))
      
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
  if(!Random(3)) CreateContents(MTWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(nil);
  // Sound("Pshshsh");
  return(1);
}


private func SpecialProduction(par0, ID)
{
  SelectProduction(ID, 500);
}


/* Raucherzeugung */

private func DoSmoke()
{
  if (Random(12)) Smoke(+16,-20,8);
  if (Random(14)) Smoke(10,-24,5+(Random(3) ?? 0));
    if (!Random(14)) Sound("Schmiede*");
  return(1);
  return(1);
}


/* Eigenschaften */

// private func NeedEnergy() { return(0); }

