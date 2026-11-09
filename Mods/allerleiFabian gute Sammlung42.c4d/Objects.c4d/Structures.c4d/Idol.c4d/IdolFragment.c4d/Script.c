#strict 3

local living_time;	// Lebenszeit

protected func Initialize() {
  SetAction("Be");
}

public func Set(phase) {
  SetPhase(phase);
  var width = GetDefCoreVal("Width", "DefCore", GetID());
  var height = GetDefCoreVal("Height", "DefCore", GetID());
  SetPicture((width ?? 0)*(phase ?? 0), nil, width, height);
}

protected func SellTo() {
  return(GetComponent(nil, nil));
}

// Timercall
protected func Timer() {
  if (S42MigrationStep(living_time, 1, true) > 60*5) {
    Split2Components();
  }
}

protected func Hit() {
  Sound("RockHit*");
}  