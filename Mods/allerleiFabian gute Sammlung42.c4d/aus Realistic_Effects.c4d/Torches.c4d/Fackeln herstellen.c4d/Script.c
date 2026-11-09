/*-- Fackeln herstellen --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(FK9Z,0,0,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,0,0,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,0,0,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,0,0,GetOwner()));
  RemoveObject(0,1);
}

public IsAnvilProduct: return(1);