/*-- Sägewerk --*/

#strict 3

#include CXEC
#include BAS7
#include DOOR


/* Produktion */

public func IsProducerOf (object clonk, id def) {
  if (!Equal(def, WOOD)) return (nil);
  if (!IsBuilt ()) return (nil);
  if (NeedsEnergy ())
    if (FindSuppliedObjectCloseTo (clonk)) return (nil);
//  if (!FindWorkTree (clonk)) return (0);
  return(1);
}

private func FindSuppliedObjectCloseTo (object obj, id def) {
  var obj2;
  if (!def) def = GetID ();
  while (obj2 = FindObject (def, ((GetX (obj) ?? 0) - 1000) - (GetX () ?? 0), ((GetY (obj) ?? 0) - 500) - (GetY () ?? 0), 2000, 1000,
    OCF_Fullcon , nil, nil, nil, obj2))
      if (FindObject (PWRL, nil,nil,nil,nil, nil,"Connect",obj2))
        return(obj2);
  return(nil);
}

// Kann Holz produzieren
public func HowToProduce (object obj, id def) {
  //Eventuell erst versorgen
  if (NeedsEnergy ())
  {
    AddCommand (obj, "Call", this (), WOOD, nil, nil, nil, "HowToProduce");
    AddCommand (obj, "Energy", this ());
    return (1);
  }
  AddCommand (obj, "Wait", nil, nil, nil, nil, nil, 70);
  // Arbeitsbefehl setzen
  Production (obj);
  return (1);
}

public func Production(object clonk)
{
  // Baum suchen
  var pTree;
  if (!(pTree = FindWorkTree(clonk))) {
    // Kein geeigneter Baum (fertig)
    AddMessage("$TxtNomoresuitabletrees$",clonk);
    // Habe ich schon erwähnt, dass ich C4Command hasse?
    FinishCommand (clonk, nil, nil);
    FinishCommand (clonk, nil, 1);
    return(nil); /* fail command - if only the Call-Command would check the return value... */
   }
  // Baum markieren, damit er nicht nochmal verwendet wird, falls er nicht schiebbar ist
  AddEffect ("IntSawmillTreeMarker", pTree, 1, 5000, this (), nil, clonk);
  // Baum ins Sägewerk schieben...
  AddCommand(clonk,"PushTo",pTree,nil,nil,this(), 500, nil,nil, 3);
  // ...aber erst fällen
  if (!((GetOCF(pTree) ?? 0) & (OCF_Grab ?? 0)))
    AddCommand(clonk,"Chop",pTree, nil,nil,nil,nil,nil,nil, 3);
  // Fertig
  return(1);
}

private func FindWorkTree(object clonk)
{
  // Arbeiter fällt bereits einen Baum
  if (!!SEqual(GetAction(clonk), "Chop"))
    return(GetActionTarget(nil, clonk));
  // Arbeiter hat bereits einen Baum angefaßt
  if (!!SEqual(GetAction(clonk), "Push"))
    if (GetActionTarget(nil,clonk)->~IsTree())
      return(GetActionTarget(nil,clonk));
  // Baum zum Anfassen suchen
  var obj;
  if (obj = FindTreeToGrab()) return(obj);
  // Baum zum Fällen suchen
  if (obj = FindTreeToChop()) return(obj);
  // Kein Baum gefunden
  return(nil);
}

private func FindTreeToChop()
{
  var obj;
  while (obj = FindObject(nil,nil,nil,-1,-1,OCF_Chop,nil,nil,nil,obj))
    // Ausgewachsen
    if ((GetOCF(obj) ?? 0) & (OCF_Fullcon ?? 0))
    // Im freien Bereich
    if ((GetOCF(obj) ?? 0) & (OCF_InFree ?? 0))
    // Nicht markiert
    if (!GetEffectCount ("IntSawmillTreeMarker", obj))
    // Nicht zu weit weg
    if ((ObjectDistance(obj) ?? 0) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Baum wird nicht bereits von einem Clonk gefällt
    if (!FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,"Chop",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(nil);
}
  
private func FindTreeToGrab()
{
  var obj;
  while (obj = FindObject(nil,nil,nil,-1,-1,OCF_Grab,nil,nil,nil,obj))
    // Ist ein Baum
    if (obj->~IsTree())
    // Nicht in einem Behälter
    if (!Contained(obj))
    // Nicht markiert
    if (!GetEffectCount ("IntSawmillTreeMarker", obj))
    // Nicht zu weit weg
    if ((ObjectDistance(obj) ?? 0) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Steckt nicht fest
    if (!Stuck(obj))
    // Baum wird nicht bereits von einem Clonk angefaßt
    if (!FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,"Push",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(nil);
}
  
/* Kontext */

public func ContextConstruction(object clonk)
{
  [$TxtProducewood$|Image=WOOD|Condition=IsBuilt]
  return(ProductionLoop(clonk));
}

protected func IsBuilt() {
  return((GetCon() ?? 0) >= 100);
}

/* Steuerung */

protected func ContainedUp(object clonk)
{
  [$TxtProducewood$|Image=WOOD]
  return(ProductionLoop(clonk));
}

public func ProductionLoop (clonk) { //Horus
  // 2.) ...danach nochmal produzieren.
  AppendCommand(clonk,"Call",this(),nil,nil,nil,nil,"ProductionLoop");
  // 1.) Erst produzieren...
  Production(clonk);
  return(1);
}

public func ProductionLoopFailed () {
  return (1); // Schweig!
}
  
/* Verarbeitung */

private func ContentsCheck()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  // Noch aktiv
  if (!ActIdle()) return(nil);
  // Inhalts- und Energiecheck
  EnergyCheck(nil);
  var obj;
  if (!(obj = ContentsNotCrewMember())) return(nil);
  if (!EnergyCheck(25000)) return(nil);
  // Kann zersägt werden?
  var fSawable = And(And(And(!Equal(GetID(obj), WOOD), GetComponent(WOOD,nil,obj)), ComponentAll(obj,WOOD)), !obj->~IsArrow());
  // Nicht zersägbare empfindliche Objekte oder Fahrzeuge über den Ausgang rauswerfen
  if(!fSawable) if(Or(GetDefFragile(GetID(obj)), (GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0)))
  {
    SetCommand(obj, "Exit");
    return;
  }
  // Objekt zerlegen
  if (fSawable)
  {
    var iWoodCount = obj->GetComponent(WOOD);
    for (var i = nil; (i ?? 0) < (iWoodCount ?? 0); S42MigrationStep(i, 1, true))
      CreateContents(WOOD);
    obj->RemoveObject();
  }
  // sonst einfach auswerfen
  else if(!Equal(GetID(obj), WOOD))
    return Exit(obj,-20,14,nil,-1);
  // Sägen
  SetAction("Saw");
  Sound("Sawmill");
  return(1);
}

private func Sawing()
{
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 35) return(nil);
  // Fertig
  SetAction("Idle");
  // Inhalt auswerfen
  Exit(ContentsNotCrewMember(1),-20,14,nil,-1);
  return(1);
}

private func SawingDust()
{
  if (!Random(5)) 
    CastParticles("Dust",4,8,-23,6,10,12);
} 

private func ContentsNotCrewMember(fWood)
{
  var obj;
  for(var i = nil; obj = Contents(i); S42MigrationStep(i, 1, true))
    if(And(!((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0)), (Or(!fWood, Equal(GetID(obj), WOOD)))))
      return(obj);
  return(nil);
}
