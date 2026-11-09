/*-- Lichter für alle --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(NLGT,0,0,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,0,0,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,0,0,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,0,0,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,0,0,GetOwner()));
  RemoveObject(0,1);
}
