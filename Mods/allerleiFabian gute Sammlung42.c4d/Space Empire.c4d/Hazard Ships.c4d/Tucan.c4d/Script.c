/*-- Tucan --*/

#strict 3
#include XFER   // Transfer-Script

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

  local kanzel1, kanzel2, hangar, arsenal, pilot, counter, bridge, team,mlight;

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(hangar),Find_Container(arsenal),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(bridge))) )
if(And(obj->~IsClonk(), Equal(GetController(), -1))) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {owner=GetOwner(obj);
  if(!Equal(GetOwner(obj), GetOwner(this())))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  SetOwner(owner,hangar);
  SetOwner(owner,arsenal);
  SetOwner(owner,kanzel1);
  SetOwner(owner,kanzel2);
  SetOwner(owner,bridge);}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(hangar),Find_Container(arsenal),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(bridge)));
if(!obj) SetController(-1);
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}

//Initialisierung
func Initialize() {

  SetEntrance(1);
  SetAction("Schwebe");

//Geschütze erstellen und anbringen
  kanzel1=CreateObject(CGG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 5 , kanzel1);
  kanzel1->Rotinit(120,70,175);
  kanzel1->Bezug(this());

  kanzel2=CreateObject(CGG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 4 , kanzel2);
  kanzel2->Rotinit(-120,-175,-70);
  kanzel2->Bezug(this());

//Brücke erstellen und integrieren
  bridge=CreateObject(TBR_,nil,nil,GetOwner());
  ObjectSetAction(bridge,"Attaching",this());
  SetActionData(256 + 14 , bridge);
  bridge->Bezug(this());

//Hangar erstellen und integrieren
  hangar=CreateObject(TCH_,nil,nil,GetOwner());
  ObjectSetAction(hangar,"Attaching",this());
  SetActionData(256 + 14 , hangar);
  hangar->Bezug(this());

//Arsenal erstellen und integrieren
  arsenal=CreateObject(TAS_,nil,nil,GetOwner());
  ObjectSetAction(arsenal,"Attaching",this());
  SetActionData(256 + 14 , arsenal);
  arsenal->Bezug(this());
  return(1);
}

func Departure(a,b,c,d,e,f)
{
Nachdeparture();
_inherited(a,b,c,d,e,f);
}

func Nachdeparture()
{
//um festhängen zu vermeiden
SetPosition(GetX(),(GetY() ?? 0)-20,this());
}

public func Activation(iTeam)
{
  team=iTeam;
  SetAction("Waiting");
}

public func Waiting()
{  
  SetAction("Schwebe");
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}


//Schaden

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(90);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if((GetDamage() ?? 0)>310) Explosion();
  if((GetDamage() ?? 0)<300) return(nil);
  counter=nil;
  SetPhysical("Float",200,2);
  SetAction("Down");
  SetComDir(COMD_Down);
  ObjectCall(Local(1),"ComDown",this());
  return(1);
}

func Down()
{
  S42MigrationStep(counter, 1, true);
  if((counter ?? 0)>300) Explosion();
  Schaden();
}

