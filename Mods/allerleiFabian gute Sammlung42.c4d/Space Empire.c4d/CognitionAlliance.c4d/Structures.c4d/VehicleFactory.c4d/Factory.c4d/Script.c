/*-- Fahrzeugfabrik-Produktionsobjekt --*/

#strict 3
#include FCOB
#include XFER   // Transfer-Script

local Hangar;

public func IsWaste(){ return(10);}

/* Initialisierung */

public func Init(par_factory, par_hangar)
{
  Factory = par_factory;
  Hangar = par_hangar;
}


/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (Equal(symbol, SYF3));
  CreateMenu(symbol,caller,this(),1,"Keine Baupläne verfügbar");
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),nil,S42MigrationStep(num, 1),C4D_Vehicle))
   if (DefinitionCall(plan,"IsVehicleProduct"))
       
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


/* spezielle Endroutine bei Produktion */

private func ProductionEndCall(productID)
{
  var obj = CreateContents(productID, Hangar);
if(!Random(2)) CreateContents(CHMU,Factory);
 CreateContents(MTWS,Factory);
if(!Random(2)) CreateContents(HMWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(nil);
  Sound("Pshshsh");
  return(1);
}


/* Raucherzeugung */

private func DoSmoke()
{
  if (!Random(8)) Smoke(-1, -34, Random(16));
  if (!Random(9)) Smoke(10, -30, Random(16));
if (!Random(12)) Sound("Schmiede*");
if (!Random(3))
    Sound("PrRefinery"); 
}

