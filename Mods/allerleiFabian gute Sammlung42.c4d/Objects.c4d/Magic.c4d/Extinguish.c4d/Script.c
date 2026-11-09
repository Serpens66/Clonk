/* Löschen */

#strict 3

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("ExtinguishPSpell", oExtiObj, 180)) return(And(!Equal(iChkEff, -1), RemoveObject()));
  // Lavafässer zu Granit werden lassen
  var obj;
  if (obj = FindContents(LBRL, oClonk))
    if (!CheckEffect("ExtinguishPSpell", obj, 180))
    {
      var mat = "Granite";
      // Granit nicht geladen -> Stein casten
      if (!MaterialLoaded(Material(mat))) mat = "Rock";
      // Stein nicht geladen -> Kohle casten
      if (!MaterialLoaded(Material(mat))) mat = "Coal";
      // Material schleudern
      var amount = Min(400, (obj->GetAmount() ?? 0)*3);
      CastPXS(mat, amount, 40, AbsX(GetX(oClonk)), AbsY(GetY(oClonk)));
      // Objekt entfernen
      RemoveObject(obj);
    }
  // Nächstes brennendes Objekt finden
  var oExtiObj, iExtiCount;
  // Den Clonk selbst?
  if(OnFire(oClonk)) 
    S42MigrationAssign(iExtiCount, "+=", (DoExtinguish(oClonk, oCaller) ?? 0));
  // Nächstliegende Objekte
  SetPosition(GetX(oCaller), GetY(oCaller));
  // Suchradius ist abhängig von Höhe des Aufrufenden - ein Zauberturm hat einen größeren Radius
  var iSearchRadius = (GetObjHeight(oCaller) ?? 0) * 3;
  while(oExtiObj = FindObject(nil, nil, nil, -1, -1, nil, nil, nil, nil, oExtiObj))
    if((ObjectDistance(oCaller, oExtiObj) ?? 0) > (iSearchRadius ?? 0))
      break;
    else if(OnFire(oExtiObj))
      S42MigrationAssign(iExtiCount, "+=", (DoExtinguish(oExtiObj, oCaller) ?? 0));
    else if (oExtiObj->~MagicExtinguish(oCaller))
      S42MigrationAssign(iExtiCount, "+=", (DoExtinguish(oExtiObj, oCaller) ?? 0));
  // Irgendwas gelöscht?
  if (!iExtiCount) return(S42MigrationFirst(nil, Message("$NoExtinguish$", oClonk), RemoveObject()));
  // Einmaliger Effekt pro Zauber
  Sound("Splash1");
  return(true);
}

private func DoExtinguish(object oExtiObj, object oCaller)
  {
  // Nicht wenn gegnerischer CrewMember
  if (Hostile(GetOwner(oExtiObj), GetOwner(oCaller)))
    if ((GetOCF(oExtiObj) ?? 0) & (OCF_CrewMember ?? 0))
      if (!ObjectCount(NTMG)) return(nil);
  // Lösche es
  Extinguish(oExtiObj);
  // Eine paar Effekte...
  CastObjects(MSTB,10,25, (GetX(oExtiObj) ?? 0)-(GetX() ?? 0), (GetY(oExtiObj) ?? 0)-(GetY() ?? 0));
  CastParticles("MSpark", 200, 50, (GetX(oExtiObj) ?? 0)-(GetX() ?? 0),(GetY(oExtiObj) ?? 0)-(GetY() ?? 0), 10, 50, RGBa(100,100,255,128), RGBa(nil,nil,255,nil));
  // 1 Objekt gelöscht
  return(1);
  }

private func MaterialLoaded(mat) {
  if (GetMaterialVal("Name", "Material", mat)) return(1);
  else return(nil);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
