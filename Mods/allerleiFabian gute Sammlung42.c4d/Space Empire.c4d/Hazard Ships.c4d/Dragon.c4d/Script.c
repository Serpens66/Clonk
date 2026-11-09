/* Dragon-Antigravpanzer */

#strict 3

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }


  local xspeed, yspeed, aspeed, maxspeed, ampli, boost, maxboost, mode, counter, pilot, waffensys, turret, turretcontrol, EMPDamaged, heat, kapsel,mlight;

//Anfangswerte
protected func Initialize()
{
  maxspeed=60;
  maxboost=30;
  boost=maxboost;
  ampli=13;
  SetAction("Landed");
  SetEntrance(1);
  //Geschütz erstellen und anbringen
  turret=CreateObject(DRG_,nil,nil,GetOwner());
  ObjectSetAction(turret,"Attaching",this());
  SetActionData(256 + 4 , turret);
  if(Random(1)) turret->Rotinit(70,-100,100);
  else			turret->Rotinit(-70,-100,100);
  turret->Bezug(this());
  turretcontrol=nil;
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

//Schaden & Aufprall
func ContactTop()
{
  return(Funken(nil,-12));
}

func ContactBottom()
{
  return(Funken(nil,12));
}

func ContactLeft()
{
  return(Funken(-30,10));
}

func ContactRight()
{
  return(Funken(30,10));
}

func Funken(x,y)
{
  if (SEqual(GetAction(),"Fly")) SetAction("Fall");
  var colspeed;
  colspeed=Sqrt((GetYDir() ?? 0)*(GetYDir() ?? 0)+(GetXDir() ?? 0)*(GetXDir() ?? 0));
  if((colspeed ?? 0)<10) return(nil);
  Sound("Dragonimpact*");
  CreateParticle("Funken",(RandomX(-3,3) ?? 0)+(x ?? 0),(RandomX(-3,3) ?? 0)+(y ?? 0),RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  if((colspeed ?? 0)>30) DoDamage((colspeed ?? 0)/2);
  return(1);
}

func Removal()
{
  if(waffensys) RemoveObject(waffensys);
  if(heat) RemoveObject(heat);
  if(turret) RemoveObject(turret);
  return(1);
}

func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
  //Wrack
  var wrack;
  wrack=CreateObject(_DRB);
  SetR(GetR(),wrack);
  SetXDir(GetXDir(),wrack);
  SetYDir(GetYDir(),wrack);
  Explode(30);
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(95);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(100);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(70);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(80);
}

func Damage()
{
  if((GetDamage() ?? 0)<200) return(nil);
  Explosion();
  return(1);
}

public func EMPShock()
{
  //Nicht zweimal.
  if( GetEffect("Damaged", this())) return(nil);
  EMPDamaged = 1;
  SetAction("EMP");
  if(heat) RemoveObject(heat);
  AddEffect("Damaged", this(), 190, 20, this(), GetID());
  SetEntrance(nil);
  return(1);
}

public func FxDamagedStart(object pTarget, int iEffectNumber, int iTemp)
{
iEffectNumber ??= 0;
iTemp ??= 0;
}

public func FxDamagedTimer(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  if(! Contained())
    CreateParticle("PxSpark", RandomX(-2, 2), RandomX(2, 2), RandomX(-2, 2), 4, 30, RGB(187, 214, 224));
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  EMPDamaged = nil;
  SetAction("Landed");
  SetEntrance(1);
}

func Landcheck() {
  //Besitzer zuweisen
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetOwner(GetOwner(pilot),turret);
  SetEntrance(nil);
  }
  if(Inside(GetR(),-2,2)) SetR(nil);
  //Schaden
  Smoke();
  return(1);   
}

func Hover()  {
  yspeed=GetYDir();
  ampli=yspeed;
  if((ampli ?? 0)>25)
  {
  //bouncen
  mode=1;
  counter=180;
  }
  else
  {
  //oszillieren
  mode=nil;
  counter=nil;
  }
  SetAction("Hover");
  return(1);
}

