/*-- Sentry Gun --*/

#strict 3

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local iPat_Dir;
local Active;
local GotTarget;
local pWeapon;
local anzahl;

func Construction()
{
SetPosition(GetX(),(GetY() ?? 0)-5);
}

/* Aufrufe */

public func TurnOn()
{
  Active = true;
}

public func TurnOff()
{
  Active = nil;
}

public func Arm(id idWeapon)
{
  // Crash-Vermeidung
  if(!idWeapon) return;
  if(!GetName(nil, idWeapon)) return;
//Log(Format("%v",anzahl));
//Geld abziehen
if ((GetWealth(GetOwner()) ?? 0)< 300) {PlayerMessage(GetOwner(),"Nicht genug Geld",this());return;}
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)- 300);
  Sound("Cash");

  //Ausrüsten mit idWeapon
  pWeapon = CreateObject(idWeapon, nil, nil, GetOwner());
  Enter(this(),pWeapon);
  //Ordnern
  SetObjectOrder(this(), pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ (Random(2) ?? 0)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this();
  Reload();
}

func UnArm(id iWeapon)
{
if(pWeapon)
RemoveObject(pWeapon);
}

func Destruction(a,b,c,d,e,f)
{
if(pWeapon) RemoveObject(pWeapon);
return(_inherited(a,b,c,d,e,f));
}


/* Script */

public func Initialize() 
{
var gnc;
gnc = FindObject2(Find_ID(_GNC),Find_Owner(GetOwner()));
if (!gnc) CreateObject(_GNC,1,1,GetOwner());
//Log("erstellt");
  //Ausrichten
  FitToTop();
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  //lieber Overlay 3 nehmen. Falls wir mal 2-overlayige waffen einbaun :]
  SetGraphics(nil,this(),GetID(),3,5,nil,nil,this());
}

public func WeaponAt(&x, &y, &r) {
  x = nil;
  y = 7000;
  r = (aim_angle ?? 0)+270;
  return(1);
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  x = (EffectVar(2, this(), number) ?? 0)/1000;
  y = (EffectVar(3, this(), number) ?? 0)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  x = (EffectVar(4, this(), number) ?? 0)/1000;
  y = (EffectVar(5, this(), number) ?? 0)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(nil);
  return(EffectVar(1, this(), number));
}

public func FitToTop()
{
  //Das Gestell passt sich selbst mal etwas besser zur Decke an.
  var x = GetX(), y = (GetY() ?? 0) - (GetObjHeight() ?? 0) / 2;
  
  while(! GBackSolid(nil, -8) )
  {
    S42MigrationAssign(y, "-=", 1);
    
    //Endlosschleife vermeiden
    if((y ?? 0) < 0) return(1);
    
    SetPosition(x, y);
  }
  return(1);
}

public func Activity()
{
anzahl=ObjectCount2(Find_ID(SEGU),Find_Owner(GetOwner()));
  var iHeight, iWidth, iAngle;
  //Wuah, haben wir eine Waffe?
  if(! GetAttWeapon()) return;
  //Sind wir im Eimer?
  if( EMPShocked()) return;
  //Sind wir aktiv?
  if(! Active) return;
  //Wenn nicht schon gesetzt: Turn-Action
  if(!SEqual(GetAction(), "Turn"))
    SetAction("Turn");

  /*Patroullie fahren*/
  
  // alle 5 Frames
  if(!((GetActTime() ?? 0)%5)) {
	  //Zu weit links?
	  if( (aim_angle ?? 0) <= (MaxRotLeft() ?? 0) )
	  {
	    //Wir fahren zurück
	    iPat_Dir = 1;
	    target_angle = MaxRotRight();
	    GotTarget = nil;
	  }
	  //Oder zu weit rechts?
	  else if( (aim_angle ?? 0) >= (MaxRotRight() ?? 0) )
	  {
	    //Hinfahren
	    iPat_Dir = -1;
	    target_angle = MaxRotLeft();
	    GotTarget = nil;
	  }
	if(!GotTarget)
		S42MigrationAssign(aim_angle, "+=", (iPat_Dir ?? 0)*3);
  }
  
  //Das Fahren selber ;)
  if(GotTarget)
        S42MigrationAssign(aim_angle, "+=", (BoundBy((target_angle ?? 0)-(aim_angle ?? 0),-8,8) ?? 0));
  
       
  /* Feinde suchen */
  
  if(!GotTarget)
  {
  	if(Shooting)
  	{
	    Shooting = nil;
   		GetAttWeapon()->StopAutoFire();
  	}
  		
	  GotTarget = Search();
	}
	else
	{
		target_angle = Angle(GetX(), (GetY() ?? 0) + 7, GotTarget->GetX(), GotTarget->GetY());
		if((Abs((aim_angle ?? 0) - (target_angle ?? 0)) ?? 0) < 15)
		{
		    Shooting = true;
		    if(!GetAttWeapon()->IsShooting())
			    GetAttWeapon()->ControlThrow(this());
                    if((GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) ?? 0) < (GetAttWeapon()->GetFMData(FM_AmmoUsage) ?? 0))

//Geld abziehen
{if ((GetWealth(GetOwner()) ?? 0)< 300) {PlayerMessage(GetOwner(),"Nicht genug Geld zum Nachladen",this());return;}
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)- 300);
  Sound("Cash");Reload();}
		}
		else
		{
	    Shooting = nil;
   		GetAttWeapon()->StopAutoFire();
		}
		
		if(!CheckTarget(GotTarget,this()))
			GotTarget = nil;
		else if(!PathFree(GetX(),(GetY() ?? 0)+7,GotTarget->GetX(), GotTarget->GetY()))
			GotTarget = nil;
		else if((ObjectDistance(GotTarget,this()) ?? 0) > (SearchLength() ?? 0))
			GotTarget = nil;
	}
	 
  /*
  //Berechnung
  iAngle = aim_angle;
  iWidth =  Sin(iAngle, SearchLength());
  iHeight = -Cos(iAngle, SearchLength());

  if(iWidth < 0)
    Search(iWidth, -iWidth, iHeight);
  else
    Search(0, iWidth, iHeight);
  */
}

