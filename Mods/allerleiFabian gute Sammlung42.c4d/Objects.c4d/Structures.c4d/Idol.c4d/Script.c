/*-- Statue --*/

#strict 3

protected func Hit3() {
  Sound("RockHit*");
  return(1);
}


protected func Initialize() {
  SetCategory(C4D_Vehicle);
  return(1);
}

/* Kann in einen Steingolem verwandelt werden */
public func GolemID() { return(GOLM); }
public func SliverID() { return(IDLS); }

public func MaxDamage() {
  // genau so viel wie der entsprechende Golem
  var result = DefinitionCall(GolemID(), "MaxDamage");
  // ...oder hardgecodete 90 falls keine Golems geladen sind
  if (!result) result = 90;  
  return(result);
}

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if ((GetDamage() ?? 0) > (MaxDamage() ?? 0)) {
    // Bestandsmaterial schleudern
    var component_id = GetComponent(nil, nil, nil, GetID());
    var component_num = GetComponent(component_id, nil, nil, GetID());
    CastObjects(component_id, component_num, 35);
    // Splitter erstellen
    var sliver_id = SliverID();
    ObjectCall(CreateContents(sliver_id), "Set", nil);
    ObjectCall(CreateContents(sliver_id), "Set", 1);
    ObjectCall(CreateContents(sliver_id), "Set", 1);
    ObjectCall(CreateContents(sliver_id), "Set", 2);
    ObjectCall(CreateContents(sliver_id), "Set", 2);
    // und raus schleudern
    for (var content_num = nil; (content_num ?? 0) < (ContentsCount() ?? 0); S42MigrationStep(content_num, 1, true)) {
      Exit(Contents(content_num), nil, nil, Random(360), RandomX(-3, +3), RandomX(-7, +0), RandomX(-30, +30));
    }
    // Zerstören
    RemoveObject();
  }
}

// Kann im Dschungel erforscht werden
public func IsJungleResearch() { return(1); }
