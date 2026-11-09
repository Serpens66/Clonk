/* Tarnzauber */

#strict 3

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;

  // Zaubernder Clonk nicht im Freien -> Abbruch
  if (Contained(clonk)) {
    NoCamouflage(caster, "$must_be_in_free$");
    return(nil);
  }

  // Sonst den zaubernden Clonk draußen tarnen
  return(Camouflage(clonk));
}

protected func Camouflage(clonk) {
  Sound("Magic1");
  // Der Zauberer wird in den magischen Stein hineinversetzt
  var container = CreateObject(MRCK, nil,nil, GetOwner());
  Enter(container, clonk);
  // Aktivitaet auf Walk zuruecksetzen, wenn attacht (wuerde sonst sofort wieder aus dem Stein rauskommen)
  if(!!SEqual(clonk->GetProcedure(), "ATTACH")) clonk->SetAction("Walk");
  // Clonk trägt Pfeil -> Stein wird geschleudert
  var comb_obj;
  comb_obj = FindContents(ARRW, clonk);
  if(!comb_obj) if(FindContents(ARWP, clonk))
	  comb_obj = FindContents(ARWP, clonk)->GetItem();
  if(comb_obj) {
    var dir_sign = -1;
    if (Equal(GetDir(clonk), DIR_Right)) dir_sign = +1;
    SetXDir((dir_sign ?? 0) * 35, container);
    SetYDir(-40, container);
    RemoveObject(comb_obj);
  }
  RemoveObject();
  return(1);
}

protected func NoCamouflage(caster, error_string) {
  Sound("Error");
  Message(error_string, caster);
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("166"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
