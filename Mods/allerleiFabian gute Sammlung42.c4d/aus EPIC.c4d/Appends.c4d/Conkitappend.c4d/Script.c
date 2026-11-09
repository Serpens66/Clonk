/*-- Neues Objekt --*/

#strict 3
#appendto CNKT

protected func CreateConstructionSite(idType,caller)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (And(!SEqual(GetAction(Contained()), "Walk"), !SEqual(GetAction(Contained()), "HoldWalk"))) return(nil);
  if (Contained(Contained())) return(nil);
  var check = 1;
  var fterrain = 1; // bestimmt, ob untergrund an baustelle angepasst werden soll
  var plusY = 10;

  // spezialbehandlungen
  if (Equal(idType, BEKP))
  		{
                if(And(And(!(FindObject(BELC, -40, -10, 80, 20)), (!(FindObject(CHLP, -40, -10, 80, 20)))), (!(FindObject(SLP1, -40, -10, 80, 20)))) ) return(Message("<c ff0000>Hier ist kein Fahrstuhlkorb!", this()));
  		var pElev=FindObject(BELC, -40, -10, 80, 20);
    if (!pElev) pElev=FindObject(CHLP, -40, -10, 80, 20);
    if (!pElev) pElev=FindObject(SLP1, -40, -10, 80, 20);
        plusY=11;
                CreateConstruction(BEKP, AbsX(GetX(pElev)), AbsY((GetY(pElev) ?? 0)+(plusY ?? 0)), GetOwner(Contained()), 1, fterrain, nil);
  		return(Restzeug(idType,caller,pSite));
  		}

    // für Superkuppel und Bunkerteile, Deckenlicht und SEGU egal ob iwas im Weg. 
  if ( Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or((Equal(idType, SPDM)), (Equal(idType, SPD2))), (Equal(idType, SPD3))), (Equal(idType, SPD4)))
 , (Equal(idType, BE5C))), (Equal(idType, HL0D))), (Equal(idType, HR0D))), (Equal(idType, BH5C))), (Equal(idType, RR5C))), (Equal(idType, BT5C))), (Equal(idType, BG5C))), (Equal(idType, BL5C))), (Equal(idType, BR5C))), (Equal(idType, BP5C))), (Equal(idType, B15C))), (Equal(idType, HR6Y))), (Equal(idType, BW5C))), (Equal(idType, SEGU))), (Equal(idType, CLG3))), (Equal(idType, TG6B))))
      {
      plusY=15;
      if (Equal(idType, TG6B)) {fterrain = nil;plusY=11;} // Mienentor
      CreateConstruction(idType,AbsX(GetX(Contained())), (AbsY(GetY(Contained())) ?? 0)+(plusY ?? 0), GetOwner(Contained()), 1, fterrain, nil);
      return(Restzeug(idType,caller,pSite));
      }
  
  if(Equal(idType, GRS3)) check=nil; // fuer flaschenzug kein untergrund check

  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(nil, 10, Contained()) ) return(nil);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if ((GetDefCoreVal ("Height","DefCore",idType) ?? 0) > (GetY() ?? 0)) return(Message("Cannot exceed map height!"));
  if (!(pSite = CreateConstruction(idType, nil, plusY, GetOwner(Contained()), 1, fterrain,check))) return(nil);
  
  return(Restzeug(idType,caller,pSite));
}

func Restzeug(idType,caller,pSite)
{
    // Meldung
  Message("$TxtConstructions2$", Contained(), GetName(pSite));
  
  // wenn baustelle erfolgreich platziert, geld abziehen
  var owner = GetOwner(caller);
  var ProdCost = nil;
  if(GetLength(hashSpielRegeln))
      ProdCost = (GetMass(nil,idType) ?? 0) * (HashGet(hashSpielRegeln, "Baustellenkosten") ?? 0)/1000;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if((GetWealth(owner) ?? 0)< (ProdCost ?? 0))
        return(S42MigrationFirst(Message("Nicht genug Geld",this()),Sound("Error"),RemoveObject(pSite)));
      SetWealth(owner,(GetWealth(owner) ?? 0)- (ProdCost ?? 0));
      Sound("Cash");
  }
  
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}