/*-- Öl herstellen --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(OBRL,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}
