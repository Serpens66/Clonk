/* Mammoth-APC-Carrier */

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }

  local xspeed, yspeed, aspeed, maxspeed, minspeed, tarspeed, ampli, mode, counter, ccounter, heat, kapsel;
  local turbine, pod1, pod2, pilot, carry,mlight;

//TimerCall
func Ownerkontrolle()
{
var owner,obj;
for (obj in FindObjects(Find_Or(Find_Container(this()),Find_Container(pod1),Find_Container(pod2),Find_Container(turbine))) )
if(obj->~IsClonk() && GetController()==-1) // in den besitz des clonks übergehen, sofern es zuvor keinen controller hatte
  {
  owner=GetOwner(obj);
  if(GetOwner(obj)!=GetOwner(this()))
    {
  Sound("BipBipBip");
  Message("%v geht in den Besitz von %v über.",this(),GetName(this()),GetPlayerName(owner));
    }
  SetOwner(owner);
  SetOwner(owner,turbine);
  SetOwner(owner,pod1);
  SetOwner(owner,pod2);
  if(heat) SetOwner(owner,heat);}
// wenn kein Clonk irgendwo an bord ist, dann Controller=-1 setzen
obj=FindObject2(Find_Or(Find_Container(this()),Find_Container(pod1),Find_Container(pod2),Find_Container(turbine))) ;
if(!obj) SetController(-1);
if(obj)
if(!obj->~IsClonk())
SetController(-1);
}

//Anfangswerte
protected func Initialize()
{
  maxspeed=18;
  minspeed=-18;
  tarspeed=0;
  carry=0;
  ampli=13;
  SetAction("Landed");
  SetEntrance(1);
  //Turbine anbringen
  turbine=CreateObject(MAT_,0,0,GetOwner());
  ObjectSetAction(turbine,"Attaching",this());
  SetActionData(256 + 9 , turbine);
  turbine->Rotinit(0,-75,75);
  //Pods
  pod1=CreateObject(MAP_,0,0,GetOwner());
  ObjectSetAction(pod1,"Attaching",this());
  SetActionData(256 + 2 , pod1);
  pod1->Pod(+175,+100,+130,this(),+10);

  pod2=CreateObject(MAP_,0,0,GetOwner());
  ObjectSetAction(pod2,"Attaching",this());
  SetActionData(256 + 3 , pod2);
  pod2->Pod(-100,-175,-130,this(),-10);
 
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}

func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}

//Schaden & Aufprall
func ContactTop()
{
  return(Funken(0,-12));
}

func ContactBottom()
{
  return(Funken(0,12));
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
  colspeed=Sqrt(GetYDir()*GetYDir()+GetXDir()*GetXDir());
  if(colspeed<20) return(0);
  Sound("Dragonimpact*");
  CreateParticle("Funken",RandomX(-3,3)+x,RandomX(-3,3)+y,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  if(colspeed>50) DoDamage(colspeed/3);
  return(1);
}

func Removal()
{
  if(heat) RemoveObject(heat);
  if(pod1) pod1->Removal();
  if(pod2) pod2->Removal();
  if(turbine) RemoveObject(turbine);
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  Removal();
  //Wrack
  var wrack;
  wrack=CreateObject(_MB1);
  SetR(GetR(),wrack);
  SetXDir(GetXDir()+20,wrack);
  SetYDir(GetYDir()+20,wrack);
  wrack=CreateObject(_MB2);
  SetR(GetR(),wrack);
  SetXDir(GetXDir()-20,wrack);
  SetYDir(GetYDir()-20,wrack);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(60); else FakeExplode(60,0,0,60);
  return(1);
}

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
  if(GetDamage()<300) return(1);
  Explosion();
  return(1);
}


func Landcheck() {
  //Besitzer zuweisen
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) 
  {
  //SetOwner(GetOwner(pilot));
  SetEntrance(0);
  }
  if(Inside(GetR(),-2,2)) SetR(0);
  //Schaden
  Smoke();
  return(1);   
}

