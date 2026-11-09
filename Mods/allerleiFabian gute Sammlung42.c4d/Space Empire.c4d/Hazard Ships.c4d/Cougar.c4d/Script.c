/*-- Cougar --*/

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

  local kanzel1, kanzel2, leitstand, pilot, counter, heat, team, ldm,mlight;

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(leitstand),Find_Container(heat))) )
if(obj->~IsClonk() && GetController()==-1) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {
  owner=GetOwner(obj);
  if(GetOwner(obj)!=GetOwner(this()))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  SetOwner(owner,kanzel1);
  SetOwner(owner,kanzel2);
  SetOwner(owner,leitstand);
  SetOwner(owner,heat);}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(leitstand),Find_Container(heat))) ;
if(!obj) SetController(-1);
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}

//Initialisierung
func Initialize() {
  SetEntrance(1);
//Geschütze erstellen und anbringen
  kanzel1=CreateObject(CGG_,0,0,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 12 , kanzel1);
  kanzel1->Rotinit(60,10,170);
  kanzel1->Bezug(this());

  kanzel2=CreateObject(CGG_,0,0,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 13 , kanzel2);
  kanzel2->Rotinit(-60,-170,-10);
  kanzel2->Bezug(this());

  leitstand=CreateObject(MLC_,0,0,GetOwner());
  ObjectSetAction(leitstand,"Eingefahren",this());
  leitstand->Bezug(this());

  heat=CreateObject(PRQ_,0,0,GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Waiting");
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
    return(80);
  if(iType & DMG_Fire)
    return(70);
  if(iType & DMG_Explosion)
    return(70);
  if(iType & DMG_Energy)
    return(40);
  if(iType & DMG_Bio)
    return(50);
  ldm=iType;
}

func Damage()
{
  if(GetDamage()>110) Explosion();
  if(GetDamage()<100) return(0);
  counter=0;
  SetPhysical("Float",300,2);
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
  if(GetDamage()>=20)
  CreateParticle("Smoke2",0 , 15,GetXDir()/2,-20,15+GetDamage()/2,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=50)
  CreateParticle("Smoke2",25 ,10,GetXDir()/2,-20,15+GetDamage()/2,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=80)
  {
  CreateParticle("Smoke2",-35 ,7,GetXDir()/2,-20,15+GetDamage()/2,RGBa(220,200,180,0),0,0); 
  CreateParticle("Thrust",-5+Random(10) ,-5+Random(10),GetXDir()/2,+2,RandomX(70,90),RGBa(150,50,50,60),0,0);
  }
  if(GetDamage()>=90)
  {
  CreateParticle("Smoke2",-3 ,15,GetXDir()/2,-20,50+GetDamage()/2,RGBa(220,200,180,0),0,0); 
  if(Random(250-GetDamage()*2)==1) DoDamage(1);
  }
  //Zugangssteuerung
  var clonk;
  if(clonk=FindObject(HZCK,-30,-30,60,60,0,0,0,NoContainer()))
  if(Hostile(GetOwner(clonk),GetOwner(this()))) return(SetEntrance(0));
  SetEntrance(1);
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
  leitstand->Explosion();
  if(kanzel1) RemoveObject(kanzel1);
  if(kanzel2) RemoveObject(kanzel2);
  if(leitstand) RemoveObject(leitstand);
  if(heat) RemoveObject(heat);
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  if(ObjectCount(GFSD) || ObjectCount(GTDM))  GameCall("Flagship",team,ldm);
  Exit(pilot);
  Removal();
  SetXDir(-30,CreateObject(_CB1,-20,20));
  SetXDir(30,CreateObject(_CB2,20,20));
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40); else FakeExplode(40,0,0,40);
  return(1);
}

func ActivateEntrance()
 {
  Sound("SteelGate2");
  return(1);
}

//Steuerung
func ContainedLeft() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Left());
  ObjectCall(Local(1),"ComLeft",this());
  return(1);
}

func ContainedRight() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Right());
  ObjectCall(Local(1),"ComRight",this());
  return(1);
}

func ContainedUp() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Up());
  ObjectCall(Local(1),"ComUp",this());
  return(1);
}

func ContainedDown() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Down());
  ObjectCall(Local(1),"ComDown",this());
  return(1);
}

func ContainedThrow(){
  return(1);
}

func ContainedDig(pByObj)
{
  pilot=pByObj;
  CreateMenu(CGC_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",M17H,Par(0));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",M17G,Par(0)); 
  AddMenuItem("%s","Statuskontrolle",M57G,Par(0)); //Anzeige Energie
  AddMenuItem("%s","Ausstieg",M47G,Par(0));    //Ausstieg
  AddMenuItem("%s","Schwebe",M27G,Par(0));   //Schwebe
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",M17H,Par(0));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",M17G,Par(0)); 
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(leitstand->Besetzt())    AddMenuItem("%s","Scavengerleitstand",M41H,Par(0));  //Leitstand
  if(!(leitstand->Besetzt())) AddMenuItem("%s","Scavengerleitstand",M41G,Par(0)); 
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
  Exit(pilot,0,10);
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
  Message("Schaden:%d%",this(),GetDamage());
  return(1);
}


func Scavengerleitstand()
{
  if(leitstand->Besetzt()) 
  {
  Message("leitstand besetzt.",this());
  Sound("Error");
  return(0);
  }
  Enter(leitstand,pilot);
  ObjectSetAction(leitstand,"Ausfahren",this());
  Sound("Airlock1");
  return(1);
}








public IsVehicleProduct: return(1);







