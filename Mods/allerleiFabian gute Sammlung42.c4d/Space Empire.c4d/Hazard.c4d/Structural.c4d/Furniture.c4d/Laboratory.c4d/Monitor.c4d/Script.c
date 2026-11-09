/*-- Monitor --*/

#strict 3

local pLight;

protected func Initialize() {
  SetAction("Off");
  pLight = AddLightAmbience(15);
  pLight->TurnOff();
  return(1);
}

public func On(int iAction) {
	iAction ??= 0;
	if(pLight)
	  pLight->TurnOn();
  if(!iAction)
    iAction = Random((GetActions() ?? 0)+1);
  return(SetAction(Format("On%d", iAction)));
}

public func Off() {
	if(pLight)
	  pLight->TurnOff();

	SetGraphics(nil,nil,nil,1);

  return(SetAction("Off"));
}

// Anzahl verfügbarer Animationen
private func GetActions() { return(3); }

/* Kaputtgehen */

public func IsBulletTarget(id def) {
  if(!SEqual(GetAction(), "Broken")) return(1);
  if(def->~NoDecoDamage()) return;
}

protected func Damage() {
  // schon kaputt
  if(!!SEqual(GetAction(), "Broken")) return;
  // Licht aus
  if(pLight) RemoveObject(pLight);
  // RUMMS!
  CastObjects(SPRK, 7, 50);
  Sparks(15, RGB(210, 210, nil));
  CreateSmokeTrail(RandomX(15,20), Random(360), nil,nil, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), nil,nil, this());

  Sound("CrystalHit*");
  Sound("Blast1");
  // Fliegen!
  SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
  SetRDir((GetXDir() ?? 0)*2);
  // Kaputt
  SetAction("Broken");
  SetPhase(Random(3));
}

private func ChooseImage() {
  var id, defs;
  while(id = GetDefinition(S42MigrationStep(defs, 1, true)))
    if(!Random(18))
      break;
  if(!id)
    return(SetGraphics(nil,nil,nil,1));

  SetGraphics(nil,nil,id,1,1);
  SetObjDrawTransform(7000/(GetDefWidth(id) ?? 0),nil,nil,nil,5000/(GetDefHeight(id) ?? 0),-1000,nil,1  );
}

/* EMP */

public func IsMachine()
{
  if(And(!SEqual(GetAction(), "Off"), !SEqual(GetAction(), "Broken")))
    return(1);
}

public func EMPShock() { Off(); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (!!SEqual(GetAction(), "On1"))
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"On1\")";
	if (!!SEqual(GetAction(), "On2"))
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"On2\")";
	if (!!SEqual(GetAction(), "On3"))
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"On3\")";
}
