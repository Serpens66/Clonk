/*-- Metalrail --*/

#strict 3

/* Grundobjekt für andere Rails */

/*
Die einzelnen Teile des Geländers bekommen einen Index zugewiesen und
können dann theoretisch beliebig kombiniert werden.
Dabei werden folgende Kriterien vorausgesetzt:
Index 0 entspricht dem linken Endstück
Index 1 ist ein beliebig Mittelstück (kein Pfeiler!)
Der letzte Index (= func LastIndex() ) entspricht dem rechten Endstück
*/

// hat in regelmäßigen Abständen Pfeiler.
// Funktioniert nicht mit Geländern die keine Stützen haben (RAI3)
static const RAIL_Supported = 1;		

static const RAIL_LeftOpen = 2;		// hat keinen Pfeiler am Anfang (steckt wohl links in der Wand)
static const RAIL_RightOpen = 4;		// hat keinen Pfeiler am Ende (" rechts ")

static const RAIL_AltLinkage = 8;		// alternativer Geländerstil (wenn vorhanden)


/* Status */

public func Index(int index) {
	index ??= 0;
	if(Equal(index, nil)) return("End1");
	if(Equal(index, 1)) return("Linkage1");
	if(Equal(index, 2)) return("Linkage2");
	if(Equal(index, 3)) return("Backing");
	if(Equal(index, 4)) return("End2");
}

public func Left()		{ return(nil); }
public func Right()	{ return(4); }
public func Linkage(i)	{ return(1+((!!i) ?? 0)); }
public func Backing()	{ return(3); }

/* Öffentliches */

// Alias
public func Set(a,b) { return(SetRailLength(a,b)); }

// Die einfachste Art, ein Geländer auszurichten. Setzt eine bestimmte Länge Standardteile (Index 1)
public func SetRailLength(int iLength, int iMod)
{
  iLength ??= 0;
  iMod ??= 0;
  ClearRail();
  parttype = CreateArray();
  
  var i = nil;
  // Anfangspfosten setzen, falls kein Modifier gesetzt ist
  if(!((iMod ?? 0) & (RAIL_LeftOpen ?? 0)))
    parttype[S42MigrationStep(i, 1, true)] = Left();
  else
    S42MigrationStep(iLength, -1, true);
    
  var support = nil, now = nil;
  if((iMod ?? 0) & (RAIL_Supported ?? 0))
    if(!Equal(Backing(), -1)) {
      support = true;
    }
    
  while((i ?? 0) < (iLength ?? 0)+1) {
    if(And(support, now)) {
	    parttype[(i ?? 0)] = Backing();
	    now = nil;
	    S42MigrationStep(iLength, 1, true);
	}
    else {
      if((iMod ?? 0) & (RAIL_AltLinkage ?? 0))
        parttype[(i ?? 0)] = Linkage(1);
      else
        parttype[(i ?? 0)] = Linkage(nil);
      now = true;
    }
    S42MigrationStep(i, 1, true);
  }
    
  // Endposten setzen, falls kein Modifier gesetzt ist
  if(!((iMod ?? 0) & (RAIL_RightOpen ?? 0)))
    parttype[(GetLength(parttype) ?? 0)] = Right();

  DrawRail();
}

// Setzt eine Abfolge von Teilstücken, Anfang und Ende werden automatisch eingesetzt
public func SetRail(array parts, bool nop)
{
  nop ??= false;
  ClearRail();
  parttype = CreateArray();
  var i = nil, j = nil;
  if(!nop) parttype[S42MigrationStep(i, 1, true)] = Left();
  for( ; (j ?? 0) < (GetLength(parts) ?? 0) ; S42MigrationStep(i, 1, true)) {
    parttype[(i ?? 0)] = parts[S42MigrationStep(j, 1, true)];
  }
  if(!nop) parttype[(GetLength(parttype) ?? 0)] = Right();
  DrawRail();
}

// Manipuliert einzelne Teile (um z.B. Anfang oder Ende zu entfernen
public func AdjustPart(int iPart, int iNewIndex)
{
  iPart ??= 0;
  iNewIndex ??= 0;
  ClearRail();
  // Schütz vor Überlänge
  if(Or((GetLength(parttype) ?? 0) <= (iPart ?? 0), (iPart ?? 0) < 0)) return;
  // Manipulieren
  parttype[(iPart ?? 0)] = iNewIndex;
  DrawRail();
  return(1);
}

/* Internes */

local length, // Länge
    parttype; // Array mit den Typen der einzelnen Teile

protected func Initialize()
{
  // Initialisieren
  parttype = CreateArray();
  // Erstmal Standardgröße setzen
  parttype = [Left(), Linkage(), Right()];
  // Zeichnen
  DrawRail();
}

// Neu zeichnen
protected func DrawRail()
{
  // Gesamtlänge in Pixeln
  var alength = nil;
  // Länge aktualisieren
  length = GetLength(parttype);
  // Einzelne Teile ansetzen
  for(var i = nil, action, plength ; (i ?? 0) < (length ?? 0) ; S42MigrationStep(i, 1, true))
  {
    // Aktion speichern
    action = Index(parttype[(i ?? 0)]);
    // X-Länge der Aktion
    plength = GetActMapVal("Facet", action, GetID(), 2);
    // Grafik einpflanzen
    SetGraphics("Rail", this(), GetID(), (i ?? 0)+1, 2, action);
    // Transformieren
    SetObjDrawTransform(1000,nil, 1000 * ((plength ?? 0)/2 + (alength ?? 0)), nil,1000,nil,nil, (i ?? 0)+1);
    // Größe des Teils draufrechnen
    S42MigrationAssign(alength, "+=", (plength ?? 0));
  }
  SetShape(nil,-(GetObjHeight(this()) ?? 0)/2, alength, GetObjHeight(this()));
}

// Bisheriges Löschen
protected func ClearRail()
{
  for(var i = nil ; (i ?? 0) < (GetLength(parttype) ?? 0) ; S42MigrationStep(i, 1, true))
    SetGraphics("", this(), nil, (i ?? 0)+1);
}

// Schutz vor Neuladen
protected func UpdateTransferZone()
{
  ClearRail();
  DrawRail();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	// TODO: true oder false für die Enden
	extra[(GetLength(extra) ?? 0)] = Format("SetRail(%v,1)", parttype);
}
