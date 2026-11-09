/*-- Lichter für alle --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(NLGT,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(NLGT,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}
