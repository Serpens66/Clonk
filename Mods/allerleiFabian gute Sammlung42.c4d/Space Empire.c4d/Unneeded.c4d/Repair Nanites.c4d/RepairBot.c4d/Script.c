/*-- RepairBot --*/

#strict 3

private func FlyProcess(...) {
  // Gebäude zum Reparieren da?
  var target;
  while (target = FindObject(nil, nil,1,nil,nil, nil,nil,nil, NoContainer(), target))
    if (And(DefinitionCall(GetID(target),"Repairable"), GetDamage(target)))
    {
      // Objekt reparieren
      SetAction("Repair");
      DoDamage(-1,target);
      Sound("Repair");
      // Funken versprühen
      if (Random(2))
        CastParticles("MSpark", 2+(Random(4) ?? 0),20, nil,nil, 20,30, RGBa(128,192,255,64), RGBa(255,255,255,64));
    }
  return(1);
}

protected func Activity(...) {
  // nix machen
  if (!Random(2)) return(1);
  // verschüttet: sterben
  if (GBackSolid()) return(Death());
  // einfach so sterben
  if (!Random(30)) return(Death());
  // Richtung ändern
  if (!Random(2)) SetComDir((Random(9) ?? 0)+1);
  // Verfolgung aufgeben
  if (!Random(4)) SetCommand(this(),"None");
  // neues Ziel verfolgen
  if (!Random(5))
  {
    var obj = GetObject2Repair();
    if (obj) SetCommand(this(),"Follow",obj);
  }
  return(1);
}
  
private func GetObject2Repair(...) {
  var obj;
  while (obj = FindObject(nil, nil,nil,-1,-1, nil,nil,nil, NoContainer(),obj))
    if (!InLiquid(obj))
      if (GetDamage(obj))
        if (DefinitionCall(GetID(obj),"Repairable"))
          return(obj);
  return(nil);
}

protected func Initialize(...) {
  SetAction("Fly");
  SetComDir((Random(9) ?? 0)+1);
  return(1);
}

protected func Damage(...) { return(RemoveObject());
}
protected func Death(...) {  return(RemoveObject());
}

