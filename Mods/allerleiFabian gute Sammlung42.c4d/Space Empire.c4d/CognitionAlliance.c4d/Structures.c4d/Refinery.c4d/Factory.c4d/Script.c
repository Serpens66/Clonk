/*-- Raffinerie-Produktionsobjekt --*/

#strict
#include FCOB

public func IsWaste(){ return(10);}

/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (symbol==SYF3);
  CreateMenu(symbol,caller,this(),1,"Keine Baupläne verfügbar");
  // sonstiges Zeugs
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),0,++num,C4D_Object()))
    if (DefinitionCall(plan,"IsRefineryProduct") || DefinitionCall(plan,"IsAnvilProduct"))
     if (!DefinitionCall(plan,"IsChemicalProduct"))
      if (!DefinitionCall(plan,"IsWeaponProduct"))
      
    if(GetLength(hashSpielRegeln))
        {
            var ProdCostAnzeige = 0;
              ProdCostAnzeige = GetValue(0,plan) * HashGet(hashSpielRegeln, "Produktionskosten")/200;
            if (HashGet(hashSpielRegeln, "Produktionskosten"))
                AddMenuItem(Format("%d C. %s herstellen",ProdCostAnzeige,GetName(0,plan)), "SelectProduction", plan,caller,0,caller);
            else
                AddMenuItem("%s herstellen", "SelectProduction", plan,caller,0,caller);
        }
      else
        AddMenuItem("%s herstellen", "SelectProduction", plan,caller,0,caller);
  
  return(1);
}

private func ProductionEndCall(productID)
{
  // überladen, wenn bei Fertigstellung was anderes passieren soll
  var obj = CreateContents(productID, Factory);
if(!Random(15)) CreateContents(CHMU,Factory);
if(!Random(10)) CreateContents(MTWS,Factory);
if(!Random(8)) CreateContents(HMWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(0);
  Sound("Pshshsh");
  return(1);
}

/* Raucherzeugung */

private func DoSmoke()
{
  if (!Random(6)) Smoke(-1, -34, Random(16));
  if (!Random(8)) Smoke(10, -30, Random(16));
  if (!Random(3))
    Sound("PrRefinery"); 
  return(1);
}

