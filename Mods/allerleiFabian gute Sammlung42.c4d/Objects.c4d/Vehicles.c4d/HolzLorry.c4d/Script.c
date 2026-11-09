/*-- Lore --*/

#strict 3

/* Status */

public func IsLorry() { return(1); }

/* Steuerung */

protected func ContactLeft() {
  if(And(Stuck(), !Random(5))) SetRDir(RandomX(-7, +7));
}

protected func ContactRight() {
  if(And(Stuck(), !Random(5))) SetRDir(RandomX(-7, +7));
}

private func ControlElevator(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Im Fahrstuhlkorb
    if(pElev->~IsElevator())
      // Steuerung an Fahrstuhl weiterleiten
      return(ObjectCall(pElev,szCommand,pObject));
  return(nil);
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Lore ist angehalten
      if (Equal(GetComDir(), COMD_Stop))
        // Steuerung an Fahrstuhl weiterleiten
        return(ObjectCall(pElev,szCommand,pObject));
  return(nil);
}

private func ControlElevatorStop(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Fahrstuhlkorb bewegt sich
      if (!Equal(GetComDir(pElev), COMD_Stop))
      {
        // Fahrstuhlkorb anhalten
        ObjectCall(pElev,szCommand,pObject); 
        // Noch nicht aussteigen
        SetComDir(COMD_Stop, pObject);
        SetComDir(COMD_Stop);
        // Bewegunsbefehl abbrechen
        return(1);
      }
  return(nil);
}

/* Füllmengenkontrolle */

private func MaxContents() { return(50); }

protected func RejectCollect(id idObj,object pObj)
{
  if((ContentsCount() ?? 0) < (MaxContents() ?? 0)) return(S42MigrationFirst(nil, Sound("Clonk")));
  if(Contained(pObj)) return(Message("$TxtLorryisfull$", this()));
  if((Abs(GetXDir(pObj)) ?? 0)>6) SetYDir(-5,pObj);
  Sound("WoodHit*");
  return(1);
}


/* Automatisches Ausleeren in Gebäuden */

protected func Entrance(object pNewContainer)
  {
  // Nur in Gebäuden (auch Burgteile)
  if ((GetCategory(pNewContainer) ?? 0) & ((C4D_StaticBack ?? 0) | (C4D_Structure ?? 0)))
    // Nicht, wenn es das Gebäude verbietet
    if (And(!pNewContainer->~NoLorryEjection(this()), !pNewContainer->~IsStaircase()))
      {
      // Lore entleeren
      pNewContainer->GrabContents(this());
      }
  }

/* Forschung */

public func IsDeepSeaResearch() { return(1); }

/* Einlade-Helfer */

protected func ContextLoadUp(object pClonk)
{
  [$TxtLoadUp$|Image=LRY1]
  // Alte Kommandos des Clonks löschen
  SetCommand(pClonk, "None");
  // Lore ist bereits voll
  if ((ContentsCount() ?? 0) >= (MaxContents() ?? 0))
    return(Message("$TxtLorryisfull$", this()));
  // Maximale noch mögliche Zuladung bestimmen
  var iMaxLoad = (MaxContents() ?? 0) - (ContentsCount() ?? 0);
  // Frei liegende Gegenstände in der Umgebung automatisch einladen
  var iRange = 60;
  var pObj, iCount;
  while (pObj = FindObject(nil, -(iRange ?? 0), -(iRange ?? 0)/2, (iRange ?? 0)*2, (iRange ?? 0)*2, OCF_Collectible, nil,nil, NoContainer(), pObj))
    if ((pObj->GetOCF() ?? 0) & (OCF_Available ?? 0))
    {
      // Maximale Zuladung berücksichtigen (auch die noch vom Clonk kommt)
      if (S42MigrationStep(iCount, 1) > (iMaxLoad ?? 0) - (pClonk->ContentsCount() ?? 0)) break;
      // Einladen
      AddCommand(pClonk, "Put", this(), nil,nil, pObj);
    }
  // Der Clonk soll seine getragenen Objekte auch einladen (ansonsten legt er sie nur irgendwo ab)
  for (var i = nil; (i ?? 0) < (Min(pClonk->ContentsCount(), iMaxLoad) ?? 0); S42MigrationStep(i, 1, true))
    AddCommand(pClonk, "Put", this(), nil,nil, pClonk->Contents(i));
}
