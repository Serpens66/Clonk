/*-- Teleporter --*/

#strict 3

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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
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
  CreateMenu(A56B,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s","BeamUp",CT_1,Par(nil));
  AddMenuItem("%s","BeamDown",CT_2,Par(nil));
  return(1);
}

func Check()
{
  
  var Beam,Crew;
  if(Beam=FindObject (SCB_, -35, -60, 70, 60))
  {
  if(!Hostile (GetOwner(Beam), GetOwner(this())))
  {
  Enter(vessel,Contents(nil,Beam));
//  Enter(this(),Contents(0,Beam));
  vessel->TelFXUp();
  pilot2 = (Beam->Hit());
//Enter(this(),pilot2);
  }
  }
  
  if(Beam=FindObject (SCB_, -35, nil, 70, 50))
  {
  if(!Hostile (GetOwner(Beam), GetOwner(this())))
  {
  Enter(vessel,Contents(nil,Beam));
//  Enter(this(),Contents(0,Beam));
  vessel->TelFXDown();
  pilot2 = (Beam->Hit());
//Enter(this(),pilot2);
  }
  }
  
  if(beamup)
  {
  if(Crew=FindObject (HZCK, -5, 20, 10, 200,nil,nil,nil,NoContainer()))
  {
  if (PathFree(GetX(), (GetY() ?? 0)+35, GetX(Crew), GetY(Crew)))
  {
  if(!Hostile (GetOwner(Crew), GetOwner(this())))
  {
  Beam=CreateObject(SCB_,(GetX(Crew) ?? 0)-(GetX() ?? 0),(GetY(Crew) ?? 0)-(GetY() ?? 0),GetOwner(this()));
//  Enter(Beam,Crew);
  Enter(this(),Crew);
  pilot2 = (Beam->Up());
  Sound("Cloak2",this());
  beamup=nil;
//  Enter(this(),pilot2);
  return(vessel->TelLight(nil));
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
  Exit(Beam,nil,60,nil,nil,20,nil);
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
  return(vessel->TelLight(nil));
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}



















