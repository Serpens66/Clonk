/*-- Technosphäre --*/

#strict 3

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

  local leitstand,heat,pilot,counter,energie,tarn,boje,schild,mlight;  

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(heat),Find_Container(leitstand),Find_Container(schild))) )
if(And(obj->~IsClonk(), Equal(GetController(), -1))) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {owner=GetOwner(obj);
  if(!Equal(GetOwner(obj), GetOwner(this())))
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

  heat=CreateObject(PRQ_,nil,nil,GetOwner());
  ObjectSetAction(heat,"Attaching",this());

  leitstand=CreateObject(TSG_,nil,nil,GetOwner());
  ObjectSetAction(leitstand,"Attaching",this());

  schild=CreateObject(TSS_,nil,nil,GetOwner());
  ObjectSetAction(schild,"Attaching",this());

  energie=10000;

  SetAction("Fliegen");
  return(1);
}


func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
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
    return(90);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(-10);
  if(Equal(iType, DMG_Energy)) {
    DoEnergy(iDmg, this(), true);
    return(-100);
    }
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if((GetDamage() ?? 0)<100) return(nil);
  counter=nil;
  SetEntrance(nil);
  SetAction("Down");
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
  Sparks((GetDamage() ?? 0)/20,RGB(nil,50,255),RandomX(-20,20),RandomX(-15,15));
  if((GetDamage() ?? 0)>=80)
  {
  CreateParticle("Thrust",-5+(Random(10) ?? 0) ,-5+(Random(10) ?? 0),(GetXDir() ?? 0)/2,+2,RandomX(70,90),RGBa(nil,150,200,60),nil,nil);
  CreateParticle("Smoke2",-10+(Random(30) ?? 0) ,-10+(Random(30) ?? 0),(GetXDir() ?? 0)/2,+5,RandomX(80,120),RGBa(220,200,180,nil),nil,nil);
  }
  if((GetDamage() ?? 0)>=90)
  if(Equal(Random(250-(GetDamage() ?? 0)*2), 1)) DoDamage(1);
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(40); else FakeExplode(40,nil,nil,40);
  return(1);
}

//Steuerung

func ContainedUp()
{
  return(SetComDir(COMD_Up));
}

func ContainedDown()
{
  return(SetComDir(COMD_Down));
}

func ContainedRight()
{
  return(SetComDir(COMD_Right));
}

func ContainedLeft()
{
  return(SetComDir(COMD_Left));
}

//Menüsteuerung
func ContainedDig()
{
  CreateMenu(A56B,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(leitstand->Besetzt())    AddMenuItem("%s","Geschuetzkanzel",M17H,Par(nil));  //Geschützkanzel
  if(!(leitstand->Besetzt())) AddMenuItem("%s","Geschuetzkanzel",M17G,Par(nil));
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  AddMenuItem("%s","System",M37G,Par(nil)); //Systemfähigkeiten
  AddMenuItem("%s","Schwebe",M27G,Par(nil));   //Schwebe
  AddMenuItem("%s","Statuskontrolle",M57G,Par(nil)); //Anzeige Energie
  AddMenuItem("%s",nil,M67G,Par(nil));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  AddMenuItem("%s","Ausstieg",M47G,Par(nil));    //Ausstieg
  SelectMenuItem(7,Par(nil));
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(nil);
  }
  return(1);
}

func Gelandet()
{
  //Schwebe
  if(GetDamage()) if(!Random(100)) Reparatur();  
  if ((counter ?? 0)>3600) counter=nil;
  S42MigrationAssign(counter, "+=", 50);
  if((GetXDir() ?? 0)>0)  SetXDir((GetXDir() ?? 0)-2);
  if((GetXDir() ?? 0)<0)  SetXDir((GetXDir() ?? 0)+2);
  SetYDir(Sin(counter,18,10),nil,10);
  Schaden();
  Getarnt();
  if((energie ?? 0)<10000) S42MigrationStep(energie, 1, true);
  //Eintritt nur für eine Person
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(nil);
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(nil);
  }
  return(1);
}

//Sonderfunktionen
public func Reparatur()
{
  DoDamage(-3);
  CastParticles("PxSpark",(Random(20) ?? 0)-10,(Random(20) ?? 0)-10,nil,10,30,40, RGB(187, 214, 224), RGB(200, 224, 250));
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
  return(nil);
  }
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
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
  SetComDir(COMD_Stop);
  return(SetAction("Gelandet"));
  }
  return(1);
}


func Ausstieg()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Sound("Airlock1");
  Exit(pilot,nil,20);
  pilot=nil;
  SetEntrance(1);
  return(1);
}


func Statuskontrolle()
{

  Message("S:%d% E:%d%",this(),GetDamage(),(energie ?? 0)/100);

  return(1);
}


func System()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  CreateMenu(A56B ,pilot,nil,nil,nil,nil);
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
  if((energie ?? 0)<2000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(nil);
  }
  RemoveObject(schild);
  SetVisibility((VIS_Allies ?? 0) | (VIS_Owner ?? 0));
  SetClrModulation(RGBa(180,180,255,100));
  SetVisibility((VIS_Allies ?? 0) | (VIS_Owner ?? 0),leitstand);
  SetClrModulation(RGBa(180,180,255,100),leitstand);
  S42MigrationAssign(energie, "-=", 2000);
  Nofx();
  Sound("Cloak1",this());
  tarn=1;
}


func Getarnt()
{
  if(tarn) 
  {
  S42MigrationAssign(energie, "-=", 5);
  if((energie ?? 0)<0) Enttarnen();
  }
  return(1);
}


func Enttarnen()
{
  tarn=nil;
  schild=CreateObject(TSS_,nil,nil,GetOwner());
  ObjectSetAction(schild,"Attaching",this());
  SetVisibility(VIS_All);
  SetClrModulation(RGBa(255,255,255,nil));
  SetVisibility(VIS_All,leitstand);
  SetClrModulation(RGBa(255,255,255,nil),leitstand);
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
  return(nil);
  }
  if((energie ?? 0)<5000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(nil);
  }
  Sound("DeEnergize");
  S42MigrationAssign(energie, "-=", 5000);
  Nofx();
  SetPosition(GetX(boje),GetY(boje));
  SetXDir(nil);
  SetYDir(nil);
  Nofx();
  return(1);
}



func Warpboje()
{
  if(boje) boje->Hit();
  if((energie ?? 0)<2000)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(nil);
  }
  S42MigrationAssign(energie, "-=", 2000);
  boje=CreateObject(TSB_);
  Sound("DeEnergize");
  return(1);
}


func Nofx()
{
  AddSpawnEffect(this(),RGBa(220,80,180,100));
  return(1);
}







public func IsVehicleProduct(...) { return(1);
}















