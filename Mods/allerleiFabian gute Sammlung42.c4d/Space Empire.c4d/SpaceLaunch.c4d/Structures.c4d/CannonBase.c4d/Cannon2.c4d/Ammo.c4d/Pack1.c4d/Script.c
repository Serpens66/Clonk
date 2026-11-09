/*-- Munitionspack --*/

#strict 3

local value;


/* Initialisierung */

protected func Initialize(...) {
  var num = 11;
  while (S42MigrationStep(num, -1))
    CreateContents(SAM1);
  value = 30;
  return(1);
}

/* Wert & Verkauf */

protected func CalcValue(...) {
  var lastvalue = value;
  value = (ContentsCount(SAM1) ?? 0)*3;
  return(lastvalue);
}

public func HasAdditionalDesc(...) { return(1);
}

public func GiveAdditionalDesc(...) {
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM1)));
}

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

