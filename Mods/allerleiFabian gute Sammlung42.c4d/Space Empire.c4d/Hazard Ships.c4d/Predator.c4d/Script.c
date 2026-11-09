/* Predator-Raumjäger */

#strict 3

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
  if(brain) brain->Killed();
}

func Explosion()
{
  if(this())
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
 else
  FakeExplode(40,nil,nil,40); //eine explosion, die kein schaden auf der erde macht
}
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
	Idlespin();
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  EMPDamaged = nil;
  SetAction("Gelandet");
  SetEntrance(1);
}


func Wasser()
{
  if((GetXDir() ?? 0)>1) SetXDir((GetXDir() ?? 0)-1);
  if((GetXDir() ?? 0)<-1) SetXDir((GetXDir() ?? 0)+1);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  Idlespin();
  return(1);
}


func Idlespin()
{
  if((GetXDir() ?? 0)>0) SetXDir((GetXDir() ?? 0)-1);
  if((GetXDir() ?? 0)<0) SetXDir((GetXDir() ?? 0)+1);
  var aspeed;
  aspeed=Sqrt((GetXDir() ?? 0)*(GetXDir() ?? 0)+(GetYDir() ?? 0)*(GetYDir() ?? 0));
  if(Or(Stuck(), GBackSolid(nil,27)))    return;
  if(And((GetR() ?? 0)>=170, !Equal(GetRDir(), nil))) return(SetRDir((GetRDir() ?? 0)-(1-(aspeed ?? 0)/80)));
  if(And((GetR() ?? 0)<=-170, !Equal(GetRDir(), nil))) return(SetRDir((GetRDir() ?? 0)+(1-(aspeed ?? 0)/80)));
  if(And((GetR() ?? 0)>=0, (GetRDir() ?? 0)<=3)) SetRDir((GetRDir() ?? 0)+1);
  if(And((GetR() ?? 0)<0, (GetRDir() ?? 0)>=-3)) SetRDir((GetRDir() ?? 0)-1);
}

//Flugkontrolle
func Fliegen()
{
  if ((speed ?? 0)<40) speed=40;
  SetXDir(Cos(Sum(GetR(),270),speed));
  SetYDir(Sin(Sum(GetR(),270),speed));
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(30,50),RGBa(220,200,180,nil),nil,nil);

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
  if((GetDamage() ?? 0)>=95) Idlespin();
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(nil);
  }
  return(1);
}

func Gelandet()
{
//Drehen
  Idlespin();
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
  if ((speed ?? 0)<100)  S42MigrationAssign(speed, "+=", 5);
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
  if ((speed ?? 0)>40)   S42MigrationAssign(speed, "-=", 5);
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
  SetRDir(-5+(GetDamage() ?? 0)/40);
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
  SetRDir( 5-(GetDamage() ?? 0)/40);
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
  if(waffensys=CreateContents(PRWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), PRWS))   ShiftContents(pilot);
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
  CreateMenu(PRE_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Fahrgestell",M36G,Par(nil)); //Fahrgestell
  AddMenuItem("%s","Triebwerk",M26G,Par(nil));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(nil)); //Ausstieg
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  if(!(mlight))AddMenuItem("%s","TurnLightOn",RAL2,Par(nil));  //Licht ein
  if(mlight)AddMenuItem("%s","TurnLightOff",RAL3,Par(nil));  //Licht aus
  AddMenuItem("%s","Ausstieg",M46G,Par(nil));    //Ausstieg
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
  heat=CreateObject(PRQ_,nil,nil,GetOwner());
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  if (SEqual(GetAction(),"Fliegen")) return(Kapsel());
  Exit(pilot);
  pilot=nil;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}


func Kapsel()
{
  kapsel=CreateContents(PRK_);
  Enter(kapsel,pilot);
  Exit(kapsel,nil,nil,GetR(),(GetXDir() ?? 0)/5,(GetYDir() ?? 0)/5,nil);
  ObjectSetAction(kapsel,"Start");
  if(heat) RemoveObject(heat);
  Sound("Rocket");
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(50);
  else FakeExplode(50,nil,nil,50);
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
SetEntrance(nil);
}

func Saboteurjaeger() {return(saboteur);}

func Typ() { return(1); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned() { return(1); }

func TurretOwner() { return(1); }



public func IsVehicleProduct(...) { return(1);
}