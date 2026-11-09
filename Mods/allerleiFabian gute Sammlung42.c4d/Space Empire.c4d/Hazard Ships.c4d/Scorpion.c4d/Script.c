/* Scorpion-Raumjäger */

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }
public func IsThreat() {return(true);}

  local speed, pilot, waffensys, turret, EMPDamaged, energy, tarn, heat, cancer, brain,mlight;
local saboteur; // wenn gesetzt, greift der Jäger Militärgebäude an

func Ejection()
{
SetController(-1);
}

func Destruction(a,b,c,d)
{
GameCallEx("Zaehler6");
return(_inherited(a,b,c,d));
}

//Anfangswerte
protected func Initialize()
{
  if(!Random(10)) saboteur =1; // jeder 10te Jäger greift Militärgebäude an
  //Spike Erstellen
  turret=CreateObject(SCS_,0,0,GetOwner());
  ObjectSetAction(turret,"Attaching",this());
  SetActionData(256 + 1 , turret);
  turret->Bezug(this());
  speed=80;
  energy=200;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}

func Bezug(_cancer)
{
  cancer=_cancer;
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))  return(1);
  if(brain) return(1);
  return(0);
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
  if(turret) RemoveObject(turret);
  if(brain) brain->Killed();
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  Removal();
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
  else FakeExplode(40,0,0,40); //eine explosion, die kein schaden auf der erde macht
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
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  EMPDamaged = false;
  SetAction("Gelandet");
  SetEntrance(1);
}

//Flugkontrolle
func Fliegen()
{
  if (speed<50) speed=50;
  SetXDir(Cos(Sum(GetR(),270),speed));
  SetYDir(Sin(Sum(GetR(),270),speed));
  if(!tarn) CreateParticle("Thrust",Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(50,50,250,60),0,0);

//Tarnung  
  if(tarn) 
  {
  energy-=1;
  if(energy<0) Enttarnen();
  }
  
  if(!tarn) 
  {
  if(energy<200) energy+=1; 
  }

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
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  SetOwner(GetOwner(pilot));
  SetOwner(GetOwner(pilot),turret);
  SetEntrance(0); 
  }
  return(1);
}

func Gelandet()
{
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
  if (speed<120)  speed+=5;
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
  if (speed>50)   speed-=5;
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
  SetRDir(-7+GetDamage()/40);
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
  SetRDir( 7-GetDamage()/40);
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
  if(waffensys=CreateContents(SCWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=SCWS)   ShiftContents(pilot);
  }
  if(brain) waffensys=CreateContents(SCWS, brain);
  waffensys->Bezug(this(),turret);  
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
  CreateMenu(SCR_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(0));  //Waffenmodi
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Tarnung",M77G,Par(0)); //Tarnung
  AddMenuItem("%s","Triebwerk",M26G,Par(0));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(0),0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(0));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(0));  //Licht aus
  AddMenuItem("%s","Ausstieg",M46G,Par(0),0,0,"Auf dem Boden normaler Ausstieg. In der Luft Schleudersitz!");    //Ausstieg
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
  if (SEqual(GetAction(),"Gelandet") || SEqual(GetAction(),"Attaching"))
  {
  heat=CreateObject(PRQ_,Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Fliegen");
  }
  return(1);
}

func Tarnung()
{
  if(tarn) 
  {
  Enttarnen();
  return(1);
  }
  if(energy<100)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(0);
  }
  SetVisibility(VIS_Allies | VIS_Owner);
  SetClrModulation(RGBa(180,180,255,100));
  SetVisibility(VIS_Allies | VIS_Owner,turret);
  SetClrModulation(RGBa(180,180,255,100),turret);
  if(GetOwner()==-1) SetVisibility(VIS_None);
  if(GetOwner()==-1) SetVisibility(VIS_None,turret);
  energy-=20;
  Nofx();
  Sound("Cloak1",this());
  tarn=1;
}

func Enttarnen()
{
  tarn=0;
  SetVisibility(VIS_All);
  SetClrModulation(RGBa(255,255,255,0));
  SetVisibility(VIS_All,turret);
  SetClrModulation(RGBa(255,255,255,0),turret);
  Nofx();
  Sound("Cloak2",this());
  return(1);
}

func Nofx()
{
  AddSpawnEffect(this(),RGBa(220,80,180,100));
  return(1);
}


func Ausstieg()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  if(SEqual(GetAction(),"Fliegen")) return(Teleport());
  Sound("Airlock1");
  Exit(pilot);
  pilot=0;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}


func Teleport()
{
  var Beam;
  Beam=CreateContents(SCB_);
  Enter(Beam,pilot);
  Exit(Beam,0,0,0,0,20,0);
  Beam->Down();
  Sound("Cloak2",this());
  if(cancer=FindObject(CAN_)) if(!Hostile(GetOwner(cancer),GetOwner(this())))
  {
  if(cancer->Scorpcheck1())
  {
  SetRDir(0);
  Nofx();
  return(cancer->Scorpdock1(this()));
  }
  if(cancer->Scorpcheck2())
  {
  SetRDir(0);
  Nofx();
  return(cancer->Scorpdock2(this()));
  }
  }
  Explosion();
  return(1);

}


func Schadenskontrolle()
{
  Message("Schaden: %d%.Energie: %d%.",this(),GetDamage(),energy/2);
  return(1);
}


//Brainfunktionen
func Brain(_brain) 
{
brain=_brain;
SetEntrance(0);
}

func Saboteurjaeger() {return(saboteur);}

func Typ() { return(2); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned(& tarned, & energized)
 {
 tarned=tarn;
 energized=(energy>100);
 return(1);
 }

func TurretOwner(_owner) { return(SetOwner(_owner,turret)); }


public IsVehicleProduct: return(1);

