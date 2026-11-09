/*-- Munitionspack --*/

#strict

local value;


/* Initialisierung */

protected Initialize:
  var num = 11;
  while (--num)
    CreateContents(SAM2);
  value = 30;
  return(1);

/* Wert & Verkauf */

protected CalcValue:
  var lastvalue = value;
  value = ContentsCount(SAM2)*3;
  return(lastvalue);

public HasAdditionalDesc: return(1);

public GiveAdditionalDesc:
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM2)));

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }
