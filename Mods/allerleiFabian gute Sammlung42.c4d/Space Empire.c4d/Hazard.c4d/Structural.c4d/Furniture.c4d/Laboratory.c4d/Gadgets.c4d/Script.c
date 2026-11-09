/*-- Apparatur --*/

#strict 3

protected func Initialize() {
	// Ein zufällig Aussehen
	Set((Random(GetActions()) ?? 0)+1);
	return(1);
}

public func Set(int i)
{
	i ??= 0;
	i = BoundBy(i,1,GetActions());
	Call(Format("Gadget%d", i));
}

// Anzahl verschiedener Daseinsformen
private func GetActions() { return(3); }

/* Für jedes Aussehen setzen wir andere Werte */

protected func Gadget1() {
  SetAction("1");
  SetVertexXY(nil, -4, 7);
  SetVertexXY(1,  4, 7);
}

protected func Gadget2() {
  SetAction("2");
  SetVertexXY(nil, -9, 9);
  SetVertexXY(1,  9, 9);
}

protected func Gadget3() {
  SetAction("3");
  SetVertexXY(nil, -9, 8);
  SetVertexXY(1,  9, 8);
}

/* Treffer! */

public func IsBulletTarget(id def) {
  if(def->~NoDecoDamage()) return;
  return(1);
}

protected func Damage() {
  // Viel Glas!
  CastParticles("SplinterGlass", 5+(Random(5) ?? 0), 50, nil,nil, 20,25, RGBa(255,255,255), RGBa(255,255,255));
  // Flüssigkeiten
  if(!SEqual(GetAction(), "3"))
  {
    if(Random(2)) CastPXS("Acid", 5+(Random(10) ?? 0), 50);
    if(Random(2)) CastPXS("DuroLava", 5+(Random(10) ?? 0), 50);
  }
  // Rauch
  CreateSmokeTrail(RandomX(15,20), Random(360), nil,nil, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), nil,nil, this());
  // Sound
  Sound("Glass");
  // Explosion oder Verschwinden
  if(Random(2)) Explode(5+(Random(5) ?? 0));
  else RemoveObject();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%s)", GetAction());
}