public func Search(int iX, int iWidth, int iHeight)
{
  iX ??= 0;
  iWidth ??= 0;
  iHeight ??= 0;
  var pAim;
  
  /*DrawParticleLine("PSpark", 0, 0, -70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));
  DrawParticleLine("PSpark", 0, 0, 70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));*/ 

	var w,h;
  
	var Targets = FindTargets(this(), SearchLength()); //FindObjects(Find_Distance(SearchLength()),Find_PathFree(),Find_NoContainer());
  for(pAim in Targets)
  {
    if(!Equal(GetOwner(), NO_OWNER))
      if(Or(Equal(pAim->GetOwner(), GetOwner()), !Hostile(pAim->GetOwner(), GetOwner())))
        continue;

		if(!CheckTarget(pAim,this()))
			continue;


    //Winkel zum Ziel
    target_angle = Angle(GetX(), (GetY() ?? 0) + 7, pAim->GetX(), pAim->GetY());

		target_angle = Normalize(target_angle, nil);
    if(Or((target_angle ?? 0) < (MaxRotLeft() ?? 0), (target_angle ?? 0) > (MaxRotRight() ?? 0)))
    	continue;

		return pAim;


    break;
  }
  
  if(And(Shooting, ! pAim))
  {
    Shooting = nil;
    GetAttWeapon()->StopAutoFire();
  }
  
  if(!pAim)
    GotTarget = true;
}

private func Reload()
{
  // Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  // Erzeugen
  Local(nil, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  // Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this());
  GetAttWeapon()->~Recharge();
  GetAttWeapon()->~StopAutoFire();
}

public func MaxDamage() {
	return( 100 );
}

public func Damage()
{
  if((GetDamage() ?? 0) > (MaxDamage() ?? 0))
    Explode(20);
}

public func EMPShock()
{
    //EMP Granate!1 :O
	EMPShockEffect(20*35);
	if(GetAttWeapon())
		GetAttWeapon()->StopAutoFire();
    return(1);
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
    i ??= 0;
    if(Equal(i, 1))
    {
      if(Active) return("$TurnOff$");
      else
          return("$TurnOn$");
    }
    if(And(Equal(i, 2), EMPShocked()))
        return("$Repair$");
}

public func ConsoleControlled(int i)
{
    i ??= 0;
    if(Equal(i, 1))
    {
        Active = nil;
    }
    if(Equal(i, 2))
    {
        Active = 1;
    };
}

//wird in "gun" skript gemacht, welches von der Konsole gesteuert wird und ruft dann bei allen Segus "arm()" oder "unarm()" auf.
/*
func ChooseWeapon()
{
CreateMenu (GetID(), GetCursor(GetOwner()), 0, 0, "wähle Waffe", 0,1, 0, 0);
 
if(!pWeapon)
AddMenuItem ("MG ausrüsten (300 C.)", "Arm", MGSA, GetCursor(GetOwner()), 0, 0, "Rüstet alle Geschütze mit einer MG aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker"); 

if(!pWeapon)
AddMenuItem ("Railgun ausrüsten (300 C.)", "Arm", RGSA, GetCursor(GetOwner()), 0, 0, "Rüstet alle Geschütze mit einer Railgun aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker"); 

if(pWeapon)
AddMenuItem ("Aktuelle Waffen entfernen", "UnArm", MGSA, GetCursor(GetOwner()), 0, 0, "Entfernt die aktuell gewählte Waffe, es gibt kein Geld zurück"); 
}
*/


public func UpdateCharge() { return(1); }

//Für das Abfragen
public func GetAttWeapon() { return(cur_Attachment); } //Waffe
public func MaxRotLeft()   { return(110); } //Maximaler Winkel links
public func MaxRotRight()  { return(250); } //Maximaler Winkel rechts
public func SearchLength() { return(250); } //Suchlänge
public func AimAngle()     { return(aim_angle); } //Winkel auf Ziel
public func ReadyToFire()  { return(1); } //Allzeit bereit
public func IsMachine()    { return(true); } //Ist eine Elektrische Anlage
public func IsBulletTarget() { return(true); } //Kugelziel
public func IsAiming()     { return(true); } // Die Sentry Gun "zielt" immer

/* Serialisierung */

public func RejectContainedSerialize(object foo) { return !nil; } // weg mit den alten Waffen

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetTeam(%d)", GetTeam());
	if (Active)
		extra[(GetLength(extra) ?? 0)] = "TurnOn()";
	if (cur_Attachment) {
		extra[(GetLength(extra) ?? 0)] = Format("Arm(%i)", GetID(cur_Attachment));
		extra[(GetLength(extra) ?? 0)] = Format("LocalN(\"aim_angle\")=%d", aim_angle);
		extra[(GetLength(extra) ?? 0)] = Format("LocalN(\"iPat_Dir\")=%d", iPat_Dir);
	}
}

public func IsThreat() { return(Active); }