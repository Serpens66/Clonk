/*-- Concordia --*/

#strict 3
#include XFER   // Transfer-Script

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(rocky1),Find_Container(rocky2),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(kanone),Find_Container(arsenal),Find_Container(bridge))) )
{if(And(obj->~IsClonk(), Equal(GetController(), -1))) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {
  owner=GetOwner(obj);
  if(!Equal(GetOwner(obj), GetOwner(this())))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  SetOwner(owner,kanzel1);
  SetOwner(owner,kanzel2);
  SetOwner(owner,rocky1);
  SetOwner(owner,rocky2);
  SetOwner(owner,kanone);
  SetOwner(owner,arsenal);
  SetOwner(owner,bridge);
  SetOwner(owner,flag);
  }}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(rocky1),Find_Container(rocky2),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(kanone),Find_Container(arsenal),Find_Container(bridge)));
if(!obj) SetController(-1);
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}

  local kanzel1,flag, kanzel2, rocky1, rocky2, kanone, arsenal, pilot, counter, bridge, team,mlight;


//Initialisierung
func Initialize() {
  SetEntrance(1);
  SetAction("Schwebe");

  //Rocketeer Erstellen
  rocky1=CreateObject(CCR_,nil,nil,GetOwner());
  ObjectSetAction(rocky1,"Attaching",this());
  SetActionData(256 + 17 , rocky1);
  rocky1->Rotinit(-80,-100,-20);
  rocky1->Bezug(this());
  
  rocky2=CreateObject(CCR_,nil,nil,GetOwner());
  ObjectSetAction(rocky2,"Attaching",this());
  SetActionData(256 + 18 , rocky2);
  rocky2->Rotinit(80,20,100);
  rocky2->Bezug(this());
  
//Geschütze erstellen und anbringen
  kanzel1=CreateObject(CCG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 9 , kanzel1);
  kanzel1->Rotinit(80,20,100);
  kanzel1->Bezug(this(),rocky2);

  kanzel2=CreateObject(CCG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 12 , kanzel2);
  kanzel2->Rotinit(-80,-100,-20);
  kanzel2->Bezug(this(),rocky1);
  
//Bodengeschütz erstellen und integrieren
  kanone=CreateObject(CCF_,nil,nil,GetOwner());
  ObjectSetAction(kanone,"Attaching",this());
  SetActionData(256 + 4 , kanone);
  kanone->Bezug(this());
    
//Brücke erstellen und integrieren
  bridge=CreateObject(CBR_,nil,nil,GetOwner());
  ObjectSetAction(bridge,"Attaching",this());
  SetActionData(256 + 14 , bridge);
  bridge->Bezug(this());

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
  flag=CreateObject(FLAG,nil,nil,GetOwner());
  Enter(this(),flag);
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
  if((GetDamage() ?? 0)>510) Explosion();
  if((GetDamage() ?? 0)<500) return(nil);
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
  if((GetDamage() ?? 0)>=100)
  CreateParticle("Smoke2",45 , 15,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=250)
  CreateParticle("Smoke2",-65 ,15,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=400)
  {
  CreateParticle("Smoke2",-20 ,-20,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Thrust",-25+(Random(10) ?? 0) ,-25+(Random(10) ?? 0),(GetXDir() ?? 0)/2,+2,RandomX(70,90),RGBa(150,50,50,60),nil,nil);
  }
  if((GetDamage() ?? 0)>=450)
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
  kanone->Explosion();
  bridge->Explosion();
  rocky1->Explosion();
  rocky2->Explosion();
  if(kanzel1) RemoveObject(kanzel1);
  if(kanzel2) RemoveObject(kanzel2);
  if(kanone) RemoveObject(kanone);
  if(bridge) RemoveObject(bridge);
  if(arsenal) RemoveObject(arsenal);
  if(rocky1) RemoveObject(rocky1);
  if(rocky2) RemoveObject(rocky2);
  return(1);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
  SetXDir(-30,CreateObject(_TB1,-20,20));
  SetXDir(30,CreateObject(_TB2,20,20));
  CreateObject(_TB3,nil,20);
   //Flaggschiffregel aktiv?
  if(ObjectCount(GFSD))  GameCall("Flagship",team);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(100); else FakeExplode(100,nil,nil,100);
  return(1);
}

func ActivateEntrance() {
  Sound("SteelGate2");
  return(1);
}


func ContainedDig(pByObj)
{
  pilot=pByObj;
  CreateMenu(CCD_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",M17G,Par(nil));
  AddMenuItem("%s","Bridge",T20G,Par(nil));  //Brücke
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",M17G,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s","Statuskontrolle",M57G,Par(nil)); //Anzeige Energie
  AddMenuItem("%s","Ausstieg",M47G,Par(nil));    //Ausstieg
  AddMenuItem("%s","Arsenal",T15G,Par(nil));  //Arsenal
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(kanone->Besetzt())    AddMenuItem("%s","Kanone",M17H,Par(nil));  //kanone
  if(!(kanone->Besetzt())) AddMenuItem("%s","Kanone",M17G,Par(nil));
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
  Exit(pilot,nil,30);
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
  Message("Schaden:%d%",this(),(GetDamage() ?? 0)/5);
  return(1);
}


func Kanone()
{
  if(kanone->Besetzt()) 
  {
  Message("Bordkanone besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(kanone,pilot);
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
  Enter(bridge,pilot);
  Sound("Airlock1");
  return(1);
}


public func IsVehicleProduct(...) { return(1);
}