func Hover()  {
  yspeed=GetYDir();
  ampli=yspeed;
  if(ampli>25)
  {
  //bouncen
  mode=1;
  counter=180;
  }
  else
  {
  //oszillieren
  mode=0;
  counter=0;
  }
  SetAction("Hover");
  return(1);
}

func Hovercheck() {
  var x,y;
  x=GetX();
  y=GetY();
  if(ampli<13) ampli=13;
  //X-Komponente
  xspeed=GetR()*2;
  if(xspeed<0) xspeed-=5; //warum auch immer notwendig.
  SetXDir(xspeed);
  //Y-Komponente
  if(counter>360) counter=0;
  counter+=10;
  if(!mode)yspeed=Sin(counter,ampli);
  if (mode && (!PathFree(x, y, x, y+45))) yspeed=Sin(counter,ampli);
  if(ampli>13) ampli-=10;
  //Carry-Rotation
  if(carry) Carryrot();
  //Steigung?
  if(!PathFree(x, y, x+4*xspeed, y+45)) yspeed-=3;
  SetYDir(yspeed);
  //Übersteuerungsschutzsystem
  if(GetR()<-15) {SetRDir(0); SetR(-15);}
  if(GetR()>+15) {SetRDir(0); SetR(+15);}
  SetR(GetR()*5,turbine);
  //Altitude-Check
  if (PathFree(x, y, x, y+70)) Fall();
  //Schaden
  Smoke();
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
  //X-Komponente
  xspeed=GetR()*2;
  if(xspeed<0) xspeed+=3;
  SetXDir(xspeed);
  //Übersteuerungsschutzsystem
  if(GetR()<-15) {SetRDir(0); SetR(-15);}
  if(GetR()>+15) {SetRDir(0); SetR(+15);}
  SetR(GetR()*5,turbine);
  //Altitude-Check
  if (!PathFree(x, y, x, y+70)) Hover();
  //Schaden
  Smoke();
  return(1);   
}

func Fly()
{
  var x,y;
  //X-Komponente
  xspeed=GetR()*3;
  if(xspeed<0) xspeed+=3;
  SetXDir(xspeed);
  //Y-Komponente
  yspeed=GetYDir();
  if(yspeed<tarspeed) {y=yspeed-Cos(GetR(turbine),20)-2;}
  if(yspeed>tarspeed) {y=yspeed+Cos(GetR(turbine),20);}
  if(counter>360) counter=0;
  counter+=10;
  if(!mode)y+=Sin(counter,13);
  SetYDir(y);
  //Carry-Rotation
  if(carry) Carryrot();
  //Rauch
  x=-Sin(GetR(turbine),18)+RandomX(-2,2);
  y=+Cos(GetR(turbine),18)+RandomX(-2,2);
  CreateParticle("Smoke2",x,y,x*2,y*2,50,RGBa(220,200,180,0),0,0); 
  //Übersteuerungsschutzsystem
  if(GetR()<-15) {SetRDir(0); SetR(-15);}
  if(GetR()>+15) {SetRDir(0); SetR(+15);}
  SetR(GetR()*5,turbine);
  //Altitude-Check
  x=GetX();
  y=GetY();  
  if (!PathFree(x, y, x, y+40)) Hover();
  //Schaden
  Smoke();
  return(1);
}

//Steuerung

func ContainedLeft()
{
  if(GetR()<=-10) return(1);
  SetRDir(-2);
  return(1);
}

func ContainedRight()
{
  if(GetR()>=10) return(1);
  SetRDir(2);
  return(1);
}

func ContainedUp()
{
  if(tarspeed<maxspeed) tarspeed+=18;
  SetRDir(0);
  return(1);
}

func ContainedDown()
{
  if(tarspeed>minspeed) tarspeed-=18;
  SetRDir(0);
  return(1);
}

func ContainedUpDouble()
{
  if (SEqual(GetAction(),"Fly")) 
  {
  return(SetAction("Fall"));
  }
  if ((SEqual(GetAction(),"Hover") || SEqual(GetAction(),"Fall"))) 
  {
  aspeed=0;
  return(SetAction("Fly"));
  }
  return(1);
}

