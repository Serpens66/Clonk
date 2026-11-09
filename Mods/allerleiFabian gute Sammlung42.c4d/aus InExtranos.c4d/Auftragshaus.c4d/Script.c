#strict

#include WBAS  // basement

local auftragaktiv;  // 0 bei nichts,  1 bei angriff, 2 bei bring
local entfernerregel;
local kosten;   // die Kosten, um einen Auftrag anzunehmen. Nach erfuellung bekommt man es zusätzlich zur Belohnung zurück. Geht darum, dass man nicht unendlich oft aufträge annimmt und abbricht, bis ein leichter dabei ist.
local derclonk; // um auch bei Jagdfertig noch zu wissen, welcher clonk den auftrag angenommen hat

ActivateEntrance: 
  if(!ActIdle())  return(1);
  SetAction("OpenDoor");
  Sound("DoorOpen");
  return(1);

DoorIsOpen:
  SetEntrance(1);
  return(1);

DoorIsClosed:
  SetEntrance(0);
  Sound("DoorClose");
  return(1);

func ContainedUp(pClonk)
{
    [Aufträge|Image=X_12]
    HauptMenu(pClonk);
}

func HauptMenu(clonk,zahl)
{
    kosten = gPlayerAge(GetOwner(clonk))*10; // das zehnfache des aktuellen zeitalters 
    if(!kosten) kosten = 5;  // im zeitalter 0
 // Hauptmenü erzeugen
  CreateMenu(X_12, clonk, this,0,0,0,1);
  
  if(!auftragaktiv)
  {
    AddMenuItem(Format("Jagd fuer %v Clunker starten.",kosten), "AngriffAuftrag", MONS, clonk, 0, clonk,Format("Waehle einen Auftrag, bei dem du eine Anzahl von Feinden erledigen musst. Die Annahme kostet dich %v Clunker.",kosten));
    AddMenuItem(Format("Bringe mir etwas. Fuer %v Clunker starten.",kosten/2), "BringAuftrag", RSR3, clonk, 0, clonk,Format("Bringe ein bestimmten Gegenstand ins Auftraghaus. Die Annahme kostet dich %v Clunker. Bringt weniger Belohnung, als die Jagd.",kosten/2)); // kostet halb soviel, bringt aber auch weniger
  }
  else 
  {
      AddMenuItem("Infos zum aktuellen Auftrag", "InfoAuftrag", RSR4, clonk, 0, clonk,"Hilfreiche Infos zum aktuellen Auftrag");
      AddMenuItem("Auftrag abbrechen", "CancelAuftrag", S__9, clonk, 0, clonk,"Bricht den aktuellen Auftrag ab. Du erhälst keine Belohnung!");
  }
  AddMenuItem("Infos", "InfoMenu", X_12, clonk, 0, clonk,"Infos, wie dieses Gebaeude funnktioniert");
  
  SelectMenuItem(zahl,clonk);
}

func InfoMenu(idImage,clonk)
{
    MessageWindow("Hier koennen allerlei Auftraege angenommen werden. Nach Erfuellung erhaelst du Clunker, Erfahrung, Forschungsplaene oder Objekte als Belohnung, die Erfahrung bekommt meistens der Auftragannehmende Clonk. Ein Auftrag kostet dich erstmal ein paar Clunker, die du nach Erfuellung aber zusaetzlich wiederbekommst. Du kannst Auftraege auch jederzeit abbrechen, bekommst dann aber weder dein Geld, noch deine Belohnung. Die Art der Auftraege richtet sich nach deinem Forschungsstand.", GetOwner(clonk));
}

func InfoAuftrag(idImage,clonk)
{
    if(auftragaktiv==1) // ein angriffsauftrag
        entfernerregel->~Activate(GetOwner(clonk)); // gibt aus, wieviele von wievielen Feinden schon erledigt sind.
    if(auftragaktiv==2) // ein Bring etwas Auftrag
        MessageWindow(Format("",), GetOwner(clonk));
}

