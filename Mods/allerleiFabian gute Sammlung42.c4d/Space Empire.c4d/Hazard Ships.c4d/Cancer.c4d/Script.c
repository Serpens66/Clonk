/*-- Cancer --*/

#strict 3
#include XFER   // Transfer-Script

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

local kanzel1, kanzel2, generator, teleporter, pilot, counter, bridge, team, scorpion1, scorpion2, heat1, heat2, pLight, sphere, nanites,mlight;

func MaxDamage() {return(510);}


//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(heat1),Find_Container(heat2),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(generator),Find_Container(teleporter),Find_Container(bridge))) )
if(And(obj->~IsClonk(), Equal(GetController(), -1))) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {owner=GetOwner(obj);
  if(!Equal(GetOwner(obj), GetOwner(this())))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  if(scorpion1) SetOwner(owner,scorpion1);
  if(scorpion2) SetOwner(owner,scorpion2);
  SetOwner(owner,heat1);
  SetOwner(owner,heat2);
  SetOwner(owner,kanzel1);
  SetOwner(owner,kanzel2);
  SetOwner(owner,generator);
  SetOwner(owner,teleporter);
  SetOwner(owner,bridge);}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(heat1),Find_Container(heat2),Find_Container(kanzel1),Find_Container(kanzel2),Find_Container(generator),Find_Container(teleporter),Find_Container(bridge))) ;
if(!obj) {SetController(-1);Empty();} // besitzer -1 und teleporter aktivieren
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}