func Hovercheck() {
  var x,y;
  x=GetX();
  y=GetY();
  if((ampli ?? 0)<13) ampli=13;
  //Booster Refresh
  if((boost ?? 0)<(maxboost ?? 0)) S42MigrationStep(boost, 1, true);
  //X-Komponente
  xspeed=GetR();
  if((xspeed ?? 0)<0) S42MigrationAssign(xspeed, "-=", 5); //warum auch immer notwendig.
  SetXDir(xspeed);
  //Y-Komponente
  if((counter ?? 0)>360) counter=nil;
  S42MigrationAssign(counter, "+=", 10);
  if(!mode)yspeed=Sin(counter,ampli);
  if (And(mode, (!PathFree(x, y, x, (y ?? 0)+25)))) yspeed=Sin(counter,ampli);
  if((ampli ?? 0)>13) S42MigrationAssign(ampli, "-=", 10);
  //Steigung?
  if(!PathFree(x, y, (x ?? 0)+4*(xspeed ?? 0), (y ?? 0)+25)) S42MigrationAssign(yspeed, "-=", 3);
  SetYDir(yspeed);
  //Übersteuerungsschutzsystem
  if((GetR() ?? 0)<-20) {SetRDir(nil); SetR(-20);}
  if((GetR() ?? 0)>(+20 ?? 0)) {SetRDir(nil); SetR(+20);}
  //Altitude-Check
  if (PathFree(x, y, x, (y ?? 0)+40)) Fall();
  //Schaden
  Smoke();
  //Rammen
  if(xspeed)
  Hitter();  
  return(1); 
}

func Fall() {
  SetAction("Fall");
  return(1);
}

func Fallcheck() {
  var x,y;
  x=GetX();
  y=GetY();
  //Booster Refresh
  if((boost ?? 0)<(maxboost ?? 0)) S42MigrationStep(boost, 1, true);
  //X-Komponente
  xspeed=GetR();
  if((xspeed ?? 0)<0) S42MigrationAssign(xspeed, "+=", 3);
  SetXDir(xspeed);
  //Übersteuerungsschutzsystem
  if((GetR() ?? 0)<-20) {SetRDir(nil); SetR(-20);}
  if((GetR() ?? 0)>(+20 ?? 0)) {SetRDir(nil); SetR(+20);}
  //Altitude-Check
  if (!PathFree(x, y, x, (y ?? 0)+40)) Hover();
  //Schaden
  Smoke();
  //Rammen
  if(xspeed)
  Hitter();
  return(1);   
}

func Fly()
{
  //Booster leer?
  if(!boost) SetAction("Fall");
  S42MigrationStep(boost, -1, true);
  var x,y;
  if(!aspeed)
  {
  xspeed=GetXDir();
  yspeed=GetYDir();
  }
  if((aspeed ?? 0)<(maxspeed ?? 0)) S42MigrationStep(aspeed, 1, true);
  x=(xspeed ?? 0)+(Sin(GetR(),aspeed) ?? 0);
  y=(yspeed ?? 0)-(Cos(GetR(),aspeed) ?? 0);
  SetXDir(x);
  SetYDir(y);
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(30,50),RGBa(220,200,180,nil),nil,nil);
  //Übersteuerungsschutzsystem
  if((GetR() ?? 0)<-20) {SetRDir(nil); SetR(-20);}
  if((GetR() ?? 0)>(+20 ?? 0)) {SetRDir(nil); SetR(+20);}
  //Altitude-Check
  x=GetX();
  y=GetY();  
  if (!PathFree(x, y, x, (y ?? 0)+20)) Hover();
  //Schaden
  Smoke();
  //Rammen
  if(xspeed)
  Hitter();
  return(1);
}

//Steuerung

func ContainedLeft()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if(turretcontrol)
  {
  turret->ContainedLeft();
  return(1);
  }
  SetRDir(-2);
  return(1);
}

func ContainedRight()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if(turretcontrol)
  {
  turret->ContainedRight();
  return(1);
  }
  if((GetR() ?? 0)>=15) return(1);
  SetRDir(2);
  return(1);
}

func ContainedUp()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if(turretcontrol)
  {
  turret->ContainedUp();
  return(1);
  }
  if((GetR() ?? 0)<=-15) return(1);
  SetRDir(nil);
  return(1);
}

