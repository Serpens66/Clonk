/*-- Neues Objekt --*/

#strict
#appendto CNKT

protected func CreateConstructionSite(idType,caller)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk" && GetAction(Contained()) ne "HoldWalk") return(0);
  if (Contained(Contained())) return(0);
  var check = 1;
  var fterrain = 1; // bestimmt, ob untergrund an baustelle angepasst werden soll
  var plusY = 10;

  // spezialbehandlungen
  if (idType == BEKP)
  		{
  		if(!(FindObject(BELC, -40, -10, 80, 20)) && (!(FindObject(CHLP, -40, -10, 80, 20))) && (!(FindObject(SLP1, -40, -10, 80, 20))) ) return(Message("<c ff0000>Hier ist kein Fahrstuhlkorb!", this()));
  		var pElev=FindObject(BELC, -40, -10, 80, 20);
    if (!pElev) pElev=FindObject(CHLP, -40, -10, 80, 20);
    if (!pElev) pElev=FindObject(SLP1, -40, -10, 80, 20);
        plusY=11;
  		CreateConstruction(BEKP, AbsX(GetX(pElev)), AbsY(GetY(pElev)+plusY), GetOwner(Contained()), 1, fterrain, 0);
  		return(Restzeug(idType,caller,pSite));
  		}

    // für Superkuppel und Bunkerteile, Deckenlicht und SEGU egal ob iwas im Weg. 
  if ( (idType==SPDM) || (idType==SPD2) || (idType==SPD3) || (idType==SPD4)
 || (idType==BE5C) || (idType==HL0D) || (idType==HR0D) || (idType==BH5C) || (idType==RR5C) || (idType==BT5C) || (idType==BG5C) || (idType==BL5C) || (idType==BR5C) || (idType==BP5C) || (idType==B15C) || (idType==HR6Y) || (idType==BW5C)  || (idType==SEGU)  || (idType==CLG3)  || (idType==TG6B))
      {
      plusY=15;
      if (idType == TG6B) {fterrain = 0;plusY=11;} // Mienentor
      CreateConstruction(idType,AbsX(GetX(Contained())), AbsY(GetY(Contained()))+plusY, GetOwner(Contained()), 1, fterrain, 0);
      return(Restzeug(idType,caller,pSite));
      }
  
  if(idType==GRS3) check=0; // fuer flaschenzug kein untergrund check

  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return(0);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if (GetDefCoreVal ("Height","DefCore",idType) > GetY()) return(Message("Cannot exceed map height!"));
  if (!(pSite = CreateConstruction(idType, 0, plusY, GetOwner(Contained()), 1, fterrain,check))) return(0);
  
  return(Restzeug(idType,caller,pSite));
}

func Restzeug(idType,caller,pSite)
{
    // Meldung
  Message("$TxtConstructions2$", Contained(), GetName(pSite));
  
  // wenn baustelle erfolgreich platziert, geld abziehen
  var owner = GetOwner(caller);
  var ProdCost = 0;
  if(GetLength(hashSpielRegeln))
      ProdCost = GetMass(0,idType) * HashGet(hashSpielRegeln, "Baustellenkosten")/1000;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if(GetWealth(owner)< ProdCost)
        return(Message("Nicht genug Geld",this()),Sound("Error"),RemoveObject(pSite));
      SetWealth(owner,GetWealth(owner)- ProdCost);
      Sound("Cash");
  }
  
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}