//Initialisierung
func Initialize() {

  counter=nil;
  nanites=25;
  SetAction("Schwebe");
  
//Wärmequelle erstellen
  heat1=CreateObject(PRQ_,nil,nil,GetOwner());
  ObjectSetAction(heat1,"Attaching",this());
  SetActionData(256 + 7 , heat1);
  
  heat2=CreateObject(PRQ_,nil,nil,GetOwner());
  ObjectSetAction(heat2,"Attaching",this());
  SetActionData(256 + 1 , heat2);
  
//Brücke erstellen und integrieren
  bridge=CreateObject(CAB_,nil,nil,GetOwner());
  ObjectSetAction(bridge,"Attaching",this());
  bridge->Bezug(this());  
  
//Geschütze erstellen und anbringen
  kanzel1=CreateObject(CAG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel1,"Attaching",this());
  SetActionData(256 + 14 , kanzel1);
  kanzel1->Rotinit(40,10,80);
  kanzel1->Bezug(this());

  kanzel2=CreateObject(CAG_,nil,nil,GetOwner());
  ObjectSetAction(kanzel2,"Attaching",this());
  SetActionData(256 + 13 , kanzel2);
  kanzel2->Rotinit(-40,-80,-10);
  kanzel2->Bezug(this());


//Generator erstellen und integrieren
  generator=CreateObject(CGE_,nil,nil,GetOwner());
  ObjectSetAction(generator,"Attaching",this());
  generator->Bezug(this());
  
//Teleporter erstellen und integrieren
  teleporter=CreateObject(CTE_,nil,nil,GetOwner());
  ObjectSetAction(teleporter,"Attaching",this());
  teleporter->Bezug(this());
  ScheduleCall(nil,"Empty",10);
  //teleporter->BeamUp();  funktioniert nicht, beamt nicht korrekt rein und Clonk ist gefangen. betreten über baubaren teleporter, danach funktioniert auch interner teleporter

//Scorpions Anhängen
//  scorpion1=CreateObject(SCR_,0,0,GetOwner());
//  ObjectSetAction(scorpion1,"Attaching",this());
//  SetActionData(256 + 15 , scorpion1);
//  SetR(-90,scorpion1);
//  scorpion1->Bezug(this());
  
//  scorpion2=CreateObject(SCR_,0,0,GetOwner());
//  ObjectSetAction(scorpion2,"Attaching",this());
//  SetActionData(256 + 16 , scorpion2);
//  SetR(90,scorpion2);
//  scorpion2->Bezug(this());
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

public func Empty()
{
  if(!Contained()) teleporter->BeamUp(); // wenn cancer nicht in gebäude und leer, dann teleporter aktivieren
  return(1);
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
  if((GetDamage() ?? 0)>(MaxDamage() ?? 0)) Explosion();
  if((GetDamage() ?? 0)<(MaxDamage() ?? 0)-10) return(nil);
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
  if((counter ?? 0)>150) Explosion();
  Schaden();
}

func Schaden()
{
  if(And(And(nanites, GetDamage()), !Random(100-(nanites ?? 0)/2)))
  {
  DoDamage(-5,this());
  S42MigrationStep(nanites, -1, true);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  Sparks(10,RGB(nil,nil,255),RandomX(-30,30),RandomX(-15,15));
  }
  //Scorpions Reparieren
  if(And(And(And(nanites, !Scorpcheck1()), GetDamage(scorpion1)), !Random(100-(nanites ?? 0)/2)))
  {
  DoDamage(-5,scorpion1);
  S42MigrationStep(nanites, -1, true);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  Sparks(10,RGB(nil,nil,255),-40,15);
  }

  if(And(And(And(nanites, !Scorpcheck2()), GetDamage(scorpion2)), !Random(100-(nanites ?? 0)/2)))
  {
  DoDamage(-5,scorpion2);
  S42MigrationStep(nanites, -1, true);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  Sparks(10,RGB(nil,nil,255),40,15);
  }
  
  //Schaden sichtbar machen
  if((GetDamage() ?? 0)>=60)
  CreateParticle("Smoke2",-10 , nil,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=150)
  CreateParticle("Smoke2",65 ,10,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=240)
  {
  CreateParticle("Smoke2",10 ,nil,(GetXDir() ?? 0)/2,-20,15+(GetDamage() ?? 0)/6,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Thrust",Random(10) ,Random(10),(GetXDir() ?? 0)/2,+2,RandomX(70,90),RGBa(10,150,150,60),nil,nil);
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
  generator->Explosion(sphere);
  teleporter->Explosion();
  bridge->Explosion(sphere);
  if(!Scorpcheck1()) scorpion1->Explosion();
  if(!Scorpcheck2()) scorpion2->Explosion();
  if(heat1) RemoveObject(heat1);
  if(heat2) RemoveObject(heat2);
  if(kanzel1) RemoveObject(kanzel1);
  if(kanzel2) RemoveObject(kanzel2);
  if(generator) RemoveObject(generator);
  if(teleporter) RemoveObject(teleporter);
  if(bridge) RemoveObject(bridge);
  return(1);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  if(!sphere)sphere=CreateObject(TSP_,nil,nil,GetOwner());
  Removal();
  SetXDir(-30,CreateObject(CB1_,-20,20));
  SetXDir(30,CreateObject(CB2_,20,20));
  if(ObjectCount(GFSD))  GameCall("Flagship",team);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(80); else FakeExplode(80,nil,nil,80);
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
  if((counter ?? 0)>15)
  {
  SetAction("Schwebe");
  counter=nil;
  }
  if((GetXDir() ?? 0)>0)  SetXDir((GetXDir() ?? 0)-2);
  if((GetXDir() ?? 0)<0)  SetXDir((GetXDir() ?? 0)+2);
  if((GetYDir() ?? 0)>0)  SetYDir((GetYDir() ?? 0)-2);
  if((GetYDir() ?? 0)<0)  SetYDir((GetYDir() ?? 0)+2);
  Schaden();
  return(1);
}

func ContainedDig(pByObj)
{
  pilot=pByObj;
  CreateMenu(CAN_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(kanzel2->Besetzt())    AddMenuItem("%s","Geschuetzkanzel2",CM_8,Par(nil));  //Geschützkanzel
  if(!(kanzel2->Besetzt())) AddMenuItem("%s","Geschuetzkanzel2",CM_7,Par(nil));
  if(bridge->Besetzt())    AddMenuItem("%s","Bridge",CM_2,Par(nil));  //Brücke
  if(!(bridge->Besetzt())) AddMenuItem("%s","Bridge",CM_1,Par(nil));
  if(kanzel1->Besetzt())    AddMenuItem("%s","Geschuetzkanzel1",CM_8,Par(nil));  //Geschützkanzel
  if(!(kanzel1->Besetzt())) AddMenuItem("%s","Geschuetzkanzel1",CM_7,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(scorpion1) if(!(scorpion1->Besetzt())) AddMenuItem("%s","Scorpion1",CM_9,Par(nil));   //Scorpion
  if(!scorpion1)  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(scorpion1)  if(scorpion1->Besetzt())    AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(generator->Besetzt())    AddMenuItem("%s","Generator",CM_4,Par(nil));  //Generator
  if(!(generator->Besetzt())) AddMenuItem("%s","Generator",CM_3,Par(nil));
  if(scorpion2) if(!(scorpion2->Besetzt())) AddMenuItem("%s","Scorpion2",CM_9,Par(nil));   //Scorpion
  if(!scorpion2)  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(scorpion2)  if(scorpion2->Besetzt())    AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  if(teleporter->Besetzt())    AddMenuItem("%s","Teleporter",CM_6,Par(nil));  //Teleporter
  if(!(teleporter->Besetzt())) AddMenuItem("%s","Teleporter",CM_5,Par(nil));
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

func Scorpcheck1()
{
  if(scorpion1) if(SEqual(GetAction(scorpion1),"Attaching")) return(nil);
  return(1);
}

func Scorpdock1(_scorp)
{
  scorpion1=_scorp;
  ObjectSetAction(scorpion1,"Attaching",this());
  SetActionData(256 + 15 , scorpion1);
  scorpion1->Bezug(this());
  SetR(-90,scorpion1);
  return(1);
}

func Scorpcheck2()
{
  if(scorpion2) if(SEqual(GetAction(scorpion2),"Attaching")) return(nil);
  return(1);
}

func Scorpdock2(_scorp)
{
  scorpion2=_scorp;
  ObjectSetAction(scorpion2,"Attaching",this());
  SetActionData(256 + 16 , scorpion2);
  scorpion2->Bezug(this());
  SetR(90,scorpion2);
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


func Generator()
{
  if(generator->Besetzt()) 
  {
  Message("Generatortraum besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(generator,pilot);
  Sound("Airlock1");
  return(1);
}


func Teleporter()
{
  if(teleporter->Besetzt()) 
  {
  Message("Teleporterraum besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(teleporter,pilot);
  Sound("Airlock1");
  return(1);
}


func Bridge(donot)
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


func Scorpion1(donot)
{
  if(scorpion1->Besetzt()) 
  {
  Message("Scorpion besetzt.",this());
  Sound("Error");
  return(nil);
  }

  Enter(scorpion1,pilot);
  Sound("Airlock1");
  
  return(1);
}

func Scorpion2(donot)
{
  if(scorpion2->Besetzt()) 
  {
  Message("Scorpion besetzt.",this());
  Sound("Error");
  return(nil);
  }

  Enter(scorpion2,pilot);
  Sound("Airlock1");
  
}

func BrainCheck()
{
  if(scorpion1) if(!(scorpion1->Besetzt())) return(scorpion1);
  if(scorpion2) if(!(scorpion2->Besetzt())) return(scorpion2);  
  return(nil);
}

func BrainScorpion(scorpion_)
{
  CreateContents(BRA_,scorpion_);
  return(1);
}


func TelFXUp()
{
  AddSpawnEffect(heat1,RGBa(220,80,180,100));
  return(1);
}

func TelFXDown()
{
  AddSpawnEffect(heat2,RGBa(220,80,180,100));
  return(1);
}

func TelLight(on)
{
  if(Equal(on, 1))
  {
  pLight=AddLightCone (500, RGB(120,nil,255), heat2);
  pLight->ChangeSizeXY(2500,5000);
  pLight->ChangeR(180);
  pLight->ChangeOffset(1,-12);
  return(1);
  }
  RemoveObject(pLight);
  return(1);
}


func Blaster()
{
  if(!counter)
  {
  if(pLight) RemoveObject(pLight);
  AddSpawnEffect(heat2,RGBa(220,80,180,100));
  pLight=AddLightCone (500, RGB(120,nil,255), heat2);
  pLight->ChangeSizeXY(1000,5000);
  pLight->ChangeR(180);
  pLight->ChangeOffset(1,-12); 
  }
  
  S42MigrationStep(counter, 1, true);
  
  var user = Contained();
  var angle = 180+(RandomX(-8,+8) ?? 0);
  var x,y;
  x=nil;
  y=25;
  var ammo = CreateObject(SHT2,x,y,GetOwner());
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600-(Random(50) ?? 0),8,400, 20,nil,60);

  // Effekte
  MuzzleFlash(30,nil,x,y,angle,RGBa(96,64,255,nil));

  if(Equal(counter, 70))
  {  
  var ammo = CreateObject(CBB_,x,(y ?? 0)+60,GetOwner());
  ammo->Down();
  }
  
  if(Equal(counter, 100))
  {
  SetAction("Schwebe");
  counter=nil;
  RemoveObject(pLight);
  }
  return(1);
}


func Naniten(do)
{
  if(do) S42MigrationAssign(nanites, "+=", 20);
  return(nanites);
}






public func IsVehicleProduct(...) { return(1);
}










