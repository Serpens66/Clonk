/*-- Tucan --*/

#strict
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
if(obj->~IsClonk() && GetController()==-1) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {owner=GetOwner(obj);
  if(GetOwner(obj)!=GetOwner(this()))
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
  kanzel1=CreateObject(CGG_,0,0,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 5 , kanzel1);
  kanzel1->Rotinit(120,70,175);
  kanzel1->Bezug(this());

  kanzel2=CreateObject(CGG_,0,0,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 4 , kanzel2);
  kanzel2->Rotinit(-120,-175,-70);
  kanzel2->Bezug(this());

//Brücke erstellen und integrieren
  bridge=CreateObject(TBR_,0,0,GetOwner());
  ObjectSetAction(bridge,"Attaching",this());
  SetActionData(256 + 14 , bridge);
  bridge->Bezug(this());

//Hangar erstellen und integrieren
  hangar=CreateObject(TCH_,0,0,GetOwner());
  ObjectSetAction(hangar,"Attaching",this());
  SetActionData(256 + 14 , hangar);
  hangar->Bezug(this());

//Arsenal erstellen und integrieren
  arsenal=CreateObject(TAS_,0,0,GetOwner());
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
SetPosition(GetX(),GetY()-20,this());
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
  return(0);
}


//Schaden

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Projectile)
    return(90);
  if(iType & DMG_Fire)
    return(80);
  if(iType & DMG_Explosion)
    return(80);
  if(iType & DMG_Energy)
    return(40);
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(GetDamage()>310) Explosion();
  if(GetDamage()<300) return(0);
  counter=0;
  SetPhysical("Float",200,2);
  SetAction("Down");
  SetComDir(COMD_Down());
  ObjectCall(Local(1),"ComDown",this());
  return(1);
}

func Down()
{
  counter++;
  if(counter>300) Explosion();
  Schaden();
}

func Schaden()
{
  //Schaden sichtbar machen
  if(GetDamage()>=60)
  CreateParticle("Smoke2",-10 , 0,GetXDir()/2,-20,15+GetDamage()/6,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=150)
  CreateParticle("Smoke2",65 ,10,GetXDir()/2,-20,15+GetDamage()/6,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=240)
  {
  CreateParticle("Smoke2",-40 ,0,GetXDir()/2,-20,15+GetDamage()/6,RGBa(220,200,180,0),0,0); 
  CreateParticle("Thrust",-45+Random(10) ,Random(10),GetXDir()/2,+2,RandomX(70,90),RGBa(150,50,50,60),0,0);
  }
  if(GetDamage()>=270)
  {
  CreateParticle("Smoke2",-3 ,15,GetXDir()/2,-20,50+GetDamage()/6,RGBa(220,200,180,0),0,0); 
  if(Random(250-GetDamage()*2)==1) DoDamage(3);
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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  SetXDir(-30,CreateObject(_TB1,-20,20));
  SetXDir(30,CreateObject(_TB2,20,20));
  CreateObject(_TB3,0,20);
  if(ObjectCount(GFSD))  GameCall("Flagship",team);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(50); else FakeExplode(50,0,0,50);
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
  CreateMenu(TCN_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(bridge->Besetzt())    AddMenuItem("%s","Bridge",T20H,Par(0));  //Brücke
  if(!(bridge->Besetzt())) AddMenuItem("%s","Bridge",T20G,Par(0)); 
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",M17H,Par(0));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",M17G,Par(0)); 
  AddMenuItem("%s","Statuskontrolle",M57G,Par(0)); //Anzeige Energie
  AddMenuItem("%s","Ausstieg",M47G,Par(0));    //Ausstieg
  AddMenuItem("%s","Arsenal",T15G,Par(0));  //Arsenal
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",M17H,Par(0));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",M17G,Par(0)); 
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(hangar->Besetzt())    AddMenuItem("%s","Hangar",T11W,Par(0));  //Hangar
  if(!(hangar->Besetzt())) AddMenuItem("%s","Hangar",T11Q,Par(0)); 
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(0));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(0));  //Licht aus
  SelectMenuItem(7,Par(0)); 
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
  Exit(pilot,0,-15);
  return(1);
}


func Geschuetzkanzel1()
{
  if(kanzel1->Besetzt()) 
  {
  Message("Kanzel besetzt.",this());
  Sound("Error");
  return(0);
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
  return(0);
  } 
  Enter(kanzel2,pilot);
  Sound("Airlock1");
  return(1);
}


func Schwebe()
{
  counter=0;
  SetComDir(COMD_Stop());
  SetAction("Bremse");
  return(1);
}


func Bremse()
{
  counter+=1;
  if(counter>15)   SetAction("Schwebe");
  if(GetXDir()>0)  SetXDir(GetXDir()-2);  
  if(GetXDir()<0)  SetXDir(GetXDir()+2);
  if(GetYDir()>0)  SetYDir(GetYDir()-2);  
  if(GetYDir()<0)  SetYDir(GetYDir()+2);
  Schaden();
  return(1);
}


func Statuskontrolle()
{
  Message("Schaden:%d%",this(),GetDamage()/3);
  return(1);
}


func Hangar()
{
  if(hangar->Besetzt()) 
  {
  Message("Hangar besetzt.",this());
  Sound("Error");
  return(0);
  }
  Enter(hangar,pilot);
  Sound("Airlock1");
  return(1);
}



func Arsenal()
{
  if(!(GetID(pilot)==HZCK))
  {
  Message("Zugang nur für Fachpersonal.",this());
  Sound("Error");
  return(0);
  }
  if(NoAmmo())
  {
  Message("Heute geschlossen.",this());
  Sound("Error");
  return(0);
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
  return(0);
  }
  Enter(bridge,pilot);
  Sound("Airlock1");
  return(1);
}




public IsVehicleProduct: return(1);






