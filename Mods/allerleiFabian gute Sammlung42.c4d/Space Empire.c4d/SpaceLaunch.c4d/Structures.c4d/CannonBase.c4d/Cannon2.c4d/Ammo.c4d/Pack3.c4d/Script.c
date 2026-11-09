/*-- Munitionspack --*/

#strict

local value;


/* Initialisierung */

protected Initialize:
  var num = 11;
  while (--num)
    CreateContents(SAM3);
  value = 40;
  return(1);

/* Wert & Verkauf */

protected CalcValue:
  var lastvalue = value;
  value = ContentsCount(SAM3)*4;
  return(lastvalue);

public HasAdditionalDesc: return(1);

public GiveAdditionalDesc:
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM3)));

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }
