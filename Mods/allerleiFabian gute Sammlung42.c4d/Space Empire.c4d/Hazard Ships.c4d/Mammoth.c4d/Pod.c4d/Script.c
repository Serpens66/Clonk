/* Mammoth-Pod */

#strict

public func ReadyToFire()   { return(true); }

//Anfangswerte
protected func Initialize()
{
  local waffensys, turret, pilot, kapsel, kaprot, mammoth, des;
  return(1);
}

func Pod(max,min,start,mam,kap)
{
  //Geschütz erstellen und anbringen
  turret=CreateObject(DRG_,0,0,GetOwner());
  ObjectSetAction(turret,"Attaching",this());
  turret->Rotinit(start,min,max);
  turret->Bezug(this());
  //Bezug
  mammoth=mam;
  kaprot=kap;
  //Kapsel nicht benutzt
  des=0;
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}

func Besetzt()
{
  if(des) return(1);
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}

func Removal()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  if(waffensys) RemoveObject(waffensys);
  if(turret) RemoveObject(turret);
  return(1);
}

//Steuerung
func ContainedLeft()
{
  turret->ContainedLeft();
  return(1);
}

func ContainedRight()
{
  turret->ContainedRight();
  return(1);
}

func ContainedDown()
{
  turret->ContainedDown();
  return(1);
}

func ContainedUp()
{
  turret->ContainedUp();
  return(1);
}

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

//Menüsteuerung
func ContainedDig()
{
  CreateMenu(DRA_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Waffenmodi",MMA1,Par(0));  //Waffenmodi
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  if(mammoth->Besetzt())    AddMenuItem("%s","Occp",MMA6,Par(0)); //Podeinstieg
  if(!(mammoth->Besetzt())) AddMenuItem("%s","Bridge",MMA3,Par(0)); //Podeinstieg
  AddMenuItem("%s","Schadenskontrolle",MMA5,Par(0)); //Schaden
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Ausstieg",MMA4,Par(0));    //Ausstieg
  SelectMenuItem(7,Par(0)); 
  return(1);
}

func Occp()
{
  Sound("Error");
  return(0);
}

func Waffensystem()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(DRWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=DRWS)   ShiftContents(pilot);
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


func Ausstieg()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GSAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GRAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Sound("Airlock1");
  return(Kapsel());
}


func Kapsel()
{
  kapsel=CreateContents(DRK_);
  Enter(kapsel,pilot);
  Exit(kapsel,0,0,GetR(mammoth)+kaprot,GetXDir(mammoth)/10+kaprot/4,GetYDir(mammoth)/10-15,0);
  ObjectSetAction(kapsel,"Start");
  Sound("Rocket");
  des=1;
  return(1);
}

func Bridge()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GSAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GRAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Enter(mammoth,pilot);
  pilot=0;
  return(1);
}

func Schadenskontrolle()
{
  if (!GetDamage(mammoth))
  {
  Message("Kein Schaden.",mammoth);
  return(1);
  }
  Message("Schaden: %d%.",mammoth,GetDamage(mammoth)/3);
  return(1);
}










