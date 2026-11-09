/*-- Technosphäre --*/

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

  local leitstand,heat,pilot,counter,energie,tarn,boje,schild,mlight;  

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(heat),Find_Container(leitstand),Find_Container(schild))) )
if(obj->~IsClonk() && GetController()==-1) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {owner=GetOwner(obj);
  if(GetOwner(obj)!=GetOwner(this()))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  SetOwner(owner,heat);
  SetOwner(owner,leitstand);
  SetOwner(owner,schild);}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(heat),Find_Container(leitstand),Find_Container(schild)));
if(!obj) SetController(-1);
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}

func Initialize() {

  SetEntrance(1);

  heat=CreateObject(PRQ_,0,0,GetOwner());
  ObjectSetAction(heat,"Attaching",this());

  leitstand=CreateObject(TSG_,0,0,GetOwner());
  ObjectSetAction(leitstand,"Attaching",this());

  schild=CreateObject(TSS_,0,0,GetOwner());
  ObjectSetAction(schild,"Attaching",this());

  energie=10000;

  SetAction("Fliegen");
  return(1);
}


func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
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
    return(90);
  if(iType & DMG_Explosion)
    return(-10);
  if(iType == DMG_Energy) {
    DoEnergy(iDmg, this(), true);
    return(-100);
    }
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(GetDamage()<100) return(0);
  counter=0;
  SetEntrance(0);
  SetAction("Down");
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
  Sparks(GetDamage()/20,RGB(0,50,255),RandomX(-20,20),RandomX(-15,15));
  if(GetDamage()>=80)
  {
  CreateParticle("Thrust",-5+Random(10) ,-5+Random(10),GetXDir()/2,+2,RandomX(70,90),RGBa(0,150,200,60),0,0);
  CreateParticle("Smoke2",-10+Random(30) ,-10+Random(30),GetXDir()/2,+5,RandomX(80,120),RGBa(220,200,180,0),0,0);
  }
  if(GetDamage()>=90)
  if(Random(250-GetDamage()*2)==1) DoDamage(1);
}

func Hit()
{
  if(SEqual(GetAction(),"Down"))  Explosion();
  return(1);
}

func Removal()
{
  if(leitstand) RemoveObject(leitstand);
  if(heat) RemoveObject(heat);
  if(schild) RemoveObject(schild);
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  Removal();
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(40); else FakeExplode(40,0,0,40);
  return(1);
}

//Steuerung

func ContainedUp()
{
  return(SetComDir(COMD_Up()));
}

func ContainedDown()
{
  return(SetComDir(COMD_Down()));
}

func ContainedRight()
{
  return(SetComDir(COMD_Right()));
}

func ContainedLeft()
{
  return(SetComDir(COMD_Left()));
}

//Menüsteuerung
func ContainedDig()
{
  CreateMenu(A56B,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(leitstand->Besetzt())    AddMenuItem("%s","Geschuetzkanzel",M17H,Par(0));  //Geschützkanzel
  if(!(leitstand->Besetzt())) AddMenuItem("%s","Geschuetzkanzel",M17G,Par(0)); 
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  AddMenuItem("%s","System",M37G,Par(0)); //Systemfähigkeiten
  AddMenuItem("%s","Schwebe",M27G,Par(0));   //Schwebe
  AddMenuItem("%s","Statuskontrolle",M57G,Par(0)); //Anzeige Energie
  AddMenuItem("%s",0,M67G,Par(0));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(0));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(0));  //Licht aus
  AddMenuItem("%s","Ausstieg",M47G,Par(0));    //Ausstieg
  SelectMenuItem(7,Par(0)); 
  return(1);
}


func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,210,200,40);
  mlight = AddLight(2000, iColor, this());
  return(1);
}

func TurnLightOff()
{
  if (mlight) {RemoveObject(mlight);}
  return(1);
}

//Bewegungszustände

func Fliegen()
{
  //Schaden sichtbar machen
  Schaden();
  Getarnt();

  //Eintritt nur für eine Person
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  }
  return(1);
}