func CancelAuftragMenu(idImage,clonk)  
{
    CreateMenu(X_12, clonk, this(),0,0,0,1);
    AddMenuItem("Zurück", "HauptMenu", S__9, clonk, 0, clonk,"Zurück zum Hauptmenü");
    AddMenuItem("Bestätigung: Auftrag abbrechen", "CancelAuftrag", X_12, clonk, 0, clonk,"Abbruch. Keine Belohnung");
}

func CancelAuftrag(idImage,clonk)
{
    if(auftragaktiv==1) // Jagd
    {
        auftragaktiv=0;
        entfernerregel->Beenden();
        entfernerregel=0;
    }
    if(auftragaktiv==2) // bring
    {
        auftragaktiv=0;
    }
    PlayerMessage (GetOwner(clonk), "Auftrag abgebrochen");
}

func BringAuftrag(idImage,clonk)
{
   return(Log("noch nicht fertig"));
   
   // genug finanzielle Rücklagen?
  if (GetWealth(GetOwner(clonk)) < kosten/2)
  {
    PlayerMessage(GetOwner(clonk), "Nicht genug Geld!", this);
    Sound("Error");
    return(HauptMenu(clonk));
  }
  DoWealth(GetOwner(clonk), -kosten/2);
  Sound("Cash");
  
  
}

