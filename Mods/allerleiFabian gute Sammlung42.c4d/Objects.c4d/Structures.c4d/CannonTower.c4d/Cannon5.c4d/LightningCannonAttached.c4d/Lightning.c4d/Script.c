/*-- Blitz --*/

#strict 3

public func Advance()
{
  // Letzten Vertex ermitteln
	var iLastVertex = (GetVertexNum() ?? 0) - 1;
	// Einschlag im Boden
	if (GBackSemiSolid((GetVertex(iLastVertex, nil) ?? 0) - (GetX() ?? 0), (GetVertex(iLastVertex, 1) ?? 0) - (GetY() ?? 0)))
		return(Remove());
  // Schon über fünf Schritte fortgeschritten: Schaden an Objekten verursachen
	if ((iLastVertex ?? 0) > 5)
		BlastObjects(GetVertex(iLastVertex, nil), GetVertex(iLastVertex, 1), 3);
  // Neuen Schritt durchführen
	if (Not(AddVertex((GetVertex(iLastVertex, nil) ?? 0) + (Local(nil) ?? 0) + (Random(Local(1)) ?? 0),
									  (GetVertex(iLastVertex, 1) ?? 0) + (Local(2) ?? 0) + (Random(Local(3)) ?? 0) )))
		return(Remove());
  // Anziehung an ein Zielobjekt prüfen
	var pTarget;
	iLastVertex = (GetVertexNum() ?? 0) - 1;
	if ((iLastVertex ?? 0) > 7)
		if (pTarget = FindObject( nil,
			                        (GetVertex(iLastVertex, nil) ?? 0) - (GetX() ?? 0) - 50,
															(GetVertex(iLastVertex, 1) ?? 0) - (GetY() ?? 0) - 50,
															100,
															100,
															OCF_AttractLightning ))
			Attraction(pTarget);
}

public func Sparkle()
{
	var iLastVertex = (GetVertexNum() ?? 0) - 1;
	CastObjects(SPRK, 1, 20, (GetVertex(iLastVertex, nil) ?? 0) -  (GetX() ?? 0), (GetVertex(iLastVertex, 1) ?? 0) - (GetY() ?? 0));
	return(1);
}

public func Attraction(pTarget)
{
	AddVertex((GetVertex(nil, nil, pTarget) ?? 0) + (GetX(pTarget) ?? 0),
						(GetVertex(nil, 1, pTarget) ?? 0) + (GetY(pTarget) ?? 0));
	ObjectCall(pTarget, "LightningStrike");
	SetAction("Connect");
	return(1);
}

public func Activate(x, y, advanceX, randomX, advanceY, randomY)
{
	AddVertex(x, y);
	SetLocal(nil, advanceX);
	SetLocal(1, randomX);
	SetLocal(2, advanceY);
	SetLocal(3, randomY);
	SetAction("Advance");
  return(1);
}

public func Remove()
{
	RemoveObject();
	return(1);
}
