/* Kleines Kraftfeld (Zauber) */

#strict 3

public func Activate(pCaster, pCaster2)
{
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  if (Contained(pCaster))
    if (Contained(pCaster)->~IsGolem())
      pCaster = Contained(pCaster);
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("FFBridgePSpell", pCaster, 150)) { RemoveObject(); return (!S42MigrationEqual(iChkEff, -1)); }
  // Kraftfeld erstellen
  ObjectSetAction(CreateObject(FRCS,nil,50,GetOwner()),"Field",pCaster);
  CastParticles("MSpark", 5, 50, nil,nil, 100, 200, RGBa(100,100,255,128), RGBa(nil,nil,255,nil));
  CreateParticle("MSpark", nil,nil, 1000, RGBa(50,50,255,150));
  RemoveObject();
  return true;
}

public func GetSpellClass(object pMage) { return WATR; }
public func GetSpellCombo(pMage) { return "266"; } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
