/*-- Gravitation rauf --*/

#strict 3

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;
  // Kombiniert mit Beton -> Clonk in den Boden stecken
  var obj;
  if (And(!Contained(clonk), (obj = FindContents(CNCR, clonk)))) {
    var height = GetDefCoreVal("Height", "DefCore", GetID(clonk));
    var radius = RandomX(25, 35);
    var x = GetX(clonk);
    var y = GetY(clonk);
    // Unter dem Clonk freischütteln und buddeln
    DigFree(x, (y ?? 0)+(height ?? 0)+(radius ?? 0), (height ?? 0)/2);
    ShakeFree(x, (y ?? 0)+(height ?? 0)+((radius ?? 0)/2), radius);
    // Tiefstes freigeschüttelte Y-Position ermitteln
    while (!GBackSolid(AbsX(x), AbsY(S42MigrationStep(y, 1, true) + (height ?? 0)))) {}
    // Effekte!
    DrawParticleLine("MSpark",
      nil, nil, nil, AbsY(y),
      (height ?? 0)*3/2, (height ?? 0)*5/2,
      RGBa(255-(Random(50) ?? 0),255-(Random(50) ?? 0), nil,128),
      RGBa(155-(Random(50) ?? 0),155-(Random(50) ?? 0), nil,128),
      -10);
    // Zauberer an ermittelte Stelle versetzen
    SetPosition(x, y, clonk);
    // Objekt entfernen
    RemoveObject(obj);
  }
  // Sonst normale Wirkung (Gravitation erhöhen)
  else {
    // Gravitation bereits auf Maximum?
    if ((GetGravity() ?? 0) >= 290)
      return(S42MigrationFirst(nil, Message("$MsgAlreadyMaxed$", caster), RemoveObject()));
    // Sonst erhöhen
    // Effekt global starten
    if (!AddEffect("GravChangeUSpell", nil, 150, 80, nil, MGDW, 20))
      return(S42MigrationFirst(nil, RemoveObject()));
    Message("$MsgRaised$", caster, GetGravity());
  }
  Sound("Magic1");
  RemoveObject();
  return(1);
}

// Zauberklassen für den Magen
public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("255"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
