//Spielregel Teamsicht

#strict

local verhalten;

func Initialize()
{
    if(GetLength(hashSpielRegeln))    verhalten=HashGet(hashSpielRegeln, "SichtradiusVerhalten");
    else {verhalten=0;}
}

 
func AddViewAllClonks() { // erstellt view objekte auf allen clonks, die nicht feindlich sind
 // Alle Spieler überprüfen
 for(var iPlr=0;iPlr<GetPlayerCount();iPlr++) {
  if(!GetCrew(iPlr)) continue;
  var pObjs=FindObjects(Find_OCF(OCF_CrewMember,OCF_Alive),Find_Not(Find_Hostile(iPlr)),Find_Not(Find_Owner(iPlr)));
  for(var j=0;j<GetLength(pObjs);j++) {
      if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(pObjs[j])))
        AddViewtoObject(pObjs[j],iPlr,ViewRangeClonk(pObjs[j]));
   }
  }
 return(1);
 }
 
func AddViewAllThings() { // erstellt view objekte auf allen Dingen, die nicht feindlich sind
 // Alle Spieler überprüfen
 for(var iPlr=0;iPlr<GetPlayerCount();iPlr++) { // 
  if(!GetCrew(iPlr)) continue;
  var pObjs=FindObjects(Find_Func("FogViewRange"),Find_OCF(OCF_Fullcon),Find_Not(Find_Hostile(iPlr)),Find_Not(Find_Owner(iPlr))); // alles mit dieser Fkt finden, was nicht uns gehört und nicht feindlich ist
  for(var j=0;j<GetLength(pObjs);j++) {
      if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(pObjs[j])))
        AddViewtoObject(pObjs[j],iPlr,pObjs[j]->FogViewRange());
    }
   }
 return(1);
 }
 
 /// timercall
 func CheckViews() // uberprueft, ob neue viewobjekte, fuer dinge mit fkt FogViewRange erstellt werden muessen. Hier auch fuer Clonks checken, ob sie mittlerweile doch feindlich sind! oder ob wieder verbundet!
 {
    if(GetLength(hashSpielRegeln))    verhalten=HashGet(hashSpielRegeln, "SichtradiusVerhalten"); // sichtradiusverhalten pruefen
    else {verhalten=0;}
    if(verhalten==0) 
    {
        if(FindObject(VIEW)) return(RemoveAll(VIEW)); // wenn es keinen nebel gibt, brauchen wir auch keine view objekte
        return();
    }
    
    // evlt neue view objekte erstellen? 
    AddViewAllClonks(); // neue views auf clonks
    AddViewAllThings(); // alle sonstigen Dinge mit FogViewRange Funktion
    
    for(var iPlr=0;iPlr<GetPlayerCount();iPlr++) { // alle spieler durchgehen und view objekte aktualisieren
        if(!GetCrew(iPlr)) continue;
        for(var viewobj in FindObjects(Find_ID(VIEW),Find_Owner(iPlr))) {// alle VIEW objekte des Spielers prüfen
            if(CrewMember(GetActionTarget(0,viewobj))) // wenn ziel ein clonk ist, dann sichtradius anpassen
            {
                viewobj->TeamViewRangethisObject(ViewRangeClonk(GetActionTarget(0,viewobj),viewobj));
            }
            if(Hostile(iPlr,GetOwner(GetActionTarget(0,viewobj)))) // wenn feindlich, dann view objekte entfernen
                RemoveObject(viewobj);
        }   
    }
    for(var obj in FindObjects(Find_Owner(NO_OWNER),Find_ID(VIEW))) // alle ohne owner entfernen
        RemoveObject(obj);
 }
 
 func AddViewtoObject(obj,iPlr,Range) { // view an ein bestimmtes obj machen
   if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(obj))) { // wenn noch kein VIEW von diesem Spieler an dem objekt klebt
    var lgt=CreateObject(VIEW,0,0,iPlr); 
    lgt->AttachTo(obj);
    lgt->TeamViewRangethisObject(Range); // die sichtweite geben
    }
  }

func ViewRangeClonk(pClonk,pView) // gibt die viewrange fuer teamsicht Clonks zurück, abhängig davon, ob Tag oder Nacht Tunnel und einstellungen
{
    var ZielSicht=pClonk->GetzielSichtweite(); // die aktuelle Zielsicht wird ständig im clonk geprüft, also einfach dort den aktuellen Wert abfragen, und nicht noch x mal hier prüfen, da sonst zu lag führt
    
    if(!pView) 
        return(ZielSicht); // wenn kein pView objekt mitgeliefert, dann wird es beim erstellen sein, da also normal.
    
    var Sicht=pView->ReturnAktuelleSicht();
    if(Abs(Sicht-ZielSicht)<15) Sicht=ZielSicht; // wenn Differenz zu klein, dann gleichsetzen.
    else
    {
        if(Sicht > ZielSicht) Sicht -= 15; // wenn pView dabei, dann wird sichtradius angepasst, dies dann in langsamen Schritten machen
        if(Sicht < ZielSicht) Sicht += 15; // 
    }

    return(Sicht);
}

protected func Activate(int plr)
{
  return MessageWindow(GetDesc(),plr);
}

func EliminatePlayer(int iPlr) {
 //Log("Log: EliminatePlayer");
 while(FindObjectOwner(VIEW,iPlr)) RemoveObject(FindObjectOwner(VIEW,iPlr));
 return(_inherited(iPlr));
 }