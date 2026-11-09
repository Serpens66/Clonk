/*-- Gebäudeobjekt --*/

#strict


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (Hostile(GetController(clonk),GetOwner())) return(0);
  // Status anzeigen
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), max-GetDamage(), max, 5);
  return(1);
}


/* Schaden */

protected func Damage(amount)
{
  // Info-Symbol
  if (amount > 0)
    if (GetPlayerParty(GetOwner()) == 1) CreateObject(CE00,0,0,GetOwner());
                                    else CreateObject(SE00,0,0,GetOwner());
  // noch im Bau?
  if (GetCon() < 100)
  {
    if (amount <= 0) return(1);
    // Bau zurücksetzen
    var dmg = amount*150/GetMaxDamage();
    if (dmg >= GetCon()) return(Destroy());
                    else return(DoCon(-dmg));
  }
  // zuviel Schaden?
  if (GetDamage() >= GetMaxDamage()) return(Destroy());
  // Anzeige
  ShowStatusBar(5);
  return(1);
}

public func ShowStatusBar(length)
{
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), max-GetDamage(), max, length);
}

public func Destroy()
{
  // Info-Symbol
  if (GetPlayerParty(GetOwner()) == 1) CreateObject(CE01,0,0,GetOwner());
                                  else CreateObject(SE01,0,0,GetOwner());
  // Zerstörung durch Explosion
  var strength = GetMaxDamage()*GetCon()/100;
  if (strength < 20)
    strength = 20;
  // Fragmente
  var i = strength / 8;
  var obj;
  while (i)
  {
    obj = CreateObject(SFRX, Random(GetObjWidth()) - GetObjWidth() / 2,
                             Random(GetObjHeight()) - GetObjHeight() / 2, GetOwner());
    obj->Settings(i*3/2, 25, 2);
    --i;
  }
  // Explosion
  obj = CreateObject(SFRX, 0,0, GetOwner());
  obj->Settings(strength/10, 45);
  SpecialExplosion();
  RemoveObject();
}

public func PreDestruction()
{
  // Energiesymbole löschen
  var obj;
  if (obj = FindObject(SNOE, 0,0,0,0, 0,0,this()))  RemoveObject(obj);
  if (obj = FindObject(SEMP, 0,0,0,0, 0,0,this()))  RemoveObject(obj);
  return(1);
}


/* EMP-Reaktion */

public EMP_Reaction: return(1);


/* spezielle Reaktion bei Vernichtung (z.B. EMP-Schockwelle) */

public SpecialExplosion: return(1);


/* Standard-Eigenschaften */

public InfoBarPos:   return(0);
public InfoBarSize:  return(2);
public Repairable:   return(1);
public GetMaxDamage: return(100);

