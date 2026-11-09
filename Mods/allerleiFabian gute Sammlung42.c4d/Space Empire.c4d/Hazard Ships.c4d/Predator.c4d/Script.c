/* Predator-Raumjäger */

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }
public func IsThreat() {return(true);}

  local speed, pilot, waffensys, EMPDamaged, kapsel, heat, brain,mlight;
local saboteur; // wenn gesetzt, greift der Jäger Militärgebäude an
func Ejection()
{
SetController(-1);
}

func Destruction(a,b,c,d)
{
GameCallEx("Zaehler5");
return(_inherited(a,b,c,d));
}

//Anfangswerte
protected func Initialize()
{
   if(!Random(5)) saboteur =1; // jeder 5te Jäger greift Militärgebäude an
  speed=60;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}

//Schaden & Aufprall
func ContactTop()
{
  if (GetXDir()>15)  return(Explosion());
  if (GetYDir()>15)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if (GetXDir()>20)  return(Explosion());
  if (GetYDir()>20)  return(Explosion());
  return(1);
}

func ContactLeft()
{
  if (GetYDir()>25)  return(Explosion());
  if (GetXDir()>25)  return(Explosion());
  if (GetXDir()<-25) return(Explosion());
  if (GetXDir()>10)  return(Funken());
  if (GetXDir()<-10) return(Funken());
  return(1);
}

func ContactRight()
{
  if (GetYDir()>25)  return(Explosion());
  if (GetXDir()>25)  return(Explosion());
  if (GetXDir()<-25) return(Explosion());
  if (GetXDir()>10)  return(Funken());
  if (GetXDir()<-10) return(Funken());
  return(1);
}

func Funken()
{
  CreateParticle("Funken",RandomX(-3,3),13,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  return(1);
}

func Removal()
{
  if(waffensys) RemoveObject(waffensys);
  if(heat) RemoveObject(heat);
  if(brain) brain->Killed();
}

func Explosion()
{
  if(this())
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  Removal();
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
 else
  FakeExplode(40,0,0,40); //eine explosion, die kein schaden auf der erde macht
}
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
  if(GetDamage()<100) return(0);
  Explosion();
  return(1);
}

public func EMPShock()
{
  //Nicht zweimal.
  if( GetEffect("Damaged", this())) return(0);
  EMPDamaged = 1;
  SetAction("EMP");
  if(heat) RemoveObject(heat);
  AddEffect("Damaged", this(), 190, 20, this(), GetID());
  SetEntrance(0);
  return(1);
}

public func FxDamagedStart(object pTarget, int iEffectNumber, int iTemp)
{
}

public func FxDamagedTimer(object pTarget, int iEffectNumber)
{
  if(! Contained())
    CreateParticle("PxSpark", RandomX(-2, 2), RandomX(2, 2), RandomX(-2, 2), 4, 30, RGB(187, 214, 224));
	Idlespin();
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  EMPDamaged = false;
  SetAction("Gelandet");
  SetEntrance(1);
}


func Wasser()
{
  if(GetXDir()>1) SetXDir(GetXDir()-1);
  if(GetXDir()<-1) SetXDir(GetXDir()+1);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  Idlespin();
  return(1);
}


func Idlespin()
{
  if(GetXDir()>0) SetXDir(GetXDir()-1);
  if(GetXDir()<0) SetXDir(GetXDir()+1);
  var aspeed;
  aspeed=Sqrt(GetXDir()*GetXDir()+GetYDir()*GetYDir());
  if(Stuck()||GBackSolid(0,27))    return();
  if(GetR()>=170  && GetRDir()!=0) return(SetRDir(GetRDir()-(1-aspeed/80)));
  if(GetR()<=-170 && GetRDir()!=0) return(SetRDir(GetRDir()+(1-aspeed/80)));
  if(GetR()>=0 && GetRDir()<=3) SetRDir(GetRDir()+1);
  if(GetR()<0 && GetRDir()>=-3) SetRDir(GetRDir()-1);
}

