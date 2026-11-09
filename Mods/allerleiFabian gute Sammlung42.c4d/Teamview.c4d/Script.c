//Spielregel Teamsicht

#strict 3

local verhalten;

func Initialize()
{
    if(GetLength(hashSpielRegeln))    verhalten=HashGet(hashSpielRegeln, "SichtradiusVerhalten");
    else {verhalten=nil;}
}

 
func AddViewAllClonks() { // erstellt view objekte auf allen clonks, die nicht feindlich sind
 // Alle Spieler überprüfen
 for(var iPlr=nil;(iPlr ?? 0)<(GetPlayerCount() ?? 0);S42MigrationStep(iPlr, 1, true)) {
  if(!GetCrew(iPlr)) continue;
  var pObjs=FindObjects(Find_OCF(OCF_CrewMember,OCF_Alive),Find_Not(Find_Hostile(iPlr)),Find_Not(Find_Owner(iPlr)));
  for(var j=nil;(j ?? 0)<(GetLength(pObjs) ?? 0);S42MigrationStep(j, 1, true)) {
      if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(pObjs[(j ?? 0)])))
        AddViewtoObject(pObjs[(j ?? 0)],iPlr,ViewRangeClonk(pObjs[(j ?? 0)]));
   }
  }
 return(1);
 }
 
func AddViewAllThings() { // erstellt view objekte auf allen Dingen, die nicht feindlich sind
 // Alle Spieler überprüfen
 for(var iPlr=nil;(iPlr ?? 0)<(GetPlayerCount() ?? 0);S42MigrationStep(iPlr, 1, true)) { //
  if(!GetCrew(iPlr)) continue;
  var pObjs=FindObjects(Find_Func("FogViewRange"),Find_OCF(OCF_Fullcon),Find_Not(Find_Hostile(iPlr)),Find_Not(Find_Owner(iPlr))); // alles mit dieser Fkt finden, was nicht uns gehört und nicht feindlich ist
  for(var j=nil;(j ?? 0)<(GetLength(pObjs) ?? 0);S42MigrationStep(j, 1, true)) {
      if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(pObjs[(j ?? 0)])))
        AddViewtoObject(pObjs[(j ?? 0)],iPlr,pObjs[(j ?? 0)]->FogViewRange());
    }
   }
 return(1);
 }
 
 /// timercall
 func CheckViews() // uberprueft, ob neue viewobjekte, fuer dinge mit fkt FogViewRange erstellt werden muessen. Hier auch fuer Clonks checken, ob sie mittlerweile doch feindlich sind! oder ob wieder verbundet!
 {
    if(GetLength(hashSpielRegeln))    verhalten=HashGet(hashSpielRegeln, "SichtradiusVerhalten"); // sichtradiusverhalten pruefen
    else {verhalten=nil;}
    if(Equal(verhalten, nil))
    {
        if(FindObject(VIEW)) return(RemoveAll(VIEW)); // wenn es keinen nebel gibt, brauchen wir auch keine view objekte
        return;
    }
    
    // evlt neue view objekte erstellen? 
    AddViewAllClonks(); // neue views auf clonks
    AddViewAllThings(); // alle sonstigen Dinge mit FogViewRange Funktion
    
    for(var iPlr=nil;(iPlr ?? 0)<(GetPlayerCount() ?? 0);S42MigrationStep(iPlr, 1, true)) { // alle spieler durchgehen und view objekte aktualisieren
        if(!GetCrew(iPlr)) continue;
        for(var viewobj in FindObjects(Find_ID(VIEW),Find_Owner(iPlr))) {// alle VIEW objekte des Spielers prüfen
            if(CrewMember(GetActionTarget(nil,viewobj))) // wenn ziel ein clonk ist, dann sichtradius anpassen
            {
                viewobj->TeamViewRangethisObject(ViewRangeClonk(GetActionTarget(nil,viewobj),viewobj));
            }
            if(Hostile(iPlr,GetOwner(GetActionTarget(nil,viewobj)))) // wenn feindlich, dann view objekte entfernen
                RemoveObject(viewobj);
        }   
    }
    for(var obj in FindObjects(Find_Owner(NO_OWNER),Find_ID(VIEW))) // alle ohne owner entfernen
        RemoveObject(obj);
 }
 
 func AddViewtoObject(obj,iPlr,Range) { // view an ein bestimmtes obj machen
   if(!FindObject2(Find_Owner(iPlr),Find_ID(VIEW),Find_ActionTarget(obj))) { // wenn noch kein VIEW von diesem Spieler an dem objekt klebt
    var lgt=CreateObject(VIEW,nil,nil,iPlr);
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
    if((Abs((Sicht ?? 0)-(ZielSicht ?? 0)) ?? 0)<15) Sicht=ZielSicht; // wenn Differenz zu klein, dann gleichsetzen.
    else
    {
        if((Sicht ?? 0) > (ZielSicht ?? 0)) S42MigrationAssign(Sicht, "-=", 15); // wenn pView dabei, dann wird sichtradius angepasst, dies dann in langsamen Schritten machen
        if((Sicht ?? 0) < (ZielSicht ?? 0)) S42MigrationAssign(Sicht, "+=", 15); //
    }

    return(Sicht);
}

protected func Activate(int plr)
{
  plr ??= 0;
  return MessageWindow(GetDesc(),plr);
}

func EliminatePlayer(int iPlr) {
 //Log("Log: EliminatePlayer");
 iPlr ??= 0;
 while(FindObjectOwner(VIEW,iPlr)) RemoveObject(FindObjectOwner(VIEW,iPlr));
 return(_inherited(iPlr));
 }