func Schaden()
{
  //Schaden sichtbar machen
  if((GetDamage() ?? 0)>=60)
  CreateParticle("Smoke2",-10 , nil,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=150)
  CreateParticle("Smoke2",65 ,10,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=240)
  {
  CreateParticle("Smoke2",-40 ,nil,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Thrust",-45+(Random(10) ?? 0) ,Random(10),(GetXDir() ?? 0)/2,+2,RandomX(70,90),RGBa(150,50,50,60),nil,nil);
  }
  if((GetDamage() ?? 0)>=270)
  {
  CreateParticle("Smoke2",-3 ,15,(GetXDir() ?? 0)/2,-20,50+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if(Equal(Random(250-(GetDamage() ?? 0)*2), 1)) DoDamage(3);
  }
}

func Hit()
{
  if(SEqual(GetAction(),"Down"))  Explosion();
  return(1);
}

func Removal()
{
  kanzel1->Explosion();
  kanzel2->Explosion();
  hangar->Explosion();
  bridge->Explosion();
  if(arsenal) RemoveObject(arsenal);  
  if(kanzel1) RemoveObject(kanzel1);
  if(kanzel2) RemoveObject(kanzel2);
  if(hangar) RemoveObject(hangar);
  if(bridge) RemoveObject(bridge);
  return(1);
}

func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  SetXDir(-30,CreateObject(_TB1,-20,20));
  SetXDir(30,CreateObject(_TB2,20,20));
  CreateObject(_TB3,nil,20);
  if(ObjectCount(GFSD))  GameCall("Flagship",team);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(50); else FakeExplode(50,nil,nil,50);
  return(1);
}

func ActivateEntrance() {
  Sound("SteelGate2");
  return(1);
}

//Steuerung
func ContainedLeft() {

  return(1);
}

func ContainedRight() {

  return(1);
}

func ContainedUp() {

  return(1);
}

func ContainedDown() {

  return(1);
}

func ContainedThrow(){

  return(1);
}

func ContainedDig(pByObj)
{
  pilot=pByObj;
  CreateMenu(TCN_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(bridge->Besetzt())    AddMenuItem("%s","Bridge",T20H,Par(nil));  //Brücke
  if(!(bridge->Besetzt())) AddMenuItem("%s","Bridge",T20G,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",M17G,Par(nil));
  AddMenuItem("%s","Statuskontrolle",M57G,Par(nil)); //Anzeige Energie
  AddMenuItem("%s","Ausstieg",M47G,Par(nil));    //Ausstieg
  AddMenuItem("%s","Arsenal",T15G,Par(nil));  //Arsenal
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",M17G,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(hangar->Besetzt())    AddMenuItem("%s","Hangar",T11W,Par(nil));  //Hangar
  if(!(hangar->Besetzt())) AddMenuItem("%s","Hangar",T11Q,Par(nil));
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  SelectMenuItem(7,Par(nil));
  return(1);
}


func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,210,200,40);
  mlight = AddLight2(2500, iColor, this());
  return(1);
}

func TurnLightOff()
{
  if (mlight) {RemoveObject(mlight);}
  return(1);
}


func Ausstieg(pByObj)
{
  Sound("Airlock1");
  Exit(pilot,nil,-15);
  return(1);
}


func Geschuetzkanzel1()
{
  if(kanzel1->Besetzt()) 
  {
  Message("Kanzel besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(kanzel1,pilot);
  Sound("Airlock1");
  return(1);
}


func Geschuetzkanzel2()
{
  if(kanzel2->Besetzt()) 
  {
  Message("Kanzel besetzt.",this());
  Sound("Error");
  return(nil);
  } 
  Enter(kanzel2,pilot);
  Sound("Airlock1");
  return(1);
}


func Schwebe()
{
  counter=nil;
  SetComDir(COMD_Stop);
  SetAction("Bremse");
  return(1);
}


func Bremse()
{
  S42MigrationAssign(counter, "+=", 1);
  if((counter ?? 0)>15)   SetAction("Schwebe");
  if((GetXDir() ?? 0)>0)  SetXDir((GetXDir() ?? 0)-2);
  if((GetXDir() ?? 0)<0)  SetXDir((GetXDir() ?? 0)+2);
  if((GetYDir() ?? 0)>0)  SetYDir((GetYDir() ?? 0)-2);
  if((GetYDir() ?? 0)<0)  SetYDir((GetYDir() ?? 0)+2);
  Schaden();
  return(1);
}


func Statuskontrolle()
{
  Message("Schaden:%d%",this(),(GetDamage() ?? 0)/3);
  return(1);
}


func Hangar()
{
  if(hangar->Besetzt()) 
  {
  Message("Hangar besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(hangar,pilot);
  Sound("Airlock1");
  return(1);
}



func Arsenal()
{
  if(!(Equal(GetID(pilot), HZCK)))
  {
  Message("Zugang nur für Fachpersonal.",this());
  Sound("Error");
  return(nil);
  }
  if(NoAmmo())
  {
  Message("Heute geschlossen.",this());
  Sound("Error");
  return(nil);
  }
  Enter(arsenal,pilot);
  Sound("Airlock1");
  return(1);
}


func Bridge()
{
  if(bridge->Besetzt()) 
  {
  Message("Brücke besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(bridge,pilot);
  Sound("Airlock1");
  return(1);
}




public func IsVehicleProduct(...) { return(1);
}






