/* Mammoth-Pod */

#strict 3

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
  turret=CreateObject(DRG_,nil,nil,GetOwner());
  ObjectSetAction(turret,"Attaching",this());
  turret->Rotinit(start,min,max);
  turret->Bezug(this());
  //Bezug
  mammoth=mam;
  kaprot=kap;
  //Kapsel nicht benutzt
  des=nil;
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

func Besetzt()
{
  if(des) return(1);
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}

func Removal()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

//Menüsteuerung
func ContainedDig()
{
  CreateMenu(DRA_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",MMA1,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  if(mammoth->Besetzt())    AddMenuItem("%s","Occp",MMA6,Par(nil)); //Podeinstieg
  if(!(mammoth->Besetzt())) AddMenuItem("%s","Bridge",MMA3,Par(nil)); //Podeinstieg
  AddMenuItem("%s","Schadenskontrolle",MMA5,Par(nil)); //Schaden
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Ausstieg",MMA4,Par(nil));    //Ausstieg
  SelectMenuItem(7,Par(nil));
  return(1);
}

func Occp()
{
  Sound("Error");
  return(nil);
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


func Ausstieg()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
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
  Exit(kapsel,nil,nil,(GetR(mammoth) ?? 0)+(kaprot ?? 0),(GetXDir(mammoth) ?? 0)/10+(kaprot ?? 0)/4,(GetYDir(mammoth) ?? 0)/10-15,nil);
  ObjectSetAction(kapsel,"Start");
  Sound("Rocket");
  des=1;
  return(1);
}

func Bridge()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GSAM,waffensys), pilot); 
  DoAmmo(GRAM, GetAmmo(GRAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  Enter(mammoth,pilot);
  pilot=nil;
  return(1);
}

func Schadenskontrolle()
{
  if (!GetDamage(mammoth))
  {
  Message("Kein Schaden.",mammoth);
  return(1);
  }
  Message("Schaden: %d%.",mammoth,(GetDamage(mammoth) ?? 0)/3);
  return(1);
}










