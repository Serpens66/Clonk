#strict 3

local iDamage;

/* Schleim-Schuss */

public func Launch(int iXDir, int iYDir, int iDmg)
{
  //Standardwerte setzen
  iXDir ??= 0;
  iYDir ??= 0;
  iDmg ??= 0;
  if(!iDmg)     iDmg = 5+(Random(5) ?? 0);

  //und zuweisen
  iDamage = iDmg;
  SetSpeed(iXDir, iYDir);

  var self = this();
  SetAction("Travel");
  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  AddAlienFlareEffect(this(),200);
  AddEffect("HitCheck", this(), 1,1, nil, SHT1);
  return(1);
}

/* Treffer */

private func Hit() {
  CreateObject(GLOB, nil, nil, GetOwner())->~Glob2Ground(iDamage);
  RemoveObject();
}

private func HitObject(object pObject) {
  CreateObject(GLOB, nil, nil, GetOwner())->~AttachTo(pObject);
  DoDmg(iDamage, DMG_Bio, pObject);
  RemoveObject();
}