func ContainedDown()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  SetRDir(nil);
  return(1);
}

func ContainedUpDouble()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if (SEqual(GetAction(),"Fly")) 
  {
  return(SetAction("Fall"));
  }
  if (And((Or(SEqual(GetAction(),"Hover"), SEqual(GetAction(),"Fall"))), (boost ?? 0)>20))
  {
  aspeed=nil;
  return(SetAction("Fly"));
  }
  return(1);
}

func ContainedThrow() 
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if(!waffensys) Waffensystem();
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

//Menüsteuerung
func ContainedDig()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  CreateMenu(DRA_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",MDR1,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Turret",MDR3,Par(nil)); //Turmsteuerung
  AddMenuItem("%s","Triebwerk",MDR2,Par(nil));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",MDR5,Par(nil)); //Ausstieg
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  AddMenuItem("%s","Ausstieg",MDR4,Par(nil));    //Ausstieg
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


func Waffensystem()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(DRWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), DRWS))   ShiftContents(pilot);
  }
  waffensys->Turret(turret);
  return(1);
}

func ContainedDigDouble()
{
  Waffenmodi();
  return(1);
}


func Waffenmodi()
{
  if(!waffensys)  Waffensystem();
  ObjectCall(waffensys,"ControlDigDouble",pilot);
  Sound("PRModus");
  return(1);
}

func Triebwerk()
{
  //Übersteuerungsschutzsystem
  if(Or((GetR() ?? 0)<-20, (GetR() ?? 0)>(+20 ?? 0)))
  {
  Message("In dieser Position nicht möglich!",this());
  Sound("Error");
  return(1);
  }
  Sound("PRModus");
  if (!SEqual(GetAction(),"Landed")) 
  {
  if(heat) RemoveObject(heat);
  return(SetAction("Landed"));
  }
  if (SEqual(GetAction(),"Landed"))
  {
  heat=CreateObject(PRQ_,nil,nil,GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Hover");
  }
  return(1);
}

func Turret()
{
  turretcontrol=!turretcontrol;
  return(1);
}

func Ausstieg()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GSAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GRAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  if (!SEqual(GetAction(),"Landed")) return(Kapsel());
  Exit(pilot);
  pilot=nil;
  SetEntrance(1);
  return(1);
}


func Kapsel()
{
  kapsel=CreateContents(DRK_);
  Enter(kapsel,pilot);
  Exit(kapsel,nil,nil,GetR(),(GetXDir() ?? 0)/10,(GetYDir() ?? 0)/10-10,nil);
  ObjectSetAction(kapsel,"Start");
  if(heat) RemoveObject(heat);
  Sound("Rocket");
  Explosion();
  return(1);
}

func Schadenskontrolle()
{
  if (!GetDamage())
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),(GetDamage() ?? 0)/2);
  return(1);
}

func Smoke()
{
  //Schaden sichtbar machen
  if((GetDamage() ?? 0)>=80)
  CreateParticle("Smoke2",20,7,(GetXDir() ?? 0)/10,-20+(GetYDir() ?? 0)/10,(GetDamage() ?? 0)/3,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=140)
  CreateParticle("Smoke2",-22,7,(GetXDir() ?? 0)/10,-20+(GetYDir() ?? 0)/10,(GetDamage() ?? 0)/3,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=190)
  {
  CreateParticle("Thrust",RandomX(-5,5),RandomX(-5,5),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",RandomX(-5,5),RandomX(-5,5),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(40,60),RGBa(220,200,180,nil),nil,nil);
  }
  if((GetDamage() ?? 0)>=190)
  if(Equal(Random(250-(GetDamage() ?? 0)*2), 1)) DoDamage(2);
  return(1);
}


func Hitter()
{
   //Gegnerische Clonks Rammen
   var pT;
   for(pT in FindObjects(Find_Distance(25),
                         Find_NoContainer(),
                         Find_And(
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",this())))) {
    Punch(pT,10);
	Fling(pT, (xspeed ?? 0)/5, -5);
  }
  if(Equal(GetID(pT), HZCK)) 	Sound("Dragonimpact*");
  return(1);
}
  
global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}











