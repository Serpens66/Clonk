/*-- Geschützkanzel --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, vessel, vertex, missle;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}


func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  if(missle) Sprengung();
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}


//Steuerung

func ContainedUp()
{
    ObjectCall(missle,"ControlUp");
    return(1);
}


func ContainedDown()
{
    ObjectCall(missle,"ControlDown");
    return(1);
}


func ContainedDownDouble()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  if(missle) 
  {
  if(SEqual(GetAction(missle),"Eingefahren"))
  {
  Enter(vessel,pilot);
  SetAction("Einfahren");
  Sound("Airlock1");
  return(1);
  }
  if(Or(SEqual(GetAction(missle),"Start"), SEqual(GetAction(missle),"Travel")))
  {
  ObjectCall(missle,"Explosion");
  return(1);
  }
  if(missle) if(!SEqual(GetAction(missle),"Einfahren"))   ObjectSetAction(missle,"Einfahren",this());
  SetActionData(256 + 3 , missle);
  Sound("SteelGate1");
  return(1);
  }
  SetAction("Einfahren");
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}


func ContainedRight()
{
    if(SEqual(GetAction(missle),"Travel"))  ObjectCall(missle,"ControlRight");
    return(1);
}


func ContainedLeft()
{
    if(SEqual(GetAction(missle),"Travel"))  ObjectCall(missle,"ControlLeft");
    return(1);
}


func ContainedThrow()
{
  if(SEqual(GetAction(missle),"Eingefahren")) 
  {
  Laden();
  return(1);
  }
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  if(SEqual(GetAction(missle),"Ready")) return(Start());
  if(missle) return(1);
  if (Or(((GetAmmo(MIAM,pilot) ?? 0)>=3), (ObjectCount(NOAM))))
  {
  Laden(); 
   if(!ObjectCount(NOAM)) DoAmmo(MIAM,-3,pilot);
  }
  else
  {
  PlayerMessage(GetOwner(pilot), "$NotEnoughAmmo$", pilot, MIAM);
  }
 
  return(1);
}


func ContainedDig() 
{
  if(SEqual(GetAction(missle),"Travel")) Sprengung();
  return(1);
}


func Laden()
{
  if(!missle) missle=CreateObject(CMM_,nil,nil,GetOwner(pilot));
  ObjectSetAction(missle,"Attaching",this());
  SetActionData(256 + 3 , missle);
  Sound("SteelGate1");
  return(1);
}


func Start()
{
  SetR(180,missle);
  Sound("Scavenger");
  ObjectSetAction(missle,"Start",this());
  SetXDir(GetXDir(this()),missle);
  return(1);
}
 

func Sprengung()
{
  ObjectCall(missle,"ControlThrow");
  return(1);
}