func ContainedThrow() 
{
  if(GetID(carry)==DRA_){ObjectSetAction(carry,"Landed"); Drop(); carry=0; return(1);}
  if(GetID(carry)==MAL_){ObjectSetAction(carry,"Start");  Drop(); carry=0; return(1);}  
  if(carry) if(!SEqual(GetAction(carry),"Attaching")){carry=0; return(1);}
  if(carry) return(1);
  if(carry=FindObject(DRA_,-25,10,50,50,0,"Landed"))
  {
  Sound("Connect");
  ObjectSetAction(carry,"Attaching",this());
  SetActionData(0,carry);
  return(1);
  }
  if(carry=FindObject(MAL_,-25,10,50,50,0,"Landed"))
  {
  Sound("Connect");
  ObjectSetAction(carry,"Attaching",this());
  SetActionData(0,carry);
  return(1);
  }
  return(1);
}

func Drop()
{
  SetXDir(GetXDir(),carry);
  SetYDir(GetYDir(),carry);
}

func Carryrot()
{
  if(ccounter>360) ccounter=0;
  ccounter+=5;
  SetR(Sin(ccounter,5),carry);
}

func ContainedDigDouble()
{
  return(1);
}

//Menüsteuerung
func ContainedDig()
{
  CreateMenu(DRA_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Schadenskontrolle",MMA5,Par(0)); //Ausstieg
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  if(pod2->Besetzt())    AddMenuItem("%s","Occp",MMA6,Par(0)); //Podeinstieg
  if(!(pod2->Besetzt())) AddMenuItem("%s","Pod2",MMA3,Par(0)); //Podeinstieg
  AddMenuItem("%s","Triebwerk",MMA2,Par(0));   //Triebwerk
  if(pod1->Besetzt())    AddMenuItem("%s","Occp",MMA6,Par(0)); //Podeinstieg
  if(!(pod1->Besetzt())) AddMenuItem("%s","Pod1",MMA3,Par(0)); //Podeinstieg
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(0));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(0));  //Licht aus
  AddMenuItem("%s","Ausstieg",MMA4,Par(0));    //Ausstieg
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

func Occp()
{
  Sound("Error");
  return(0);
}

func Triebwerk()
{
  //Übersteuerungsschutzsystem
  if(GetR()<-10 || GetR()>+10) 
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
  heat=CreateObject(PRQ_,0,0,GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Hover");
  }
  return(1);
}


func Ausstieg()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Sound("Airlock1");
  if (!SEqual(GetAction(),"Landed")) return(Kapsel());
  Exit(pilot);
  pilot=0;
  SetEntrance(1);
  return(1);
}


func Kapsel()
{
  kapsel=CreateContents(DRK_);
  Enter(kapsel,pilot);
  Exit(kapsel,0,0,GetR(),GetXDir()/10,GetYDir()/10-10,0);
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
  Message("Schaden: %d%.",this(),GetDamage()/3);
  return(1);
}

func Smoke()
{
  //Schaden sichtbar machen
  if(GetDamage()>=80)
  CreateParticle("Smoke2",43+RandomX(-2,2),15+RandomX(-2,2),GetXDir()/10,-20+GetYDir()/10,GetDamage()/3,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=140)
  CreateParticle("Smoke2",-43+RandomX(-2,2),15+RandomX(-2,2),GetXDir()/10,-20+GetYDir()/10,GetDamage()/3,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=190)
  {
  CreateParticle("Thrust",RandomX(-5,5),RandomX(-5,5),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(255,200,200,60),0,0);
  CreateParticle("Smoke2",RandomX(-5,5),RandomX(-5,5),GetXDir()/2,GetYDir()/2,RandomX(40,60),RGBa(220,200,180,0),0,0);
  }
  if(GetDamage()>=190)
  if(Random(250-GetDamage()*2)==1) DoDamage(2);
  return(1);
}

func Pod1()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(pod1,pilot);
  pilot=0;
  SetEntrance(1);
}

func Pod2()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(pod2,pilot);
  pilot=0;
  SetEntrance(1);
}

global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

  return(iAngle);
}




public IsVehicleProduct: return(1);







