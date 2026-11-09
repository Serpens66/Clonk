/* Unsichtbarkeitszauber mit Effektsystem */

#strict 3

// EffectVars:
//   0 - Vorheriger Sichtbarkeitsstatus
//   1 - Vorherige Farbmodulation

public func Activate(object pCaster, object pCaster2)
  {
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  // Magie kann man hören, ganz klar ;)
  Sound("Magic*");
  // Wenn Objektkomponente vorhanden, mehrere Objekte unsichtbar machen
  var pCom=FindContents(LNKT,pCaster),pObj,iCnt;
  // Objektkomponente vorhanden
  if(pCom) {
    // alle Objekte in der Nähe zur Unsichtbarmachung in Erwägung ziehen
    while(pObj=FindObject(nil,nil,nil,-1,-1,nil,nil,nil,NoContainer(),pObj)) {
      // nur Freunde (außer wenn eine bestimmte Regel aktiviert ist)
      if(And(Hostile(GetOwner(pObj),GetOwner(pCaster)), !ObjectCount(NTMG))) continue;
      // nur Objekte die einen Besitzer haben
      if(Equal(GetOwner(pObj), -1)) continue;
      // Objekt zu weit weg oder schon genug Objekte unsichtbar gemacht 
      if(Or((ObjectDistance(pObj,pCaster) ?? 0)>100, (S42MigrationStep(iCnt, 1, true))>(RandomX(3,5) ?? 0))) break;
      AddEffect("InvisPSpell",pObj,1400,1111,nil,GetID());
    }
  }
  if((iCnt ?? 0)>1) RemoveObject(pCom);
  // Zauberer unsichtbar machen (40sec)
  else AddEffect("InvisPSpell", pCaster, 200, 1400, nil, GetID());
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }

protected func FxInvisPSpellStart(object pTarget, int iEffectNumber, int iTemp)
  {
  // Vorherige Sichtbarkeit des Zauberers speichern
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar(nil, pTarget, iEffectNumber) = GetVisibility(pTarget);
  EffectVar(1, pTarget, iEffectNumber) = GetClrModulation(pTarget);
  // Zauberer unsichtbar machen
  SetVisibility((VIS_Owner ?? 0) | (VIS_Allies ?? 0) | (VIS_God ?? 0), pTarget);
  // Halbdurchsichtig bläulich für den Besitzer und Verbündete
  SetClrModulation(ModulateColor(GetClrModulation(pTarget),RGBa(127,127,255,127)), pTarget);
  // Fertig
  return(1);
  }

protected func FxInvisPSpellStop(object pTarget, int iEffectNumber, int iCause, bool fTemp)
  {
  // Status wiederherstellen
  iEffectNumber ??= 0;
  iCause ??= 0;
  fTemp ??= false;
  SetVisibility(   EffectVar(nil, pTarget, iEffectNumber), pTarget);
  SetClrModulation(EffectVar(1, pTarget, iEffectNumber), pTarget);
  // Fertig
  return(1);
  }

protected func FxInvisPSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Andere Unsichtbarkeitszauber aufnehmen
  if (SEqual(szNewEffect, "InvisPSpell")) return(-2);
  // Sonst keine Behandlung
  return;
  }

protected func FxInvisPSpellAdd(object pTarget, int iEffectNumber, string szNewEffect, int iNewTimer)
  {
  // Effekt aufrechnen: Rest vom alten Timer ermitteln
  iEffectNumber ??= 0;
  iNewTimer ??= 0;
  var iOldTimer = (GetEffect(nil, pTarget, iEffectNumber, 3) ?? 0) - (GetEffect(nil, pTarget, iEffectNumber, 6) ?? 0);
  // Diesen Effekt mit neuem Timer weiterführen
  ChangeEffect(nil, pTarget, iEffectNumber, szNewEffect, (iOldTimer ?? 0) + (iNewTimer ?? 0));
  // Fertig
  return(1);
  }

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("122"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