//Flugkontrolle
func Fliegen()
{
  if (speed<40) speed=40;
  SetXDir(Cos(Sum(GetR(),270),speed));
  SetYDir(Sin(Sum(GetR(),270),speed));
  CreateParticle("Thrust",Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(255,200,200,60),0,0);
  CreateParticle("Smoke2",Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetXDir()/2,GetYDir()/2,RandomX(30,50),RGBa(220,200,180,0),0,0);

//Schaden sichtbar machen
  if(GetDamage()>=20)
  CreateParticle("Smoke2",Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetXDir()/2,GetYDir()/2,GetDamage()*2,RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=50)
  CreateParticle("Smoke2",Cos( GetR() + 270 , -10+Random(20)),Sin( GetR() + 270, -10+Random(20)),GetXDir()/2,GetYDir()/2,GetDamage(),RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=80)
  {
  CreateParticle("Thrust",Cos( GetR() + 270 , Random(10)),Sin( GetR() + 270, Random(10)),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(255,200,200,60),0,0);
  CreateParticle("Smoke2",Cos( GetR() + 270 , Random(10)),Sin( GetR() + 270, Random(10)),GetXDir()/2,GetYDir()/2,RandomX(30,50),RGBa(220,200,180,0),0,0);
  }
  if(GetDamage()>=90)
  if(Random(250-GetDamage()*2)==1) DoDamage(1);
  if(GetDamage()>=95) Idlespin();
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  }
  return(1);
}

func Gelandet()
{
//Drehen
  Idlespin();
//Schaden sichtbar machen
  if(GetDamage()>=50)
  CreateParticle("Smoke2",Cos( GetR() + 270 , -10+Random(20)),Sin( GetR() + 270, -10+Random(20)),GetXDir()/2,GetYDir()/2,GetDamage(),RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=80)
  {
  CreateParticle("Smoke2",Cos( GetR() + 270 , Random(10)),Sin( GetR() + 270, Random(10)),GetXDir()/2,GetYDir()/2,RandomX(30,50),RGBa(220,200,180,0),0,0);
  }
//Besitzer zuweisen
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) 
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  return(1);
  }
}

//Steuerung & Schubregelung
func ContainedUp()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if (speed<100)  speed+=5;
  SetRDir(0);
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
  if (speed>40)   speed-=5;
  SetRDir(0);
  return(1);
}

func ContainedLeft()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir(-5+GetDamage()/40);
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
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir( 5-GetDamage()/40);
  return(1);
}

//Waffensysteme

func ContainedThrow() 
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  if(!waffensys) Waffensystem();
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  if(brain) pilot=brain;
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(PRWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=PRWS)   ShiftContents(pilot);
  }
  if(brain) waffensys=CreateContents(PRWS, brain);
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
  CreateMenu(PRE_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(0));  //Waffenmodi
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Fahrgestell",M36G,Par(0)); //Fahrgestell
  AddMenuItem("%s","Triebwerk",M26G,Par(0));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(0)); //Ausstieg
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(0));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(0));  //Licht aus
  AddMenuItem("%s","Ausstieg",M46G,Par(0));    //Ausstieg
  SelectMenuItem(7,Par(0)); 
  return(1);
}

func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,210,200,40);
  mlight = AddLight2(1500, iColor, this());
  return(1);
}

func TurnLightOff()
{
  if (mlight) {RemoveObject(mlight);}
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
  Sound("PRModus");
  if (SEqual(GetAction(),"Fliegen")) 
  {
  if(heat) RemoveObject(heat);
  return(SetAction("Gelandet"));
  }
  if (SEqual(GetAction(),"Gelandet"))
  {
  heat=CreateObject(PRQ_,0,0,GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Fliegen");
  }
  return(1);
}

func Fahrgestell()
{
  Sound("Airlock1");
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  if(heat) RemoveObject(heat);
  ChangeDef(PRL_);
  this()->SetAction("Gelandet"); 
  return(1);
}

func Ausstieg()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  if (SEqual(GetAction(),"Fliegen")) return(Kapsel());
  Exit(pilot);
  pilot=0;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}


func Kapsel()
{
  kapsel=CreateContents(PRK_);
  Enter(kapsel,pilot);
  Exit(kapsel,0,0,GetR(),GetXDir()/5,GetYDir()/5,0);
  ObjectSetAction(kapsel,"Start");
  if(heat) RemoveObject(heat);
  Sound("Rocket");
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(50);
  else FakeExplode(50,0,0,50);
  return(1);
}

func Schadenskontrolle()
{
  if (!GetDamage())
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),GetDamage());
  return(1);
}

//Brainfunktionen
func Brain(_brain) 
{
brain=_brain;
SetEntrance(0);
}

func Saboteurjaeger() {return(saboteur);}

func Typ() { return(1); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned() { return(1); }

func TurretOwner() { return(1); }



public IsVehicleProduct: return(1);