func Gelandet()
{
  //Schwebe
  if(GetDamage()) if(!Random(100)) Reparatur();  
  if (counter>3600) counter=0;
  counter+=50;
  if(GetXDir()>0)  SetXDir(GetXDir()-2);  
  if(GetXDir()<0)  SetXDir(GetXDir()+2);
  SetYDir(Sin(counter,18,10),0,10);
  Schaden();
  Getarnt();
  if(energie<10000) energie++;
  //Eintritt nur für eine Person
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  }
  return(1);
}

func Wasser()
{
  //Schaden sichtbar machen
  if(!Random(4)) Bubble();
  Schaden();
  Getarnt();
  //Eintritt nur für eine Person
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  }
  return(1);
}

//Sonderfunktionen
public func Reparatur()
{
  DoDamage(-3);
  CastParticles("PxSpark",Random(20)-10,Random(20)-10,0,10,30,40, RGB(187, 214, 224), RGB(200, 224, 250));
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  return(1);
}


func Geschuetzkanzel()
{
  if(leitstand->Besetzt()) 
  {
  Message("Kanzel besetzt.",this());
  Sound("Error");
  return(0);
  }
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(leitstand,pilot);
  SetEntrance(1);
  Sound("Airlock1");
  return(1);
}

func Schwebe()
{
  Sound("PRModus");
  if (SEqual(GetAction(),"Gelandet")) return(SetAction("Fliegen"));
  if (SEqual(GetAction(),"Fliegen"))  return(SetAction("Gelandet"));
  {
  SetComDir(COMD_Stop());
  return(SetAction("Gelandet"));
  }
  return(1);
}


func Ausstieg()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Sound("Airlock1");
  Exit(pilot,0,20);
  pilot=0;
  SetEntrance(1);
  return(1);
}


func Statuskontrolle()
{

  Message("S:%d% E:%d%",this(),GetDamage(),energie/100);

  return(1);
}


func System()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  CreateMenu(A56B ,pilot,0,0,0,0);
  AddMenuItem("%s","Tarnung",M77G,pilot);   //Tarnung
  AddMenuItem("%s","Warp",M97G,pilot);      //Warpen
  AddMenuItem("%s","Warpboje",M87G,pilot);  //Tarnung
}


func Tarnung()
{
  if(tarn) 
  {
  Enttarnen();
  return(1);
  }
  if(energie<2000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(0);
  }
  RemoveObject(schild);
  SetVisibility(VIS_Allies | VIS_Owner);
  SetClrModulation(RGBa(180,180,255,100));
  SetVisibility(VIS_Allies | VIS_Owner,leitstand);
  SetClrModulation(RGBa(180,180,255,100),leitstand);
  energie-=2000;
  Nofx();
  Sound("Cloak1",this());
  tarn=1;
}


func Getarnt()
{
  if(tarn) 
  {
  energie-=5;
  if(energie<0) Enttarnen();
  }
  return(1);
}


func Enttarnen()
{
  tarn=0;
  schild=CreateObject(TSS_,0,0,GetOwner());
  ObjectSetAction(schild,"Attaching",this());
  SetVisibility(VIS_All);
  SetClrModulation(RGBa(255,255,255,0));
  SetVisibility(VIS_All,leitstand);
  SetClrModulation(RGBa(255,255,255,0),leitstand);
  Nofx();
  Sound("Cloak2",this());
  return(1);
}

func Warp()
{
  if(!boje)
  {
  Sound("Error");
  Message("Keine Warpboje gesetzt.", this());
  return(0);
  }
  if(energie<5000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(0);
  }
  Sound("DeEnergize");
  energie-=5000;
  Nofx();
  SetPosition(GetX(boje),GetY(boje));
  SetXDir(0);
  SetYDir(0);
  Nofx();
  return(1);
}



func Warpboje()
{
  if(boje) boje->Hit();
  if(energie<2000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(0);
  }
  energie-=2000;
  boje=CreateObject(TSB_);
  Sound("DeEnergize");
  return(1);
}


func Nofx()
{
  AddSpawnEffect(this(),RGBa(220,80,180,100));
  return(1);
}







public IsVehicleProduct: return(1);















