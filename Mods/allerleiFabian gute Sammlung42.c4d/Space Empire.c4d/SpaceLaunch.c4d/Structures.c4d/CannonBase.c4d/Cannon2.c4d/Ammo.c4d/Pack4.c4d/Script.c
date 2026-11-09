/*-- Munitionspack --*/

#strict 3

local value;


/* Initialisierung */

protected func Initialize(...) {
  var num = 11;
  while (S42MigrationStep(num, -1))
    CreateContents(SAM4);
  value = 40;
  return(1);
}

/* Wert & Verkauf */

protected func CalcValue(...) {
  var lastvalue = value;
  value = (ContentsCount(SAM4) ?? 0)*4;
  return(lastvalue);
}

public func HasAdditionalDesc(...) { return(1);
}

public func GiveAdditionalDesc(...) {
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM4)));
}

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

