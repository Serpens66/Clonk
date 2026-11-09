/*-- Sand aus Erde --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(SAND,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}
