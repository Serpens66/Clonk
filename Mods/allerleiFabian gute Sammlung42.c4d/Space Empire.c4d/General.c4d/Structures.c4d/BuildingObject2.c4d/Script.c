/*-- Gebäudeobjekt --*/

#strict 3


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (Hostile(GetController(clonk),GetOwner())) return(nil);
  // Status anzeigen
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0), max, 5);
  return(1);
}


/* Schaden */

protected func Damage(amount)
{
  // Info-Symbol
  if ((amount ?? 0) > 0)
    if (Equal(GetPlayerParty(GetOwner()), 1)) CreateObject(CE00,nil,nil,GetOwner());
                                    else CreateObject(SE00,nil,nil,GetOwner());
  // noch im Bau?
  if ((GetCon() ?? 0) < 100)
  {
    if ((amount ?? 0) <= 0) return(1);
    // Bau zurücksetzen
    var dmg = (amount ?? 0)*150/(GetMaxDamage() ?? 0);
    if ((dmg ?? 0) >= (GetCon() ?? 0)) return(Destroy());
                    else return(DoCon(-(dmg ?? 0)));
  }
  // zuviel Schaden?
  if ((GetDamage() ?? 0) >= (GetMaxDamage() ?? 0)) return(Destroy());
  // Anzeige
  ShowStatusBar(5);
  return(1);
}

public func ShowStatusBar(length)
{
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0), max, length);
}

public func Destroy()
{
  // Info-Symbol
  if (Equal(GetPlayerParty(GetOwner()), 1)) CreateObject(CE01,nil,nil,GetOwner());
                                  else CreateObject(SE01,nil,nil,GetOwner());
  // Zerstörung durch Explosion
  var strength = (GetMaxDamage() ?? 0)*(GetCon() ?? 0)/100;
  if ((strength ?? 0) < 20)
    strength = 20;
  // Fragmente
  var i = (strength ?? 0) / 8;
  var obj;
  while (i)
  {
    obj = CreateObject(SFRX, (Random(GetObjWidth()) ?? 0) - (GetObjWidth() ?? 0) / 2,
                             (Random(GetObjHeight()) ?? 0) - (GetObjHeight() ?? 0) / 2, GetOwner());
    obj->Settings((i ?? 0)*3/2, 25, 2);
    S42MigrationStep(i, -1);
  }
  // Explosion
  obj = CreateObject(SFRX, nil,nil, GetOwner());
  obj->Settings((strength ?? 0)/10, 45);
  SpecialExplosion();
  RemoveObject();
}

public func PreDestruction()
{
  // Energiesymbole löschen
  var obj;
  if (obj = FindObject(SNOE, nil,nil,nil,nil, nil,nil,this()))  RemoveObject(obj);
  if (obj = FindObject(SEMP, nil,nil,nil,nil, nil,nil,this()))  RemoveObject(obj);
  return(1);
}


/* EMP-Reaktion */

public func EMP_Reaction(...) { return(1);
}


/* spezielle Reaktion bei Vernichtung (z.B. EMP-Schockwelle) */

public func SpecialExplosion(...) { return(1);
}


/* Standard-Eigenschaften */

public func InfoBarPos(...) {   return(nil);
}
public func InfoBarSize(...) {  return(2);
}
public func Repairable(...) {   return(1);
}
public func GetMaxDamage(...) { return(100);
}

