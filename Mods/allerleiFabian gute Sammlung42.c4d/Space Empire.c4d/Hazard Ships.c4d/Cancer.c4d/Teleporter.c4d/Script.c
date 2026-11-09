/*-- Teleporter --*/

#strict

func Initialize()
{
   local vessel, pilot, beamup, pilot2;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
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
  CreateMenu(A56B,Par(0),0,0,0,0);
  AddMenuItem("%s","BeamUp",CT_1,Par(0)); 
  AddMenuItem("%s","BeamDown",CT_2,Par(0));   		
  return(1);
}

func Check()
{
  
  var Beam,Crew;
  if(Beam=FindObject (SCB_, -35, -60, 70, 60))
  {
  if(!Hostile (GetOwner(Beam), GetOwner(this())))
  {
  Enter(vessel,Contents(0,Beam));
//  Enter(this(),Contents(0,Beam));
  vessel->TelFXUp();
  pilot2 = (Beam->Hit());
//Enter(this(),pilot2);
  }
  }
  
  if(Beam=FindObject (SCB_, -35, 0, 70, 50))
  {
  if(!Hostile (GetOwner(Beam), GetOwner(this())))
  {
  Enter(vessel,Contents(0,Beam));
//  Enter(this(),Contents(0,Beam));
  vessel->TelFXDown();
  pilot2 = (Beam->Hit());
//Enter(this(),pilot2);
  }
  }
  
  if(beamup)
  {
  if(Crew=FindObject (HZCK, -5, 20, 10, 200,0,0,0,NoContainer()))
  {
  if (PathFree(GetX(), GetY()+35, GetX(Crew), GetY(Crew)))
  {
  if(!Hostile (GetOwner(Crew), GetOwner(this())))
  {
  Beam=CreateObject(SCB_,GetX(Crew)-GetX(),GetY(Crew)-GetY(),GetOwner(this()));
//  Enter(Beam,Crew);
  Enter(this(),Crew);
  pilot2 = (Beam->Up());
  Sound("Cloak2",this());
  beamup=0;
//  Enter(this(),pilot2);
  return(vessel->TelLight(0));
  }
  }
  }
  }
  return(1);
}


func BeamDown()
{
  var Beam;
  Beam=CreateContents(SCB_);
  Enter(Beam,pilot);
  Beam->Down();
  Exit(Beam,0,60,0,0,20,0);
  Sound("Cloak2",this());
  return(1);
}



func BeamUp()
{
  beamup=!beamup;
  Sound("Bip",this());
  if(beamup) Message("Teleporter sucht...",this());
  if(!beamup)
  {
  Message("Suchmodus deaktiviert.",this());
  return(vessel->TelLight(0));
  }
  vessel->TelLight(1); 
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



















