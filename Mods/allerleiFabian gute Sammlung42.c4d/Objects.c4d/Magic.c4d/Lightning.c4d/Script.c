/*-- Blitz-Zauber --*/

#strict 3

func Activate(pCaster,pRealcaster) {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  var iResult;
  if (iResult = CheckEffect("LightningNSpell", nil, 125)) return(And(!Equal(iResult, -1), RemoveObject()));
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this())) return(1);

  // Blitz erzeugen
  var obj = CreateObject(LGTS, -(GetX() ?? 0), -(GetY() ?? 0), GetOwner(pClonk));
  obj->Launch((GetX(pCaster) ?? 0)+(GetVertex(nil, VTX_X, pCaster) ?? 0), (GetY(pCaster) ?? 0)+(GetVertex(nil, VTX_Y, pCaster) ?? 0), ((!GetDir(pCaster)) ?? 0)*1800);
  Sound("Thunder*");
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // Blitz erzeugen
  iAngle ??= 0;
  var obj = CreateObject(LGTS, -(GetX() ?? 0), -(GetY() ?? 0), GetOwner(pCaller));
  obj->Launch(GetX(pCaller), GetY(pCaller), (900-(iAngle ?? 0)*10));
  Sound("Thunder*");

  // Objekt löschen
  RemoveObject();
  return(1);
  }

// Zielspruch (für Artefaktzauber)
public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
