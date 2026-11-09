/*-- Fernsteuerung --*/

#strict 3

local target, controller;


/* Initialisierung */

public func Init(par_target, par_controller)
{
  target = par_target;
  controller = par_controller;
  // alte Objekte dieser Sorte entfernen
  var obj = FindObject(GetID(),nil,nil,nil,nil,nil,"Attach",target);
  if (obj) RemoveObject(obj);
  // an das Zielobjekt heften
  SetAction("Attach",target);
  SetVertex(nil,nil,GetVertex(nil,nil,target));
  SetVertex(nil,1,GetVertex(nil,1,target));
  // zu Crew hinzufügen
  SetOwner(GetOwner(controller));
  MakeCrewMember(this(),GetOwner());
  SetName(Format("Fernsteuerung %s",GetName(target)));
  // als aktiv markieren und andere abwählen 
  SetCursor(GetOwner(),this());
  var clonk;
  while (clonk = FindObjectOwner(nil,GetOwner(),nil,nil,nil,nil,OCF_CrewMember,nil,nil,clonk))
    if (!Equal(clonk, this()))
      SelectCrew(GetOwner(),clonk,nil);
  // unsichtbar für andere Spieler
  SetVisibility(VIS_Owner);
}


/* Timer-Call */

protected func Check()
{
  // außer Reichweite, Ziel zerstört, Clonk im Fahrzeug?
  var dist = (Max((ObjectDistance(controller) ?? 0)-100, nil) ?? 0) / 3;
  if (Or(Or(((dist ?? 0) > 100), !target), FindContents(HZCK,target)))
    return(LostRC());
  DoEnergy(100-(dist ?? 0) - (GetEnergy() ?? 0));
  // immer schön attached bleiben
  if (ActIdle())
    SetAction("Attach",target);
}


/* Steuerung weiterleiten */

// normaler Tastendruck
protected func ControlLeft(...) {  return(ProtectedCall(target,"ContainedLeft",this()));
}
protected func ControlRight(...) { return(ProtectedCall(target,"ContainedRight",this()));
}
protected func ControlUp(...) {    return(ProtectedCall(target,"ContainedUp",this()));
}
protected func ControlDown(...) {  return(ProtectedCall(target,"ContainedDown",this()));
}
protected func ControlThrow(...) { return(ProtectedCall(target,"ContainedThrow",this()));
}
protected func ControlDig(...) {   return(ProtectedCall(target,"ContainedDig",this()));
}
// einfach
protected func ControlLeftSingle(...) {  return(ProtectedCall(target,"ContainedLeftSingle",this()));
}
protected func ControlRightSingle(...) { return(ProtectedCall(target,"ContainedRightSingle",this()));
}
protected func ControlUpSingle(...) {    return(ProtectedCall(target,"ContainedUpSingle",this()));
}
protected func ControlDownSingle(...) {  return(ProtectedCall(target,"ContainedDownSingle",this()));
}
protected func ControlThrowSingle(...) { return(ProtectedCall(target,"ContainedThrowSingle",this()));
}
protected func ControlDigSingle(...) {   return(ProtectedCall(target,"ContainedDigSingle",this()));
}
// doppelt
protected func ControlLeftDouble(...) {  return(ProtectedCall(target,"ContainedLeftDouble",this()));
}
protected func ControlRightDouble(...) { return(ProtectedCall(target,"ContainedRightDouble",this()));
}
protected func ControlUpDouble(...) {    return(ProtectedCall(target,"ContainedUpDouble",this()));
}
protected func ControlDownDouble(...) {  return(ProtectedCall(target,"ContainedDownDouble",this()));
}
protected func ControlThrowDouble(...) { return(ProtectedCall(target,"ContainedThrowDouble",this()));
}
protected func ControlDigDouble(...) {   return(ProtectedCall(target,"ContainedDigDouble",this()));
}

// Abbruch
protected func ControlSpecial(...) {  return(AbortRC());
}
protected func ControlSpecial2(...) { return(AbortRC());
}


/* Abbruch der Verbindung */

protected func AbortRC()
{
  PlayerMessage(GetOwner(), "Verbindung abgebrochen", controller);
  SetCursor(GetOwner(), controller);
  var clonk;
  while (clonk = FindObjectOwner(nil,GetOwner(),nil,nil,nil,nil,OCF_CrewMember,nil,nil,clonk))
    if (!Equal(clonk, controller))
      SelectCrew(GetOwner(),clonk);
  return(RemoveObject());
}

protected func LostRC()
{
  PlayerMessage(GetOwner(), "Verbindung verloren", controller);
  SetCursor(GetOwner(), controller);
  var clonk;
  while (clonk = FindObjectOwner(nil,GetOwner(),nil,nil,nil,nil,OCF_CrewMember,nil,nil,clonk))
    if (!Equal(clonk, controller))
      SelectCrew(GetOwner(),clonk);
  return(RemoveObject());
}

// Eigenschaften
public func IsRefineryProduct() { return(1); }
