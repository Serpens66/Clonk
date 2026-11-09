/* Stippel Moloch */

#strict

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
  clawl=CreateObject(MOL_,0,0,GetOwner());
  ObjectSetAction(clawl,"Attaching",this());
  SetActionData(256 + 5 , clawl);
  clawr=CreateObject(MOC_,0,0,GetOwner());
  ObjectSetAction(clawr,"Attaching",this());
  SetActionData(256 + 4 , clawr);
  clawtimer=0;
  killtimer=0;
  clawmov=0;
  SetR(115+GetR(),clawl);
  SetR(-130+GetR(),clawr);
  speed=50;
  SetAction("Gelandet");
  //Hirn einsetzen
  SetEntrance(0);
  SetOwner(-1);
  CreateContents(BRA_);
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
  if (GetXDir()>15)  return(Hit());
  if (GetYDir()>15)  return(Hit());
  return(1);
}

func ContactBottom()
{
  if (GetXDir()>20)  return(Hit());
  if (GetYDir()>20)  return(Hit());
  return(1);
}

func ContactLeft()
{
  if (GetYDir()>25)  return(Hit());
  if (GetXDir()>25)  return(Hit());
  if (GetXDir()<-25) return(Hit());
  return(1);
}

func ContactRight()
{
  if (GetYDir()>25)  return(Hit());
  if (GetXDir()>25)  return(Hit());
  if (GetXDir()<-25) return(Hit());
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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  Removal();
  Sound("MOTot");
  Sound("MOSchrei*");
  CastParticles ("StippelFetzen",30,60,-5,0,10,10); 
  CastParticles ("StippelFetzen",30,60,5,0,10,10); 
  CastParticles ("StippelFetzen",30,60,0,-5,10,10); 
  CastParticles ("StippelFetzen",30,60,0,5,10,10); 
  GameCallEx("Zaehler7");  
  RemoveObject();
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Projectile)
    return(80);
  if(iType & DMG_Fire)
    return(40);
  if(iType & DMG_Explosion)
    return(80);
  if(iType & DMG_Energy)
    return(40);
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(GetDamage()<33)
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
  CreateParticle("Thrust",Cos( GetR() + 270 , -5),Sin( GetR() + 270, -5),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(50,50,250,60),0,0);
//Sporen Generieren
  if(!Random(2300) && !GetAmmo(MIAM,brain)) DoAmmo(MIAM,1,brain);
  if(!Random(1300) && GetAmmo(STAM,brain)<50) DoAmmo(STAM,50,brain);
//Klauen bewegen
  if(!clawmov)clawtimer++;
  else clawtimer--;
  SetR(115+GetR()+clawtimer,clawl);
  SetR(-130+GetR()-clawtimer,clawr);
  if(clawtimer>=30) clawmov=1;
  if(clawtimer<0) clawmov=0;
//Schaden heilen
  if(GetDamage()>=98) Idlespin();
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(0); 
  }
  return(1);
}

func Gelandet()
{
  killtimer++;
  if(killtimer>70) Hit();
//Drehen
  Idlespin();
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
  if (speed<100)  speed+=5;
  SetRDir(0);
  return(1);
}

func ContainedDown()
{
  if (speed>40)   speed-=5;
  SetRDir(0);
  return(1);
}

func ContainedLeft()
{
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir(-5+GetDamage()/40);
  return(1);
}

func ContainedRight()
{
  if (SEqual(GetAction(),"Gelandet")) return(1);
  SetRDir( 5-GetDamage()/40);
  return(1);
}

//Waffensysteme

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  if(brain) pilot=brain;
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(MOWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=MOWS)   ShiftContents(pilot);
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
  CreateMenu(MOO_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(0));  //Waffenmodi
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Triebwerk",M26G,Par(0));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(0)); //Damage
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Ausstieg",M46G,Par(0));    //Ausstieg
  SelectMenuItem(7,Par(0)); 
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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  Exit(pilot);
  pilot=0;
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
  Message("Schaden: %d%.",this(),GetDamage()*3);
  return(1);
}

//Brainfunktionen
func Brain(_brain) 
{
brain=_brain;
SetEntrance(0);
}

func Typ() { return(3); }

func Speed() { return(speed); }

func Waffe() { return(waffensys); }

func Tarned() { return(1); }

func TurretOwner() { return(1); }