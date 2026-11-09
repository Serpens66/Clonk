/*--- Scythe ---*/

#strict

public func Activate(pClonk) {
	var trad = FindObject2(Find_Distance(30), Find_Func("IsCuttable"));        
	if (!trad) {
		Message("Du musst dich vor etwas Durchtrennbarem befinden",this());
		return(0);
	}	
	else {
		SetAction("Cut");
		return(1);
	}
	return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}

protected func Sounds() {
  var nalle = FindObject2(Find_Distance(30), Find_Func("IsCuttable"));
  if(nalle) {
    return(Sound("Wirecut"));
  } else {
    SetAction("Idle");
    Message("Du musst schon beim ziel stehenbleiben", this);
  }
}

protected func Cut() {
  if(!Contained()) return(SetAction("Idle"));
  var nalle = FindObject2(Find_Distance(30), Find_Func("IsCuttable"));
  if(nalle) {
    Message("Durchtrennt!", this);
    nalle->~WireCut();
  }
}
/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
