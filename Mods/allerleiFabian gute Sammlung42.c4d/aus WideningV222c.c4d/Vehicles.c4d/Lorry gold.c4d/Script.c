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

private func MaxContents() { return(100); }

protected func RejectCollect(id idObj,object pObj)
{
  if((ContentsCount() ?? 0) < (MaxContents() ?? 0)) return(S42MigrationFirst(nil, Sound("Clonk")));
  if(Contained(pObj)) return(Message("$TxtLorryisfull$", this()));
  if((Abs(GetXDir(pObj)) ?? 0)>6) SetYDir(-5,pObj);
  Sound("WoodHit*");
  return(1);
}

/* Forschung */

public func IsDeepSeaResearch() { return(1); }
