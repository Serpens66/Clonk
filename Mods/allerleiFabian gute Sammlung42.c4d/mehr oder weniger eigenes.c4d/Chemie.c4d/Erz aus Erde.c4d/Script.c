/*-- Erz aus Erde --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(ORE1,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}
