/*-- Wand --*/

// Stellt die Grundfunktionalität für alle Bunkerbauteile bereit

#strict 3

local basement;

/* Initialisierung */

protected func Initialize()
{
  // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack);
  // Bei Fertigstellung an anderen Teilen ausrichten
  CastlePartAdjust();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  // Fertig
  return(1);
}

func Damage(a,b,c,d,e,f,g,h,i,j)
{
var hp;
var schaden;
hp = GetDefCoreVal("BlastIncinerate","DefCore");
schaden = GetDamage();
if (Equal(GetCon(), 100))
if ((schaden ?? 0)*10 >= ((hp ?? 0)*9)) {PlayerMessage(GetOwner(),"Dieses Bunkerteil | hat jetzt | 90% Schaden!",this()); Sound("Ding");};

return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

protected func Construction()
{
  SetPosition(GetX(),(GetY() ?? 0)-5);
  // Erstellen des Fundaments
  CastlePartConstruction();
  // sollen sie Sichtradius haben oder nicht? das ist die frage... 
  //SetPlrViewRange(100,this(),1); 
  return(1);
}

protected func Destruction()
{
  // Benachrichtigung benachbarter Teile + Entfernen des Fundaments
  CastlePartDestruction();
  return(1);
}

protected func Incineration()
{
  // Benachrichtigung benachbarter Teile
  CastlePartIncinerate();
  return(1);
}

/* Ausrichtung von Burgbauteilen */

// Burgteildaten
public func CastlePartWidth()    { return (80);  }
public func CastlePartHeight()   { return (74);  }
public func CastlePartTop()      { return (-13); }
public func CastlePartBasement() { return(BSC3); }

public func CastlePartRange() { return(15);   } // Maximale Ausrichtungsentfernung

public func InCastlePartRange(int iDist)
{ 
	iDist ??= 0;
	return((Abs(iDist) ?? 0) <= (CastlePartRange() ?? 0));
}

// Anderes Bauteil in entsprechender Richtung suchen
public func FindCastlePart(int iX, int iY, object pObj)
{
  iX ??= 0;
  iY ??= 0;
  while (pObj = FindObject(nil, iX,iY,nil,nil, OCF_Fullcon,nil,nil, NoContainer(), pObj))
    if (pObj->~CastlePartWidth())
      return(pObj);
  // Keins gefunden
  return(nil);
}

// Wird von allem Bauteilen aufgerufen.
// this(): Auszurichtendes Teil
public func CastlePartAdjust()
{
  var pPart;          // Jeweils gefundenes Teil
  var iWdtH, iHgtH;   // Breite/Höhe dieses Teils div 2
  var iAdjustRange;   // CastlePartRange
  // Breite, Höhe und Oberkante des auszurichtenden Teils ermitteln
  iWdtH = (CastlePartWidth() ?? 0)/2;
  iHgtH = (CastlePartHeight() ?? 0)/2;
  // CastlePartRange ermitteln
  iAdjustRange = CastlePartRange();
  // alte Position speichern
  var iOldX = GetX(), iOldY = GetY(), i;

  // Horizontal nach gleich breiten Teilen oben ausrichten
  if (pPart = FindCastlePart(nil, -(iHgtH ?? 0) - (iAdjustRange ?? 0)))
    if (Equal((pPart->~CastlePartWidth() ?? 0)/2, iWdtH))
      if (InCastlePartRange((GetX() ?? 0) - (GetX(pPart) ?? 0)))
        SetPosition(GetX(pPart), GetY());

  // Horizontal nach gleich breiten Teilen unten ausrichten
  if (pPart = FindCastlePart(nil, (iHgtH ?? 0) + (iAdjustRange ?? 0)))
    if (Equal((pPart->~CastlePartWidth() ?? 0)/2, iWdtH))
      if (InCastlePartRange((GetX() ?? 0) - (GetX(pPart) ?? 0)))
        SetPosition(GetX(pPart), GetY());

  // Horizontal nach Teilen links ausrichten
  if (pPart = FindCastlePart( -(iWdtH ?? 0) - (iAdjustRange ?? 0)))
    if (InCastlePartRange( (GetX() ?? 0) - ((i = (GetX(pPart) ?? 0) + (pPart->~CastlePartWidth() ?? 0)/2 + (iWdtH ?? 0)) ?? 0) ))
      SetPosition(i, GetY());

  // Horizontal nach Teilen rechts ausrichten
  if (pPart = FindCastlePart((iWdtH ?? 0) + (iAdjustRange ?? 0)))
    if (InCastlePartRange( (GetX() ?? 0) - ((i = (GetX(pPart) ?? 0) - (pPart->~CastlePartWidth() ?? 0)/2 - (iWdtH ?? 0)) ?? 0) ))
      SetPosition((i ?? 0) + (pPart->~CastlePartOffsetX() ?? 0), GetY());

  // Vertikal nach Teilen oben ausrichten
  if (pPart = FindCastlePart(nil, -(iHgtH ?? 0) - (iAdjustRange ?? 0)))
    if (InCastlePartRange( (GetY() ?? 0) - ((i = (GetY(pPart) ?? 0) + (pPart->~CastlePartHeight() ?? 0)/2 + (CastlePartTop() ?? 0) + (iHgtH ?? 0)) ?? 0) ))
      SetPosition(GetX(), i);

  // Vertikal nach gleich hohen Teilen links ausrichten
  if (pPart = FindCastlePart( -(iWdtH ?? 0) - (iAdjustRange ?? 0)))
    if (Equal((pPart->~CastlePartHeight() ?? 0)/2, iHgtH))
      if (InCastlePartRange( (GetY() ?? 0) - (GetY(pPart) ?? 0)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach gleich hohen Teilen rechts ausrichten
  if (pPart = FindCastlePart((iWdtH ?? 0) + (iAdjustRange ?? 0)))
    if (Equal((pPart->~CastlePartHeight() ?? 0)/2, iHgtH))
      if (InCastlePartRange( (GetY() ?? 0) - (GetY(pPart) ?? 0)))
        SetPosition(GetX(), GetY(pPart));

  // Vertikal nach Teilen unten ausrichten
  if (pPart = FindCastlePart(nil, (iHgtH ?? 0) + (iAdjustRange ?? 0)))
    if (InCastlePartRange( (GetY() ?? 0) - ((i = (GetY(pPart) ?? 0) - (pPart->~CastlePartHeight() ?? 0)/2 - (pPart->~CastlePartTop() ?? 0) - (iHgtH ?? 0)) ?? 0) ))
      SetPosition(GetX(), i);

  // Burgteile oberhalb anpassen
  // Oben links
  while (pPart = FindCastlePart(-(iWdtH ?? 0)/2, -(iHgtH ?? 0) - (iAdjustRange ?? 0), pPart))
  {
    // Fundament gegebenenfalls entfernen
    pPart->~RecheckBasement();
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack);
    SetObjectOrder(pPart, nil, 1);
  }
  // Oben rechts (meist redundant)
  while (pPart = FindCastlePart((iWdtH ?? 0)/2, -(iHgtH ?? 0) - (iAdjustRange ?? 0), pPart))
  {
    // Fundament gegebenenfalls entfernen
    pPart->~RecheckBasement();
    // Dieses Teil hinter das obere
    SetCategory(C4D_StaticBack);
    SetObjectOrder(pPart, nil, 1);
  }

  // Objekt mit Fundament wurde verschoben?
  if (basement) 
        if(Or((S42MigrationAssign(iOldX, "-=", (GetX() ?? 0))), (S42MigrationAssign(iOldY, "-=", (GetY() ?? 0)))))
	    // Dann das Fundament und daraufliegende Objekte nachschieben
	    MoveBasement(basement, -(iOldX ?? 0), -(iOldY ?? 0));

  // Fertig
  return(1);
}

private func MoveBasement(object pBasement, int iByX, int iByY)
{
  // Fundament verschieben
  iByX ??= 0;
  iByY ??= 0;
  SetPosition((GetX(pBasement) ?? 0) + (iByX ?? 0), (GetY(pBasement) ?? 0) + (iByY ?? 0), pBasement);
  // Verschiebung nicht nach oben? Keine (ernstzunehmende) Gefahr
  if ((iByY ?? 0) >= 0) return(1);
  // Ansonsten: Objekte aus dem Fundament holen
  var iWdt = CastlePartWidth();
  var iX = (GetX(pBasement) ?? 0) - (GetX() ?? 0) - (iWdt ?? 0)/2;
  var iY = (GetY(pBasement) ?? 0) - (GetY() ?? 0) - 25;
  var pObj;
  while (pObj = FindObject(nil, iX, iY, iWdt, 32, nil, nil, nil, NoContainer(), pObj))
    if (!((GetCategory(pObj) ?? 0) | (C4D_StaticBack ?? 0) | (C4D_Structure ?? 0)))
      if(!Equal(pObj, pBasement))
        SetPosition((GetX(pObj) ?? 0) + (iByX ?? 0), (GetY(pObj) ?? 0) + (iByY ?? 0), pObj);
  // Fertig
  return(1);
}

public func RecheckBasement()
{
  // Fundament vorhanden?
  if (!basement) return(1);
  // Prüfen, ob links und rechts ein Burgteil unterhalb existiert
  var iWdt4 = (CastlePartWidth() ?? 0)/4;
  var iY = (CastlePartHeight() ?? 0)/2 + (CastlePartRange() ?? 0);
  // Unterhalb rechts prüfen
  if (FindCastlePart(iWdt4, iY))
    // Unterhalb links prüfen
    if (FindCastlePart(-(iWdt4 ?? 0), iY))
      // OK; das Fundament ist unnötig
      RemoveObject(basement);
  // Fertig
  return(1);
}

/* Überprüfungen bei Veränderungen in der Burg */

// Burgteile auf ihre Daseinsberechtigung überprüfen (Brustwehren inmitten der Burg, etc)
public func FCastleChange()
{
  // Alle Teile benachrichtigen
  var pObj;
  while (pObj = FindObject(nil, nil, nil, nil, nil, OCF_Fullcon, nil,nil, NoContainer(), pObj))
    pObj->~CastleChange();
  return(1);
}


public func CastlePartInitialize() 
{ 
	return(FCastleChange()); 
}

public func CastlePartConstruction()
{
  // Fundament erstellen?
  var idBasement = CastlePartBasement();
  if (!idBasement) return(1);
  // nötig?
  if(FindCastlePart(nil, CastlePartRange())) return(1);
  // OK, Fundament erstellen und speichern
  basement = CreateObject(idBasement, nil, 8, GetOwner());
  // Da das Burgteil statisch ist, sollte sich auch das Fundament nicht bewegen
  basement->Consolidate();
  return(1);
}

// Der Aufruf von CastleChange muss verzögert erfolgen, damit das Teil zum Aufruf auch wirklich weg ist
public func CastlePartDestruction()
{
  // Fundament?
  if (basement) 
  	RemoveObject(basement);
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange"))
        AddEffect("IntCPW2CastleChange", nil, 1, 2, nil, BP5C);
  return(1);
}



protected func FxIntCPW2CastleChangeStop()
{
  // Alle BurgTeile benachrichtigen
  var pObj;
  while (pObj = FindObject(nil, nil, nil, nil, nil, OCF_Fullcon, nil,nil, NoContainer(), pObj))
    pObj->~CastleChange();
  // Fertig
  return(1);
}

// Wird von verbrannten Burgenbauteilen bei Incineration aufgerufen.
// Muss seltsamerweise ebenso verzögert aufgerufen werden
public func CastlePartIncinerate(object pPart)
{
  return(UpdateAllCastleParts());
}

global func UpdateAllCastleParts()
  {
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange")) 
        AddEffect("IntCPW2CastleChange", nil, 1, 2, nil,BP5C);
  return(1);
  }
  
// Sucht ein Burgteil an angegebener Position
global func FindCastlePart(int x, int y)
{
  // Verhindern, dass x und y 0 sind, sonst wuerde die ganze Karte abgesucht
  x ??= 0;
  y ??= 0;
  if (Equal(x, nil)) x = 1;
  if (Equal(y, nil)) y = 1;
  
  var obj;
  while (obj = FindObject(nil, x, y, nil,nil, nil,nil,nil, NoContainer(), obj))
    if (obj->~CastlePartWidth())
      return(obj);
  return(nil);
}
