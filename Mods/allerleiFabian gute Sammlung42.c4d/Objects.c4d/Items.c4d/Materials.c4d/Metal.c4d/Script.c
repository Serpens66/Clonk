/*-- Metall --*/

#strict 3
#include LOAM

protected func Initialize()
  {
  // Kein Komponentenupdate aus LOAM
  }

public func BridgeMaterial() {
  return(Material("Granite"));
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

private func RuleNoStructure(clonk) {
  if (FindObject(NMTT)) {
    var obj;
    while (obj = FindObject(nil, nil, 1, nil, nil, nil, nil, nil, NoContainer(), obj))
      if ((GetCategory(obj) ?? 0) & (C4D_Structure ?? 0))
        if (Hostile(GetOwner(obj), GetOwner(clonk))) {
          Message("$TxtNobridgeallowedhere$", this());
          Sound("Error");
          return(1);
        }
  }
  return(nil);
}

private func OnBridge() {
  // Eventuelle Bewegung anhalten
  SetXDir(nil, Contained());
  SetComDir(COMD_Stop, Contained());
}

public func Activate(caller) {
  [$TxtBuildmetalbridge$]
  if (RuleNoStructure(caller)) return(nil);
  return(_inherited(caller));
}

// Wir leiten von LOAM ab, sind aber kein Alchemie-Container
public func IsAlchemContainer() { return(nil); }

// Und auch kein Indianerzeug
public func IsIndianResearch() { return(nil); }
public func IsIndianHandcraft() { return(nil); }
