/*-- Gebäudeobjekt --*/

#strict


/* Türsteuerung: Die ActMap muss natürlich darauf abgestimmt sein */

private OpenEntrance:  return(SetEntrance(1));
private CloseEntrance: return(SetEntrance(0));

private func SoundOpenDoor()
{
  Sound("SteelGate2");
  // Status anzeigen
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), max-GetDamage(), max, 5);
  return(1);
}

private func SoundCloseDoor()
{
  Sound("SteelGate2");
  // verfeindete Clonks gleich rausschmeissen
  var doorX = GetDefCoreVal("Entrance","DefCore",0,0) + GetDefCoreVal("Entrance","DefCore",0,2) / 2;
  var doorY = GetDefCoreVal("Entrance","DefCore",0,1) + GetDefCoreVal("Entrance","DefCore",0,3);
  var clonk;
  while (clonk = FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this(),clonk))

// nur wenn basis.. 
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(0,upl1);
if(!obj) obj = GetActionTarget(0,upl2);
if(!obj) obj = GetActionTarget(0,flag);
  if (Hostile(GetOwner(),GetOwner(clonk)) && obj)
      Exit(clonk,doorX,doorY);
  return(1);
}

public func ActivateEntrance(clonk)
{
  // verfeindet und von außen?  -> nur wenn Gebäude Basis ist!

var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(0,upl1);
if(!obj) obj = GetActionTarget(0,upl2);
if(!obj) obj = GetActionTarget(0,flag);
  if (Hostile(GetOwner(),GetOwner(clonk)) && obj)
    if (!Contained(clonk))
    {
      Message("Eingang versperrt!",this());
      return(0);
    }
  // Türe öffnen
  if (ActIdle()) SetAction("OpenDoor");
  return(1);
}

public func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(++num))
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
  // Inhalt rausschleudern
  var obj;
  while (obj = Contents())
    Exit(obj,0,0,Random(360), Random(9)-4, -Random(5), Random(51)-25);
  // Zerstörung durch Explosion
  var strength = GetMaxDamage()*GetCon()/100;
  if (strength < 20)
    strength = 20;
  // Fragmente
  var i = strength / 8;
  while (i)
  {
    obj = CreateObject(SFRX, Random(GetObjWidth()) - GetObjWidth() / 2,
                             Random(GetObjHeight()) - GetObjHeight() / 2, GetOwner());
    obj->Settings(i*3/2, 30, 2);
    --i;
  }
  // Explosion
  obj = CreateObject(SFRX, 0,0, GetOwner());
  obj->Settings(strength/10, 60);
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

