/*-- Fabrik-Objekt --*/

#strict
#include FCOB

public func IsWaste(){ return(10);}

/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  Endless = (symbol==SYF3);
  CreateMenu(symbol,caller, this(),1,"Keine Baupläne vorhanden");
  var num = -1;
  var plan;
  while (plan = GetPlrKnowledge(GetController(caller),0,++num,C4D_All()))
    if (DefinitionCall(plan,"IsWeaponProduct"))
        
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
if(!Random(10)) CreateContents(CHMU,Factory);
if(!Random(5)) CreateContents(MTWS,Factory);
if(!Random(3)) CreateContents(HMWS,Factory);
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  if (!obj) return(0);
  Sound("Pshshsh");
  return(1);
}


/* Raucheffekt */

private func DoSmoke()
{
  // Fabrikschlote rauchen lassen
  if (!Random(10)) Smoke(-40,-26, 5+Random(6));
  if (!Random(11)) Smoke(-38,-26, 5+Random(6));
  if (!Random(12)) Smoke(-28,-26, 5+Random(6));
  if (!Random(13)) Smoke(-23,-26, 5+Random(6));
  // if (!Random(10))
    Sound("PrWeapon"); 
  return(1);
}

