/*-- Generator--*/

#strict 3

func Initialize()
{
   local vessel, pilot, energy;
   energy=2000;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Explosion(sphere)
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(sphere,pilot);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}


//Steuerung

func ContainedDownDouble()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}

func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(CAN_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s","Status",CG_1,Par(nil));
  AddMenuItem("%s","Transfer",CG_2,Par(nil));
  AddMenuItem("%s","Zelle",CG_3,Par(nil));
  AddMenuItem("%s","Blaster",CG_4,Par(nil));
  AddMenuItem("%s","Naniten",CG_5,Par(nil));
  AddMenuItem("%s","KI",CG_6,Par(nil));
  return(1);
}

func Status()
{
  Message("Energieniveau:%d%. Naniten:%d.",this(),(energy ?? 0)/50,vessel->Naniten());
  return(1);
}

func Transfer()
{
  if(And((energy ?? 0)<3000, !ObjectCount(UNFI)))
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  var scorpion1,scorpion2;
  if(vessel->Scorpcheck1())
  {
  scorpion1=CreateObject(SCR_,nil,nil,GetOwner());
  ObjectSetAction(scorpion1,"Attaching",this());
  SetActionData(256 + 15 , scorpion1);
  SetR(-90,scorpion1);
  vessel->Scorpdock1(scorpion1);
  scorpion1->Nofx();
  if(!ObjectCount(UNFI))S42MigrationAssign(energy, "-=", 3000);
  Sound("Energize",this());
  return(1);
  }
  if(vessel->Scorpcheck2())
  {
  scorpion2=CreateObject(SCR_,nil,nil,GetOwner());
  ObjectSetAction(scorpion2,"Attaching",this());
  SetActionData(256 + 16 , scorpion2);
  SetR(90,scorpion2);
  vessel->Scorpdock2(scorpion2);
  scorpion2->Nofx();
  S42MigrationAssign(energy, "-=", 3000);
  Sound("Energize",this());
  return(1);
  }
  Message("Alle Docks belegt.",this());
  Sound("Error",this());
  return(1);
}

func Zelle()
{
  if((GetAmmo(ENAM,pilot) ?? 0)>=200)
  {
  Message("Munitionsgürtel voll.",this());
  return(nil);
  }
  if((energy ?? 0)<500)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  DoAmmo(ENAM,100,pilot);
  Message("100 Energie-Einheiten abgezapft.",this());
  S42MigrationAssign(energy, "-=", 500);
  Sound("Energize",this());
}

func Blaster()
{
  if((energy ?? 0)<4000)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  S42MigrationAssign(energy, "-=", 4000);
  Sound("BlasterCH",this());
  ObjectSetAction(vessel,"Blaster",this());
  return(1);
}

func Naniten()
{
  if((vessel->Naniten() ?? 0)>80)
  {
  Sound("Error",this());
  return( Message("Maximum an Naniten erreicht.",this()));
  }
  if((energy ?? 0)<1500)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  S42MigrationAssign(energy, "-=", 1500);
  vessel->Naniten(1);
  Sound("Energize",this());
  return( Message("Naniten beginnen mit Reperatur.",this()));
}

func KI()
{
  if((energy ?? 0)<1000)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  var scorp;
  if(scorp=vessel->BrainCheck())
  {
  S42MigrationAssign(energy, "-=", 1000);
  vessel->BrainScorpion(scorp);
  return(1);
  }  
  Sound("Error",this());
  return( Message("Kein Scorpion verfügbar.",this()));
  return(1);
}

func ContainedUp()
{
    return(1);
}

func ContainedDown()
{
    return(1);
}


func ContainedThrow()
{
  return(1);
}


func ContainedLeft()
{
    return(1);
}


func ContainedRight()
{
    return(1);
}


func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}

func Energy()
{
  if((energy ?? 0)<=5000) S42MigrationStep(energy, 1, true);
  return(1);
}

















