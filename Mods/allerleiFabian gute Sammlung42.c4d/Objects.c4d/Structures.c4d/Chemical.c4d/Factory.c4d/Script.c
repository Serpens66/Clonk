/*-- Frühe Chemielabor-Produktionsobjekt --*/

#strict
#include FCOB

public func IsWaste(){ return(20);}


/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (symbol==SYF3);
  CreateMenu(symbol,caller, this(),1,"Keine Baupläne verfügbar");
  // Standard-Zeugs
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),0,++num,C4D_Object()))
    if(plan->~IsChemicalProduct() || plan->~IsFlintProduct())
        // nur wenn in Forschungsbaum 0 bis 9 . ODER wenn Regel CHHH (alle chemieprodukte von anfang an) oder Regel SZRE (szenarioforschung) aktiv
      if((InArray(gForschungsliste(0), plan) || InArray(gForschungsliste(1), plan) || InArray(gForschungsliste(2), plan) || InArray(gForschungsliste(3), plan) || InArray(gForschungsliste(4), plan) || InArray(gForschungsliste(5), plan) || InArray(gForschungsliste(6), plan) || InArray(gForschungsliste(7), plan) || InArray(gForschungsliste(8), plan) || InArray(gForschungsliste(9), plan)) || FindObject(CHHH) || FindObject(SZRE))
        
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
  if(!Random(3)) CreateContents(MTWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(0);
  Sound("Pshshsh");
  return(1);
}


private func SpecialProduction(par0, ID)
{
  SelectProduction(ID, 500);
}


/* Raucherzeugung */

private func DoSmoke()
{
  if (!Random(6)) Smoke(+7, -27, 8);
  if (!Random(8)) Smoke(-5, -28, 5 + Random(3));
if (!Random(8))Sound("Chemie*");
}


/* Eigenschaften */

// private func NeedEnergy() { return(0); }

