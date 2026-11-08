/*-- Forschungsbaum appednto --*/

// hier wird für alle objekte die Forschungsbasis, die Kosten und die Zeit anhand der gForschungsliste aus dem ForschungsbaumFKT.c skript festgelegt.
#strict 2

#appendto *

public func GetResearchTime(pBuilding,pClonk) // kann abhängig vom forschungsgebäude und forschung initierenden Clonk gesetzt werden. 
{
    if(FindObject(SZRE)) if(_inherited(pBuilding,pClonk)) return(_inherited(pBuilding,pClonk)); else return(50); // den Wert returnen, der im objekt skript gespeichert ist, wenn es keinen gibt, dann 50.
    for(var a=0;a<23;a++) // zurzeit haben wir von 0 bis 22 Wissensstände in der fkt gForschungsliste(a)
        if (InArray(gForschungsliste(a), GetID())) // wenn ID dieses objekts in einer Liste gefunden, dann die entsprechenden voraussetzungen returnen
        {
            return(3+a*2);  // evlt noch bessere Formel finden
        }
    return(100); // alles andere erstmal 100 returnen    
}  

public func GetResearchCosts(pBuilding,pClonk) // kann abhängig vom forschungsgebäude und forschung initierenden Clonk gesetzt werden. 
{
    if(FindObject(SZRE)) if(_inherited(pBuilding,pClonk)) return(_inherited(pBuilding,pClonk)); else return(5); // den Wert returnen, der im objekt skript gespeichert ist, wenn es keinen gibt, dann 5. // alles guenstig, kann mit kostenmultiplikator noch verändert werden wenn gewünscht
    for(var a=0;a<23;a++) // zurzeit haben wir von 0 bis 22 Wissensstände in der fkt gForschungsliste(a)
        if (InArray(gForschungsliste(a), GetID())) // wenn ID dieses objekts in einer Liste gefunden, dann die entsprechenden voraussetzungen returnen
        {
            return(1+a);  // evlt noch bessere Formel finden
        }
    return(100); // alles andere erstmal 100 returnen   
}  
// in forschungslabor skript wird als noetige basis fuer eine forschung sowohl dies, als auch dies mit zahl, als auch liste verwendet!
public func GetResearchBase() {if(InArray(gForschungverbotenliste(), GetID())) return(GetID()); if(FindObject(SZRE)) return(_inherited()); return; } //  // alle alten voraussetzungen damit überschreiben.. ist nötig,wenn meine einteilung anders ist.. also vorsichtshalber machen.. ausser es ist die regel szenario forschung aktiv. SZRE

public func GetResearchBaseList()
{
    if (InArray(gForschungverbotenliste(), GetID())) return([GetID()]);// wenn objekt verboten sein soll, dann nur sich selbst returnen
    
    for(var a=0;a<23;a++) // zurzeit haben wir von 0 bis 22 Wissensstände in der fkt gForschungsliste(a)
        if (InArray(gForschungsliste(a), GetID())) // wenn ID dieses objekts in einer Liste gefunden, dann die entsprechenden voraussetzungen returnen
        {
            var bases = gForschungsliste(a-1); // falls a kleiner 0, wird einfach eine leere liste returned
            ArrayRemoveArrayEntries(bases, gForschungfreiwilligliste()); //Löscht alle einträge von Array2 in Array1, alle verbotenen forschungen sind freiwillig, sodass sie fortschritt nicht verhindern
            return(bases); 
        }
    return([]); // alle die so nicht angesprochen wurden, bekommen eine leere liste als antwort
}


