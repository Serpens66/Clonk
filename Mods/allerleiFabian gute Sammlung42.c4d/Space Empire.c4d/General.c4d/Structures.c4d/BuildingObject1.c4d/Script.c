/*-- Gebäudeobjekt --*/

#strict 3


/* Türsteuerung: Die ActMap muss natürlich darauf abgestimmt sein */

private func OpenEntrance(...) {  return(SetEntrance(1));
}
private func CloseEntrance(...) { return(SetEntrance(nil));
}

private func SoundOpenDoor()
{
  Sound("SteelGate2");
  // Status anzeigen
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0), max, 5);
  return(1);
}

private func SoundCloseDoor()
{
  Sound("SteelGate2");
  // verfeindete Clonks gleich rausschmeissen
  var doorX = (GetDefCoreVal("Entrance","DefCore",nil,nil) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,2) ?? 0) / 2;
  var doorY = (GetDefCoreVal("Entrance","DefCore",nil,1) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,3) ?? 0);
  var clonk;
  while (clonk = FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this(),clonk))

// nur wenn basis.. 
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
  if (And(Hostile(GetOwner(),GetOwner(clonk)), obj))
      Exit(clonk,doorX,doorY);
  return(1);
}

public func ActivateEntrance(clonk)
{
  // verfeindet und von außen?  -> nur wenn Gebäude Basis ist!

var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
  if (And(Hostile(GetOwner(),GetOwner(clonk)), obj))
    if (!Contained(clonk))
    {
      Message("Eingang versperrt!",this());
      return(nil);
    }
  // Türe öffnen
  if (ActIdle()) SetAction("OpenDoor");
  return(1);
}

public func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
    {
      GrabContents(obj);
      SetCommand(obj,"Exit");
    }
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
  // Inhalt rausschleudern
  var obj;
  while (obj = Contents())
    Exit(obj,nil,nil,Random(360), (Random(9) ?? 0)-4, -(Random(5) ?? 0), (Random(51) ?? 0)-25);
  // Zerstörung durch Explosion
  var strength = (GetMaxDamage() ?? 0)*(GetCon() ?? 0)/100;
  if ((strength ?? 0) < 20)
    strength = 20;
  // Fragmente
  var i = (strength ?? 0) / 8;
  while (i)
  {
    obj = CreateObject(SFRX, (Random(GetObjWidth()) ?? 0) - (GetObjWidth() ?? 0) / 2,
                             (Random(GetObjHeight()) ?? 0) - (GetObjHeight() ?? 0) / 2, GetOwner());
    obj->Settings((i ?? 0)*3/2, 30, 2);
    S42MigrationStep(i, -1);
  }
  // Explosion
  obj = CreateObject(SFRX, nil,nil, GetOwner());
  obj->Settings((strength ?? 0)/10, 60);
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

