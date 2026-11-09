/*-- Pflänzchen --*/

#strict 3

local broken;

func Completion() {
SetCategory(C4D_Vehicle);
  var random = Random(PlantCount());
  SetAction(Format("Plant%d", (random ?? 0)+1));
  var x = GetX(), y = (GetY() ?? 0) -2;
    SetPosition(x, y);
  return(1);
}

func Damage() {
  if(broken) return;
  broken = true;

  SetGraphics("Broken");
  SetRDir(RandomX(-10,10));
  SetSpeed(RandomX(-10,10),-20);

  CreateParticle("HalfPot", -2, nil, -10-(Random(5) ?? 0), -2, 50, RGBa(255,255,255,nil));
  CreateParticle("HalfPot",  2, nil,  10+(Random(5) ?? 0), -2, 50, RGBa(255,255,255,nil));

  // lacks of a sound :'(
}

protected func FadeOutt() {
  if(!broken) return;
  if((GetRGBaValue(GetClrModulation()) ?? 0) >= 255) return(RemoveObject());

  SetClrModulation(RGBa(255,255,255, (GetRGBaValue(GetClrModulation()) ?? 0)+1));
}

// Anzahl Pflanzen
private func PlantCount() { return(2); }

// Kann von Kugeln getroffen werden
public func IsBulletTarget(id def) {
  if(broken) return;
  if(def->~NoDecoDamage()) return;
  return(Random(2));
}
