/*-- Generator--*/

#strict

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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(sphere,pilot);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}


//Steuerung

func ContainedDownDouble()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}

func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(CAN_,Par(0),0,0,0,0);
  AddMenuItem("%s","Status",CG_1,Par(0)); 
  AddMenuItem("%s","Transfer",CG_2,Par(0));   		
  AddMenuItem("%s","Zelle",CG_3,Par(0));
  AddMenuItem("%s","Blaster",CG_4,Par(0));
  AddMenuItem("%s","Naniten",CG_5,Par(0));
  AddMenuItem("%s","KI",CG_6,Par(0));
  return(1);
}

func Status()
{
  Message("Energieniveau:%d%. Naniten:%d.",this(),energy/50,vessel->Naniten());
  return(1);
}

func Transfer()
{
  if(energy<3000 && !ObjectCount(UNFI))
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  var scorpion1,scorpion2;
  if(vessel->Scorpcheck1())
  {
  scorpion1=CreateObject(SCR_,0,0,GetOwner());
  ObjectSetAction(scorpion1,"Attaching",this());
  SetActionData(256 + 15 , scorpion1);
  SetR(-90,scorpion1);
  vessel->Scorpdock1(scorpion1);
  scorpion1->Nofx();
  if(!ObjectCount(UNFI))energy-=3000;
  Sound("Energize",this());
  return(1);
  }
  if(vessel->Scorpcheck2())
  {
  scorpion2=CreateObject(SCR_,0,0,GetOwner());
  ObjectSetAction(scorpion2,"Attaching",this());
  SetActionData(256 + 16 , scorpion2);
  SetR(90,scorpion2);
  vessel->Scorpdock2(scorpion2);
  scorpion2->Nofx();
  energy-=3000;
  Sound("Energize",this());
  return(1);
  }
  Message("Alle Docks belegt.",this());
  Sound("Error",this());
  return(1);
}

func Zelle()
{
  if(GetAmmo(ENAM,pilot)>=200)
  {
  Message("Munitionsgürtel voll.",this());
  return(0);
  }
  if(energy<500)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  DoAmmo(ENAM,100,pilot);
  Message("100 Energie-Einheiten abgezapft.",this());
  energy-=500;
  Sound("Energize",this());
}

func Blaster()
{
  if(energy<4000)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  energy-=4000;
  Sound("BlasterCH",this());
  ObjectSetAction(vessel,"Blaster",this());
  return(1);
}

func Naniten()
{
  if(vessel->Naniten()>80)
  {
  Sound("Error",this());
  return( Message("Maximum an Naniten erreicht.",this()));
  }
  if(energy<1500)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  energy-=1500;
  vessel->Naniten(1);
  Sound("Energize",this());
  return( Message("Naniten beginnen mit Reperatur.",this()));
}

func KI()
{
  if(energy<1000)
  {
  Sound("Error",this());
  return( Message("Energieniveau zu gering.",this()));
  }
  var scorp;
  if(scorp=vessel->BrainCheck())
  {
  energy-=1000;
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
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}

func Energy()
{
  if(energy<=5000) energy++;
  return(1);
}

















