/*-- Cougar --*/

#strict 3

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

  local kanzel1, kanzel2, leitstand, pilot, counter, heat, team, ldm,mlight;

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(leitstand),Find_Container(heat))) )
if(And(obj->~IsClonk(), Equal(GetController(), -1))) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
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
  kanzel1=CreateObject(CGG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 12 , kanzel1);
  kanzel1->Rotinit(60,10,170);
  kanzel1->Bezug(this());

  kanzel2=CreateObject(CGG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 13 , kanzel2);
  kanzel2->Rotinit(-60,-170,-10);
  kanzel2->Bezug(this());

  leitstand=CreateObject(MLC_,nil,nil,GetOwner());
  ObjectSetAction(leitstand,"Eingefahren",this());
  leitstand->Bezug(this());

  heat=CreateObject(PRQ_,nil,nil,GetOwner());
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
    return(80);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(70);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(70);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
  ldm=iType;
}

func Damage()
{
  if((GetDamage() ?? 0)>110) Explosion();
  if((GetDamage() ?? 0)<100) return(nil);
  counter=nil;
  SetPhysical("Float",300,2);
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
  if((GetDamage() ?? 0)>=20)
  CreateParticle("Smoke2",nil , 15,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/2,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=50)
  CreateParticle("Smoke2",25 ,10,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/2,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=80)
  {
  CreateParticle("Smoke2",-35 ,7,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/2,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Thrust",-5+(Random(10) ?? 0) ,-5+(Random(10) ?? 0),(GetXDir() ?? 0)/2,+2,RandomX(70,90),RGBa(150,50,50,60),nil,nil);
  }
  if((GetDamage() ?? 0)>=90)
  {
  CreateParticle("Smoke2",-3 ,15,(GetXDir() ?? 0)/2,-20,50+(GetDamage() ?? 0)/2,RGBa(220,200,180,nil),nil,nil);
  if(Equal(Random(250-(GetDamage() ?? 0)*2), 1)) DoDamage(1);
  }
  //Zugangssteuerung
  var clonk;
  if(clonk=FindObject(HZCK,-30,-30,60,60,nil,nil,nil,NoContainer()))
  if(Hostile(GetOwner(clonk),GetOwner(this()))) return(SetEntrance(nil));
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  if(Or(ObjectCount(GFSD), ObjectCount(GTDM)))  GameCall("Flagship",team,ldm);
  Exit(pilot);
  Removal();
  SetXDir(-30,CreateObject(_CB1,-20,20));
  SetXDir(30,CreateObject(_CB2,20,20));
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40); else FakeExplode(40,nil,nil,40);
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
  SetComDir(COMD_Left);
  ObjectCall(Local(1),"ComLeft",this());
  return(1);
}

func ContainedRight() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Right);
  ObjectCall(Local(1),"ComRight",this());
  return(1);
}

func ContainedUp() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Up);
  ObjectCall(Local(1),"ComUp",this());
  return(1);
}

func ContainedDown() {
  if(SEqual(GetAction(),"Down")) return(1);
  SetComDir(COMD_Down);
  ObjectCall(Local(1),"ComDown",this());
  return(1);
}

func ContainedThrow(){
  return(1);
}

func ContainedDig(pByObj)
{
  pilot=pByObj;
  CreateMenu(CGC_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",M17G,Par(nil));
  AddMenuItem("%s","Statuskontrolle",M57G,Par(nil)); //Anzeige Energie
  AddMenuItem("%s","Ausstieg",M47G,Par(nil));    //Ausstieg
  AddMenuItem("%s","Schwebe",M27G,Par(nil));   //Schwebe
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",M17H,Par(nil));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",M17G,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(leitstand->Besetzt())    AddMenuItem("%s","Scavengerleitstand",M41H,Par(nil));  //Leitstand
  if(!(leitstand->Besetzt())) AddMenuItem("%s","Scavengerleitstand",M41G,Par(nil));
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
  Exit(pilot,nil,10);
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
  Message("Schaden:%d%",this(),GetDamage());
  return(1);
}


func Scavengerleitstand()
{
  if(leitstand->Besetzt()) 
  {
  Message("leitstand besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(leitstand,pilot);
  ObjectSetAction(leitstand,"Ausfahren",this());
  Sound("Airlock1");
  return(1);
}








public func IsVehicleProduct(...) { return(1);
}







