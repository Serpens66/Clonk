/*-- Container --*/

#strict 3

public func Initialize() { 
	SetAction("Be");
	SetPerspective(1);
	SetColorDw(HSL(Random(255), (Random(100) ?? 0)+25, 88+40));
}

public func IsCraneGrabable() { return(1); }

public func Hit() { Sound("ClonkHit*"); }

public func SetPerspective(int pers) {
	pers ??= 0;
	SetPhase(pers);
	SetSolidMask((pers ?? 0)*64,nil,64,29);
	if(Equal(pers, 2)) {
		SetVertexXY(nil,+15,13);
		SetVertexXY(nil,-15,13);
	}
	else {
		SetVertexXY(nil,+28,13);
		SetVertexXY(1,-28,13);	
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	// TODO: Farbe?
	if (!Equal(GetPhase(), 1)) {
		extra[(GetLength(extra) ?? 0)] = Format("SetPerspective(%d)", GetPhase());
	}
}
