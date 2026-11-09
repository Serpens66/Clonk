/*-- Verteidigungssteuerung --*/

#strict 3

local bunker,on;


func Initialize() {
  bunker = CreateObject(DFB_,nil,20);
  on=nil;
}

public func ControlDigDouble(user)
{
  SetOwner(GetOwner(user));
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  on=(!on);
  bunker->SetUser(user);
  if(on) 
  {
  bunker->SetAction("Search");
  Sound("Energize");
  }
  else
  {
  bunker->Off();
  Sound("DeEnergize");
  }
  return(1);
}

public func Active()
{
  bunker->SetAction("Search");
}
public func ControlUpDouble()
{
  if (!GetDamage(bunker))
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),(GetDamage(bunker) ?? 0)/3);
  return(1);
}
