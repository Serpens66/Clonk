#strict 3

#include WBAS  // basement

local auftragaktiv;  // 0 bei nichts,  1 bei angriff, 2 bei bring
local entfernerregel;
local kosten;   // die Kosten, um einen Auftrag anzunehmen. Nach erfuellung bekommt man es zusätzlich zur Belohnung zurück. Geht darum, dass man nicht unendlich oft aufträge annimmt und abbricht, bis ein leichter dabei ist.
local derclonk; // um auch bei Jagdfertig noch zu wissen, welcher clonk den auftrag angenommen hat

func ActivateEntrance(...) {
  if(!ActIdle())  return(1);
  SetAction("OpenDoor");
  Sound("DoorOpen");
  return(1);
}

func DoorIsOpen(...) {
  SetEntrance(1);
  return(1);
}

func DoorIsClosed(...) {
  SetEntrance(nil);
  Sound("DoorClose");
  return(1);
}

func ContainedUp(pClonk)
{
    [Aufträge|Image=X_12]
    HauptMenu(pClonk);
}

func HauptMenu(clonk,zahl)
{
    kosten = (gPlayerAge(GetOwner(clonk)) ?? 0)*10; // das zehnfache des aktuellen zeitalters
    if(!kosten) kosten = 5;  // im zeitalter 0
 // Hauptmenü erzeugen
  CreateMenu(X_12, clonk, this,nil,nil,nil,1);
  
  if(!auftragaktiv)
  {
    AddMenuItem(Format("Jagd fuer %v Clunker starten.",kosten), "AngriffAuftrag", MONS, clonk, nil, clonk,Format("Waehle einen Auftrag, bei dem du eine Anzahl von Feinden erledigen musst. Die Annahme kostet dich %v Clunker.",kosten));
    AddMenuItem(Format("Bringe mir etwas. Fuer %v Clunker starten.",(kosten ?? 0)/2), "BringAuftrag", RSR3, clonk, nil, clonk,Format("Bringe ein bestimmten Gegenstand ins Auftraghaus. Die Annahme kostet dich %v Clunker. Bringt weniger Belohnung, als die Jagd.",(kosten ?? 0)/2)); // kostet halb soviel, bringt aber auch weniger
  }
  else 
  {
      AddMenuItem("Infos zum aktuellen Auftrag", "InfoAuftrag", RSR4, clonk, nil, clonk,"Hilfreiche Infos zum aktuellen Auftrag");
      AddMenuItem("Auftrag abbrechen", "CancelAuftrag", S__9, clonk, nil, clonk,"Bricht den aktuellen Auftrag ab. Du erhälst keine Belohnung!");
  }
  AddMenuItem("Infos", "InfoMenu", X_12, clonk, nil, clonk,"Infos, wie dieses Gebaeude funnktioniert");
  
  SelectMenuItem(zahl,clonk);
}

func InfoMenu(idImage,clonk)
{
    MessageWindow("Hier koennen allerlei Auftraege angenommen werden. Nach Erfuellung erhaelst du Clunker, Erfahrung, Forschungsplaene oder Objekte als Belohnung, die Erfahrung bekommt meistens der Auftragannehmende Clonk. Ein Auftrag kostet dich erstmal ein paar Clunker, die du nach Erfuellung aber zusaetzlich wiederbekommst. Du kannst Auftraege auch jederzeit abbrechen, bekommst dann aber weder dein Geld, noch deine Belohnung. Die Art der Auftraege richtet sich nach deinem Forschungsstand.", GetOwner(clonk));
}

func InfoAuftrag(idImage,clonk)
{
    if(Equal(auftragaktiv, 1)) // ein angriffsauftrag
        entfernerregel->~Activate(GetOwner(clonk)); // gibt aus, wieviele von wievielen Feinden schon erledigt sind.
    if(Equal(auftragaktiv, 2)) // ein Bring etwas Auftrag
        MessageWindow(Format("",), GetOwner(clonk));
}

func CancelAuftragMenu(idImage,clonk)  
{
    CreateMenu(X_12, clonk, this(),nil,nil,nil,1);
    AddMenuItem("Zurück", "HauptMenu", S__9, clonk, nil, clonk,"Zurück zum Hauptmenü");
    AddMenuItem("Bestätigung: Auftrag abbrechen", "CancelAuftrag", X_12, clonk, nil, clonk,"Abbruch. Keine Belohnung");
}

func CancelAuftrag(idImage,clonk)
{
    if(Equal(auftragaktiv, 1)) // Jagd
    {
        auftragaktiv=nil;
        entfernerregel->Beenden();
        entfernerregel=nil;
    }
    if(Equal(auftragaktiv, 2)) // bring
    {
        auftragaktiv=nil;
    }
    PlayerMessage (GetOwner(clonk), "Auftrag abgebrochen");
}

