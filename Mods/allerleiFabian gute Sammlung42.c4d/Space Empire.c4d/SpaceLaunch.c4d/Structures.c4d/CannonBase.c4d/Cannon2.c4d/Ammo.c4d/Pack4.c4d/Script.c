/*-- Munitionspack --*/

#strict

local value;


/* Initialisierung */

protected Initialize:
  var num = 11;
  while (--num)
    CreateContents(SAM4);
  value = 40;
  return(1);

/* Wert & Verkauf */

protected CalcValue:
  var lastvalue = value;
  value = ContentsCount(SAM4)*4;
  return(lastvalue);

public HasAdditionalDesc: return(1);

public GiveAdditionalDesc:
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM4)));

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

