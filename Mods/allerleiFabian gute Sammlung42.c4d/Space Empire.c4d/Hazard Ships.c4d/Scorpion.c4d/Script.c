/* Scorpion-Raumjäger */

#strict 3

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
  turret=CreateObject(SCS_,nil,nil,GetOwner());
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))  return(1);
  if(brain) return(1);
  return(nil);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

//Schaden & Aufprall
func ContactTop()
{
  if ((GetXDir() ?? 0)>15)  return(Explosion());
  if ((GetYDir() ?? 0)>15)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if ((GetXDir() ?? 0)>20)  return(Explosion());
  if ((GetYDir() ?? 0)>20)  return(Explosion());
  return(1);
}

func ContactLeft()
{
  if ((GetYDir() ?? 0)>25)  return(Explosion());
  if ((GetXDir() ?? 0)>25)  return(Explosion());
  if ((GetXDir() ?? 0)<-25) return(Explosion());
  if ((GetXDir() ?? 0)>10)  return(Funken());
  if ((GetXDir() ?? 0)<-10) return(Funken());
  return(1);
}

func ContactRight()
{
  if ((GetYDir() ?? 0)>25)  return(Explosion());
  if ((GetXDir() ?? 0)>25)  return(Explosion());
  if ((GetXDir() ?? 0)<-25) return(Explosion());
  if ((GetXDir() ?? 0)>10)  return(Funken());
  if ((GetXDir() ?? 0)<-10) return(Funken());
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
  else FakeExplode(40,nil,nil,40); //eine explosion, die kein schaden auf der erde macht
  return(1);
}

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
  if((GetDamage() ?? 0)<100) return(nil);
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
  SetAction("Gelandet");
  SetEntrance(1);
}

//Flugkontrolle
func Fliegen()
{
  if ((speed ?? 0)<50) speed=50;
  SetXDir(Cos(Sum(GetR(),270),speed));
  SetYDir(Sin(Sum(GetR(),270),speed));
  if(!tarn) CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(50,50,250,60),nil,nil);

//Tarnung  
  if(tarn) 
  {
  S42MigrationAssign(energy, "-=", 1);
  if((energy ?? 0)<0) Enttarnen();
  }
  
  if(!tarn) 
  {
  if((energy ?? 0)<200) S42MigrationAssign(energy, "+=", 1);
  }

//Schaden sichtbar machen
  if((GetDamage() ?? 0)>=20)
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,(GetDamage() ?? 0)*2,RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=50)
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10+(Random(20) ?? 0)),Sin( (GetR() ?? 0) + 270, -10+(Random(20) ?? 0)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,GetDamage(),RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=80)
  {
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , Random(10)),Sin( (GetR() ?? 0) + 270, Random(10)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , Random(10)),Sin( (GetR() ?? 0) + 270, Random(10)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(30,50),RGBa(220,200,180,nil),nil,nil);
  }
  if((GetDamage() ?? 0)>=90)
  if(Equal(Random(250-(GetDamage() ?? 0)*2), 1)) DoDamage(1);
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetOwner(GetOwner(pilot),turret);
  SetEntrance(nil);
  }
  return(1);
}

func Gelandet()
{
//Schaden sichtbar machen
  if((GetDamage() ?? 0)>=50)
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10+(Random(20) ?? 0)),Sin( (GetR() ?? 0) + 270, -10+(Random(20) ?? 0)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,GetDamage(),RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=80)
  {
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , Random(10)),Sin( (GetR() ?? 0) + 270, Random(10)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(30,50),RGBa(220,200,180,nil),nil,nil);
  }
//Besitzer zuweisen
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(nil);
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
  if ((speed ?? 0)<120)  S42MigrationAssign(speed, "+=", 5);
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
  if ((speed ?? 0)>50)   S42MigrationAssign(speed, "-=", 5);
  SetRDir(nil);
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
  SetRDir(-7+(GetDamage() ?? 0)/40);
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
  SetRDir( 7-(GetDamage() ?? 0)/40);
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  if(brain) pilot=brain;
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(SCWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), SCWS))   ShiftContents(pilot);
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
  CreateMenu(SCR_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Tarnung",M77G,Par(nil)); //Tarnung
  AddMenuItem("%s","Triebwerk",M26G,Par(nil));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(nil),nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  AddMenuItem("%s","Ausstieg",M46G,Par(nil),nil,nil,"Auf dem Boden normaler Ausstieg. In der Luft Schleudersitz!");    //Ausstieg
  SelectMenuItem(7,Par(nil));
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
  if (Or(SEqual(GetAction(),"Gelandet"), SEqual(GetAction(),"Attaching")))
  {
  heat=CreateObject(PRQ_,Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),GetOwner());
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
  if((energy ?? 0)<100)
  {
  Message("Nicht genug Energie.",this());
  Sound("Error");
  return(nil);
  }
  SetVisibility((VIS_Allies ?? 0) | (VIS_Owner ?? 0));
  SetClrModulation(RGBa(180,180,255,100));
  SetVisibility((VIS_Allies ?? 0) | (VIS_Owner ?? 0),turret);
  SetClrModulation(RGBa(180,180,255,100),turret);
  if(Equal(GetOwner(), -1)) SetVisibility(VIS_None);
  if(Equal(GetOwner(), -1)) SetVisibility(VIS_None,turret);
  S42MigrationAssign(energy, "-=", 20);
  Nofx();
  Sound("Cloak1",this());
  tarn=1;
}

func Enttarnen()
{
  tarn=nil;
  SetVisibility(VIS_All);
  SetClrModulation(RGBa(255,255,255,nil));
  SetVisibility(VIS_All,turret);
  SetClrModulation(RGBa(255,255,255,nil),turret);
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  if(SEqual(GetAction(),"Fliegen")) return(Teleport());
  Sound("Airlock1");
  Exit(pilot);
  pilot=nil;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}


func Teleport()
{
  var Beam;
  Beam=CreateContents(SCB_);
  Enter(Beam,pilot);
  Exit(Beam,nil,nil,nil,nil,20,nil);
  Beam->Down();
  Sound("Cloak2",this());
  if(cancer=FindObject(CAN_)) if(!Hostile(GetOwner(cancer),GetOwner(this())))
  {
  if(cancer->Scorpcheck1())
  {
  SetRDir(nil);
  Nofx();
  return(cancer->Scorpdock1(this()));
  }
  if(cancer->Scorpcheck2())
  {
  SetRDir(nil);
  Nofx();
  return(cancer->Scorpdock2(this()));
  }
  }
  Explosion();
  return(1);

}


func Schadenskontrolle()
{
  Message("Schaden: %d%.Energie: %d%.",this(),GetDamage(),(energy ?? 0)/2);
  return(1);
}


//Brainfunktionen
func Brain(_brain) 
{
brain=_brain;
SetEntrance(nil);
}

func Saboteurjaeger() {return(saboteur);}

func Typ() { return(2); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned(& tarned, & energized)
 {
 tarned=tarn;
 energized=((energy ?? 0)>100);
 return(1);
 }

func TurretOwner(_owner) { return(SetOwner(_owner,turret)); }


public func IsVehicleProduct(...) { return(1);
}