func BringAuftrag(idImage,clonk)
{
   return(Log("noch nicht fertig"));
   
   // genug finanzielle Rücklagen?
  if ((GetWealth(GetOwner(clonk)) ?? 0) < (kosten ?? 0)/2)
  {
    PlayerMessage(GetOwner(clonk), "Nicht genug Geld!", this);
    Sound("Error");
    return(HauptMenu(clonk));
  }
  DoWealth(GetOwner(clonk), -(kosten ?? 0)/2);
  Sound("Cash");
  
  
}

func AngriffAuftrag(idImage,clonk)
{
   derclonk = clonk;
   // genug finanzielle Rücklagen?
  if ((GetWealth(GetOwner(clonk)) ?? 0) < (kosten ?? 0))
  {
    PlayerMessage(GetOwner(clonk), "Nicht genug Geld!", this);
    Sound("Error");
    return(HauptMenu(clonk));
  }
  DoWealth(GetOwner(clonk), -(kosten ?? 0));
  Sound("Cash");
    
    var Age = gPlayerAge(GetOwner(clonk));
    if((Age ?? 0)<=4)  // ab zeitalter 5 hat man armbrust usw.
    {
        var liste = ["Zufall1","Zufall2"];
        var imonster = liste[(RandomX(nil,1) ?? 0)]; // zufall1 oder zufall2, relativ leichte monster
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(2,10);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(And((Age ?? 0)>4, (Age ?? 0)<11))  // ab zeitalter 11 hat man zukunftswaffen, ab 5 hat man immerhin armbrust usw.
    {
        var liste = ["Zufall1","Zufall2","Zufall3"];
        var imonster = liste[(RandomX(nil,2) ?? 0)];  // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(2,10);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(And((Age ?? 0)>=11, (Age ?? 0)<13))  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var liste = ["Zufall1","Zufall2","Zufall3","Zufall4"];
        var imonster = liste[(RandomX(nil,3) ?? 0)]; // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier. Und zufall4 Aliens
        var ikeinflug = nil;
        var ierledigen=RandomX(2,13);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if((Age ?? 0)>=13)  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var imonster = "Zufall";  // Alles, auch Hazardclonks, assasine und kleine raumschiffe möglich
        var ikeinflug = nil;
        var ierledigen=RandomX(2,20);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    
    // imonster="Monster";  // zum testen
    // ierledigen=1;
    
    entfernerregel = MMonsterAttack(imonster, iaufeinmal, ierledigen,2,ikeinflug); 
    auftragaktiv=1;
    PlayerMessage(GetOwner(clonk),"Hier ist dein Auftrag");
    InfoAuftrag(idImage,clonk);
}

func Jagdfertig(pRegel) // entfernerregel ruft in allen auftragshäusern diese fkt auf, wenn erledigt
{
    if (!(Equal(entfernerregel, pRegel))) return;  // nur weitermachen, wenn dies auch das auftragshaus ist, was den auftrag vergeben hat
    if(!auftragaktiv) return;  // wenn fertig, weil abgebrochen, dann returnen
    // Belohnung ausgeben
    DoWealth(GetOwner(derclonk), kosten);  // gezahlte Kosten zurück
    var aBelohnungen = ["Clunker","Erfahrung","Items","Clunker","Bauplaene"]; // clunker 2 mal, damit wahrscheinlicher
    var belohnung = aBelohnungen[(RandomX(nil,4) ?? 0)];
    // belohnung = "Items";
    
    var iPlr = GetOwner(derclonk), alter;
    if (Or(Equal(belohnung, "Bauplaene"), Equal(belohnung, "Items"))) alter = gPlayerAge(iPlr);
    if(And(Equal(belohnung, "Bauplaene"), (alter ?? 0)>=16)) belohnung = aBelohnungen[(RandomX(nil,3) ?? 0)]; // wenn zeitalter 16 oder mehr, dann keine Baupläne mehr als belohnung
    
    if (Equal(belohnung, "Clunker"))
    {
        var menge = RandomX(kosten,(kosten ?? 0)*6);  // einsatz wird mindestens verdoppelt
        var xp = RandomX(kosten,(kosten ?? 0)*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk);               // nur der Clonk, der den Auftrag angenommen hat
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Du bekommst |%v Clunker |und |%v Erfahrung |als Belohnung.",menge,xp));
        DoWealth(GetOwner(derclonk), menge);
        Sound("Cash");
    }
    else if (Equal(belohnung, "Erfahrung"))
    {
        var xp = RandomX((kosten ?? 0)*10,(kosten ?? 0)*30); // viel XP und jeder clonk des spielers bekommt sie
        for(var i=nil; (i ?? 0)<(GetCrewCount(GetOwner(derclonk)) ?? 0); S42MigrationStep(i, 1, true))
        {
            DoCrewExp(xp,GetCrew(GetOwner(derclonk), i));
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Jeder deiner Mannschaftsmitglieder bekommt |%v Erfahrung |als Belohnung.",xp));
    }
    else if (Equal(belohnung, "Bauplaene"))
    {
        var menge = RandomX(1,5); // 1 bis 5 forschungspläne
        var xp = RandomX(kosten,(kosten ?? 0)*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk); 
        var aErforscht = [], verboten = gForschungverbotenliste();
        for (var n = nil; And((n ?? 0) < (menge ?? 0), (alter ?? 0) < 23); S42MigrationStep(n, 1, true))
        {
            var kandidaten = [];
            for (var plan in gForschungsliste(alter))
            {
                if (Or(Or(GetPlrKnowledge(iPlr, plan), InArray(verboten, plan)), !GetName(nil, plan)))
                    continue;
                ArrayAdd(kandidaten, plan, true);
            }
            if (!GetLength(kandidaten)) break;
            var bauplan = ArrayRandomEntry(kandidaten); // Einen zufaelligen unbekannten Plan der aktuellen Forschungsstufe vergeben.
            if (!SetPlrKnowledge(iPlr, bauplan)) break;
            ArrayAdd(aErforscht, GetName(nil, bauplan));
            if ((n ?? 0) + 1 < (menge ?? 0)) alter = gPlayerAge(iPlr); // Nach der letzten Stufe liefert gPlayerAge den Wert 23.
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Hier habe ich folgende Bauplaene fuer euch:|%v Erfahrung|%s |%s |%s |%s |%s",xp,ArrayData(aErforscht,nil,""),ArrayData(aErforscht,1,""),ArrayData(aErforscht,2,""),ArrayData(aErforscht,3,""),ArrayData(aErforscht,4,"")));
    }
    else if (Equal(belohnung, "Items"))
    {
        var menge = RandomX(1,5); // 1 bis 5 Items
        var xp = RandomX(kosten,(kosten ?? 0)*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk); 
        var counter = nil;
        var aItemsName = [];
        var i,idDef;
        var forschliste = gForschungsliste(alter);
        var aAlleItems = [];
        while(idDef = GetDefinition(S42MigrationStep(i, 1, true), C4D_Object)) // klappt irgendwie nicht mit 3600 ...
        {
            if (!(And(And(And(((GetCategory(nil,idDef) ?? 0) & (C4D_Object ?? 0)), ((GetCategory(nil,idDef) ?? 0) & (C4D_SelectMaterial() ?? 0))), ((GetCategory(nil,idDef) ?? 0) & (C4D_SelectKnowledge() ?? 0))), ((GetCategory(nil,idDef) ?? 0) & (C4D_SelectHomebase() ?? 0)))))// extra kontrolle
                continue;
            if (GetDefCoreVal("NoSell", "DefCore", idDef))  // keine objekte die nicht verkaufbar sind, wie Müll
                continue;
            if(And(!GetPlrKnowledge(iPlr,idDef),  !InArray(forschliste, idDef)) ) // wenn es weder erforscht, noch im aktuellen forschungsplan ist, dann nicht nehmen
                continue;
            ArrayAdd(aAlleItems,idDef); // hier erstmal alle in frage kommenden Items eintragen.
        } 
        while(And((counter ?? 0)<(menge ?? 0), GetLength(aAlleItems))) // alle objekte der kategorie 3600 , also alle die herstellbare und in heimatbasis einstellbare gegenstände sind
        {
            idDef = ArrayRandomEntry(aAlleItems);  // zufälliges element aus den erlaubten items...    flüssigkeitsgeschütz ... wieso geschütz, das ist nicht 3600 ... 
            ArrayAdd(aItemsName,GetName(nil,idDef)); // zum array zufügen
            S42MigrationStep(counter, 1, true);
            CreateContents(idDef,this); // die items im auftragshaus erstellen
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Hier habe ich folgende Items fuer euch:|%v Erfahrung|%s |%s |%s |%s |%s",xp,ArrayData(aItemsName,nil,""),ArrayData(aItemsName,1,""),ArrayData(aItemsName,2,""),ArrayData(aItemsName,3,""),ArrayData(aItemsName,4,""))); // ArrayData sorgt dafür, dass statt einem Fehler, der 3te paramter ausgegeben wird, wenn eintrag nicht exisitiert
    }
    
    auftragaktiv=nil;
    entfernerregel=nil;
}

