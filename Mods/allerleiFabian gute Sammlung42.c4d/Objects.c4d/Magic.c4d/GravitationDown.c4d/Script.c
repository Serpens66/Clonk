/*-- Gravitation runter --*/

#strict 3

public func Activate(pCaster, pCaster2)
  {
  // Gültigkeit prüfen
  if((GetGravity() ?? 0)<=30) { Message("$MsgGravityMinimum$",pCaster); return(S42MigrationFirst(nil, RemoveObject())); }
  // Magie kann man hören, ganz klar ;)
  Sound("Magic*");
  // Effekt global starten
  if (!AddEffect("GravChangeUSpell", nil, 150, 80, nil, GetID(), -20))
    return(S42MigrationFirst(nil, RemoveObject()));
  // Gravitation anzeigen
  Message("$MsgGravityDec$",pCaster,GetGravity());
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }


/*-- Gravitationseffekt --*/
// Wird auch vom anderen Gravitationszauber verwendet

// EffectVars: 0 - Vorherige Gravitation
//             1 - Änderung durch den Zauber

protected func FxGravChangeUSpellStart(pTarget, iEffectNumber, iTemp, iChange)
  {
  // Effektstart
  if (!iTemp) EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Vorherige Gravitation sichern
  var iOldGrav = EffectVar(nil, pTarget, iEffectNumber) = GetGravity();
  // Für nichttemporäre Aufrufe wird iChange übergeben, und auf den Änderungswert aufgerechnet
  if (iChange) S42MigrationAssign(EffectVar(1, pTarget, iEffectNumber), "+=", (iChange ?? 0));
  // Gravitationsänderung setzen
  // Die Änderung kann in temporären Aufrufen auch ungleich iChange sein
  SetGravity((iOldGrav ?? 0) + (EffectVar(1, pTarget, iEffectNumber) ?? 0));
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellTimer(pTarget, iEffectNumber)
  {
  // Gravitation in Richtung Normalwert schrauben
  var iGravChange = EffectVar(1, pTarget, iEffectNumber);
  // Fertig?
  if (Inside(iGravChange, -1, 1)) return(-1);
  // Anpassen
  var iDir = -(iGravChange ?? 0)/(Abs(iGravChange) ?? 0);
  if ((Abs(iGravChange) ?? 0) > 40) S42MigrationAssign(iDir, "*=", 2);
  S42MigrationAssign(EffectVar(1, pTarget, iEffectNumber), "+=", (iDir ?? 0));
  SetGravity((GetGravity() ?? 0) + (iDir ?? 0));
  return(1);
  }

protected func FxGravChangeUSpellStop(pTarget, iEffectNumber)
  {
  // Gravitation Wiederherstellen
  SetGravity(EffectVar(nil, pTarget, iEffectNumber));
  // Effekt entfernen
  return(1);
  }

protected func FxGravChangeUSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Falls der neu hinzugefügte Effekt auch eine Gravitationsänderung ist, Interesse am Übernehmen anmelden
  if (SEqual(szNewEffect, "GravChangeUSpell")) return (-3);
  // Ansonsten ignorieren
  return;
  }

protected func FxGravChangeUSpellAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iChange)
  {
  // Aufruf erfolgt, wenn der Effekt übernommen werden konnte
  // Gravitationsänderung auf diesen Effekt aufrechnen
  S42MigrationAssign(EffectVar(1, pTarget, iEffectNumber), "+=", (iChange ?? 0));
  SetGravity((GetGravity() ?? 0) + (iChange ?? 0));
  // Grafikeffekt
  EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellDoSparks(pTarget, iEffectNumber, iMovement)
  {
  // Graphischer Effekt
  var lwdt=LandscapeWidth(),lhgt=LandscapeHeight(),x=GetX(),y=GetY();
  var i=(LandscapeWidth() ?? 0)*(LandscapeHeight() ?? 0)/10000;
  while (S42MigrationStep(i, -1, true)) CreateParticle("MSpark", (Random(lwdt) ?? 0)-(x ?? 0),(Random(lhgt) ?? 0)-(y ?? 0), nil,(iMovement ?? 0)*2, 30+(Random(50) ?? 0), RGBa(255-(Random(50) ?? 0),255-(Random(50) ?? 0),nil,128));
  }

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