func AngriffAuftrag(idImage,clonk)
{
   derclonk = clonk;
   // genug finanzielle Rücklagen?
  if (GetWealth(GetOwner(clonk)) < kosten)
  {
    PlayerMessage(GetOwner(clonk), "Nicht genug Geld!", this);
    Sound("Error");
    return(HauptMenu(clonk));
  }
  DoWealth(GetOwner(clonk), -kosten);
  Sound("Cash");
    
    var Age = gPlayerAge(GetOwner(clonk));
    if(Age<=4)  // ab zeitalter 5 hat man armbrust usw.
    {
        var liste = ["Zufall1","Zufall2"];
        var imonster = liste[RandomX(0,1)]; // zufall1 oder zufall2, relativ leichte monster
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(2,10);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>4 && Age<11)  // ab zeitalter 11 hat man zukunftswaffen, ab 5 hat man immerhin armbrust usw.
    {
        var liste = ["Zufall1","Zufall2","Zufall3"];
        var imonster = liste[RandomX(0,2)];  // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(2,10);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>=11 && Age<13)  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var liste = ["Zufall1","Zufall2","Zufall3","Zufall4"];
        var imonster = liste[RandomX(0,3)]; // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier. Und zufall4 Aliens
        var ikeinflug = 0;   
        var ierledigen=RandomX(2,13);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>=13)  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var imonster = "Zufall";  // Alles, auch Hazardclonks, assasine und kleine raumschiffe möglich
        var ikeinflug = 0;  
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
    if (!(entfernerregel==pRegel)) return;  // nur weitermachen, wenn dies auch das auftragshaus ist, was den auftrag vergeben hat
    if(!auftragaktiv) return;  // wenn fertig, weil abgebrochen, dann returnen
    // Belohnung ausgeben
    DoWealth(GetOwner(derclonk), kosten);  // gezahlte Kosten zurück
    var aBelohnungen = ["Clunker","Erfahrung","Items","Clunker","Bauplaene"]; // clunker 2 mal, damit wahrscheinlicher
    var belohnung = aBelohnungen[RandomX(0,4)];
    // belohnung = "Items";
    
    var iPlr = GetOwner(derclonk), alter;
    if (belohnung == "Bauplaene" || belohnung == "Items") alter = gPlayerAge(iPlr);
    if(belohnung=="Bauplaene" && alter>=16) belohnung = aBelohnungen[RandomX(0,3)]; // wenn zeitalter 16 oder mehr, dann keine Baupläne mehr als belohnung
    
    if (belohnung=="Clunker")
    {
        var menge = RandomX(kosten,kosten*6);  // einsatz wird mindestens verdoppelt
        var xp = RandomX(kosten,kosten*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk);               // nur der Clonk, der den Auftrag angenommen hat
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Du bekommst |%v Clunker |und |%v Erfahrung |als Belohnung.",menge,xp));
        DoWealth(GetOwner(derclonk), menge);
        Sound("Cash");
    }
    else if (belohnung=="Erfahrung")
    {
        var xp = RandomX(kosten*10,kosten*30); // viel XP und jeder clonk des spielers bekommt sie
        for(var i=0; i<GetCrewCount(GetOwner(derclonk)); i++)
        {
            DoCrewExp(xp,GetCrew(GetOwner(derclonk), i));
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Jeder deiner Mannschaftsmitglieder bekommt |%v Erfahrung |als Belohnung.",xp));
    }
    else if (belohnung=="Bauplaene")
    {
        var menge = RandomX(1,5); // 1 bis 5 forschungspläne
        var xp = RandomX(kosten,kosten*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk); 
        var aErforscht = [], verboten = gForschungverbotenliste();
        for (var n = 0; n < menge && alter < 23; n++)
        {
            var kandidaten = [];
            for (var plan in gForschungsliste(alter))
            {
                if (GetPlrKnowledge(iPlr, plan) || InArray(verboten, plan) || !GetName(0, plan))
                    continue;
                ArrayAdd(kandidaten, plan, true);
            }
            if (!GetLength(kandidaten)) break;
            var bauplan = ArrayRandomEntry(kandidaten);
            if (!SetPlrKnowledge(iPlr, bauplan)) break;
            ArrayAdd(aErforscht, GetName(0, bauplan));
            if (n + 1 < menge) alter = gPlayerAge(iPlr);
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Hier habe ich folgende Bauplaene fuer euch:|%v Erfahrung|%s |%s |%s |%s |%s",xp,ArrayData(aErforscht,0,""),ArrayData(aErforscht,1,""),ArrayData(aErforscht,2,""),ArrayData(aErforscht,3,""),ArrayData(aErforscht,4,"")));
    }
    else if (belohnung=="Items")
    {
        var menge = RandomX(1,5); // 1 bis 5 Items
        var xp = RandomX(kosten,kosten*6); // immer auch ein bisschen XP
        DoCrewExp(xp,derclonk); 
        var counter = 0;
        var aItemsName = [];
        var i,idDef;
        var forschliste = gForschungsliste(alter);
        var aAlleItems = [];
        while(idDef = GetDefinition(i++, C4D_Object())) // klappt irgendwie nicht mit 3600 ... 
        {
            if (!((GetCategory(0,idDef) & C4D_Object()) && (GetCategory(0,idDef) & C4D_SelectMaterial()) && (GetCategory(0,idDef) & C4D_SelectKnowledge()) && (GetCategory(0,idDef) & C4D_SelectHomebase())))// extra kontrolle
                continue;
            if (GetDefCoreVal("NoSell", "DefCore", idDef))  // keine objekte die nicht verkaufbar sind, wie Müll
                continue;
            if(!GetPlrKnowledge(iPlr,idDef) &&  !InArray(forschliste, idDef) ) // wenn es weder erforscht, noch im aktuellen forschungsplan ist, dann nicht nehmen
                continue;
            ArrayAdd(aAlleItems,idDef); // hier erstmal alle in frage kommenden Items eintragen.
        } 
        while(counter<menge && GetLength(aAlleItems)) // alle objekte der kategorie 3600 , also alle die herstellbare und in heimatbasis einstellbare gegenstände sind
        {
            idDef = ArrayRandomEntry(aAlleItems);  // zufälliges element aus den erlaubten items...    flüssigkeitsgeschütz ... wieso geschütz, das ist nicht 3600 ... 
            ArrayAdd(aItemsName,GetName(0,idDef)); // zum array zufügen
            counter++;
            CreateContents(idDef,this); // die items im auftragshaus erstellen
        }
        PlayerMessage(GetOwner(derclonk),Format("Danke fuer deine Hilfe! Hier habe ich folgende Items fuer euch:|%v Erfahrung|%s |%s |%s |%s |%s",xp,ArrayData(aItemsName,0,""),ArrayData(aItemsName,1,""),ArrayData(aItemsName,2,""),ArrayData(aItemsName,3,""),ArrayData(aItemsName,4,""))); // ArrayData sorgt dafür, dass statt einem Fehler, der 3te paramter ausgegeben wird, wenn eintrag nicht exisitiert
    }
    
    auftragaktiv=0;
    entfernerregel=0;
}

