/*-- Meteor --*/

#strict 3

local explosion_base;

protected func SmokeTrail() {
  Smoke(nil, nil, 5);
  Smoke(nil, -5, Random(7));
  var i = Random(50);
  while (S42MigrationStep(i, -1, true)) {
    var c = 20+(Random(41) ?? 0);
    CreateParticle("PxSpark", (Random(11) ?? 0)-5, (Random(11) ?? 0)-5, nil, nil, c,
      RGBa(255, Random(100), (c ?? 0)*4, 128));
  }
  CreateParticle("PxSpark", nil, nil, nil, nil, 20,
    RGBa(255, 155, nil, 128));
}

public func Completion() {
  var iColor;
  iColor = RGBa(235,150,10,50);
  AddLight3(1000, iColor, this());
  explosion_base = 10;
  SetAction("Evaporate");
}

public func SetExplosionBase(new_base) {
  explosion_base = new_base;
}

protected func Hit() {
  Explode((explosion_base ?? 0)+(Random(12) ?? 0));
}
