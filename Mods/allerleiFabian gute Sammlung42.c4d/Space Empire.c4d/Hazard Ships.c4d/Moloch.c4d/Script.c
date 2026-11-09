/* Stippel Moloch */

#strict 3

local speed, pilot, waffensys, EMPDamaged, kapsel, heat, clawl, clawr ,clawtimer ,clawmov ,brain, killtimer;

public func IsStippel()     { return(true); }
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func ReadyToFire()   { return(true); }
public func IsThreat() {return(true);}

func Death(a,b,c,d) // kein lebewesen daher bringt death nichts
{
GameCallEx("Zaehler7");  
return(_inherited(a,b,c,d));
}

//Anfangswerte
protected func Initialize()
{
 //Klauen anbringen
  clawl=CreateObject(MOL_,nil,nil,GetOwner());
  ObjectSetAction(clawl,"Attaching",this());
  SetActionData(256 + 5 , clawl);
  clawr=CreateObject(MOC_,nil,nil,GetOwner());
  ObjectSetAction(clawr,"Attaching",this());
  SetActionData(256 + 4 , clawr);
  clawtimer=nil;
  killtimer=nil;
  clawmov=nil;
  SetR(115+(GetR() ?? 0),clawl);
  SetR(-130+(GetR() ?? 0),clawr);
  speed=50;
  SetAction("Gelandet");
  //Hirn einsetzen
  SetEntrance(nil);
  SetOwner(-1);
  CreateContents(BRA_);
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
  if ((GetXDir() ?? 0)>15)  return(Hit());
  if ((GetYDir() ?? 0)>15)  return(Hit());
  return(1);
}

func ContactBottom()
{
  if ((GetXDir() ?? 0)>20)  return(Hit());
  if ((GetYDir() ?? 0)>20)  return(Hit());
  return(1);
}

func ContactLeft()
{
  if ((GetYDir() ?? 0)>25)  return(Hit());
  if ((GetXDir() ?? 0)>25)  return(Hit());
  if ((GetXDir() ?? 0)<-25) return(Hit());
  return(1);
}

func ContactRight()
{
  if ((GetYDir() ?? 0)>25)  return(Hit());
  if ((GetXDir() ?? 0)>25)  return(Hit());
  if ((GetXDir() ?? 0)<-25) return(Hit());
  return(1);
}

func Removal()
{
  if(waffensys) RemoveObject(waffensys);
  if(heat) RemoveObject(heat);
  if(brain) brain->Killed();
  RemoveObject(clawl);
  RemoveObject(clawr);
  return(1);
}

func Hit()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  Removal();
  Sound("MOTot");
  Sound("MOSchrei*");
  CastParticles ("StippelFetzen",30,60,-5,nil,10,10);
  CastParticles ("StippelFetzen",30,60,5,nil,10,10);
  CastParticles ("StippelFetzen",30,60,nil,-5,10,10);
  CastParticles ("StippelFetzen",30,60,nil,5,10,10);
  GameCallEx("Zaehler7");  
  RemoveObject();
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(40);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if((GetDamage() ?? 0)<33)
  {
  Sound("MOSchaden*");
  return(1);
  }
  SetAction("Gelandet");
  if(!GetRDir()) SetRDir(RandomX(-4,+4));
  Sound("MOSchrei*");
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
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -5),Sin( (GetR() ?? 0) + 270, -5),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(50,50,250,60),nil,nil);
//Sporen Generieren
  if(And(!Random(2300), !GetAmmo(MIAM,brain))) DoAmmo(MIAM,1,brain);
  if(And(!Random(1300), (GetAmmo(STAM,brain) ?? 0)<50)) DoAmmo(STAM,50,brain);
//Klauen bewegen
  if(!clawmov)S42MigrationStep(clawtimer, 1, true);
  else S42MigrationStep(clawtimer, -1, true);
  SetR(115+(GetR() ?? 0)+(clawtimer ?? 0),clawl);
  SetR(-130+(GetR() ?? 0)-(clawtimer ?? 0),clawr);
  if((clawtimer ?? 0)>=30) clawmov=1;
  if((clawtimer ?? 0)<0) clawmov=nil;
//Schaden heilen
  if((GetDamage() ?? 0)>=98) Idlespin();
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(nil);
  }
  return(1);
}

func Gelandet()
{
  S42MigrationStep(killtimer, 1, true);
  if((killtimer ?? 0)>70) Hit();
//Drehen
  Idlespin();
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
  if ((speed ?? 0)<100)  S42MigrationAssign(speed, "+=", 5);
  SetRDir(nil);
  return(1);
}

func ContainedDown()
{
  if ((speed ?? 0)>40)   S42MigrationAssign(speed, "-=", 5);
  SetRDir(nil);
  return(1);
}

func ContainedLeft()
{
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir(-5+(GetDamage() ?? 0)/40);
  return(1);
}

func ContainedRight()
{
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir( 5-(GetDamage() ?? 0)/40);
  return(1);
}

//Waffensysteme

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  if(brain) pilot=brain;
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(MOWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), MOWS))   ShiftContents(pilot);
  }
  if(brain) waffensys=CreateContents(MOWS, brain);
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
  CreateMenu(MOO_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Triebwerk",M26G,Par(nil));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(nil)); //Damage
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Ausstieg",M46G,Par(nil));    //Ausstieg
  SelectMenuItem(7,Par(nil));
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
  //heat=CreateObject(PRQ_,0,0,GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  SetAction("Fliegen");
  }
  return(1);
}


func Ausstieg()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  Exit(pilot);
  pilot=nil;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}



func Schadenskontrolle()
{
  if (!GetDamage())
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),(GetDamage() ?? 0)*3);
  return(1);
}

//Brainfunktionen
func Brain(_brain) 
{
brain=_brain;
SetEntrance(nil);
}

func Typ() { return(3); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned() { return(1); }

func TurretOwner() { return(1); }