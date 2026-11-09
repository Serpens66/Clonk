/*-- Munitionspack --*/

#strict

local value;


/* Initialisierung */

protected Initialize:
  var num = 11;
  while (--num)
    CreateContents(SAM1);
  value = 30;
  return(1);

/* Wert & Verkauf */

protected CalcValue:
  var lastvalue = value;
  value = ContentsCount(SAM1)*3;
  return(lastvalue);

public HasAdditionalDesc: return(1);

public GiveAdditionalDesc:
  return(Format("Inhalt: %d Schuss",ContentsCount(SAM1)));

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

