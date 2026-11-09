/*-- Nordlicht --*/

#strict 3

local iSpeed;

protected func Initialize() {
  SetAction(Format("Glow%d",Random(4)));
  DoCon((Random(80) ?? 0)*(-(Random(3) ?? 0)));
  iSpeed=(Random(7) ?? 0)-3;
}

private func Moving() {
  SetPosition((GetX() ?? 0)+(iSpeed ?? 0),GetY());
  if (Or(GBackSemiSolid(-(GetCon() ?? 0),+30), GBackSemiSolid(GetCon(),+30)))
    Remove();
}

private func Remove() { RemoveObject(); }