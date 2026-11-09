/*-- Fernsteuerung --*/

#strict

local target, controller;


/* Initialisierung */

public func Init(par_target, par_controller)
{
  target = par_target;
  controller = par_controller;
  // alte Objekte dieser Sorte entfernen
  var obj = FindObject(GetID(),0,0,0,0,0,"Attach",target);
  if (obj) RemoveObject(obj);
  // an das Zielobjekt heften
  SetAction("Attach",target);
  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,1,target));
  // zu Crew hinzufügen
  SetOwner(GetOwner(controller));
  MakeCrewMember(this(),GetOwner());
  SetName(Format("Fernsteuerung %s",GetName(target)));
  // als aktiv markieren und andere abwählen 
  SetCursor(GetOwner(),this());
  var clonk;
  while (clonk = FindObjectOwner(0,GetOwner(),0,0,0,0,OCF_CrewMember(),0,0,clonk))
    if (clonk != this())
      SelectCrew(GetOwner(),clonk,0);
  // unsichtbar für andere Spieler
  SetVisibility(VIS_Owner());
}


/* Timer-Call */

protected func Check()
{
  // außer Reichweite, Ziel zerstört, Clonk im Fahrzeug?
  var dist = Max(ObjectDistance(controller)-100, 0) / 3;
  if ((dist > 100) || !target || FindContents(HZCK,target))
    return(LostRC());
  DoEnergy(100-dist - GetEnergy());
  // immer schön attached bleiben
  if (ActIdle())
    SetAction("Attach",target);
}


/* Steuerung weiterleiten */

// normaler Tastendruck
protected ControlLeft:  return(ProtectedCall(target,"ContainedLeft",this()));
protected ControlRight: return(ProtectedCall(target,"ContainedRight",this()));
protected ControlUp:    return(ProtectedCall(target,"ContainedUp",this()));
protected ControlDown:  return(ProtectedCall(target,"ContainedDown",this()));
protected ControlThrow: return(ProtectedCall(target,"ContainedThrow",this()));
protected ControlDig:   return(ProtectedCall(target,"ContainedDig",this()));
// einfach
protected ControlLeftSingle:  return(ProtectedCall(target,"ContainedLeftSingle",this()));
protected ControlRightSingle: return(ProtectedCall(target,"ContainedRightSingle",this()));
protected ControlUpSingle:    return(ProtectedCall(target,"ContainedUpSingle",this()));
protected ControlDownSingle:  return(ProtectedCall(target,"ContainedDownSingle",this()));
protected ControlThrowSingle: return(ProtectedCall(target,"ContainedThrowSingle",this()));
protected ControlDigSingle:   return(ProtectedCall(target,"ContainedDigSingle",this()));
// doppelt
protected ControlLeftDouble:  return(ProtectedCall(target,"ContainedLeftDouble",this()));
protected ControlRightDouble: return(ProtectedCall(target,"ContainedRightDouble",this()));
protected ControlUpDouble:    return(ProtectedCall(target,"ContainedUpDouble",this()));
protected ControlDownDouble:  return(ProtectedCall(target,"ContainedDownDouble",this()));
protected ControlThrowDouble: return(ProtectedCall(target,"ContainedThrowDouble",this()));
protected ControlDigDouble:   return(ProtectedCall(target,"ContainedDigDouble",this()));

// Abbruch
protected ControlSpecial:  return(AbortRC());
protected ControlSpecial2: return(AbortRC());


/* Abbruch der Verbindung */

protected func AbortRC()
{
  PlayerMessage(GetOwner(), "Verbindung abgebrochen", controller);
  SetCursor(GetOwner(), controller);
  var clonk;
  while (clonk = FindObjectOwner(0,GetOwner(),0,0,0,0,OCF_CrewMember(),0,0,clonk))
    if (clonk != controller)
      SelectCrew(GetOwner(),clonk);
  return(RemoveObject());
}

protected func LostRC()
{
  PlayerMessage(GetOwner(), "Verbindung verloren", controller);
  SetCursor(GetOwner(), controller);
  var clonk;
  while (clonk = FindObjectOwner(0,GetOwner(),0,0,0,0,OCF_CrewMember(),0,0,clonk))
    if (clonk != controller)
      SelectCrew(GetOwner(),clonk);
  return(RemoveObject());
}

// Eigenschaften
public func IsRefineryProduct() { return(1); }
