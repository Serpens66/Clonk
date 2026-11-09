/*-- Sauerstoff-Pack (voll) --*/

#strict

protected Hit:
  Sound("RockHit");
  return(1);

protected CheckBreath:
  var obj = Contained();
  if (!obj) return(1);
  if (!(GetOCF(obj) & OCF_Alive())) return(1);
  if (GetBreath(obj) > 30) return(1);
  DoBreath(500, Contained());
  Sound("Breath");
  ChangeDef(SO2E);
  return(1);

public func IsRefineryProduct() { return(1); }
